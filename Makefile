OBJECTS=symbol_table.o code_generator.o draw.o V.o P.o color.o EdgeList.o face.o m_matri.o p_matrix.o rm.o sm.o tm.o
LDFLAGS=-lm
CC=g++ -std=c++17
CFLAGS=-g

file=simple_anim.mdl
# todo reorder
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

V.o: scalables/V.cpp scalables/V.h
	$(CC) $(CFLAGS) -c scalables/V.cpp

P.o: scalables/P.cpp scalables/P.h
	$(CC) $(CFLAGS) -c scalables/P.cpp

color.o: scalables/color.cpp scalables/color.h
	$(CC) $(CFLAGS) -c scalables/color.cpp

EdgeList.o: matrices/EdgeList.cpp matrices/EdgeList.h
	$(CC) $(CFLAGS) -c matrices/EdgeList.cpp

face.o: matrices/face.cpp matrices/face.h
	$(CC) $(CFLAGS) -c matrices/face.cpp

m_matri.o: matrices/m_matri.cpp matrices/m_matri.h
	$(CC) $(CFLAGS) -c matrices/m_matri.cpp

p_matrix.o: matrices/p_matrix.cpp matrices/p_matrix.h
	$(CC) $(CFLAGS) -c matrices/p_matrix.cpp

rm.o: matrices/rm.cpp matrices/rm.h
	$(CC) $(CFLAGS) -c matrices/rm.cpp

sm.o: matrices/sm.cpp matrices/sm.h
	$(CC) $(CFLAGS) -c matrices/sm.cpp

tm.o: matrices/tm.cpp matrices/tm.h
	$(CC) $(CFLAGS) -c matrices/tm.cpp

clean:
	rm y.tab.c y.tab.h lex.yy.c *.o *.zzz -rf ./build