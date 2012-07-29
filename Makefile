VERSION = 3.02
CC      = gcc
CFLAGS  =  `pkg-config --cflags libglfw` -Wall -g -D_REENTRANT -DVERSION=\"$(VERSION)\"
LDFLAGS = -lGL -lGLU -lglut -lGLEW -lglfw -lm -lpthread
SERVER_LDFLAGS = -lm -lpthread -lglfw

OBJ = Hrend.o Hinput.o Hcam.o Hnav.o Hfio.o Hgui.o Hutil.o Hshade.o Hauto.o PerlinNoise.o main.o
SERVER_OBJ = Hauto.o Hutil.o PerlinNoise.o server.o

all: prog server

server: $(SERVER_OBJ)
	$(CC) -w $(CFLAGS) -o server $(SERVER_OBJ) $(SERVER_LDFLAGS)

prog: $(OBJ)
	$(CC) -w $(CFLAGS) -o prog $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) -w $(CFLAGS) -c $<

clean:
	rm -rf *.o prog
