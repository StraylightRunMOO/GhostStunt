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

#ifndef Options_h
#define Options_h 1

/******************************************************************************
 * The server is prepared to keep a log of every command entered by players
 * since the last checkpoint.  The log is flushed whenever a checkpoint is
 * successfully begun and is dumped into the server log when and if the server
 * panics.  Define LOG_COMMANDS to enable this logging.
 */

/* #define LOG_COMMANDS */

/******************************************************************************
 * When enabled, this option will cause .program and set_verb_code to write
 * a line in the log file indicating who changed which verb.
 */

#define LOG_CODE_CHANGES

/******************************************************************************
 * When enabled, this option will cause the eval() builtin to write
 * input to the log file.
 */

/* #define LOG_EVALS */

/******************************************************************************
 * Define ENABLE_GC to enable automatic garbage collection of cyclic data
 * structures.  This is safe but adds overhead to the reference counting
 * mechanism -- currently about a 5% penalty, even for operations that do not
 * involve anonymous objects.  If disabled the server may leak memory
 * because it will lose track of cyclic data structures.
 */

/* #define ENABLE_GC */

/* #define GC_ROOTS_LIMIT 2000 */

/******************************************************************************
 * Enable/disable the custom allocator and set some basic options below.
 * More advanced options including stats / fine-grained performance tuning
 * can be found in /src/dependencies/rpmalloc.c
*/

/*
#define USE_RPMALLOC

#ifdef USE_RPMALLOC
  #define ALLOC_PAGE_SIZE   0
  #define ALLOC_ENABLE_HUGE 1
#endif 
*/

/******************************************************************************
 * Define LOG_GC_STATS to enabled logging of reference cycle collection
 * stats and debugging information while the server is running.
 */

/* #define LOG_GC_STATS */

/******************************************************************************
 * The server normally forks a separate process to make database checkpoints;
 * the original process continues to service user commands as usual while the
 * new process writes out the contents of its copy of memory to a disk file.
 * This checkpointing process can take quite a while, depending on how big your
 * database is, so it's usually quite convenient that the server can continue
 * to be responsive while this is taking place.  On some systems, however,
 * there may not be enough memory to support two simultaneously running server
 * processes.  Define UNFORKED_CHECKPOINTS to disable server forking for
 * checkpoints.
 */

/* #define UNFORKED_CHECKPOINTS */

/******************************************************************************
 * If OUT_OF_BAND_PREFIX is defined as a non-empty string, then any lines of
 * input from any player that begin with that prefix will bypass both normal
 * command parsing and any pending read()ing task, instead spawning a server
 * task invoking #0:do_out_of_band_command(word-list).  This is intended for
 * use by fancy clients that need to send reliably-understood messages to the
 * server.
 */

#define OUT_OF_BAND_PREFIX "#$#"

/******************************************************************************
 * If OUT_OF_BAND_QUOTE_PREFIX is defined as a non-empty string, then any
 * lines of input from any player that begin with that prefix will be
 * stripped of that prefix and processed normally (whether to be parsed a
 * command or given to a pending read()ing task), even if the resulting line
 * begins with OUT_OF_BAND_PREFIX.  This provides a means of quoting lines
 * that would otherwise spawn #0:do_out_of_band_command tasks
 */

#define OUT_OF_BAND_QUOTE_PREFIX "#$\""

/******************************************************************************
 * The following constants define the execution limits placed on all MOO tasks.
 *
 * DEFAULT_MAX_STACK_DEPTH is the default maximum depth allowed for the MOO
 *	verb-call stack, the maximum number of dynamically-nested calls at any
 *	given time.  If defined in the database and larger than this default,
 *	$server_options.max_stack_depth overrides this default.
 * DEFAULT_FG_TICKS and DEFAULT_BG_TICKS are the default maximum numbers of
 *	`ticks' (basic operations) any task is allowed to use without
 *	suspending.  If defined in the database, $server_options.fg_ticks and
 *	$server_options.bg_ticks override these defaults.
 * DEFAULT_FG_SECONDS and DEFAULT_BG_SECONDS are the default maximum numbers of
 *	real-time seconds any task is allowed to use without suspending.  If
 *	defined in the database, $server_options.fg_seconds and
 *	$server_options.bg_seconds override these defaults.
 *
 * The *FG* constants are used only for `foreground' tasks (those started by
 * either player input or the server's initiative and that have never
 * suspended); the *BG* constants are used only for `background' tasks (forked
 * tasks and those of any kind that have suspended).
 *
 * The values given below are documented in the LambdaMOO Programmer's Manual,
 * so they should either be left as they are or else the manual should be
 * updated.
 */

