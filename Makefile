NOM = Traini

FLAGS = -Wall -Wno-unused-function -Wextra -pedantic -std=gnu99 -O3
#FLAGS = -Wall -Wno-unused-function -Wextra -pedantic -std=gnu99 -O3 -Wno-unused-parameter
LIBS = -lgc
GCC = gcc $(FLAGS)
#GCC = clang $(FLAGS)

HEADERS = regex.h
SOURCES = utils.c tp3-$(NOM).c y.tab.c lex.yy.c
OBJECTS = $(SOURCES:.c=.o)



current: regex

lexer: lex.yy.c

lex.yy.c: tp4-$(NOM).l
	lex tp4-$(NOM).l

parser: regex.tab.c

y.tab.c: tp4-$(NOM).y
	yacc -d tp4-$(NOM).y

# bug in some versions of flex: generate harmless warning
lex.yy.o: lex.yy.c
	$(GCC) -Wno-sign-compare -c lex.yy.c

%.o: %.c $(HEADERS)
	$(GCC) -c $<

regex: $(HEADERS) main.c $(OBJECTS)
	$(GCC) main.c $(OBJECTS) -o regex $(LIBS)

clean:
	rm -f regex a.out gmon.out *.o regex.tab.* y.* lex.*

.PHONY: current lexer parser clean
