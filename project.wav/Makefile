CC=gcc
CFLAGS= -Wall -std=c99
EXEC=svg2pdf
OBJECT = menu.o main.o wav.o #args.o
all: $(EXEC)
main.o : menu.h
%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)
svg2pdf: $(OBJECT)
	$(CC) -o $@ $(OBJECT) $(CFLAGS)
clean :
		rm $(EXEC) $(OBJECT)
		