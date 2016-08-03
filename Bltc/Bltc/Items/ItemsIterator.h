// Author : Gamma Chen

#ifndef ITEMSITERATOR_H_HEADER_INCLUDED_B7D2C86B
#define ITEMSITERATOR_H_HEADER_INCLUDED_B7D2C86B

#include "BltcBasicType.h"
#include "ModelConfig.h"
#include "Iterator.h"

#define MAX_TEST_ITEMS	140

//##ModelId=480FFA4A033B
class ItemsIterator : public Iterator<U8> {
  public:
    //##ModelId=481145CF031A
    ItemsIterator();
    //##ModelId=481145CF032A
    virtual void First();
    //##ModelId=481145CF0359
    virtual void Next();
    //##ModelId=481145CF0387
    virtual void Previous();
    //##ModelId=481145CF03B6
    virtual bool IsDone() const;
    //##ModelId=481145CF03E5
    virtual U8 CurrentItem() const;
    //##ModelId=481145CF02BD
    U8		_itemNr;
};

#endif /* ITEMSITERATOR_H_HEADER_INCLUDED_B7D2C86B */
