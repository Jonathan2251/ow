copy test_vm.cc test_vm.cpp
copy test.cc test.cpp
cl /c test_vm.cpp test.cpp
link test.obj test_vm.obj