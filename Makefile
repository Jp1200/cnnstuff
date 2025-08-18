TARGET = main

all:
	clang *.c -o $(TARGET)

clean:
	rm -f $(TARGET)
