all: debug

debug: stringgen.h
	nvcc -g -o cudahsh.out stringgen.c cudahsh.cu -Wno-deprecated-gpu-targets

clean:
	rm -f *.o *.d *.out output
