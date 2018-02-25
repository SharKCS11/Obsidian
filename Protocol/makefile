CC=g++
OBJDIR = obj
LIBDIR = lib
TESTDIR = test
SOURCES = meshchat.cpp

#EXECUTABLE NAME
################
EXEC = meshchat.exe
################
CFLAGS = -std=c++14 -O3 -Wall

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(SOURCES)  -o $(EXEC)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -MM -c -o $@ $<

clean:
	rm *.exe *.o