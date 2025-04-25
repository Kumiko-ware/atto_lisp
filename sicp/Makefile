
all: memory.o env.o exp.o printer.o reader.o eval.o

memory.o: memory.c memory.h
env.o: env.c memory.h
exp.o: exp.c exp.h memory.h
printer.o: printer.c memory.h
reader.o: reader.c reader.h
eval.o: eval.c eval.h
primitives.o: primitives.c primitives.h
test_mallocs: test_mallocs.c memory.o env.o exp.o
test_printer: test_printer.c memory.o env.o exp.o printer.o
test_reader: test_reader.c memory.o env.o exp.o reader.o printer.o
test_repl: test_repl.c memory.o env.o exp.o printer.o reader.o eval.o primitives.o

.PHONY: all
