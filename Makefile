INCLUDE = -I/usr/include/
LIBDIR= -L/usr/X11R6/lib

CXX = g++
CFLAGS = -Wall -g -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm -lstdc++ -ldl 

SRCS = world.cpp 
OBJS = $(SRCS:.cpp=.o)

default: khoom

%.o: %.cpp %.hpp 
	#$(CXX) $(CFLAGS) -c $< -o $@
	$(CXX) $(CFLAGS)  -c $< -o $@ 

khoom: khoom.cpp $(OBJS)
	$(CXX) $(CFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCS) 
	$(CXX) -MM $^ > $@

run: khawm
	./khawm
clean:
	rm -f *.o 
