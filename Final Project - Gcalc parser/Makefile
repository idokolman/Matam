CXX=/usr/local/bin/g++
CC=/usr/local/bin/gcc
OBJS = GcalcAux.o CalcGraph.o calculator.o gcalc.o 
ZIP_OBJS = Makefile test_in.txt test_out.txt design.pdf graph.i gcalc.cpp graph_wrap_py.h graph_wrap_py.cpp CalcGraph.h GcalcAux.cpp GcalcAux.h Exceptions.h calculator.cpp calculator.h CalcGraph.cpp
LIBGRAPH_OBJS = GcalcAux.o CalcGraph.o graph_wrap_py.o 
CLEAN_OBJS = $(OBJS) $(LIBGRAPH_OBJS) $(EXEC) libgraph.a
EXEC = gcalc
DEBUG_FLAG = #not used for now
COMP_FLAG = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG -fPIC

$(EXEC) :$(OBJS)
	$(CXX) $(DEBUG_FLAG) $(OBJS) -o $@

libgraph.a: $(LIBGRAPH_OBJS)
	ar -rs $@ $^

gcalc.o: gcalc.cpp CalcGraph.h GcalcAux.h Exceptions.h calculator.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

CalcGraph.o: CalcGraph.cpp CalcGraph.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

calculator.o: calculator.cpp calculator.h CalcGraph.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

GcalcAux.o: GcalcAux.cpp GcalcAux.h Exceptions.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

graph_wrap_py.o: GcalcAux.h CalcGraph.h Exceptions.h graph_wrap_py.h graph_wrap_py.cpp
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

tar: $(ZIP_OBJS)
	zip gcalc.zip $(ZIP_OBJS) short.gc

clean:
	rm -f $(CLEAN_OBJS)