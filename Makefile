#OBJS specifies which files to compile as part of the project
# OBJS = main.cpp baseObject.cpp eagle.cpp enemy.cpp func.cpp Game.cpp gameMap.cpp menu.cpp player.cpp timer.cpp
OBJS = src/*.cpp
#HDRS specifies which header files to include
HDRS = header/*.h

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:/SDL2/include -I./header

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:/SDL2/lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w 
# -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Where'sGnuol.exe

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(HDRS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
