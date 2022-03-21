OCVHOME		:= /usr/local/lib
OCVVERS		:= 3.4.0


CC              := g++ -g3 -std=c++11
CFLAGS          := -I$(OCVHOME)/include -L/usr/local/lib
OBJECTS         := 
LIBRARIES	:= $(OCVHOME)/lib/libopencv_world.so.$(OCVVERS) -lianpr1.7


.PHONY: all clean

all: sample

sample: 
	$(CC)  $(CFLAGS) -c *.cpp
	$(CC)  $(CFLAGS) *.o $(LIBRARIES) -o capture
        
clean:
	rm -f *.o
	rm -f capture


