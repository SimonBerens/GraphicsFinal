# Simon Berens | Jason Tung
## Team we_watch_anime

### specifications
vary declarations and set frames must occur before calling the command

### modified features
#### Treating knobs as variables -- done

ex: `move x y z knob` -> `move knob knob knob`

`move x y z knob` -> `light rknob gknob bknob xknob yknob zknob`


you can move in x,y and z according to some equation with respect to the frames over which it is moving (either absolutely or relatively)

#### Set knob value from arbitrary equation and optional equation mode -- done

knobs are now equations that support +, -, *, /, ^, sin, cos, tan, arcsin, arccos, arctan

addition and multiplication can take an arbitary number of arguments, but subtraction, division, and exponentiation only take 2 parameters. the trig functions, of course, only take 1 parameter in the form of an angle measured in radians.

every character must be separated by a space

ex: ( - ( ^ ( + 3 x ) 9 ) x )

#### Relative mode (default): value for knob equations is substituted relative to instantiation of an object

#### Absolute mode: value for knob equations is substituted relative to beginning of gif

#### Replace push and pop with { and } respectively for clearer scripts

### new features

#### Specify lifetime of objects 
ex: `push 0 140` 

give this layer from frames 0 to 140

#### Light
ex: `light r g b x y z`

make light with rgb (r,g,b) at location (x,y,z)
