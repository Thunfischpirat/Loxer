o_files := main.o lox.o environment.o interpreter.o scanner.o token.o parser.o
loxer: ${o_files}
	echo $<
	g++ build/*.o -o build/loxer
main.o: src/main.cpp 
	g++ -c src/main.cpp -o build/main.o -std=c++23
lox.o: src/lox.cpp
	g++ -c src/lox.cpp -o build/lox.o -std=c++23
environment.o: src/interpreter/environment.cpp
	g++ -c src/interpreter/environment.cpp -o build/environment.o -std=c++23
interpreter.o: src/interpreter/interpreter.cpp
	g++ -c src/interpreter/interpreter.cpp -o build/interpreter.o -std=c++23
scanner.o: src/scanner/scanner.cpp
	g++ -c src/scanner/scanner.cpp -o build/scanner.o -std=c++23
token.o: src/scanner/token.cpp
	g++ -c src/scanner/token.cpp -o build/token.o -std=c++23
parser.o: src/parser/parser.cpp
	g++ -c src/parser/parser.cpp -o build/parser.o -std=c++23
clean:
	rm -f build/*
