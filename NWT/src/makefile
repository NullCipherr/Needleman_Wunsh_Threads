CC = gcc
CFLAGS = -Wall -pthread

SOURCES = main.c sequences.c scores_matrix.c alignment.c utils.c
OBJECTS = $(SOURCES:.c=.o)

EXECUTABLE = needleman_wunsch

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(EXECUTABLE)