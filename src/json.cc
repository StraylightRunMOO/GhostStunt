/******************************************************************************
  Copyright 2010 Todd Sundsted. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY TODD SUNDSTED ``AS IS'' AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
  EVENT SHALL TODD SUNDSTED OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  The views and conclusions contained in the software and documentation are
  those of the authors and should not be interpreted as representing official
  policies, either expressed or implied, of Todd Sundsted.
 *****************************************************************************/

#include <stdio.h>
#include <errno.h>

#include <string.h>
#include <stdlib.h>

#include "functions.h"
#include "json.h"
#include "list.h"
#include "log.h"
#include "map.h"
#include "numbers.h"
#include "server.h"
#include "storage.h"
#include "streams.h"
#include "unparse.h"
#include "utils.h"
#include "dependencies/yajl/yajl_gen.h"
#include "dependencies/yajl/yajl_lex.h"
#include "dependencies/yajl/yajl_parse.h"

/*
  Handle many modes of mapping between JSON and internal MOO types.

  JSON defines types that MOO (currently) does not support, such as
  boolean true and false, and null.  MOO uses types that JSON does not
  support, such as object references and errors.

  Mode 0 -- Common Subset

  In Mode 0, only the common subset of types (strings and numbers) are
  translated with fidelity between MOO types and JSON types.  All
  other types are treated as alternative representations of the string
  type.  Furthermore, all MOO types used as keys (_both_ strings and
  numbers) are treated as strings.  Neither MOO lists nor maps may be
  used as keys in this mode.

  Mode 0 is useful for data transfer with non-MOO applications.

  Mode 1 -- Embedded Types

  In Mode 1, MOO types are encoded as strings, suffixed with type
  information.  Strings and numbers, which are valid JSON types, carry
  implicit type information, if type information is not specified.
  The boolean values and null are still interpreted as short-hand for
  explicit string notation.  Neither MOO lists nor maps may be used as
  keys in this mode.

  Mode 1 is useful for serializing/deserializing MOO types.
 */

struct stack_item {
    struct stack_item *prev;
    Var v;
};

static void
push(struct stack_item **top, Var v)
{
    struct stack_item *item = (struct stack_item *)mymalloc(sizeof(struct stack_item), M_STRUCT);
    item->prev = *top;
    item->v = v;
    *top = item;
}

static Var
pop(struct stack_item **top)
{
    struct stack_item *item = *top;
    *top = item->prev;
    Var v = item->v;
    myfree(item, M_STRUCT);
    return v;
}

#define PUSH(top, v) push(&(top), v)

#define POP(top) pop(&(top))

typedef enum {
    MODE_COMMON_SUBSET, MODE_EMBEDDED_TYPES, MODE_INFERRED_TYPES
} mode_type;

struct parse_context {
    struct stack_item stack;
    struct stack_item *top;
    mode_type mode;
    int depth;
};

struct generate_context {
    mode_type mode;
};

static const char *
value_to_literal(Var v)
{
    static Stream *s = nullptr;
    if (!s)
        s = new_stream(100);
    unparse_value(s, v);
    return reset_stream(s);
}

/* If type information is present, extract it and return the type. */
static var_type
valid_type(const char **val, size_t *len)
{
    /* format: "...|<TYPE>"
       where <TYPE> is a MOO type string: "obj", "int", "float", "err", "str" */
    if (*len > 3 && !strncmp(*val + *len - 4, "|obj", 4)) {
        *len = *len - 4;
        return TYPE_OBJ;
    } else if (*len > 3 && !strncmp(*val + *len - 4, "|int", 4)) {
        *len = *len - 4;
        return TYPE_INT;
    } else if (*len > 5 && !strncmp(*val + *len - 6, "|float", 6)) {
        *len = *len - 6;
        return TYPE_FLOAT;
    } else if (*len > 3 && !strncmp(*val + *len - 4, "|err", 4)) {
        *len = *len - 4;
        return TYPE_ERR;
    } else if (*len > 3 && !strncmp(*val + *len - 4, "|str", 4)) {
        *len = *len - 4;
        return TYPE_STR;
    } else
        return TYPE_NONE;
}

/* Append type information. */
static const char *
append_type(const char *str, var_type type)
{
    static Stream *stream = nullptr;
    if (nullptr == stream)
        stream = new_stream(20);
    stream_add_string(stream, str);
    switch (type) {
        case TYPE_OBJ:
            stream_add_string(stream, "|obj");
            break;
        case TYPE_INT:
            stream_add_string(stream, "|int");
            break;
        case TYPE_FLOAT:
            stream_add_string(stream, "|float");
            break;
        case TYPE_ERR:
            stream_add_string(stream, "|err");
            break;
        case TYPE_STR:
            stream_add_string(stream, "|str");
            break;
        default:
            panic_moo("Unsupported type in append_type()");
    }
    return reset_stream(stream);
}