#define DEFAULT_MAX_STACK_DEPTH	 50

#define DEFAULT_FG_TICKS         60000
#define DEFAULT_BG_TICKS         30000

#define DEFAULT_FG_SECONDS       5
#define DEFAULT_BG_SECONDS       3

#define DEFAULT_LAG_THRESHOLD    5.0

/******************************************************************************
 * DEFAULT_PORT is the TCP port number on which the server listenes when no
 * port argument is given on the command line.
 */

#define DEFAULT_PORT 		7777

/******************************************************************************
 * MP_SELECT	 The server will assume that the select() system call exists.
 * MP_POLL	    The server will assume that the poll() system call exists.
 *
 * Usually, it works best to leave MPLEX_STYLE undefined and let the code at
 * the bottom of this file pick the right value.
 */

/* #define MPLEX_STYLE MP_POLL */

/******************************************************************************
 * The built-in MOO function open_network_connection(), when enabled,
 * allows (only) wizard-owned MOO code to make outbound network connections
 * from the server.  When disabled, it raises E_PERM whenever called.
 *
 * The --outbound (-o) and --no-outbound (-O) command line options can explicitly
 * enable and disable this function.  If neither option is supplied, the definition
 * given to OUTBOUND_NETWORK here determines the default behavior.
 * (use 0 to disable by default, 1 or blank to enable by default).
 *
 * If OUTBOUND_NETWORK is not defined at all,
 * open_network_connection() is permanently disabled and +O is ignored.
 *
 * *** THINK VERY HARD BEFORE ENABLING THIS FUNCTION ***
 * In some contexts, this could represent a serious breach of security.
 */

/* disable by default, --outbound (or -o) enables: */
/* #define OUTBOUND_NETWORK 0 */

/* enable by default, --no-outbound (or -O) disables: */
#define OUTBOUND_NETWORK 1

/******************************************************************************
 * These options control the default values for the TCP keep-alive connection option.
 * When set_connection_option(<con>, "keep-alive", <value>) is given a true <value>,
 * these defaults will be used. When given a MAP of options for <value>, the defaults
 * will be used for values that don't appear in the option MAP. These options are:
 * 
 * KEEP_ALIVE_DEFAULT:   Whether keep-alive is enabled for all new connections by default.
 * KEEP_ALIVE_IDLE:      The time (in seconds) before starting keep-alive probes.
 * KEEP_ALIVE_INTERVAL:: The time (in seconds) between keep-alive probes.
 * KEEP_ALIVE_COUNT:     The number of failed keep-alive probes before disconnecting.
 */
#define KEEP_ALIVE_DEFAULT    true
#define KEEP_ALIVE_IDLE       300
#define KEEP_ALIVE_INTERVAL   120
#define KEEP_ALIVE_COUNT      5

/******************************************************************************
 * The server supports secure TLS connections using the OpenSSL library.
 * If USE_TLS is defined, you will be able to listen() for TLS connections
 * and connect to TLS servers using open_network_connection().
 *
 * If VERIFY_TLS_PEERS is defined, the peer certificate must be signed with a CA
 * or the connection will fail.
 *
 * If USE_TLS is not defined at all, TLS options are disabled and
 * the --tls-port (-t) flag is ignored.
 *
 * If LOG_TLS_CONNECTIONS is defined, each connection will be accompanied by
 * a TLS negotiation message which includes the ciphersuite. The ciphersuite is
 * also available from the connection_info() built-in function, which will be
 * unaffected by this option.
 *
 * DEFAULT_TLS_CERT can be overridden with the command-line option --tls-cert (-r)
 * DEFAULT_TLS_KEY can be overridden with the command-line option --tls-key (-k)
 */

#define USE_TLS
#define VERIFY_TLS_PEERS
#define DEFAULT_TLS_CERT    "/etc/letsencrypt/live/fullchain.pem"
#define DEFAULT_TLS_KEY     "/etc/letsencrypt/live/privkey.pem"
#define LOG_TLS_CONNECTIONS

