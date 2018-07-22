d_compress.o:d_compress.h d_compress.cpp
	g++ -c d_compress.cpp

target:d_compress.o compress.cpp
	g++ -g compress.cpp -o target

clean: 
	rm *.o target
