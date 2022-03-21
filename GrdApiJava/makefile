#
# define compiler and compiler flag variables
#

JFLAGS = -encoding cp1251  
DEST = -d bin/
SOURCE = src/guardant/*.java
JC = javac 
JR = jar

default: clean GrdApiJava GrdApiJava.jar

GrdApiJava.jar:
	$(JR) cvf0 bin/GrdApiJava.jar -C bin guardant

GrdApiJava: 
	$(JC) $(JFLAGS) $(DEST) $(SOURCE)

clean:
	$(RM) bin/guardant/*.class
	$(RM) bin/GrdApiJava.jar

