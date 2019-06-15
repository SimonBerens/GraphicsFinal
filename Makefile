CC=g++ -std=c++17
CFLAGS=-O3

file=simple_anim.mdl

all: exec.zzz
	./exec.zzz $(file)

exec.zzz: main.o compiler.o command.o equation.o obj.o light.o surface.o point.o vec.o color.o point_matrix.o modifier_matrix.o face_list.o rotation_matrix.o scale_matrix.o translation_matrix.o
	$(CC) $(CFLAGS) -o exec.zzz main.o compiler.o command.o equation.o obj.o light.o surface.o point.o vec.o color.o point_matrix.o modifier_matrix.o face_list.o rotation_matrix.o scale_matrix.o translation_matrix.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

compiler.o: parsing/compiler.cpp parsing/compiler.h
	$(CC) $(CFLAGS) -c parsing/compiler.cpp

command.o: parsing/command.h parsing/command.cpp
	$(CC) $(CFLAGS) -c parsing/command.cpp

equation.o: parsing/equation.cpp parsing/equation.h
	$(CC) $(CFLAGS) -c parsing/equation.cpp

obj.o: parsing/obj.cpp parsing/obj.h
	$(CC) $(CFLAGS) -c parsing/obj.cpp

light.o: parsing/light.h parsing/light.cpp
	$(CC) $(CFLAGS) -c parsing/light.cpp

surface.o: parsing/surface.cpp parsing/surface.h
	$(CC) $(CFLAGS) -c parsing/surface.cpp

point.o: scalables/point.cpp scalables/point.h scalables/scalable.h
	$(CC) $(CFLAGS) -c scalables/point.cpp

vec.o: scalables/vec.cpp scalables/vec.h scalables/scalable.h
	$(CC) $(CFLAGS) -c scalables/vec.cpp

color.o: scalables/color.cpp scalables/color.h scalables/scalable.h
	$(CC) $(CFLAGS) -c scalables/color.cpp

point_matrix.o: matrices/point_matrix.cpp matrices/point_matrix.h
	$(CC) $(CFLAGS) -c matrices/point_matrix.cpp

modifier_matrix.o: matrices/modifier_matrix.cpp matrices/modifier_matrix.h
	$(CC) $(CFLAGS) -c matrices/modifier_matrix.cpp

face_list.o: matrices/face_list.cpp matrices/face_list.h
	$(CC) $(CFLAGS) -c matrices/face_list.cpp

rotation_matrix.o: matrices/rotation_matrix.cpp matrices/rotation_matrix.h
	$(CC) $(CFLAGS) -c matrices/rotation_matrix.cpp

scale_matrix.o: matrices/scale_matrix.cpp matrices/scale_matrix.h
	$(CC) $(CFLAGS) -c matrices/scale_matrix.cpp

translation_matrix.o: matrices/translation_matrix.cpp matrices/translation_matrix.h
	$(CC) $(CFLAGS) -c matrices/translation_matrix.cpp

clean:
	rm *.o *.zzz