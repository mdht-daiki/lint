CC := gcc
CFLAGS := -g

main: main.o lint_fbao.o lint_subfunc.o
main.o lint_fbao.o lint_subfunc.o: lint_fbao.h lint_subfunc.h lint_struct.h

clean:
	$(RM) *.o
	$(RM) main