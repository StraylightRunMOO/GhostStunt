/******************************************************************************
  Copyright (c) 1992, 1995, 1996 Xerox Corporation.  All rights reserved.
  Portions of this code were written by Stephen White, aka ghond.
  Use and copying of this software and preparation of derivative works based
  upon this software are permitted.  Any distribution of this software or
  derivative works must comply with all applicable United States export
  control laws.  This software is made available AS IS, and Xerox Corporation
  makes no warranty about the software, its performance or its conformity to
  any specification.  Any person obtaining a copy of this software is requested
  to send their name and post office or electronic mail address to:
    Pavel Curtis
    Xerox PARC
    3333 Coyote Hill Rd.
    Palo Alto, CA 94304
    Pavel@Xerox.Com
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <regex>

#include "config.h"
#include "db.h"
#include "functions.h"
#include "log.h"
#include "structures.h"
#include "server.h"
#include "match.h"
#include "parse_cmd.h"
#include "storage.h"
#include "unparse.h"
#include "utils.h"
#include "tasks.h"
#include "list.h"

enum match_type {
    MATCH_NONE,
    MATCH_FULL,
    MATCH_PART,
};

static inline Var 
get_aliases(Objid what, Objid player)
{
    Var aliases;
    if(server_flag_option("match_aliases_verb", 0) > 0) {
        run_server_task(player, Var::new_obj(what), "aliases", new_list(0), "", &aliases);
        if(aliases.type == TYPE_LIST) return aliases;
        free_var(aliases);
    }

    aliases = db_property_value_default(what, "aliases", new_list(0));
    if(aliases.type != TYPE_LIST) {
        free_var(aliases);
        aliases = new_list(0);
    }    

    Var name = str_dup_to_var(db_object_name(what));
    aliases = listconcat(var_ref(aliases), explode(name, ' ', false));
    free_var(name);

    return aliases;
}

static inline Var
get_contents(Objid what, Objid player)
{
    Var contents;
    if(server_flag_option("match_contents_verb", 0) > 0) {
        run_server_task(player, Var::new_obj(what), "contents", new_list(0), "", &contents);
        if(contents.type == TYPE_LIST) return contents;
        free_var(contents);
    }

    contents = var_ref(db_property_value_default(what, "contents", new_list(0)));
    return contents;
}

static inline Var
contents_for_match(Objid player)
{
    Var contents = new_list(0);

    int step;
    Objid oid, loc = db_object_location(player);
    for (oid = player, step = 0; step < 2; oid = loc, step++) {
        if (!valid(oid)) continue;
        contents = listconcat(contents, get_contents(oid, player));
    }

    return contents;
}

static inline Var
prepare_tokens(Var tokens)
{
    if(tokens.type != TYPE_LIST) {
        free_var(tokens);
        return new_list(0);
    }

    Var r = new_list(0);
    listforeach(tokens, [&r](Var value, int index) -> int {
        if(value.type == TYPE_STR && value.str() != nullptr && value.str()[0] != '\0' && value.length() >= 3) {
            r = listconcat(r, explode(lowercase(value), ' ', false));
        }
        return 0;
    });

    free_var(tokens);
    if(!r.length()) return r;

    std::qsort(
        (void*)&r.v.list[1],
        r.length(),
        sizeof(Var),
        [](const void* x, const void* y)
        {
            const Var lhs = *static_cast<const Var*>(x);
            const Var rhs = *static_cast<const Var*>(y);
            return strcasecmp(lhs.str(), rhs.str());
        }
    );

    return r;
}

static enum match_type 
match_tokens(Var tokens, Var aliases)
{
    enum match_type token_match, match = MATCH_NONE;
    
    for(auto i=1; i<=tokens.length(); i++) {
        token_match = MATCH_NONE;
        auto len = strlen(tokens[i].str());
        for(auto j=1; j<=aliases.length(); j++) {
            auto len_alias = strlen(aliases[j].str());
            if(len > len_alias) continue;
            
            if(strncmp(aliases[j].str(), tokens[i].str(), len) == 0) {
                token_match = (len == len_alias) ? MATCH_FULL : MATCH_PART;
                break;
            }
        }

        if(token_match == MATCH_NONE) return token_match;
        match = std::max(match, token_match);
    }

    return match;
}

static std::pair<Var, Var> 
parse_ordinal(Var word) 
{
    std::string needle = word.str();

    // Matching for "first thing", "second thing", etc.
    if (auto search = english_ordinals.find(needle); search != english_ordinals.end()) {
        return std::make_pair(Var::new_int(search->second), str_dup_to_var(""));
    }

    std::pair<Var, Var> result = std::make_pair(Var::new_int(0), word);

    std::smatch sm;
    if (std::regex_search(needle, sm, match_numeric) && sm.size() > 1) {
        try { // Matching for "1.thing", "2.thing", etc.
            result = std::make_pair(Var::new_int(std::stoi(sm.str(1))), substr(word, sm.str(0).size()+1, word.length()));
        } catch (...) { }
    } else if (std::regex_search(needle, sm, match_ordinal) && sm.size() > 1) {
        try { // Matching for "1st thing", "2nd thing", etc.
            result = std::make_pair(Var::new_int(std::stoi(sm.str(1))), str_dup_to_var(""));
        } catch (...) { }
    }

    return result;
}

static Var
complex_match(Var subject, Var targets, bool use_ordinal = true)
{
    if (!targets.length()) {
        free_var(subject);
        return Var::new_obj(FAILED_MATCH);
    }

    Var ordinal = Var::new_int(0);
    Var tokens  = explode(var_ref(subject), ' ', false);

    if(use_ordinal)
        std::tie(ordinal, tokens[1]) = parse_ordinal(tokens[1]);

    tokens = prepare_tokens(tokens);

    if(!tokens.length()) {
        free_var(subject);
        return Var::new_obj(FAILED_MATCH);
    }

    Var full_matches = new_list(0);
    Var part_matches = new_list(0);

    listforeach(targets, [&tokens, &full_matches, &part_matches](Var target, int i) -> int {
        Var aliases = prepare_tokens(var_ref(target));

        if(!aliases.length()) return 0;

        switch(match_tokens(tokens, aliases)) {
        case MATCH_NONE: break;
        case MATCH_FULL: full_matches = listappend(full_matches, Var::new_int(i));
        case MATCH_PART: part_matches = listappend(part_matches, Var::new_int(i));
        }

        return 0;
    });

    Var matches;
    if(use_ordinal && ordinal.num() > 0) {
        if(full_matches.length() >= ordinal.num())
            matches = var_ref(full_matches[ordinal]);
        else if(part_matches.length() >= ordinal.num())
            matches = var_ref(part_matches[ordinal]);
        else
            matches = complex_match(var_ref(subject), var_ref(targets), false);
    } else {
        if(full_matches.length() > 0) {
            matches = var_ref(full_matches);
        } else if(part_matches.length() > 0) {
            matches = var_ref(part_matches);
        } else {
            matches = Var::new_obj(FAILED_MATCH);
        }
    }

    if(matches.length() == 1) {
        Var r = var_ref(matches[1]);
        free_var(matches);
        matches = r;
    }

    free_var(tokens);
    free_var(subject);
    free_var(targets);
    free_var(full_matches);
    free_var(part_matches);

    return matches;
}

static Objid
match_contents(Objid player, const char *name)
{
    if (!valid(player)) return FAILED_MATCH;

    Objid ret    = AMBIGUOUS;
    Var aliases  = new_list(0);
    Var subject  = str_dup_to_var(name);
    Var contents = contents_for_match(player);

    listforeach(contents, [&player, &aliases](Var value, int index) -> int {
        aliases = listappend(aliases, get_aliases(value.obj(), player));
        return 0;
    });

    Var matches = complex_match(var_ref(subject), var_ref(aliases));

    if(matches.type == TYPE_INT) 
        ret = contents[matches].obj();
    else if(matches.type == TYPE_OBJ)
        ret = matches.obj();

    free_var(subject);
    free_var(aliases);
    free_var(matches);
    free_var(contents);

    return ret;
}

Objid
match_object(Objid player, const char *name)
{
    if (name[0] == '\0')
        return NOTHING;
    if (name[0] == '#') {
        char *p;
        Objid r = strtol(name + 1, &p, 10);

        if (*p != '\0' || !valid(r))
            return FAILED_MATCH;

        return r;
    }

    if (!valid(player))
        return FAILED_MATCH;
    if (!strcasecmp(name, "me"))
        return player;
    if (!strcasecmp(name, "here"))
        return db_object_location(player);

    return match_contents(player, name);
}

static package 
bf_parse_ordinal(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var ordinal, tokens = explode(arglist[1], ' ', false);

    std::tie(ordinal, tokens[1]) = parse_ordinal(tokens[1]);

    if(memo_strlen(tokens[1].str()) == 0)
        tokens = sublist(tokens, 2, tokens.length());

    Var r = new_list(2);
    r[1] = ordinal;
    r[2] = implode(tokens, str_dup_to_var(" "));

    free_var(arglist);
    return make_var_pack(r);
}

static package 
bf_tokenize(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var r = prepare_tokens(var_ref(arglist[1]));
    free_var(arglist);
    return make_var_pack(r);
}

static package
bf_complex_match(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var r = complex_match(var_ref(arglist[1]), var_ref(arglist[2]));
    free_var(arglist);
    return make_var_pack(r);
}

void
register_match(void)
{
    register_function("parse_ordinal", 1, 1, bf_parse_ordinal, TYPE_STR);
    register_function("tokenize",      1, 1, bf_tokenize, TYPE_LIST);
    register_function("complex_match", 2, 2, bf_complex_match, TYPE_STR, TYPE_LIST);
}
