VERSION = 3.02
CC      = gcc
CFLAGS  =  `pkg-config --cflags libglfw` -Wall -g -D_REENTRANT -DVERSION=\"$(VERSION)\"
LDFLAGS = -lm -lGL -lGLU -lglut -lGLEW -lglfw -lm -lpthread

OBJ = Hrend.o Hinput.o Hcam.o Hnav.o Hfio.o Hgui.o Hutil.o Hshade.o Hauto.o PerlinNoise.o main.o

prog: $(OBJ)
	$(CC) -w $(CFLAGS) -o prog $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) -w $(CFLAGS) -c $<

clean:
	rm -rf *.o prog
