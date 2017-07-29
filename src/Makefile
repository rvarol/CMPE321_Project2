CC=g++ -std=c++11
CFLAGS=-I. -std=c++11
DEPS = page.hpp defs.hpp fileManager.hpp cliManager.hpp
OBJ = main.o page.o fileManager.o cliManager.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $(CFLAGS) $<

StorageManager: $(OBJ)
	g++ -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) $(OUT)

.PHONY: all clean