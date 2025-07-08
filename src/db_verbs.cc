/******************************************************************************
  Copyright (c) 1995, 1996 Xerox Corporation.  All rights reserved.
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

/*****************************************************************************
 * Routines for manipulating verbs on DB objects
 *****************************************************************************/

#include <assert.h>
#include <ctype.h>

#include <stdlib.h>
#include <string.h>
#include <chrono>

#include "config.h"
#include "db.h"
#include "db_private.h"
#include "db_tune.h"
#include "list.h"
#include "log.h"
#include "map.h"
#include "parse_cmd.h"
#include "program.h"
#include "server.h"
#include "storage.h"
#include "utils.h"

static Var verb_cache;

/*********** Prepositions ***********/

#define MAXPPHRASE 3        /* max. number of words in a prepositional phrase */

/* NOTE: New prepositional phrases should only be added to this list at the
 * end, never in the middle, and no entries should every be removed from this
 * list; the list indices are stored raw in the DB file.
 */
static const char *prep_list[] =
{
    "with/using",
    "at/to",
    "in front of",
    "in/inside/into",
    "on top of/on/onto/upon",
    "out of/from inside/from",
    "over",
    "through",
    "under/underneath/beneath",
    "behind",
    "beside",
    "for/about",
    "is",
    "as",
    "off/off of",
};

#define NPREPS Arraysize(prep_list)

typedef struct pt_entry {
    int nwords;
    char *words[MAXPPHRASE];
    struct pt_entry *next;
} pt_entry;

struct pt_entry *prep_table[NPREPS];

void
dbpriv_build_prep_table(void)
{
    int i, j;
    int nwords;
    char **words;
    char cprep[100];
    const char *p;
    char *t;
    pt_entry *current_alias, **prev;

    for (i = 0; i < NPREPS; i++) {
        p = prep_list[i];
        prev = &prep_table[i];
        while (*p) {
            t = cprep;
            if (*p == '/')
                p++;
            while (*p && *p != '/')
                *t++ = *p++;
            *t = '\0';

            /* This call to PARSE_INTO_WORDS() is the reason that this function
             * is called from DB_INITIALIZE() instead of from the first call to
             * DB_FIND_PREP(), below.  You see, PARSE_INTO_WORDS() isn't
             * re-entrant, and DB_FIND_PREP() is called from code that's still
             * using the results of a previous call to PARSE_INTO_WORDS()...
             */
            words = parse_into_words(cprep, &nwords);

            current_alias = (struct pt_entry *)mymalloc(sizeof(struct pt_entry), M_PREP);
            current_alias->nwords = nwords;
            current_alias->next = nullptr;
            for (j = 0; j < nwords; j++)
                current_alias->words[j] = str_dup(words[j]);
            *prev = current_alias;
            prev = &current_alias->next;
        }
    }
}

db_prep_spec
db_find_prep(int argc, char *argv[], int *first, int *last)
{
    pt_entry *alias;
    int i, j, k;
    int exact_match = (first == nullptr || last == nullptr);

    for (i = 0; i < argc; i++) {
        for (j = 0; j < NPREPS; j++) {
            for (alias = prep_table[j]; alias; alias = alias->next) {
                if (i + alias->nwords <= argc) {
                    for (k = 0; k < alias->nwords; k++) {
                        if (strcasecmp(argv[i + k], alias->words[k]))
                            break;
                    }
                    if (k == alias->nwords
                            && (!exact_match || i + k == argc)) {
                        if (!exact_match) {
                            *first = i;
                            *last = i + alias->nwords - 1;
                        }
                        return (db_prep_spec) j;
                    }
                }
            }
        }
        if (exact_match)
            break;
    }
    return PREP_NONE;
}