/******************************************************************************
 * The following constants define certain aspects of the server's network
 * behavior.
 *
 * MAX_QUEUED_OUTPUT is the maximum number of output characters the server is
 *		     willing to buffer for any given network connection before
 *		     discarding old output to make way for new.  The server
 *		     only discards output after attempting to send as much as
 *		     possible on the connection without blocking.
 * MAX_QUEUED_INPUT is the maximum number of input characters the server is
 *		    willing to buffer from any given network connection before
 *		    it stops reading from the connection at all.  The server
 *		    starts reading from the connection again once most of the
 *		    buffered input is consumed.
 * MAX_LINE_BYTES is the maximum amount of bytes that a line sent to the server
 *          can consist of prior to the connection unceremoniously being closed,
 *		    to prevent memory allocation panics.
 * DEFAULT_CONNECT_TIMEOUT is the default number of seconds an un-logged-in
 *			   connection is allowed to remain idle without being
 *			   forcibly closed by the server; this can be
 *			   overridden by defining the `connect_timeout'
 *			   property on $server_options or on L, for connections
 *			   accepted by a given listener L.
 *
 * If defined in the database, $server_options.max_queued_output will override
 * the default value specified here.
 */

#define MAX_QUEUED_OUTPUT         65536
#define MAX_QUEUED_INPUT          MAX_QUEUED_OUTPUT
#define MAX_LINE_BYTES            5242880
#define DEFAULT_CONNECT_TIMEOUT   300

/* In order to avoid weirdness from these limits being set too small,
 * we impose the following (arbitrary) respective minimum values.
 * That is, a positive value for $server_options.max_queued_output that
 * is less than MIN_MAX_QUEUED_OUTPUT will be silently increased, and
 * likewise for the other options.
 */

#define MIN_MAX_QUEUED_OUTPUT         2048

/******************************************************************************
 * On connections that have not been set to binary mode, the server normally
 * discards incoming characters that are not printable ASCII, including
 * backspace (8) and delete(127).  If INPUT_APPLY_BACKSPACE is defined,
 * backspace and delete cause the preceding character (if any) to be removed
 * from the input stream.  (Comment this out to restore pre-1.8.3 behavior)
 */
#define INPUT_APPLY_BACKSPACE

/******************************************************************************
 * The server maintains a cache of the most recently used patterns from calls
 * to the match(), rmatch(), and pcre_match() built-in functions.
 * PATTERN_CACHE_SIZE controls how many past patterns are remembered by the
 * server for the former and PCRE_PATTERN_CACHE_SIZE for the latter.
 * Do not set either value to a number less than 1.
 */

#define PATTERN_CACHE_SIZE      20
#define PCRE_PATTERN_CACHE_SIZE 20

/******************************************************************************
 * Prior to 1.8.4 property lookups were required on every reference to a
 * built-in property due to the possibility of that property being protected.
 * This used to be expensive.  IGNORE_PROP_PROTECTED thus existed to entirely
 * disable the use of $server_options.protect_<property> for those who
 * did not actually make use of protected builtin properties.  Since all
 * protect_<property> options are now cached, this #define is now deprecated.
 ******************************************************************************
 */

/* #define IGNORE_PROP_PROTECTED */

/******************************************************************************
 * The code generator can now recognize situations where the code will not
 * refer to the value of a variable again and generate opcodes that will
 * keep the interpreter from holding references to the value in the runtime
 * environment variable slot.  Before, when doing something like x=f(x), the
 * interpreter was guaranteed to have a reference to the value of x while f()
 * was running, meaning that f() always had to copy x to modify it.  With
 * BYTECODE_REDUCE_REF enabled, f() could be called with the last reference
 * to the value of x.  So for example, x={@x,y} can (if there are no other
 * references to the value of x in variables or properties) just append to
 * x rather than make a copy and append to that.  If it *does* have to copy,
 * the next time (if it's in a loop) it will have the only reference to the
 * copy and then it can take advantage.
 *
 * NOTE WELL    NOTE WELL    NOTE WELL    NOTE WELL    NOTE WELL
 *
 * This option affects the length of certain bytecode sequences.
 * Suspended tasks in a database from a server built with this option
 * are not guaranteed to work with a server built without this option,
 * and vice versa.  It is safe to flip this switch only if there are
 * no suspended tasks in the database you are loading.  (It might work
 * anyway, but hey, it's your database.)  This restriction will be
 * lifted in a future version of the server software.  Consider this
 * option as being BETA QUALITY until then.
 *
 * NOTE WELL    NOTE WELL    NOTE WELL    NOTE WELL    NOTE WELL
 *
 ******************************************************************************
 */

#define BYTECODE_REDUCE_REF /* */