static int
handle_null(void *ctx)
{
    struct parse_context *pctx = (struct parse_context *)ctx;
    Var v;
    v.type = TYPE_STR;
    v.str(str_dup("null"));
    PUSH(pctx->top, v);
    return 1;
}

static int
handle_boolean(void *ctx, int boolean)
{
    struct parse_context *pctx = (struct parse_context *)ctx;
    Var v;
    v.type = TYPE_BOOL;
    v.v.truth = boolean;
    PUSH(pctx->top, v);
    return 1;
}

static int
handle_number(void *ctx, const char *numberVal, unsigned int numberLen, yajl_tok tok)
{
    struct parse_context *pctx = (struct parse_context *)ctx;
    Var v;

    if (yajl_tok_integer == tok) {

        long int i = 0;

        errno = 0;
        i = strtol(numberVal, nullptr, 10);

        if (0 == errno && (i >= MININT && i <= MAXINT)) {
            v = Var::new_int(i);
            PUSH(pctx->top, v);
            return 1;
        }
    }

    double d = 0.0;

    errno = 0;
    d = strtod(numberVal, nullptr);

    if (0 == errno) {
        v.type = TYPE_FLOAT;
        v.v.fnum = d;
        PUSH(pctx->top, v);
        return 1;
    }

    return 0;
}

