a.out: test.o test_vm.o
	g++ -Xlinker test.o test_vm.o
test_vm.o: test_vm.cc
	g++ -c test_vm.cc
test.o: test.cc
	g++ -c test.cc
clean:
	rm -f *.o a.out
