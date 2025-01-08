# 👻 GhostStunt

A fork of [ToastStunt](https://github.com/lisdude/toaststunt) featuring various enhancements and ill-advised schemes.

### This is experimental software. Caveat emptor!

### New Types
#### Verb Call Handles (TYPE_CALL)
```c
// A simple verb with aliases
@verb $some_thing:"paper rock scissors" this none this
@prog $some_thing:paper
$debug(verb);
.

// Store the calls in a list on $some_thing
@property $some_thing.moves
;$some_thing.moves = {$some_thing::paper, $some_thing::rock, $some_thing::scissors}
=> {$some_thing::paper, $some_thing::rock, $some_thing::scissors}

// Choose handle randomly and call it, five rounds
;;for i in [1..5] $some_thing.moves[random($)]:call(); endfor 

DEBUG => {"rock"} [caller=> $some_thing:rock, line 1]
DEBUG => {"scissors"} [caller=> $some_thing:scissors, line 1]
DEBUG => {"rock"} [caller=> $some_thing:rock, line 1]
DEBUG => {"paper"} [caller=> $some_thing:paper, line 1]
DEBUG => {"paper"} [caller=> $some_thing:paper, line 1]

// Simulate some work
@verb $some_thing:do_work this none this
@prog $some_thing:do_work
{value, callback} = args;
suspend(5);
value = value * 2;
callback:call(value);
.

// Callback verb
@verb $some_thing:cb this none this
@prog $some_thing:cb
$debug(@args);
.

// Test it out!
;$some_thing:do_work(5, $some_thing::cb);
DEBUG => {10} [caller=> $some_thing:cb, line 1]

```
#### Complex Numbers (TYPE_COMPLEX)
```c
;(3 + 4i) + (2 - 1i)
=> (5 + 3i)

;sqrt(-1.0)
=> 1i

;(2.5 + 0.5i) * 3
=> (7.5 + 1.5i)

;(4 + 3i) / (1 - 1i)
=> (0.5 + 3.5i)

;(5 + 1.5i) % 0
=> 5.22015325445528
```
#### New Built-in Functions
```c
  all_contents()    => Recursively get contents of an object
  corified_as()     => Get corified string, i.e. corified_as(#20) => "$string_utils";
  make()            => Fast allocation of lists, makes a list with x copies of y
  range()           => Builtin $list_utils:range(), make list from a range of integers
  shuffle()         => Shuffles a list
  map_args()        => Maps a verb call handle over a list
  intersection()    => Set intersection, fast version of $set_utils:intersection();
  implode()         => Opposite of explode(), joins a list into a string
  str_trim()        => Trim a string
  str_triml()       => Trim left side of a string 
  str_trimr()       => Trim right side of a string
  str_pad()         => Pad a string with characters
  str_padr()        => Pad right side of a string with characters
  str_padl()        => Pad left side of a string with characters
  str_uppercase()   => Convert string to uppercase
  str_lowercase()   => Convert string to lowercase
  str_escape()      => Escape newlines and special characters in string
  str_unescape()    => Unescape newlines and special characters in string
  time_fmt()        => Format strings from timestamps like std::put_time
  time_parse()      => Parse time strings back into timestamps
  parse_ordinal()   => Ordinal ref parsing: parse_ordinal("third jacket") => {3, "jacket"};
  complex_match()   => Advanced object matcher handling ordinal references
  tokenize()        => Process a list of words into tokens for keyword matching 
  tocomplex()       => Convert a value to a complex number
  log2()            => Base-2 logarithm function
  logn()            => Base-n logarithm function
  rand_splitmix64() => Splitmix64 random number generator
  verb_meta()       => Store/retrieve metadata for a verb
  xxhash()          => Fast hash function returning an INT
```
#### Deprecated Functions
```c
  reverse(x)  => x[$..^]
  parent()    => `parents()[1] ! E_RANGE => $nothing'
  chparent(x) => chparents({x})
 ```
 **Note:** The parser fixes these automatically when old DBs are upgraded.

## Build Instructions
### **Docker**
```bash

# Build from git repository
docker build -t ghoststunt https://github.com/StraylightRunMOO/GhostStunt.git

# Or build locally
git clone https://github.com/StraylightRunMOO/GhostStunt.git
cd GhostStunt && docker build -t ghoststunt .

# Run with default options
docker run --rm -dt -p 7777:7777/tcp localhost/ghoststunt:latest

# Run with additional options passed to the 'moo' command 
docker run --rm -dt --name ghoststunt-server -p 7777:7777/tcp \
  localhost/ghoststunt:latest
  -i /data/files \
  -x /data/exec \
  /data/db/ghostcore.db /data/db/ghostcore.out.db 7777

# Mount folder as a volume, map to port 8888 
mkdir -p data/db
docker run --rm -dt -v=$(pwd)/data/:/data/ -p 8888:7777/tcp localhost/ghoststunt:latest

```

Additional build instructions and information can be found in the original [ToastStunt documentation](https://github.com/StraylightRunMOO/GhostStunt/blob/main/docs/ToastStunt.md).

