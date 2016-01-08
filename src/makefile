# définition des actions
.PHONY: clean

# définition des variables
CC = gcc
CCFLAGS =  
LDFLAGS = 
SOURCES = spp_aco.c
OBJECTS = $(SOURCES:.c =.o)

# regles
all: $(OBJECTS)
	$(CC) -o ../bin/spp_aco $^ $(CCFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CCFLAGS) $(LDFLAGS)

# action clean
clean:
	-rm *.o ../bin/spp_aco *~
