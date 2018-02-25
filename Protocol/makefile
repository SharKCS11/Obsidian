CC=g++
OBJDIR = obj
LIBDIR = lib
TESTDIR = test
SOURCES = meshchat.cpp

#EXECUTABLE NAME
################
EXEC = meshchat.exe
################
CFLAGS = -pthread -std=c++14 -O3 -Wall

all: meshchat.cpp
	$(CC) $(CFLAGS) meshchat.cpp -o meshchat.exe

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -MM -c -o $@ $<

clean:
	rm *.exe *.o