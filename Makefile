TARGET = main

all:
	clang main.c -o $(TARGET)

clean:
	rm -f $(TARGET)