/******************************************************************************
 * The server can merge duplicate strings on load to conserve memory.  This
 * involves a rather expensive step at startup to dispose of the table used
 * to find the duplicates.  This should be improved eventually, but you may
 * want to trade off faster startup time for increased memory usage.
 *
 * You might want to turn this off if you see a large delay before the
 * INTERN: lines in the log at startup.
 ******************************************************************************
 */

#define STRING_INTERNING /* */

/******************************************************************************
 * For size operations, store the data with the type rather than recomputing.
 * String:     Store the length of the string.
 * List / Map: Store the number of bytes of storage used.
 ******************************************************************************
*/

#define MEMO_SIZE

/******************************************************************************
 * DEFAULT_MAX_STRING_CONCAT,      if set to a positive value, is the length
 *                                 of the largest constructible string.
 * DEFAULT_MAX_LIST_VALUE_BYTES,   if set to a positive value, is the number of
 *                                 bytes in the largest constructible list.
 * DEFAULT_MAX_MAP_VALUE_BYTES,    if set to a positive value, is the number of
 *                                 bytes in the largest constructible map.
 * Limits on "constructible" values apply to values built by concatenation,
 * splicing, index/subrange assignment and various builtin functions.
 *
 * If defined in the database, $server_options.max_string_concat,
 * $server_options.max_list_value_bytes and $server_options.max_map_value_bytes
 * override these defaults.  A zero value disables limit checking.
 *
 * $server_options.max_concat_catchable, if defined, causes an E_QUOTA error
 * to be raised when an overly-large value is spotted.  Otherwise, the task
 * is aborted as if it ran out of seconds (see DEFAULT_FG_SECONDS),
 * which was the original behavior in this situation (i.e., if we
 * were lucky enough to avoid a server memory allocation panic).
 ******************************************************************************
 */

#define DEFAULT_MAX_STRING_CONCAT    64537861
#define DEFAULT_MAX_LIST_VALUE_BYTES 64537861
#define DEFAULT_MAX_MAP_VALUE_BYTES  64537861

/* In order to avoid weirdness from these limits being set too small,
 * we impose the following (arbitrary) respective minimum values.
 * That is, a positive value for $server_options.max_string_concat that
 * is less than MIN_STRING_CONCAT_LIMIT will be silently increased, and
 * likewise for the list and map limits.
 */

#define MIN_STRING_CONCAT_LIMIT    1021
#define MIN_LIST_VALUE_BYTES_LIMIT 1021
#define MIN_MAP_VALUE_BYTES_LIMIT  1021

/******************************************************************************
 * In the original LambdaMOO server, last chance command processing
 * occured in the `huh' verb defined on the player's location.  The
 * following option changes that behavior so that it occurs in the
 * `huh' verb defined on the player.  If you are running a legacy core
 * and are concerned about incompatibility, you can disable this
 * feature.
 ******************************************************************************
 */

#define PLAYER_HUH 1

/******************************************************************************
 * Configurable options for the Exec subsystem.  EXEC_SUBDIR is the
 * directory inside the working directory in which all executable
 * files must reside. This can be overridden with the command-line option
 * --exec-dir (-x)
 ******************************************************************************
 */

#define EXEC_SUBDIR "executables/"
#define EXEC_MAX_PROCESSES 256

/******************************************************************************
 * Configurable options for the FileIO subsystem.  FILE_SUBDIR is the
 * directory inside the working directory in which all files must
 * reside. This can be overridden with the command-line option --file-dir (-i)
 *
 * FILE_IO_MAX_FILES indicated the maximum number of files that can be open at
 * once. It can be overridden in-database by adding
 * the $server_options.file_io_max_files property and calling load_server_options()
 ******************************************************************************
 */

#define FILE_SUBDIR "files/"
#define FILE_IO_BUFFER_LENGTH 4096
#define FILE_IO_MAX_FILES     256

/******************************************************************************
 * Enable log output colorization.
 ******************************************************************************
 */

#define COLOR_LOGS 1

/******************************************************************************
 * Turn on WAIF_DICT for Jay Carlson's patch that makes waif[x]=y and waif[x]
 * work by calling verbs on the waif.
 * Verbs:
 *   :_index(KEY) => Returns the value at KEY index.
 *   :_set_index(KEY, VALUE) => Sets the KEY to VALUE.
 ******************************************************************************
*/
#define WAIF_DICT

/******************************************************************************
 * Enable the obsolete in-server ownership quota management.
 ******************************************************************************
*/
/* #define OWNERSHIP_QUOTA */

