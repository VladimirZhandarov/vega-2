OCVHOME		:= /usr/local/lib
OCVVERS		:= 3.4.0
LIBRARIES	:= $(OCVHOME)/lib/libopencv_world.so.$(OCVVERS) -lianprcapture1.7 -lianpr1.7

CC              := g++ -g3 -std=c++11
CFLAGS          := -I$(OCVHOME)/include -L/usr/local/lib
OBJECTS         := 


.PHONY: all clean

all: sample

sample: 
	$(CC)  $(CFLAGS) -c *.cpp
	$(CC)  $(CFLAGS) *.o $(LIBRARIES) -o ianprcapture
        
clean:
	rm -f *.o
	rm -f ianprcapture


