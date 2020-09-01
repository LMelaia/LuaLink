# LuaLink
A simple hack that essentially makes binary executable Lua scripts possible.
Written as an easy alternative to bat files and lua to machine code compilation.

### Theory
The LuaLink program acts as a standalone Win32 executable that can be treated like any
other executable. When executed, LuaLink will attempt to find a specified Lua
Interpreter and Lua script to run a visible command window. The Interpreter/Script
is found using a combination of environment variables, working directory paths, 
and the executable name. 

The end result is an executable which can be renamed and placed anywhere, that
will act as an executable shortcut to a lua script.

## Personal / Test Project
This project was written a test and for personal use. Additionally, it was written in C++,
a language fairly unfamiliar to the developer. Extreme caution is advised!

## License
This project is released into the public domain, with no restrictions, limitations,
or guarantees of any kind. Use at your own risk.