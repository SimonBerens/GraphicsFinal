CC=g++ -std=c++17
CFLAGS=-g

all: exec.zzz
	./exec.zzz

exec.zzz: main.o symt.o cmd.o sym.o equation_parser.o P.o V.o color.o p_matrix.o m_matri.o face.o rm.o sm.o tm.o
	$(CC) $(CFLAGS) -o exec.zzz main.o symt.o cmd.o sym.o equation_parser.o P.o V.o color.o p_matrix.o m_matri.o face.o rm.o sm.o tm.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

symt.o: parsing/symt.cpp parsing/symt.h
	$(CC) $(CFLAGS) -c parsing/symt.cpp

cmd.o: parsing/cmd.h parsing/cmd.cpp
	$(CC) $(CFLAGS) -c parsing/cmd.cpp
	
sym.o: parsing/sym.h parsing/sym.cpp
	$(CC) $(CFLAGS) -c parsing/sym.cpp

equation_parser.o: equation_parser.cpp equation_parser.h
	$(CC) $(CFLAGS) -c equation_parser.cpp

P.o: scalables/P.cpp scalables/P.h scalables/scalable.h
	$(CC) $(CFLAGS) -c scalables/P.cpp

V.o: scalables/V.cpp scalables/V.h scalables/scalable.h
	$(CC) $(CFLAGS) -c scalables/V.cpp

color.o: scalables/color.cpp scalables/color.h scalables/scalable.h
	$(CC) $(CFLAGS) -c scalables/color.cpp

p_matrix.o: matrices/p_matrix.cpp matrices/p_matrix.h
	$(CC) $(CFLAGS) -c matrices/p_matrix.cpp

m_matri.o: matrices/m_matri.cpp matrices/m_matri.h
	$(CC) $(CFLAGS) -c matrices/m_matri.cpp

face.o: matrices/face.cpp matrices/face.h
	$(CC) $(CFLAGS) -c matrices/face.cpp

rm.o: matrices/rm.cpp matrices/rm.h
	$(CC) $(CFLAGS) -c matrices/rm.cpp

sm.o: matrices/sm.cpp matrices/sm.h
	$(CC) $(CFLAGS) -c matrices/sm.cpp

tm.o: matrices/tm.cpp matrices/tm.h
	$(CC) $(CFLAGS) -c matrices/tm.cpp

clean:
	rm *.o *.zzz