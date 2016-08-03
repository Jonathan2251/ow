// Author : Gamma Chen

#include "Subject.h"
#include "Observer.h"
#include "TiLib_RS232.h"

//##ModelId=47356875016A
Subject::~Subject()
{
}

//##ModelId=473568AF014B
void Subject::Attach(Observer *observer)
{
	if (_size < 10) {
		_observer[_size] = observer;
		_size++;
	}
//	PDEBUG("Subject::Attach(), _size = %d\r\n", _size);
}

//##ModelId=47356A9103E2
void Subject::Detach(Observer *observer)
{
	int i, j;
	
	for (i = 0; i < _size; i++) {
		if (_observer[i] == observer) {
			for (j = i; j < _size - 1; j++) {
				_observer[j] = _observer[j+1];
			}
			_size--;
		}
	}
//	PDEBUG("Subject::Detach(), _size = %d\r\n", _size);
}

//##ModelId=47356ADE02BA
void Subject::Notify()
{
	int i;
	
//	PDEBUG("Subject::Notify\r\n");
	for (i = 0; i < _size; i++) {
		_observer[i]->Update(this);
	}
}

//##ModelId=47356AFB027C
Subject::Subject() : _size(0)
{
}

