// Author : Gamma Chen

#include "ItemsIterator.h"
#include "Items.h"

//##ModelId=481145CF031A
ItemsIterator::ItemsIterator()
{
	First();
}

//##ModelId=481145CF032A
void ItemsIterator::First()
{
	_itemNr = 0;
}

//##ModelId=481145CF0359
void ItemsIterator::Next()
{
	_itemNr++;
	if (_itemNr > MAX_TEST_ITEMS) {
		_itemNr = 0;
	}
	while (gItem[_itemNr].type == NO_ITEM_TYPE) {
		_itemNr++;
		if (_itemNr > MAX_TEST_ITEMS) {
			_itemNr = 0;
		}
	}
}

//##ModelId=481145CF0387
void ItemsIterator::Previous()
{
	_itemNr--;
	if (_itemNr > MAX_TEST_ITEMS) {
		_itemNr = MAX_TEST_ITEMS;
	}
	while (gItem[_itemNr].type == NO_ITEM_TYPE) {
		_itemNr--;
		if (_itemNr > MAX_TEST_ITEMS) {
			_itemNr = MAX_TEST_ITEMS;
		}
	}
}

//##ModelId=481145CF03B6
bool ItemsIterator::IsDone() const
{
	return false;
}

//##ModelId=481145CF03E5
U8 ItemsIterator::CurrentItem() const
{
	return _itemNr;
}
