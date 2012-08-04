VERSION = 3.02
CC      = gcc
CFLAGS  =  `pkg-config --cflags libglfw` -Wall -g -D_REENTRANT -DVERSION=\"$(VERSION)\"
LDFLAGS = -lGL -lGLU -lglut -lGLEW -lglfw -lm -lpthread
SERVER_LDFLAGS = -lm -lpthread -lglfw

OBJ = Hamlib/Hrend.o Hamlib/Hinput.o Hamlib/Hcam.o Hamlib/Hnav.o Hamlib/Hfio.o Hamlib/Hgui.o Hamlib/Hutil.o Hamlib/Hshade.o Hamlib/Hauto.o Draw.o Game.o Automat.o Generate.o Cell.o gui.o main.o
SERVER_OBJ = Hauto.o Hutil.o PerlinNoise.o server.o

all: prog server

server: $(SERVER_OBJ)
	$(CC) -w $(CFLAGS) -o server $(SERVER_OBJ) $(SERVER_LDFLAGS)

prog: $(OBJ)
	cd Hamlib; make; cd ..;
	$(CC) -w $(CFLAGS) -o prog $(OBJ) $(LDFLAGS);
	rm H*.o; rm *.o

%.o: %.c
	$(CC) -w $(CFLAGS) -c $<

clean:
	rm -rf *.o prog server