db_prep_spec
db_match_prep(const char *prepname)
{
    db_prep_spec prep;
    int argc;
    char *ptr;
    char **argv;
    char *s, first;

    s = str_dup(prepname);
    first = s[0];
    if (first == '#')
        first = (++s)[0];
    prep = (db_prep_spec)strtol(s, &ptr, 10);
    if (*ptr == '\0') {
        free_str(s);
        if (!isdigit(first) || prep >= NPREPS)
            return PREP_NONE;
        else
            return prep;
    }
    ptr = strchr(s, '/');
    if (ptr != nullptr)
        *ptr = '\0';

    argv = parse_into_words(s, &argc);
    prep = db_find_prep(argc, argv, nullptr, nullptr);
    free_str(s);
    return prep;
}

const char *
db_unparse_prep(db_prep_spec prep)
{
    if (prep == PREP_NONE)
        return "none";
    else if (prep == PREP_ANY)
        return "any";
    else
        return prep_list[prep];
}

/*********** Verbs ***********/

#define DOBJSHIFT  4
#define IOBJSHIFT  6
#define OBJMASK    0x3
#define PERMMASK   0xF

int
db_add_verb(Var obj, const char *vnames, Objid owner, unsigned flags,
            db_arg_spec dobj, db_prep_spec prep, db_arg_spec iobj)
{
    Object *o = dbpriv_dereference(obj);
    Verbdef *v, *newv;
    int count;

    db_priv_affected_callable_verb_lookup();

    newv = (Verbdef *)mymalloc(sizeof(Verbdef), M_VERBDEF);
    newv->name = vnames;
    newv->owner = owner;
    
    Var meta;
    meta.type = TYPE_NONE;
    newv->meta = meta;
    
    newv->perms = flags | (dobj << DOBJSHIFT) | (iobj << IOBJSHIFT);
    newv->prep = prep;
    newv->next = nullptr;
    newv->program = nullptr;
    if (o->verbdefs) {
        for (v = o->verbdefs, count = 2; v->next; v = v->next, ++count);
        v->next = newv;
    } else {
        o->verbdefs = newv;
        count = 1;
    }
    return count;
}

static Verbdef *
find_verbdef_by_name(Object * o, const char *vname, int check_x_bit)
{
    Verbdef *v;

    for (v = o->verbdefs; v; v = v->next)
        if (verbcasecmp(v->name, vname)
                && (!check_x_bit || (v->perms & VF_EXEC)))
            break;

    return v;
}

int
db_count_verbs(Var obj)
{
    int count = 0;
    Object *o = dbpriv_dereference(obj);
    Verbdef *v;

    for (v = o->verbdefs; v; v = v->next)
        count++;

    return count;
}

int
db_for_all_verbs(Var obj,
                 int (*func) (void *data, const char *vname),
                 void *data)
{
    Object *o = dbpriv_dereference(obj);
    Verbdef *v;

    for (v = o->verbdefs; v; v = v->next)
        if ((*func) (data, v->name))
            return 1;

    return 0;
}

void
db_delete_verb(db_verb_handle vh)
{
    verb_handle *h = (verb_handle *)vh.ptr;
    Object *o = dbpriv_find_object(h->definer.obj());
    Verbdef *v = h->verbdef;
    Verbdef *vv;

    db_priv_affected_callable_verb_lookup();

    vv = o->verbdefs;
    if (vv == v)
        o->verbdefs = v->next;
    else {
        while (vv->next != v)
            vv = vv->next;
        vv->next = v->next;
    }

    if (v->program)
        free_program(v->program);
    if (v->name)
        free_str(v->name);
    myfree(v, M_VERBDEF);
}

db_verb_handle
db_find_command_verb(Objid oid, const char *verb,
                     db_arg_spec dobj, unsigned prep, db_arg_spec iobj)
{
    Object *o;
    Verbdef *v;
    static verb_handle h;
    db_verb_handle vh;

    Var ancestors;
    Var ancestor;
    int i, c;

    ancestors = db_ancestors(Var::new_obj(oid), true);

    FOR_EACH(ancestor, ancestors, i, c) {
        o = dbpriv_find_object(ancestor.v.obj);
        for (v = o->verbdefs; v; v = v->next) {
            db_arg_spec vdobj = (db_arg_spec)((v->perms >> DOBJSHIFT) & OBJMASK);
            db_arg_spec viobj = (db_arg_spec)((v->perms >> IOBJSHIFT) & OBJMASK);

            if (verbcasecmp(v->name, verb)
                    && (vdobj == ASPEC_ANY || vdobj == dobj)
                    && (v->prep == PREP_ANY || v->prep == prep)
                    && (viobj == ASPEC_ANY || viobj == iobj)) {
                h.definer = ancestor;
                h.verbdef = v;
                vh.ptr = &h;
                vh.oid = ancestor.obj();
                vh.verbname = str_dup_to_var(v->name);

                free_var(ancestors);

                return vh;
            }
        }
    }

    free_var(ancestors);

    vh.ptr = nullptr;

    return vh;
}

