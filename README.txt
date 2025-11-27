MICROTOOLS

Microtools is a collection of simple tools for the development of graphical desktop applications

- building guide: include glfw, glew, and stb_image in your project and set the variables in the top of the makefile to the correct paths regarding your project structure. Use make to build, that should properly put the .a file and the .h file in the install folder. To link with your project, add the install path to the include path (-I) and to the library path (-L), and make sure of listing the microtools lib (-lmicrotools) before listing the glfw and glew.
- example building guide: setup the variables in the top of the makefile just as done with the microtools build. Use make to build, then run the desired executable in the bin folder through the explorer (or you can cd into the folder and run through the terminal)
