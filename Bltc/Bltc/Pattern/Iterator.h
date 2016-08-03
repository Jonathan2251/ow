#ifndef ITERATOR_H_HEADER_INCLUDED
#define ITERATOR_H_HEADER_INCLUDED

// Iterator pattern
// Author : Gamma Chen

//##ModelId=480EA2030323
template <class T>
class Iterator {
  public:
    //##ModelId=480EA75D0258
    virtual void First() = 0;
    //##ModelId=480EA75D0267
    virtual void Next() = 0;
    //##ModelId=480EA75D0287
    virtual bool IsDone() const = 0;
    //##ModelId=480EA75D02B5
    virtual T CurrentItem() const = 0;
  protected:
    //##ModelId=480EA75D02D5
    Iterator() {};
};


#endif /* ITERATOR_H_HEADER_INCLUDED */