#ifdef USE_VERB_CACHE

void
db_priv_affected_callable_verb_lookup(void)
{
    if(!verb_cache.v.map) return;

    free_var(verb_cache);
    verb_cache.v.map = nullptr;
}

Var
db_verb_cache_stats(void)
{
    return Var::new_int(0);
}

void
db_log_cache_stats(void)
{

}

#endif

/*
 * Used by `db_find_callable_verb' once a suitable starting point
 * is found.  The function iterates through all ancestors looking
 * for a matching verbdef.
 */
struct verbdef_definer_data {
    Object *o;
    Verbdef *v;
};

static struct verbdef_definer_data
find_callable_verbdef(Object *start, const char *verb, Var ancestors)
{
    Object *o  = nullptr;
    Verbdef *v = nullptr;

    if ((v = find_verbdef_by_name(start, verb, 1)) != nullptr) {
        struct verbdef_definer_data data;
        data.o = start;
        data.v = v;
        return data;
    }

    listforeach(ancestors, [&verb, &o, &v](Var value, int index) -> int {
        o = dbpriv_dereference(value);
        return ((v = find_verbdef_by_name(o, verb, 1)) != nullptr) ? 1 : 0;
    });

    struct verbdef_definer_data data;
    data.o = o;
    data.v = v;
    return data;
}

static inline Var alloc_call(Var obj, Var verbname, bool found = true) {
    db_verb_handle *vh = (db_verb_handle*)mymalloc(sizeof(db_verb_handle), M_CALL);
    vh->ptr = nullptr;
    vh->oid = obj.obj();
    vh->verbname = var_ref(verbname);

    Var c;
    c.type = TYPE_CALL;
    c.v.call = vh;

    return c;
}

static inline Var cache_timestamp() {
    using clock     = std::chrono::steady_clock;
    using duration  = std::chrono::duration<double, std::ratio<1>>;
    using ms        = std::chrono::time_point<clock, duration>;

    duration t = clock::now().time_since_epoch();

    return Var::new_float(t.count());
}

static inline void prune_verb_cache() {
    /*
    Var entries = new_list(0);
    mapforeach(verb_cache, [&entries](Var obj, Var cache, int i) -> int {
        mapforeach(cache, [&entries, &obj](Var verb, Var entry, int j) -> int {
            Var item = new_list(3);
            item[1] = obj;
            item[2] = var_ref(verb);
            item[3] = Var::new_float((cache_timestamp().fnum() - entry[3].fnum()) / entry[2].fnum());
            entries = listappend(entries, item);
            return 0;
        });
        return 0;
    });

    if(entries.length() > 1) {
        std::qsort(
            (void*)&entries.v.list[1],
            entries.length(),
            sizeof(Var),
            [](const void* x, const void* y)
            {
                const Var lhs = *static_cast<const Var*>(x);
                const Var rhs = *static_cast<const Var*>(y);
                return lhs.v.list[3].fnum() < rhs.v.list[3].fnum() ? -1 : 1;
            }
        );
    }

    int dc = 0;
    Var current, cache;
    for(auto i=entries.length(); i > 100; i--) {
        current = entries[i];
        const map_entry *m = maplookup(verb_cache, current[1], nullptr, 0);
        if(m != nullptr && mapdelete(m->value, var_ref(current[2]))) 
            dc++;
    }

    entries = sublist(entries, 1, entries.length() - dc);

    free_var(entries);
    */
}

