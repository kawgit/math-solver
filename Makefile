all: comp.o eval.o identities.o move.o node_manip.o node.o parse.o search.o system.o main.exe

comp.o: comp.cpp comp.h
	clang++ -c -Ofast comp.cpp -o comp.o
eval.o: eval.cpp eval.h
	clang++ -c -Ofast eval.cpp -o eval.o
identities.o: identities.cpp identities.h
	clang++ -c -Ofast identities.cpp -o identities.o
move.o: move.cpp move.h
	clang++ -c -Ofast move.cpp -o move.o
node_manip.o: node_manip.cpp node_manip.h
	clang++ -c -Ofast node_manip.cpp -o node_manip.o
node.o: node.cpp node.h
	clang++ -c -Ofast node.cpp -o node.o
parse.o: parse.cpp parse.h
	clang++ -c -Ofast parse.cpp -o parse.o
search.o: search.cpp search.h
	clang++ -c -Ofast search.cpp -o search.o
system.o: system.cpp system.h
	clang++ -c -Ofast system.cpp -o system.o

main.exe: comp.o eval.o identities.o move.o node_manip.o node.o parse.o search.o system.o main.cpp
	clang++ -Ofast comp.o eval.o identities.o move.o node_manip.o node.o parse.o search.o system.o main.cpp -o main.exe

clean: 
	del *.o