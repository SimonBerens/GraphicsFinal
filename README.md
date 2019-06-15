# Simon Berens | Jason Tung
## Team Parsival

### Features
* Knobs are determined by equations
* Use of knobs as variables
* Light command
* Ambient command
* Specification of object lifetime

### Specifics
##### Vary:
The new syntax for vary is `vary <name> <equation>`

Equations are in the form `<value>` or `( <operation> <equation> )`

Values can be any of the following: `<double>`, `pi`, `e`, `x`, where x will be substituted for the relative frame
of the calling object (see the push command). 

Valid operations are: `+`, `-`, `*`, `/`, `^`, `sin`, `cos`, `tan`, `arcsin`, `arccos`, `arctan`

Note: Whitespace is important, parentheses, operations, and values must be separated by at least one space. In addition,
certain operations (`-`, `/`, `^`, `sin`, `cos`, `tan`, `arcsin`, `arccos`, `arctan`) are undefined for all but one
specific number of parameters.

Once a knob has been declared, it can be used in the place of a value 
(except for `frames`, and object lifetime specification). For example, the following is valid:
```
vary example ( ^ ( sin ( * pi 2 x ) ) 2 )
light example 0 255 example example 7
constants example example 0.5 example 0.999 0.1 0 example 1
move example example example
torus 0 0 example example 100
```
This also means that you can no longer write `move 500 500 500 <knob>`

Note: The declaration for the knob (and constant) must precede any of its uses.

##### Push:
The new syntax for push is `push <int start_frame> <int end_frame>`. This will draw all objects draw on or above this 
stack level. Inner objects may have a more specific range defined, but if their `start_frame` and `end_frame` are 
outside of their parent's then they will only be drawn in the intersection of the ranges. Equations will be evaluated
relative to `start_frame`, e.g in
```
vary example ( * x 2 )
push 10 20
box example 0 0 100 100 100
pop
```
at frame 10 example will be evaluated to be 0, at frame 11 to be 2, etc.
This is the case for each individual object, regardless if multiple utilize the same equation.

##### Minor Changes:
This version of MDL does not support line, save, or display. Save will be automatically performed if frames is not set
(to basename.png)

Rotate now takes radians instead of degrees for better integration with equations
##### Other:
This entire program is written in modern c++17! No raw pointers or arrays, so copy constructors, move constructors, 
and destructors are all trivial (compiler generated), so memory management is effectively automatic!

##### Things that Make Me Sad:
Due to parsing being hard without bison and our obsession with clean code, there is no option for default