static inline Var ancestors_with_verbs(Var obj) {
    Object *o;
    Var r = new_list(0);
    Var ancestors = db_ancestors(obj, false);

    listforeach(ancestors, [&o, &r](Var value, int index) -> int {
        if((o = dbpriv_dereference(value)) != nullptr && o->verbdefs != nullptr && is_valid(Var::new_obj(o->id)))
            r = listappend(r, Var::new_obj(o->id));
        return 0;
    });

    free_var(ancestors);

    return r;
}

Var set_entry(Var obj, Var verbname, Var ancestors) {
    Var entry, call = alloc_call(obj, verbname);

    struct verbdef_definer_data data = find_callable_verbdef(dbpriv_dereference(obj), verbname.str(), ancestors);
    if (data.o != nullptr && data.v != nullptr) {
        verb_handle *h = (verb_handle*)mymalloc(sizeof(verb_handle), M_STRUCT);
        h->definer = Var::new_obj(data.o->id);
        h->verbdef = data.v;
        call.v.call->ptr = h;
    }

    entry = new_list(2);
    entry[1] = Var::new_int(1);
    entry[2] = cache_timestamp();

    verb_cache = mapinsert(verb_cache, call, entry);

    free_var(ancestors);
    return call;
}

Var get_entry(Var obj, Var verbname, Var ancestors) {
    static db_verb_handle vh;
    vh.oid = obj.obj();
    vh.verbname = verbname;

    Var call;
    call.type = TYPE_CALL;
    call.v.call = &vh;

    if(verb_cache.v.map == nullptr) {
        verb_cache = new_map(512, false);
        call.v.call->ptr = nullptr;
        free_var(ancestors);
        return call;
    }

    map_entry *m = (map_entry*)maplookup(verb_cache, call, nullptr, 0);
    if(m != nullptr) {
        m->value[1].v.num++;
        free_var(ancestors);
        return var_ref(m->key);
    }

    return set_entry(obj, verbname, ancestors);
}

/* does NOT consume `recv' and `verb' */
db_verb_handle
db_find_callable_verb(Var recv, const char *verb)
{
    if (!recv.is_object()) panic_moo("DB_FIND_CALLABLE_VERB: Not an object!");

    Var ancestors;

    #ifdef USE_VERB_CACHE
        Var c, verbname = str_dup_to_var(verb);

        c = get_entry(recv, verbname, ancestors);
        if(c.v.call->ptr != nullptr) {
            free_var(verbname);
            c.v.call->oid = recv.obj();
            return *c.v.call;
        }

        ancestors = ancestors_with_verbs(recv);
        listforeach(ancestors, [&c, &verbname, &ancestors](Var value, int index) -> int {
            c = get_entry(value, verbname, sublist(var_ref(ancestors), index+1, ancestors.length()));
            return c.v.call->ptr != nullptr ? 1 : 0;
        });

        /*
        if(c.v.call->ptr == nullptr) {
            c = set_entry(recv, verbname, ancestors);
        }
        */
        free_var(ancestors);
        free_var(verbname);
        c.v.call->oid = recv.obj();
        return *c.v.call;
    #else
        static verb_handle h;

        db_verb_handle vh;
        vh.oid = recv.obj();
        vh.ptr = nullptr;
        vh.verbname = str_dup_to_var(verb);

        Object *o = (recv.is_object() && is_valid(recv)) ? dbpriv_dereference(recv) : nullptr;

        ancestors = ancestors_with_verbs(recv);
        struct verbdef_definer_data data = find_callable_verbdef(o, verb, ancestors);
        if (data.o != nullptr && data.v != nullptr) {
            h.definer = Var::new_obj(data.o->id);
            h.verbdef = data.v;
            vh.ptr = &h;
        }

        free_var(ancestors);
        return vh;
    #endif
}

Var db_verb_cache() {
    if(verb_cache.v.map)
        return var_ref(verb_cache);
    else
        return new_map(0);
}

