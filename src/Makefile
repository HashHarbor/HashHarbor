#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need SDL2 (http://www.libsdl.org):
# Linux:
#   apt-get install libsdl2-dev
# Mac OS X:
#   brew install sdl2
# MSYS2:
#   pacman -S mingw-w64-i686-SDL2
#

#CXX = g++
#CXX = clang++

EXE = abc
IMGUI_DIR = ../

SOURCES = main.cpp graphic/graphic.cpp imageHandler/imageHandler.cpp character/characterManager.cpp character/characterBuilder.cpp movement/movementHandler.cpp
SOURCES += $(IMGUI_DIR)/imgui/imgui.cpp $(IMGUI_DIR)/imgui/imgui_demo.cpp $(IMGUI_DIR)/imgui/imgui_draw.cpp $(IMGUI_DIR)/imgui/imgui_tables.cpp $(IMGUI_DIR)/imgui/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++17 -I$(IMGUI_DIR)/imgui -I$(IMGUI_DIR)/backends
CXXFLAGS += -g -Wall -Wformat -lpthread
LIBS =

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2
## If you're on a Raspberry Pi and want to use the legacy drivers,
## use the following instead:
# LINUX_GL_LIBS = -L/opt/vc/lib -lbrcmGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) -ldl `sdl2-config --libs`

	CXXFLAGS += `sdl2-config --cflags`
	CFLAGS = $(CXXFLAGS)

	# Obtain OpenCV flags using pkg-config
    OPENCV_FLAGS := $(shell pkg-config --cflags --libs opencv4)

    # Add OpenCV flags to CXXFLAGS and LIBS
    CXXFLAGS += $(OPENCV_FLAGS)
    LIBS += $(OPENCV_FLAGS)

endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += `sdl2-config --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)

	# Path to OpenCV includes and libraries
   	OPENCV_INCLUDES := -I/usr/local/include/opencv4
    OPENCV_LIBS := -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

    # Add OpenCV flags to CXXFLAGS and LIBS
    CXXFLAGS += $(OPENCV_INCLUDES)
    LIBS += $(OPENCV_LIBS)
endif

ifeq ($(OS), Windows_NT)
    ECHO_MESSAGE = "MinGW"
    LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

    CXXFLAGS += `pkg-config --cflags sdl2`
    CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


%.o:character/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:graphic/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:imageHandler/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:movement/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<	

%.o:$(IMGUI_DIR)/imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
