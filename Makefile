src := $(shell ls *.c) 
objs := $(patsubst %.c,%.o,$(src))
output := main

CC := cc
CFLAGS := -lSDL2 -g
RM := rm

.PHONY: clean
$(output): $(objs)
	$(CC) $(CFLAGS) $(objs) -o $(output)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(objs)
