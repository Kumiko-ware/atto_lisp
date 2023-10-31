g++ -c reader.cpp
g++ -c builtin.cpp
g++ -c eval-apply.cpp
g++ -c printer.cpp
g++ -o lispy  repl.cpp *.o
rm *.o
