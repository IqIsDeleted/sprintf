CC=gcc
CFLAGS= -std=c11 -Werror -Wextra -Wall
LDFLAGS=
SOURCES= my_sprintf.c test.c
OBJECTS=$(SOURCES:.c.h=.o)
EXECUTABLE=s21_string

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clang:
	clang-format -i $(SOURCES)

clean:
	 rm -f $(EXECUTABLE)