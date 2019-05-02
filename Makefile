objects = main.cpp Database.cpp Table.cpp sql_server.cpp SQL.cpp
all: $(objects)
	g++ -std=c++14 -o main $(objects)
.PHONY: clean
clean: 
	-rm database