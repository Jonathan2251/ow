#ifndef OBSERVER_H_HEADER_INCLUDED_B8CAD9A4
#define OBSERVER_H_HEADER_INCLUDED_B8CAD9A4

// Observer pattern
// Author : Gamma Chen
// Cowork with Subject class

class Subject;

#if 0
#include "Subject.h"
#endif

// Observer pattern
// Author : Gamma Chen
// Cowork with Subject class
//##ModelId=47356B330309
class Observer
{
  public:
    //##ModelId=47356B73005A
    virtual ~Observer();

    //##ModelId=47356B810174
    virtual void Update(Subject *theChangedSubject);

  protected:
    //##ModelId=47356BD201A4
    Observer();

};



#endif /* OBSERVER_H_HEADER_INCLUDED_B8CAD9A4 */
