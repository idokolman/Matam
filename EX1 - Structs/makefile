CC = gcc
OBJS = election.o areaVotes.o electionTestsExample.o utilities.o
EXEC = election
DEBUG_FLAG = #not used for now
COMP_FLAG =  -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@ -L. -lmap

electionTestsExample.o : tests/electionTestsExample.c election.h mtm_map/map.h test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/$*.c

areaVotes.o : areaVotes.c mtm_map/map.h areaVotes.h utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

election.o : election.c election.h mtm_map/map.h areaVotes.h utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

utilities.o: utilities.c utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS) $(EXEC)