Var make_call(Objid o, const char *vname) {
    db_verb_handle h = db_find_callable_verb(Var::new_obj(o), vname);
    db_verb_handle *vh = (db_verb_handle*)mymalloc(sizeof(db_verb_handle), M_CALL);

    vh->ptr = h.ptr;
    vh->oid = o;
    vh->verbname = str_dup_to_var(vname);
    
    Var c;
    c.type = TYPE_CALL;
    c.v.call = vh;
    return c;
}

bool destroy_call(Var c) {
    db_verb_handle *h = c.v.call;
    myfree(h->ptr, M_STRUCT);
    free_var(h->verbname);
    return true;
}

db_verb_handle
db_find_defined_verb(Var obj, const char *vname, int allow_numbers)
{
    Object *o = dbpriv_dereference(obj);
    Verbdef *v;
    char *p;
    int num, i;
    static verb_handle h;
    db_verb_handle vh;

    if (!allow_numbers ||
            (num = strtol(vname, &p, 10),
             (isspace(*vname) || *p != '\0')))
        num = -1;

    for (i = 0, v = o->verbdefs; v; v = v->next, i++)
        if (i == num || verbcasecmp(v->name, vname))
            break;

    if (v) {
        h.definer = obj;
        h.verbdef = v;
        vh.ptr = &h;

        return vh;
    }
    vh.ptr = nullptr;

    return vh;
}

db_verb_handle
db_find_indexed_verb(Var obj, unsigned index)
{
    Object *o = dbpriv_dereference(obj);
    Verbdef *v;
    unsigned i;
    static verb_handle h;
    db_verb_handle vh;

    for (v = o->verbdefs, i = 0; v; v = v->next)
        if (++i == index) {
            h.definer = obj;
            h.verbdef = v;
            vh.ptr = &h;

            return vh;
        }

    vh.ptr = nullptr;

    return vh;
}

Var
db_verb_definer(db_verb_handle vh)
{
    verb_handle *h = (verb_handle *) vh.ptr;
    Var r = h ? h->definer : Var::new_obj(NOTHING);
    return r;
}

const char *
db_verb_names(db_verb_handle vh)
{
    verb_handle *h = (verb_handle*)vh.ptr;
    return h ? h->verbdef->name : "";
}

void
db_set_verb_names(db_verb_handle vh, const char *names)
{
    verb_handle *h = (verb_handle *) vh.ptr;

    db_priv_affected_callable_verb_lookup();

    if (!h) panic_moo("DB_SET_VERB_NAMES: Null verb_handle!");

    if (h->verbdef->name)
        free_str(h->verbdef->name);
    
    h->verbdef->name = names;    
}

Objid
db_verb_owner(db_verb_handle vh)
{
    verb_handle *h = (verb_handle *) vh.ptr;

    if (!h) panic_moo("DB_VERB_OWNER: Null verb_handle!");
        
    return h->verbdef->owner;
}

void
db_set_verb_owner(db_verb_handle vh, Objid owner)
{
    verb_handle *h = (verb_handle *) vh.ptr;

    if (!h) panic_moo("DB_SET_VERB_OWNER: Null verb_handle!");

    h->verbdef->owner = owner;
}

Var
db_verb_meta(db_verb_handle vh)
{
    verb_handle *h = (verb_handle *) vh.ptr;

    if (!h) panic_moo("DB_VERB_META: Null verb_handle!");

    if(h->verbdef->meta.type == TYPE_MAP) {
        return h->verbdef->meta;
    } else if(h->verbdef->meta.type == TYPE_LIST && h->verbdef->meta.length() > 0) {
        Var m = new_map(h->verbdef->meta.length());
        listforeach(h->verbdef->meta, [&m](Var value, int index) -> int {
            if(value.type != TYPE_LIST || value.length() <= 1)
                m = mapinsert(m, Var::new_int(index), var_ref(value));
            else if(value.length() == 2)
                m = mapinsert(m, var_ref(value[1]), var_ref(value[2]));
            else
                m = mapinsert(m, var_ref(value[1]), sublist(var_ref(value), 2, value.length()));
            return 0;
        });

        return m;
    }

    return new_map(0);
}

