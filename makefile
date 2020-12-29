CC=gcc									# gcc is the compiler
CCLINK=$(CC) 							# Read variable CC using $(CC)
CFLAGS=-g -Wall -std=c99				# flags for the compiler
OBJS=ransom.o 							#all of out object files
EXEC=prog.exe 							#our execute file
RM=rm -rf *.o *.exe 					# make clean command

#linker command, creates the execute file from the objects file
$(EXEC): $(OBJS)					
	$(CCLINK) $(OBJS) -o prog.exe

# compiler commands
# The rules we made for compiling into object files, according to the dependency graph of our files
ransom.o: ransom.c
	$(CC) $(CFLAGS) -c ransom.c

# the command "make clean" will call this rule
clean:
	$(RM)			#reading the variable RM

