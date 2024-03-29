CC = /usr/bin/g++

#OPENGL LIBS FOR LINUX
#GLLIB :=  -lGL -lGLEW -lGLU -lglut
#OPENGL LIBS FOR MAC
GLLIB := -framework OpenGL -framework GLUT

#COMPILER FLAGS
CCFLAGS := -w

#include directories
#should include gl.h glut.h etc...
INCDIR := -I/usr/include
LDLIBS := $(GLLIB)

TARGET = 2ddraw
OBJS = main.o curve.o


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $^ $(LDLIBS) -o $@

%.o : %.cpp
	$(CC) $(CCFLAGS) -o $@ -c $(INCDIR) $<

clean:
	rm -rf $(OBJS) $(TARGET)

