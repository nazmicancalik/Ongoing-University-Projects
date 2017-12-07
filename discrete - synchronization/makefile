all:	main.o PCB.o SchedulingManager.o
	g++ -std=c++11 main.o PCB.o SchedulingManager.o -o program.out

main.o:	SchedulingManager.h
	g++ -c -std=c++11 main.cpp 

SchedulingManager.o:	PCB.h SchedulingManager.h
	g++ -c -std=c++11 SchedulingManager.cpp 

PCB.o:	PCB.h IOManager.h
	g++ -c -std=c++11 PCB.cpp 

run: program.out
	./program.out

clean:
	rm -f scheduler.out *.o
 