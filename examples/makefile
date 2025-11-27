MICROTOOLS_INSTALL_PATH = ../install
SHARED_BIN_EXT = dll
GLEW_BIN_PATH = dependencies/glew-install/bin
GLEW_LIB_PATH = dependencies/glew-install/lib
GLEW_INCLUDE_PATH = dependencies/glew-install/include
GLFW_LIB_PATH = dependencies/glfw-install/lib
GLFW_INCLUDE_PATH = dependencies/glfw-install/include
STB_IMAGE_PATH = dependencies

CC = gcc
ROOT = $(CURDIR)
BUILD = $(ROOT)/build
BIN = $(ROOT)/bin

SRC = $(wildcard $(ROOT)/*.c)
OUT = $(SRC:$(ROOT)/%.c=$(BUILD)/%.o)
EXE = $(SRC:$(ROOT)/%.c=$(BIN)/%.exe)

.PHONY: build
build: $(BIN) $(BUILD) $(BIN)/glew32.$(SHARED_BIN_EXT) $(EXE) 

.PHONY: halfbuild
halfbuild: $(BUILD) $(OUT) 

$(BIN)/%.exe: $(BUILD)/%.o
	gcc -g $^ -o $@ \
		-L$(GLEW_LIB_PATH) -L$(GLFW_LIB_PATH) -L$(MICROTOOLS_INSTALL_PATH) \
		-lmicrotools -lglew32 -lglfw3 -lopengl32 -lgdi32 -luser32 -lwinmm -lshell32
		

$(BUILD)/%.o: $(ROOT)/%.c
	gcc -g -I$(MICROTOOLS_INSTALL_PATH) -I$(GLEW_INCLUDE_PATH) \
		-I$(GLFW_INCLUDE_PATH) -I$(STB_IMAGE_PATH) \
		-c $^ -o $@

$(BUILD):
	mkdir $(BUILD)

$(BIN):
	mkdir $(BIN)

$(BIN)/glew32.$(SHARED_BIN_EXT):
	cp $(GLEW_BIN_PATH)/glew32.$(SHARED_BIN_EXT) $(BIN)
