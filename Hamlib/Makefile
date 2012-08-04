VERSION = 3.02
CC      = gcc
CFLAGS  =  `pkg-config --cflags libglfw` -Wall -g -D_REENTRANT -DVERSION=\"$(VERSION)\"
LDFLAGS = -lGL -lGLU -lglut -lGLEW -lglfw -lm -lpthread
SERVER_LDFLAGS = -lm -lpthread -lglfw

OBJ = Hrend.o Hinput.o Hcam.o Hnav.o Hfio.o Hgui.o Hutil.o Hshade.o Hauto.o

all: $(OBJ)

%.o: %.c
	$(CC) -w $(CFLAGS) -c $<

clean:
	rm -rf *.o
