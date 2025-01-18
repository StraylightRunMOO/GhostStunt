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

#include "bf_register.h"
#include "collection.h"
#include "functions.h"
#include "list.h"
#include "map.h"
#include "utils.h"

int
ismember(Var lhs, Var rhs, int case_matters)
{
    if (rhs.type == TYPE_LIST) {
        return listforeach(rhs, [&lhs, &case_matters](Var value, int index) -> int {
            return equality(value, lhs, case_matters) ? index : 0;
        });
    } else if (rhs.type == TYPE_MAP) {
        return mapforeach(rhs, [&lhs, &case_matters](Var key, Var value, int index) -> int {
            return equality(value, lhs, case_matters) ? index : 0;
        });
    }

    return 0;
}

/**** built in functions ****/

static package
bf_is_member(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var r;
    Var rhs = arglist[2];

    if (rhs.type != TYPE_LIST && rhs.type != TYPE_MAP) {
        free_var(arglist);
        return make_error_pack(E_INVARG);
    }

    bool case_matters = arglist.length() < 3 || (arglist.length() >= 3 && is_true(arglist[3]));

    r.type = TYPE_INT;
    r.v.num = ismember(arglist[1], rhs, case_matters);
    free_var(arglist);
    return make_var_pack(r);
}

void
register_collection(void)
{
    register_function("is_member", 2, 3, bf_is_member, TYPE_ANY, TYPE_ANY, TYPE_INT);
}
