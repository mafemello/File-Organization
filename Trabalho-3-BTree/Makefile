all:
	gcc -o t *.c -Wall
run:
	./t
test:
	valgrind ./t

zip: clear
	zip c.zip *.c *.h Makefile
clear:
	rm c.zip
