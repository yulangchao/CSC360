.c.o:
	gcc -g -c $?

# compile client and server
all: diskinfo diskget disklist diskput 

diskinfo: diskinfo.o 
	gcc diskinfo.c -o diskinfo 

diskget: diskget.o 
	gcc diskget.c -o diskget 

disklist: disklist.o 
	gcc disklist.c -o disklist 

diskput: diskput.o 
	gcc diskput.c -o diskput 
