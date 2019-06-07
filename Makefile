OBJECTS=draw.o math_vec.o custom_matrices.o matrix.o color.o symbol_table.o code_generator.o
LDFLAGS=-lm
CC=g++ -std=c++17
CFLAGS=-g

file=simple_anim.mdl

all: parser
	./exec.zzz $(file)

parser: lex.yy.c y.tab.c y.tab.h $(OBJECTS)
	$(CC) $(CFLAGS) lex.yy.c y.tab.c $(OBJECTS) $(LDFLAGS) -o exec.zzz

lex.yy.c: mdl.l y.tab.h
	flex -I mdl.l

y.tab.c: mdl.y symbol_table.h command.h
	bison -d -y mdl.y

y.tab.h: mdl.y
	bison -d -y mdl.y

symbol_table.o: symbol_table.cpp symbol_table.h
	$(CC) $(CFLAGS) -c symbol_table.cpp

code_generator.o: code_generator.h code_generator.cpp
	$(CC) $(CFLAGS) -c code_generator.cpp

draw.o: draw.cpp draw.h
	$(CC) $(CFLAGS) -c draw.cpp

math_vec.o: math_vec.cpp math_vec.h
	$(CC) $(CFLAGS) -c math_vec.cpp

custom_matrices.o: custom_matrices.cpp custom_matrices.h
	$(CC) $(CFLAGS) -c custom_matrices.cpp

matrix.o: matrix.cpp matrix.h
	$(CC) $(CFLAGS) -c matrix.cpp

color.o: color.cpp color.h
	$(CC) $(CFLAGS) -c color.cpp

clean:
	rm y.tab.c y.tab.h lex.yy.c *.o *.zzz -rf ./build