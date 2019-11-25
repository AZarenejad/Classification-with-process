all : fileDir LinearClassifier Voter EnsembleClassifier.out 

fileDir:
	@mkdir -p build
	@mkdir -p bin
	@mkdir -p namedPipe
	@mkdir -p namedPipe1


# LinearClassifier : 
LinearClassifier : build/LinearClassifierMain.o build/LinearClassifier.o 
	@echo "Making LinearClassifier executable file!"
	g++ -g -std=c++11 build/LinearClassifierMain.o build/LinearClassifier.o  -o bin/LinearClassifier
	
build/LinearClassifierMain.o : LinearClassifierMain.cpp LinearClassifier.cpp LinearClassifier.hpp
	g++ -g -std=c++11 -c LinearClassifierMain.cpp -o build/LinearClassifierMain.o

build/LinearClassifier.o :  LinearClassifier.cpp LinearClassifier.hpp
	g++  -std=c++11 -c LinearClassifier.cpp -o build/LinearClassifier.o

# Voter :
Voter : build/VoterMain.o build/Voter.o 
	@echo "Making Voter executable file"
	g++ -std=c++11 build/VoterMain.o build/Voter.o -o bin/Voter


build/VoterMain.o : VoterMain.cpp Voter.cpp Voter.hpp
	g++ -std=c++11 -c VoterMain.cpp -o build/VoterMain.o 

build/Voter.o :  Voter.cpp Voter.hpp
	g++ -std=c++11 -c Voter.cpp -o build/Voter.o

# EnsembleClassifier
EnsembleClassifier.out :  build/Main.o build/EnsembleClassifier.o 
	@echo "Linking..."

	g++ -std=c++11 build/Main.o build/EnsembleClassifier.o  -o EnsembleClassifier.out

build/Main.o : Main.cpp EnsembleClassifier.hpp EnsembleClassifier.cpp
	g++ -std=c++11 -c Main.cpp -o build/Main.o


build/EnsembleClassifier.o : EnsembleClassifier.cpp EnsembleClassifier.hpp
	g++ -std=c++11 -c EnsembleClassifier.cpp -o build/EnsembleClassifier.o

clean : 
	rm -f -r namedPipe
	rm -f -r namedPipe1
	rm -f -r bin
	rm -f EnsembleClassifier.out
	rm -f -r build
	
