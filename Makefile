.PHONY : make
make :
	gcc -c randomize.c
	gcc -c buildTree.c
	gcc -c debug.c
	gcc -c searchTree.c
	gcc -c -msse4.2 -msse4a searchSIMDTree.c
	gcc -c timing.c
	gcc -c run.c
	gcc randomize.o buildTree.o searchTree.o searchSIMDTree.o debug.o timing.o run.o -o build
.PHONY : clean
clean :
	rm -f build \
	*.o \
	*.a
	rm -rf *.swp
