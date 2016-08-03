#include "Group.h"

using namespace std;

bool PhoneBookEntry::operator>(PhoneBookEntry &entry) {
    if (name > entry.name) {
        return true;
    }
    else {
        return false;
    }
};
               
bool PhoneBookEntry::operator==(PhoneBookEntry &entry) {
    if (name == entry.name) {
        return true;
    }
    else {
        return false;
    }
};

PhoneBook::PhoneBook() : _size(0) {
};

//------------------------------------------------------------------
//  if _size == 40 then
//      do nothing.
//  else
//      replace _entry[_size] with _entry[_size-1], ...,
//          _entry[k+1] with _entry[k], _entry[k] with entry.
//------------------------------------------------------------------
void PhoneBook::Insert(int k, PhoneBookEntry &entry) {
    if (_size == 40) {
        return;
    }
    for (int i = _size; i > k; i--) {
        _entry[i] = _entry[i - 1];
    }
    _entry[k] = entry;
    _size++;
};

PhoneBookEntry PhoneBook::Item(int i) {
    return _entry[i];
};

int PhoneBook::Size() {
    return _size;
};

int PhoneBook::Find(string name) {
    for (int i = 0; i < _size; i++) {
        if (_entry[i].name == name) {
            return i;
        }
    }
    return -1;
};

//  if _size == 40 then
//      do nothing.
//  else
//      add a entry to PhoneBook according the order of name. If name is exist, Update the tel.
void PhoneBook::Add(PhoneBookEntry &entry) {
    if (_size == 40) {
        return;
    }
    for (int i = 0; i < _size; i++) {
        if (_entry[i] == entry) {
            _entry[i] = entry;
        }
        else if (_entry[i] > entry) {
            Insert(i, entry);
            return;
        }
    }
    _entry[_size] = entry;
    _size++;
};

// delete _entry[k]
void PhoneBook::Delete(int k) {
    if (k < _size) {
        for (int i = k; i < _size - 1; i++) {
            _entry[i] = _entry[i + 1];
        }
        _size--;
    }
};

void PhoneBook::UpdateOfficeTel(int k, string tel) {
    _entry[k].tel.office = tel;
};

void PhoneBook::UpdateMobileTel(int k, string tel) {
    _entry[k].tel.mobile = tel;
};

void PhoneBook::UpdateFaxTel(int k, string tel) {
    _entry[k].tel.fax = tel;
};

// quick search : eg. pb = {"John", "Mary", "Tom"}, return 1 when input s == "M"
int PhoneBook::QuickSearch(string s) {
    for (int i = 0; i < _size; i++) {
        if (s.size() <= _entry[i].name.size() && _entry[i].name.substr(0, s.size()) == s) {
            return i;
        }
    }
    return 0;
};

void PhoneBook::DeleteAll() {
    _size = 0;
};
