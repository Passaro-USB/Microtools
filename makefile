GLEW_INCLUDE_PATH = examples/dependencies/glew-install/include
GLFW_INCLUDE_PATH = examples/dependencies/glfw-install/include
STB_IMAGE_PATH = examples/dependencies

CC = gcc
BUILD = $(ROOT)/build
INSTALL = $(ROOT)/install

ROOT = $(CURDIR)
SRC = $(wildcard $(ROOT)/*.c)
OUT = $(SRC:$(ROOT)/%.c=$(BUILD)/%.o)

.PHONY: install
install: $(BUILD)/libmicrotools.a
	mkdir install
	cp microtools.h install
	cp $(BUILD)/libmicrotools.a install

.PHONY: build
build: $(BUILD)/libmicrotools.a
	

$(BUILD)/libmicrotools.a: $(BUILD) $(OUT)
	ar rcs  $@ $(OUT)

$(BUILD):
	mkdir $(BUILD)

$(BUILD)/%.o: $(ROOT)/%.c
	$(CC) -I$(GLEW_INCLUDE_PATH) -I$(GLFW_INCLUDE_PATH) -I$(STB_IMAGE_PATH) -I. \
		-c $^ -o $@