static inline Var infer_type(const char *str, size_t len) {
    if(len > 20) return str_dup_to_var(str);

    if(len > 1 && str[0] == '#') {
        auto i = 1;

        if(str[i] == '-' && len > 2) i++;

        while(i < len)
            if(str[i] < '0' || str[i] > '9')
                return str_dup_to_var(str);
            else
                i++;

        Var result;
        if(sscanf(str+1, "%lld", &(result.v.obj)) > 0)
            result.type = TYPE_OBJ;
        else
            result = str_dup_to_var(str);

        return result;
    } else if(len > 2 && str[0]=='E' && str[1]=='_') {
        int e = parse_error(str);

        if(e >= 0) {
            Var error;
            error.type = TYPE_ERR;
            error.v.err = (enum error)e;
            return error;
        }
        
        return str_dup_to_var(str);
    }

    for(auto i = 0; i < len; i++)
        if((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '-' && str[i] != 'e' && str[i] != '+')
            return str_dup_to_var(str);

    Var result;
    if(strchr(str, '.') && sscanf(str, "%lf", &(result.v.fnum)) > 0) {
        result.type = TYPE_FLOAT;
    } else if(sscanf(str, "%lld", &(result.v.num)) > 0) {
        result.type = TYPE_INT;
    } else {
        result = str_dup_to_var(str);
    }

    return result;
}

static int
handle_string(void *ctx, const unsigned char *stringVal, unsigned int stringLen)
{
    struct parse_context *pctx = (struct parse_context *)ctx;
    var_type type;
    Var v;

    size_t len = (size_t)stringLen;
    char *val = (char*)mymalloc(len + 1, M_STRUCT);
    strncpy(val, (const char*)stringVal, len);
    val[len] = '\0';

    if(MODE_INFERRED_TYPES == pctx->mode) {
        v = infer_type(val, len);
    } else if (MODE_EMBEDDED_TYPES == pctx->mode && TYPE_NONE != (type = valid_type((const char**)&val, &len))) {
        switch (type) {
            case TYPE_OBJ:
            {
                char *p;
                if (*val == '#')
                    val++;
                v.type = TYPE_OBJ;
                v.v.num = strtol(val, &p, 10);
                break;
            }
            case TYPE_INT:
            {
                char *p;
                v = Var::new_int(strtol(val, &p, 10));
                break;
            }
            case TYPE_FLOAT:
            {
                char *p;
                v.type = TYPE_FLOAT;
                v.v.fnum = strtod(val, &p);
                break;
            }
            case TYPE_ERR:
            {
                char temp[len + 1];
                strncpy(temp, val, len);
                temp[len] = '\0';
                v.type = TYPE_ERR;
                int err = parse_error(temp);
                v.v.err = err > -1 ? (error)err : E_NONE;
                break;
            }
            case TYPE_STR:
            {
                char temp[len + 1];
                strncpy(temp, val, len);
                temp[len] = '\0';
                v.type = TYPE_STR;
                v.str(str_dup(temp));
                break;
            }
            default:
                panic_moo("Unsupported type in handle_string()");
        }
    } else {
        char temp[len + 1];
        strncpy(temp, val, len);
        temp[len] = '\0';
        v.type = TYPE_STR;
        v.str(str_dup(temp));
        v = str_dup_to_var(val);
    }

    myfree(val, M_STRUCT);

    PUSH(pctx->top, v);
    return 1;
}

static int
handle_start_map(void *ctx)
{
    struct parse_context *pctx = (struct parse_context *)ctx;

    if (pctx->depth >= server_int_option("json_max_parse_depth", JSON_MAX_PARSE_DEPTH))
        return 0;

    Var k, v;
    k.type = TYPE_CLEAR;
    PUSH(pctx->top, k);
    v.type = TYPE_CLEAR;
    PUSH(pctx->top, v);
    pctx->depth++;
    return 1;
}

static int
handle_end_map(void *ctx)
{
    struct parse_context *pctx = (struct parse_context *)ctx;
    Var map = new_map(0);
    Var k, v;
    for (v = POP(pctx->top), k = POP(pctx->top);
            (int)v.type != TYPE_CLEAR && (int)k.type != TYPE_CLEAR;
            v = POP(pctx->top), k = POP(pctx->top)) {
        map = mapinsert(map, k, v);
    }
    PUSH(pctx->top, map);
    pctx->depth--;
    return 1;
}

static int
handle_start_array(void *ctx)
{
    struct parse_context *pctx = (struct parse_context *)ctx;

    if (pctx->depth >= server_int_option("json_max_parse_depth", JSON_MAX_PARSE_DEPTH))
        return 0;

    Var v;
    v.type = TYPE_CLEAR;
    PUSH(pctx->top, v);
    pctx->depth++;
    return 1;
}

static int
handle_end_array(void *ctx)
{
    struct parse_context *pctx = (struct parse_context *)ctx;
    Var list = new_list(0);
    Var v;
    for (v = POP(pctx->top); (int)v.type != TYPE_CLEAR;
            v = POP(pctx->top)) {
        list = listinsert(list, v, 1);
    }
    PUSH(pctx->top, list);
    pctx->depth--;
    return 1;
}

static yajl_gen_status
generate_key(yajl_gen g, Var v, void *ctx)
{
    struct generate_context *gctx = (struct generate_context *)ctx;

    switch (v.type) {
        case TYPE_OBJ:
        case TYPE_INT:
        case TYPE_FLOAT:
        case TYPE_ERR:
        {
            const char *tmp = value_to_literal(v);
            if (MODE_EMBEDDED_TYPES == gctx->mode)
                tmp = append_type(tmp, v.type);
            return yajl_gen_string(g, (const unsigned char *)tmp, strlen(tmp));
        }
        case TYPE_STR:
        {
            const char *tmp = v.str();
            size_t len = strlen(tmp);
            if (MODE_EMBEDDED_TYPES == gctx->mode)
                if (TYPE_NONE != valid_type(&tmp, &len))
                    tmp = append_type(tmp, v.type);
            return yajl_gen_string(g, (const unsigned char *)tmp, strlen(tmp));
        }
        case TYPE_ANON:
        case TYPE_WAIF:
            break;
        default:
            panic_moo("Unsupported type in generate_key()");
    }

    return yajl_gen_keys_must_be_strings;
}

static yajl_gen_status
generate(yajl_gen g, Var v, void *ctx);

struct do_closure {
    yajl_gen g;
    struct generate_context *gctx;
    yajl_gen_status status;
};

static yajl_gen_status
generate(yajl_gen g, Var v, void *ctx)
{
    struct generate_context *gctx = (struct generate_context *)ctx;

    switch (v.type) {
        case TYPE_INT:
            return yajl_gen_integer(g, v.v.num);
        case TYPE_FLOAT:
            return yajl_gen_double(g, v.v.fnum);
        case TYPE_OBJ:
        case TYPE_ERR:
        {
            const char *tmp = value_to_literal(v);
            if (MODE_EMBEDDED_TYPES == gctx->mode)
                tmp = append_type(tmp, v.type);
            return yajl_gen_string(g, (const unsigned char *)tmp, strlen(tmp));
        }
        case TYPE_STR:
        {
            const char *tmp = v.str();
            size_t len = strlen(tmp);
            if (MODE_EMBEDDED_TYPES == gctx->mode)
                if (TYPE_NONE != valid_type(&tmp, &len))
                    tmp = append_type(tmp, v.type);
            return yajl_gen_string(g, (const unsigned char *)tmp, strlen(tmp));
        }
        case TYPE_MAP:
        {
            struct do_closure dmc;
            dmc.g = g;
            dmc.gctx = gctx;
            dmc.status = yajl_gen_status_ok;
            yajl_gen_map_open(g);

            if (mapforeach(v, [&dmc](Var key, Var value, int index) -> int {
               dmc.status = generate_key(dmc.g, key, dmc.gctx);
               if (yajl_gen_status_ok != dmc.status)
                   return 1;

               dmc.status = generate(dmc.g, value, dmc.gctx);
               if (yajl_gen_status_ok != dmc.status)
                   return 1;

               return 0;
            })) return dmc.status;

            yajl_gen_map_close(g);
            return yajl_gen_status_ok;
        }
        case TYPE_LIST:
        {
            struct do_closure dmc;
            dmc.g = g;
            dmc.gctx = gctx;
            dmc.status = yajl_gen_status_ok;
            yajl_gen_array_open(g);

            if (listforeach(v, [&dmc](Var value, int index) -> int {
                dmc.status = generate(dmc.g, value, dmc.gctx);
                return (dmc.status == yajl_gen_status_ok) ? 0 : 1;
            })) return dmc.status;

            yajl_gen_array_close(g);
            return yajl_gen_status_ok;
        }
        case TYPE_BOOL:
        {
            return yajl_gen_bool(g, v.v.num);
        }
    }

    return (yajl_gen_status) - 1;
}

static yajl_callbacks callbacks = {
    handle_null,
    handle_boolean,
    nullptr,
    nullptr,
    handle_number,
    handle_string,
    handle_start_map,
    handle_string,
    handle_end_map,
    handle_start_array,
    handle_end_array
};

/**** built in functions ****/

static package
bf_parse_json(Var arglist, Byte next, void *vdata, Objid progr)
{
    yajl_handle hand;
    yajl_parser_config cfg = { 1, 1 };
    yajl_status stat;

    struct parse_context pctx;
    pctx.top = &pctx.stack;
    pctx.stack.v.type = TYPE_INT;
    pctx.stack.v.v.num = 0;
    pctx.mode = MODE_INFERRED_TYPES;
    pctx.depth = 0;

    const char *str = arglist[1].str();
    size_t len = strlen(str);

    package pack;

    int done = 0;

    if (arglist.length() >= 2) {
        if (!strcasecmp(arglist[2].str(), "inferred-types")) {
            pctx.mode = MODE_INFERRED_TYPES;
        } else if (!strcasecmp(arglist[2].str(), "common-subset")) {
            pctx.mode = MODE_COMMON_SUBSET;
        } else if (!strcasecmp(arglist[2].str(), "embedded-types")) {
            pctx.mode = MODE_EMBEDDED_TYPES;
        } else {
            free_var(arglist);
            return make_error_pack(E_INVARG);
        }
    }

    hand = yajl_alloc(&callbacks, &cfg, nullptr, (void *)&pctx);

    while (!done) {
        if (len == 0)
            done = 1;

        if (done)
            stat = yajl_parse_complete(hand);
        else
            stat = yajl_parse(hand, (const unsigned char *)str, len);

        len = 0;

        if (done) {
            if (stat != yajl_status_ok) {
                /* clean up the stack */
                while (pctx.top != &pctx.stack) {
                    Var v = POP(pctx.top);
                    free_var(v);
                }
                pack = make_error_pack(E_INVARG);
            } else {
                Var v = POP(pctx.top);
                pack = make_var_pack(v);
            }
        }
    }

    yajl_free(hand);

    free_var(arglist);
    return pack;
}

static package
bf_generate_json(Var arglist, Byte next, void *vdata, Objid progr)
{
    yajl_gen g;
    yajl_gen_config cfg = { 0, "" };

    struct generate_context gctx;
    gctx.mode = MODE_COMMON_SUBSET;

    const char *buf;
    unsigned int len;

    Var json;
    package pack;

    if (arglist.length() >= 2) {
        if (!strcasecmp(arglist[2].str(), "common-subset") || !strcasecmp(arglist[2].str(), "inferred-types")) {
            gctx.mode = MODE_COMMON_SUBSET;
        } else if (!strcasecmp(arglist[2].str(), "embedded-types")) {
            gctx.mode = MODE_EMBEDDED_TYPES;
        } else {
            free_var(arglist);
            return make_error_pack(E_INVARG);
        }
    }

    g = yajl_gen_alloc(&cfg, nullptr);

    if (yajl_gen_status_ok == generate(g, arglist[1], &gctx)) {
        yajl_gen_get_buf(g, (const unsigned char **)&buf, &len);

        json.type = TYPE_STR;
        json.str(str_dup(buf));

        pack = make_var_pack(json);
    } else {
        pack = make_error_pack(E_INVARG);
    }

    yajl_gen_clear(g);
    yajl_gen_free(g);

    free_var(arglist);
    return pack;
}

void
register_yajl(void)
{
    register_function("parse_json", 1, 2, bf_parse_json, TYPE_STR, TYPE_STR);
    register_function("generate_json", 1, 2, bf_generate_json, TYPE_ANY, TYPE_STR);
}
