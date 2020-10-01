CC := gcc
CFLAGS := -g

main: main.o lint_func.o lint_subfunc.o
main.o lint_func.o lint_subfunc.o: lint_func.h lint_subfunc.h lint_struct.h

clean:
	$(RM) *.o
	$(RM) main