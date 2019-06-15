# Simon Berens | Jason Tung
## Team Parsival

### specifications
vary declarations and set frames must occur before calling them in future commands
ex invalid syntax:
```
light 0 0 blueval 250 250 0
vary blueval ( * ( sin ( * 3 x ) ) ( cos x ) 100)
```
ex valid syntax:
```
vary blueval ( * ( sin ( * 3 x ) ) ( cos x ) 100)
light 0 0 blueval 250 250 0
```


### modified features
#### Improved lexer

all features now are implemented into the lexer

#### Treating knobs as variables 

parameters for commands like move and light can be replaced with knobs/equations now

ex: `move x y z knob` -> `move knob knob knob`

`move x y z knob` -> `light rknob gknob bknob xknob yknob zknob`


you can move in x,y and z according to some equation with respect to the frames over which it is moving (either absolutely or relatively)

#### Set knob value from arbitrary equation and optional equation mode 

a knob is defined as an equation which will evaluate with x as the current frame (either relatively or absolutely, clarified in the next section)

knobs are now equations that support `+, -, * , /, ^, sin, cos, tan, arcsin, arccos, arctan` and constants like `e and pi`

addition and multiplication can take an arbitary number of arguments, but subtraction, division, and exponentiation only take 2 parameters. the trig functions, of course, only take 1 parameter in the form of an angle measured in radians.

every character must be separated by a space

ex: `vary knob_name ( * ( - ( ^ ( + 3 x e ) 9 ) x ) pi x 2 6 )`

#### Relative mode (default): value for knob equations is substituted relative to instantiation of an object

#### Absolute mode: value for knob equations is substituted relative to beginning of gif

### new features

#### Specify lifetime of objects 
ex: `push 0 140` 

give this layer from frames 0 to 140

#### Light

create lights of some rgb value at some location

ex: `light r g b x y z`

make light with rgb (r,g,b) at location (x,y,z)
