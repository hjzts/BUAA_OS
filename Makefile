.PHONY: clean

out: calc case_all
# Your code here.
case_add:
	gcc casegen.c -o casegen
	./casegen add 100 > case_add
case_sub:
	gcc casegen.c -o casegen
	./casegen sub 100 > case_sub
case_mul:
	gcc casegen.c -o casegen
	./casegen mul 100 > case_mul
case_div:
	gcc casegen.c -o casegen
	./casegen div 100 > case_div

calc:
	# make case_all
	gcc calc.c -o calc
	./calc < case_all > out

case_all:
	gcc casegen.c -o casegen
	./casegen add 100 > case_add
	cat case_add >> case_all
	./casegen sub 100 > case_sub
	cat case_sub >> case_all
	./casegen mul 100 > case_mul
	cat case_mul >> case_all
	./casegen div 100 > case_div
	cat case_div >> case_all

clean:
	rm -f out calc casegen case_* *.o
