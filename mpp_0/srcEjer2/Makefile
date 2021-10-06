CC = gcc
CFLAGS = -O3 -Wall -std=c99 -g

N = 
M = 
N_GEN = 
T_POB = 

EXEC = sec
OUTFILE = ../output/out.txt
C_FILES = main.c io.c mdp.c

sec: $(C_FILES)
	$(CC) $(CFLAGS) $(C_FILES) -o $(EXEC) -DTIME -DDEBUG

test_sec:
	./$(EXEC) $(N) $(M) $(N_GEN) $(T_POB) >> $(OUTFILE)

clean:
	rm -f $(EXEC)
