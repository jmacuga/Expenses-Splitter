all: main logic_tests file_operations_tests

main: main.o Person.o Trip.o Transactions.o ui_actions.o
	g++ -o main.out main.o Person.o Trip.o Transactions.o ui_actions.o

logic_tests: logic_tests.o Person.o Trip.o Transactions.o
	g++ -o logic_tests.out logic_tests.o Person.o Trip.o Transactions.o

file_operations_tests: file_operations_tests.o Person.o Trip.o Transactions.o
	g++ -o file_operations_tests.out file_operations_tests.o Person.o Trip.o Transactions.o

ui_actions.o: ui_actions.o
	g++ -c -g ui_actions.cpp

main.o: main.o
	g++ -c -g main.cpp

Trip.o: Trip.o
	g++ -c -g Trip.cpp

Person.o: Person.o
	g++ -c -g Person.cpp

logic_tests.o: logic_tests.o
	g++ -c -g logic_tests.cpp

Transactions.o: Transactions.o
	g++ -c -g Transactions.cpp

file_operations_tests.o: file_operations_tests.o
	g++ -c -g file_operations_tests.cpp


clean:
	rm -r main.o main.out Trip.o Person.o logic_tests.o logic_tests.out Transactions.o
	rm -r ui_actions.o
	rm -r file_operations_tests.o file_operations_tests.out
