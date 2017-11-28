all: debug

debug: stringgen.h
	nvcc -g -o cudahsh.out cudahsh.cu stringgen.c

clean:
	rm -f *.o *.d *.out