void
db_set_verb_meta(db_verb_handle vh, Var meta)
{
    verb_handle *h = (verb_handle *) vh.ptr;

    if (!h) return panic_moo("DB_SET_VERB_META: Null verb_handle!");

    free_var(h->verbdef->meta);

    switch(meta.type) {
        case TYPE_INT:
        case TYPE_OBJ:
        case TYPE_ERR:
        case TYPE_FLOAT:
        case TYPE_COMPLEX:
        case TYPE_MATRIX:
        case TYPE_CALL:
        case TYPE_STR:
        case TYPE_BOOL:
        case TYPE_ANON:
        case TYPE_WAIF:
        {
            h->verbdef->meta = enlist_var(var_ref(meta));
            break;
        }
        case TYPE_LIST:
        case TYPE_MAP:
        {
            Var m;
            if(meta.length() > 0) {
                m = var_ref(meta);
            } else {                
                m.type = TYPE_NONE;
            }
            h->verbdef->meta = m;
            break;
        }
        default:
        {
            Var m;
            m.type = TYPE_NONE;
            h->verbdef->meta = m;
            break;
        }
    }

    free_var(meta);
}

unsigned
db_verb_flags(db_verb_handle vh)
{
    verb_handle *h = (verb_handle *) vh.ptr;

    if (!h) panic_moo("DB_VERB_FLAGS: Null verb_handle!");

    return h->verbdef->perms & PERMMASK;
}

void
db_set_verb_flags(db_verb_handle vh, unsigned flags)
{
    verb_handle *h = (verb_handle *) vh.ptr;

    db_priv_affected_callable_verb_lookup();

    if (!h) panic_moo("DB_SET_VERB_FLAGS: Null verb_handle!");

    h->verbdef->perms &= ~PERMMASK;
    h->verbdef->perms |= flags;        
}

Program *
db_verb_program(db_verb_handle vh)
{
    verb_handle *h = (verb_handle *) vh.ptr;

    if (!h) panic_moo("DB_VERB_PROGRAM: Null verb_handle!");

    Program *p = h->verbdef->program;
    return p ? p : null_program();
}

void
db_set_verb_program(db_verb_handle vh, Program * program)
{
    verb_handle *h = (verb_handle *) vh.ptr;

    if (!h) panic_moo("DB_SET_VERB_PROGRAM: Null verb_handle!");

    if (h->verbdef->program)
        free_program(h->verbdef->program);

    h->verbdef->program = program;      
}

void
db_verb_arg_specs(db_verb_handle vh,
                  db_arg_spec * dobj, db_prep_spec * prep, db_arg_spec * iobj)
{
    verb_handle *h = (verb_handle *) vh.ptr;

    if (!h) panic_moo("DB_VERB_ARG_SPECS: Null verb_handle!");

    *dobj = (db_arg_spec)((h->verbdef->perms >> DOBJSHIFT) & OBJMASK);
    *prep = (db_prep_spec)h->verbdef->prep;
    *iobj = (db_arg_spec)((h->verbdef->perms >> IOBJSHIFT) & OBJMASK);        
}

void
db_set_verb_arg_specs(db_verb_handle vh,
                      db_arg_spec dobj, db_prep_spec prep, db_arg_spec iobj)
{
    verb_handle *h = (verb_handle *) vh.ptr;

    db_priv_affected_callable_verb_lookup();

    if (!h) panic_moo("DB_SET_VERB_ARG_SPECS: Null verb_handle!");

    h->verbdef->perms = ((h->verbdef->perms & PERMMASK)
                                 |  (dobj << DOBJSHIFT)
                                 | (iobj << IOBJSHIFT));
    h->verbdef->prep = prep;      
}

int
db_verb_allows(db_verb_handle h, Objid progr, db_verb_flag flag)
{
    return ((db_verb_flags(h) & flag)
            || progr == db_verb_owner(h)
            || is_wizard(progr));
}

void
db_clear_verb_cache(void)
{
#ifdef USE_VERB_CACHE /*Just in case */
    if(!verb_cache.v.map) return;
    free_var(verb_cache);
    verb_cache.v.map = nullptr;
#endif
}

