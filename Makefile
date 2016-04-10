.PHONY : make
make :
	gcc -c randomize.c
	gcc -c buildTree.c
	gcc -c debug.c
	gcc -c searchTree.c
	gcc -c timing.c
	gcc -c run.c
	gcc randomize.o buildTree.o searchTree.o debug.o timing.o run.o -o treeTraversal
.PHONY : clean
clean :
	rm -f treeTraversal \
	*.o \
	*.a
	rm -rf *.swp
