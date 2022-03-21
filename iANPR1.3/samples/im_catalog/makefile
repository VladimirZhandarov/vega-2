CC              := g++ -g3 
CFLAGS          := -I/usr/local/include/opencv -L/usr/local/lib
OBJECTS         := 
LIBRARIES       := -lopencv_core -lopencv_imgproc -lopencv_highgui -lianpr1.3

.PHONY: all clean

all: sample

sample: 
	$(CC)  $(CFLAGS) -c *.cpp
	$(CC)  $(CFLAGS) *.o $(LIBRARIES) -o im_catalog
        
clean:
	rm -f *.o
	rm -f im_catalog


