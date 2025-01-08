/******************************************************************************
  Copyright 2013 Todd Sundsted. All rights reserved.

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

#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "storage.h"
#include "streams.h"
#include "structures.h"
#include "utils.h"

#include "system.h"

/**** built in functions ****/

static package
bf_getenv(Var arglist, Byte next, void *vdata, Objid progr)
{
    package pack;

    if (!is_wizard(progr)) {
        pack = make_error_pack(E_PERM);
    }
    else {
        const char *str;

        if ((str = getenv(arglist[1].v.str)) != nullptr) {
            pack = make_var_pack(str_dup_to_var(raw_bytes_to_binary(str, strlen(str))));
        } else {
            pack = no_var_pack();
        }
    }

    free_var(arglist);

    return pack;
}

void
register_system(void)
{
    register_function("getenv", 1, 1, bf_getenv, TYPE_STR);
}