/******************************************************************************
 * Cache ancestor lists until a parent changes. This is a quick and dirty solution
 * that will allow loops performing property lookups to happen much faster. Its
 * use in other applications is minimal, as the cache is invalidated often.
 ******************************************************************************
*/

#define USE_ANCESTOR_CACHE

/******************************************************************************
 * Typically, in text mode, FIO will drop any character that doesn't have a
 * graphical representation. This process, however, can be quite slow.
 * If you're positive that your text files are all safe, you can disable this
 * option to speed up text file reads at the expense of theoretical safety.
 * Newlines are still stripped from the ends of lines.
 * NOTE: Files containing a mixture of text and binary data will NOT have the
 *       binary data stripped in text mode. This breaks a fundamental assumption
 *       about files in text mode and, as such, unit tests will fail. Only enable
 *       this option if you're sure you don't mind this.
 ******************************************************************************
*/
/* #define UNSAFE_FIO */

/******************************************************************************
 * When functions leave the interpreter, the server can store certain information
 * about that function's execution and total time spent. This can be useful for
 * debugging server-locking functions at the expense of time spent storing data.
 * (The number defined is how many tasks will get saved.)
 ******************************************************************************
*/
/* #define SAVE_FINISHED_TASKS 15 */

/******************************************************************************
 * For debugging tracebacks, it is possible to capture the variables for the
 * environment prior to passing them to `handle_uncaught_error',
 * `handle_task_timeout', and `handle_lagging_task' inside the database.
 * Be aware that this includes all runtime environment variables including
 * pre-set constants, and the information returned can be very extensive
 * (and possibly memory intensive).
 ******************************************************************************
*/
/* #define INCLUDE_RT_VARS */

/******************************************************************************
 * The server supports 64-bit integers. If you don't want the added memory usage
 * and don't need the larger integers, you can disable that here. NOTE: Disabling
 * this option and loading a database that has saved 64-bit integers will probably
 * not end well.
 ******************************************************************************
*/
/* #define ONLY_32_BITS */

/******************************************************************************
 * The Argon2 functions can be threaded for performance reasons, but there is a
 * significant caveat to be aware of: do_login_command cannot be suspended.
 * Since threading implicitly suspends the MOO task, you won't be able to directly
 * use Argon2 in do_login_command. Instead, you'll have to devise a new solution
 * for logins that doesn't directly involve calling Argon2 in do_login_command.
 ******************************************************************************
*/
/* #define THREAD_ARGON2 */

/******************************************************************************
 * This option controls whether or not the server will fix object ownership
 * when calling recycle().
 * When enabled, any object, property, or verb owned by the recycled object
 * will be chowned to $nothing. This incurs a slight speed reduction compared
 * to traditional recycling. If you carefully manage ownership in your database,
 * you can disable this option for a speed boost in larger databases.
 ******************************************************************************
*/
#define SAFE_RECYCLE

/******************************************************************************
 * Configurable options for the background subsystem.
 * TOTAL_BACKGROUND_THREADS is the total number of pthreads that will be created
 * at runtime to process background MOO tasks.
 * DEFAULT_THREAD_MODE dictates the default behavior of threaded MOO functions
 * without a call to set_thread_mode. When set to true, the default behavior is
 * to thread these functions, requiring a call to set_thread_mode(0) to disable.
 * When false, the default behavior is unthreaded and requires a call to
 * set_thread_mode(1) to enable threading for the functions in that verb.
 ******************************************************************************
 */

#define TOTAL_BACKGROUND_THREADS    0
#define DEFAULT_THREAD_MODE         false

/******************************************************************************
 * By default, the server will resolve DNS hostnames from IP addresses for all
 * connections. If you intend to use in-database threaded DNS lookups, or just
 * always want numeric IP addresses, you can disable name lookups here.
 * NOTE: This option can be controlled in the database by setting the property
 *       $server_options.no_name_lookup to 0 or 1. Because of this, you should
 *       not comment out this define. Instead, set it to 1 or 0. If no option
 *       is specified in-DB, it will fall back to the value defined here.
 ******************************************************************************
 */

#define NO_NAME_LOOKUP 0

/******************************************************************************
 * This constant controls the maximum recursive depth that parse_json will
 * allow before giving up. A value too large has the potential to crash the
 * server, so caution is advised.
*/

#define JSON_MAX_PARSE_DEPTH 1000

/******************************************************************************
 * The default maximum number of seconds a curl() transfer can last.
*/

#define CURL_TIMEOUT 60

/******************************************************************************
 * The default maximum number of seconds a curl() transfer can last.
*/

