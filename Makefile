all:PA2

CXX = g++ -g

OFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops 

PA2: main.o parser.o c_net.o fp.o contour.o sa.o
	$(CXX)   main.o parser.o c_net.o fp.o sa.o contour.o -o PA2

main.o: main.cpp parser.h
	$(CXX)   main.cpp -c
parser.o: parser.cpp parser.h
	$(CXX)   parser.cpp -c
c_net.o: c_net.cpp fp.h
	$(CXX)   c_net.cpp -c
fp.o: fp.cpp fp.h parser.h
	$(CXX)   fp.cpp -c
contour.o: contour.cpp fp.h 
	$(CXX)   contour.cpp -c
sa.o: sa.cpp fp.h
	$(CXX)   sa.cpp -c
	
	

clean:
	rm -rf *.o *.gch output.txt hw3 
read1:
	./hw3 0.5 testcase/ami33.block testcase/ami33.nets output.txt
read2:
	./hw3 0.5 testcase/ami3.block testcase/ami3.nets output.txt
read3:
	./hw3 0.5 testcase/ami6.block testcase/ami6.nets output.txt
read4:
	./hw3 0.5 testcase/apte.block testcase/apte.nets output.txt
read5:
	./hw3 0.5 testcase/ami49.block testcase/ami49.nets output.txt
read6:
	./hw3 0.5 testcase/hp.block testcase/hp.nets output.txt
read7:
	./hw3 0.5 testcase/xerox.block testcase/xerox.nets output.txt
