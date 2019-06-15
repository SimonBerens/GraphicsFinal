CC=g++ -std=c++17
CFLAGS=-O3

all: exec.zzz
	./exec.zzz

exec.zzz: main.o symt.o cmd.o sym.o equation_parser.o P.o V.o color.o p_matrix.o m_matri.o face.o rm.o sm.o tm.o
	$(CC) $(CFLAGS) -o exec.zzz main.o symt.o cmd.o sym.o equation_parser.o P.o V.o color.o p_matrix.o m_matri.o face.o rm.o sm.o tm.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

symt.o: parsing/compiler.cpp parsing/compiler.h
	$(CC) $(CFLAGS) -c parsing/symt.cpp

cmd.o: parsing/command.h parsing/command.cpp
	$(CC) $(CFLAGS) -c parsing/cmd.cpp
	
sym.o: parsing/light.h parsing/light.cpp
	$(CC) $(CFLAGS) -c parsing/sym.cpp

equation_parser.o: parsing/equation.cpp parsing/equation.h
	$(CC) $(CFLAGS) -c equation_parser.cpp

P.o: scalables/point.cpp scalables/point.h scalables/scalable.h
	$(CC) $(CFLAGS) -c scalables/P.cpp

V.o: scalables/vec.cpp scalables/vec.h scalables/scalable.h
	$(CC) $(CFLAGS) -c scalables/V.cpp

color.o: scalables/color.cpp scalables/color.h scalables/scalable.h
	$(CC) $(CFLAGS) -c scalables/color.cpp

p_matrix.o: matrices/point_matrix.cpp matrices/point_matrix.h
	$(CC) $(CFLAGS) -c matrices/p_matrix.cpp

m_matri.o: matrices/modifier_matrix.cpp matrices/modifier_matrix.h
	$(CC) $(CFLAGS) -c matrices/m_matri.cpp

face.o: matrices/face_list.cpp matrices/face_list.h
	$(CC) $(CFLAGS) -c matrices/face.cpp

rm.o: matrices/rotation_matrix.cpp matrices/rotation_matrix.h
	$(CC) $(CFLAGS) -c matrices/rm.cpp

sm.o: matrices/scale_matrix.cpp matrices/scale_matrix.h
	$(CC) $(CFLAGS) -c matrices/sm.cpp

tm.o: matrices/translation_matrix.cpp matrices/translation_matrix.h
	$(CC) $(CFLAGS) -c matrices/tm.cpp

clean:
	rm *.o *.zzz