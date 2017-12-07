all:	main.o PCB.o SchedulingManager.o Semaphore.o
	g++ -std=c++11 main.o PCB.o SchedulingManager.o Semaphore.o -o program.out

main.o:	SchedulingManager.h
	g++ -c -std=c++11 main.cpp 

SchedulingManager.o:	PCB.h SchedulingManager.h Semaphore.h
	g++ -c -std=c++11 SchedulingManager.cpp 

PCB.o:	PCB.h IOManager.h
	g++ -c -std=c++11 PCB.cpp 

Semaphore.o: Semaphore.h
	g++ -c -std=c++11 Semaphore.cpp

run: program.out
	./program.out

clean:
	rm -f scheduler.out *.o
 
