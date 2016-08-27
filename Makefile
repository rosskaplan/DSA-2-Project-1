spell.exe: spellcheck.o hash.o
	g++ -std=c++11 -o spell.out spellcheck.o hash.o

spellcheck.o: spellcheck.cpp hash.h
	g++ -std=c++11 -c spellcheck.cpp

hash.o: hash.cpp hash.h
	g++ -std=c++11 -c hash.cpp

make all: hash.o spellcheck.o spell.exe
	make hash.o
	make spellcheck.o
	make spell.exe

debug:
	g++ -std=c++11 -g -o spellDebug.exe spellcheck.cpp hash.cpp

clean:
	rm -rf *.exe *.o *.stackdump *~ .*.swp .*.*.swp *.*.dSYM

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
