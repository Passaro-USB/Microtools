ext = -I.. -I../dependencies/glfw-install/include -I../dependencies/glew-install/include -I../dependencies -L../dependencies/glfw-install/lib -L../dependencies/glew-install/lib -lglew32 -lglfw3 -lopengl32 -luser32 -lgdi32 -lkernel32  

.PHONY: run
run: glew32.dll
	rm -rf main.exe
	gcc -g  test.c ../*.c $(ext) -o main.exe
	./main.exe

.PHONY: debug
debug: glew32.dll
	rm -rf main.exe
	gcc -g test.c ../*.c $(ext) -o main.exe
	gdb main.exe

glew32.dll:
	cp ../dependencies/glew-install/bin/glew32.dll .
