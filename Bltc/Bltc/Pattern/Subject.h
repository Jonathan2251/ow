#ifndef SUBJECT_H_HEADER_INCLUDED_B8CAAB23
#define SUBJECT_H_HEADER_INCLUDED_B8CAAB23

// Observer pattern
// Author : Gamma Chen
// Cowork with Observer class

#include "Observer.h"

// Observer pattern
// Author : Gamma Chen
// Cowork with Observer class
//##ModelId=4735676B0214
class Subject
{
  public:
    //##ModelId=47356875016A
    virtual ~Subject();
    //##ModelId=473568AF014B
    virtual void Attach(Observer *observer);
    //##ModelId=47356A9103E2
    virtual void Detach(Observer *observer);
    //##ModelId=47356ADE02BA
    virtual void Notify();

  protected:
    //##ModelId=47356AFB027C
    Subject();

  private:

    //##ModelId=47356D0D0253
    int _size;
    //##ModelId=481543270095
    Observer *_observer[10];
};

#endif /* SUBJECT_H_HEADER_INCLUDED_B8CAAB23 */