#define MAP_SAVE_INSERTION_ORDER_DEFAULT 1
#define MAP_HASH_SEED0 1099511627776
#define MAP_HASH_SEED1 549755813881
#define MAP_HASH_FUNCTION hashmap_sip

/*****************************************************************************
 ********** You shouldn't need to change anything below this point. **********
 *****************************************************************************/

#ifndef MAP_SAVE_INSERTION_ORDER_DEFAULT
#define MAP_SAVE_INSERTION_ORDER_DEFAULT 0
#endif

#ifndef PLAYER_HUH
#define PLAYER_HUH 0
#endif

#ifndef NETWORK_PROTOCOL
#define NETWORK_PROTOCOL NP_TCP
#endif

#ifndef NETWORK_STYLE
#define NETWORK_STYLE NS_BSD
#endif

#ifndef OUT_OF_BAND_PREFIX
#define OUT_OF_BAND_PREFIX ""
#endif
#ifndef OUT_OF_BAND_QUOTE_PREFIX
#define OUT_OF_BAND_QUOTE_PREFIX ""
#endif

#if DEFAULT_MAX_STRING_CONCAT < MIN_STRING_CONCAT_LIMIT
#error DEFAULT_MAX_STRING_CONCAT < MIN_STRING_CONCAT_LIMIT ??
#endif
#if DEFAULT_MAX_LIST_VALUE_BYTES < MIN_LIST_VALUE_BYTES_LIMIT
#error DEFAULT_MAX_LIST_VALUE_BYTES < MIN_LIST_VALUE_BYTES_LIMIT ??
#endif
#if DEFAULT_MAX_MAP_VALUE_BYTES < MIN_MAP_VALUE_BYTES_LIMIT
#error DEFAULT_MAX_MAP_VALUE_BYTES < MIN_MAP_VALUE_BYTES_LIMIT ??
#endif

#if PATTERN_CACHE_SIZE < 1
#  error Illegal match() pattern cache size!
#endif

#define NP_TCP		1

#define NS_BSD		1

#define MP_SELECT	1
#define MP_POLL		2

#include "config.h"

#ifndef USE_TLS
 #define USE_TLS_BOOL
 #define TLS_PORT_TYPE
 #define TLS_CERT_PATH
 #define TLS_CERT_PATH_DEF
 #define USE_TLS_BOOL_DEF
 #define SSL_CONTEXT_1_ARG
 #define SSL_CONTEXT_1_DEF
 #define SSL_CONTEXT_2_ARG
 #define SSL_CONTEXT_2_DEF
#else
/* With TLS being optional and needing to pass around so many arguments,
 * I decided this would look nicer than splitting everything up
 * with ifdefs everywhere... */
 #define USE_TLS_BOOL , use_tls
 #define TLS_PORT_TYPE , port_type
 #define USE_TLS_BOOL_DEF , bool use_tls
 #define TLS_CERT_PATH , certificate_path, key_path
 #define TLS_CERT_PATH_DEF , const char *certificate_path, const char *key_path
 #define SSL_CONTEXT_2_ARG , &tls
 #define SSL_CONTEXT_2_DEF , SSL **tls
 #define SSL_CONTEXT_1_ARG , tls
 #define SSL_CONTEXT_1_DEF , SSL *tls
#endif

#if !defined(MPLEX_STYLE)
#  if NETWORK_STYLE == NS_BSD
#    if HAVE_POLL
#       define MPLEX_STYLE MP_POLL
#    elif HAVE_SELECT
#      define MPLEX_STYLE MP_SELECT
#    else
#      #error Could not find select() or poll()!
#    endif
#   endif
#endif

/* make sure OUTBOUND_NETWORK has a value;
   for backward compatibility, use 1 if none given */
#if defined(OUTBOUND_NETWORK) && (( 0 * OUTBOUND_NETWORK - 1 ) == 0)
#undef OUTBOUND_NETWORK
#define OUTBOUND_NETWORK 1
#endif


#if NETWORK_PROTOCOL != NP_TCP
#  error Illegal value for "NETWORK_PROTOCOL"
#endif

#if NETWORK_STYLE != NS_BSD
#  error Illegal value for "NETWORK_STYLE"
#endif

#if defined(MPLEX_STYLE) 	\
    && MPLEX_STYLE != MP_SELECT \
    && MPLEX_STYLE != MP_POLL
#  error Illegal value for "MPLEX_STYLE"
#endif

#endif				/* !Options_h */
