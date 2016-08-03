
#include "test.h"

int main()
{
    EFSM efsm;

    Event1 = 0;
    Event2 = 1;
    Event3 = 1;
    efsm.Print();
// E-C-G-A
//     H-D
    efsm.Run();
    efsm.Print();
// E-C-G-B
//     H-B

    Event1 = 0;
    Event2 = 0;
    Event3 = 1;
    efsm.Run();
    efsm.Print();
// E-C-G-A
//     H-B

    Event1 = 1;
    Event2 = 0;
    Event3 = 1;
    efsm.Run();
    efsm.Print();
// E-C-G-A
//     H-B

    Event1 = 0;
    Event2 = 1;
    Event3 = 1;
    efsm.Run();
    efsm.Print();
// E-C-G-B
//     H-B

    Event1 = 1;
    Event2 = 1;
    Event3 = 1;
    efsm.Run();
    efsm.Print();
// E-F-D

    Event1 = 1;
    Event2 = 1;
    Event3 = 1;
    efsm.Run();
    efsm.Print();
// E-B

    Event1 = 0;
    Event2 = 1;
    Event3 = 0;
    efsm.Run();
    efsm.Print();
// E-F-D

    Event1 = 0;
    Event2 = 0;
    Event3 = 1;
    efsm.Run();
    efsm.Print();
// E-C-G-A
//     H-D

    return 0;
}
