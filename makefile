COMPILER = g++
LIBS = include/
SRC = main.cpp parser.cpp utils.cpp run.cpp 
TARGET = bin/enma

make:	
	$(COMPILER) $(SRC) -I$(LIBS) -o $(TARGET)
	
nt:
	x86_64-w64-mingw32-g++ $(SRC) -I$(LIBS) -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm $(TARGET)
