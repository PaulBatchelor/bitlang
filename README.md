# Bitlang

Bitlang is a tiny stack-based DSL written in ANSI-C,
designed for the sole purpose of producing procedurally
generated 1-bit art.

## Sample

In bitlang, this expression:

    x y + abs x y - abs 1 + ^ 2 << 3 % !

Can be used to generate the following image:

![1-bit art based on work by Foldster](foldster.png)

## Building

Bitlang is written in a literate style.
Before building, bitlang must be tangled into C code
using [worgle](git.sr.ht/~pbatch/worgle).

The example program can be built using the `build.sh` script.
Running this program will generate a PBM file, which can
be converted to a PNG file using imagemagick.

The files for bitlang (bitlang.c, bitlang.h) can be tangled
with:

    worgle -g -Werror bitlang.org

## Language Overview

Bitlang is a stack-based language, similar to RPN
calculators and programs like dc.

    2 3 +

Adds 2 and 3 together and pushes 5 onto the stack.

Special variables x, y, w, h, and t correspond to
XY position, width, height, and time (as a frame number).

The following operations are currently supported:

Basic arithmetic: +, -, *, /

Modulo: %

Left/right shift: <<, >>

Equals operation: =

Logical and Bitwise OR: ||, |

Bitwise AND: &

XOR: ^

Logical and Bitwise NOT: !, ~

Absolute Value: abs

## Using Bitlang

Bitlang tangles out into 2 files `bitlang.c` and
`bitlang.h`. These can be dropped.

For API usage, see [example.c](./example.c).