# Name of executable
BINS = checklinks 

# Setting the flags
CFLAGS = -g -Wall -Wextra -Wstrict-prototypes

# Link in support for debugging
LDFLAGS = -g

# Default target produced by entering "make" alone
.PHONY: default
default: $(BINS)

# Compile *.c into *.o
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove *.o files, but leave executable
.PHONY: clean
clean:
	rm -f core* src/*.o *~

# Link *.o files into an executable
checklinks: src/checklinks.o src/find.o src/helpers.o
	$(CC) $(LDFLAGS) $^ -o $@

# Run tests
test: checklinks # test with URL
	./checklinks https://users.tricity.wsu.edu/~bobl/cpts360/

testf: checklinks # test with local file
	./checklinks -f test/index.html

# Check for memory leaks
mem: checklinks
	valgrind --leak-check=full --show-leak-kinds=all -s ./checklinks https://users.tricity.wsu.edu/~bobl/cpts360/

# Remove all files that can be reconstructed through "make"
.PHONY: immaculate
immaculate: clean
	rm -f $(BINS)
