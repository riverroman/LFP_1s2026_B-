cd src/
g++ -std=c++17 -O2 -o backend \
    server.cpp LexicalAnalyzer.cpp SyntaxAnalyzer.cpp ErrorManager.cpp
./backend