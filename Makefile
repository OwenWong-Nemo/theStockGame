SFML_PATH = /opt/homebrew/Cellar/sfml/3.0.2
# Finds all cpp files in src and its subfolders (model, view, utils, etc.)
cppFileNames := $(shell find ./src -type f -name "*.cpp")

all: compile

compile:    
	mkdir -p bin
	g++ -std=c++20 $(cppFileNames) \
	-I$(SFML_PATH)/include \
	-o bin/main \
	-L$(SFML_PATH)/lib \
	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

run:
	./bin/main