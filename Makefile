VERSION = 3.02
CC      = gcc
CFLAGS  =  `pkg-config --cflags libglfw` -O2 -Wall -g -D_REENTRANT -DVERSION=\"$(VERSION)\"
LDFLAGS = -lrt -lGL -lGLU -lglut -lGLEW -lglfw -lm -lpthread
SERVER_LDFLAGS = -lm -lpthread -lglfw

HAMLIB_DIR = Hamlib/
HAMLIB_OBJ = Hrend.o Hinput.o Hcam.o Hnav.o Hfio.o Hgui.o Hutil.o Hshade.o Hauto.o
OBJ = Draw.o Game.o Automat.o Generate.o Cell.o gui.o Statistics.o Client.o

all: prog


prog: $(OBJ)
	cd $(HAMLIB_DIR); make; cd ..;
	$(CC) -w $(CFLAGS) -o prog $(OBJ) $(addprefix $(HAMLIB_DIR), $(HAMLIB_OBJ)) $(LDFLAGS);

%.o: %.c
	$(CC) -w $(CFLAGS) -c $<

clean:
	rm -rf *.o prog server
	cd $(HAMLIB_DIR); make clean; cd ..;
