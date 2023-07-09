CC = clang
LLC = llc

all: bpf

bpf:
	$(CC) -O2 -emit-llvm -c dbg_endsid_kern.c -o - | $(LLC) -march=bpf -filetype=obj -o dbg_endsid_kern.o

clean:
	rm *.o
