#ifndef _ITERS_HPP_
#define _ITERS_HPP_

#include <unistd.h>
#include <memory>
using namespace std;

template <class T>
class ListBase {
    T data;
    shared_ptr<ListBase<T>> next;
public:
    ListBase(T data);
    T GetItem();
    shared_ptr<ListBase<T>> Next();
    void Append(T el);
    void Dump();
};

template <class T>
ListBase<T>::ListBase(T data) {
    this->data = data;
    this->next = nullptr;
}

template <class T>
T ListBase<T>::GetItem() {
    return data;
}

template <class T>
shared_ptr<ListBase<T>> ListBase<T>::Next() {
    return this->next;
}

template <class T>
void ListBase<T>::Append(T el){
    if (this->next == nullptr) {
        this->next = make_shared<ListBase<T>>(el);
        return;
    }
    auto it = this->Next();
    while (it->Next() != nullptr) {
        it = it->Next();
    }
    it->Append(el);
}

template <class T>
void ListBase<T>::Dump() {
    cout << "ListBase at: " << this << endl;
    cout << "data: " << this->data << endl;
    if (this->next != nullptr) {
        cout << "next: " << this->next << endl;
    } else {
        cout << "next: nullptr\n";
    }
}



template <class T>
class Iterator {
public:
    virtual void First() = 0;
    virtual T GetCurrentItem() = 0;
    virtual void Next() = 0;
    virtual bool Finished() = 0;
};

template <class T>
class ArrayIterator: public Iterator<T> {
    T *_arr;
    size_t _size;
    size_t _current;
public:
    ArrayIterator(T *ptr, size_t size);
    ~ArrayIterator();
    void First();
    T GetCurrentItem();
    void Next();
    bool Finished();
};

template <class T>
class ListIterator: public Iterator<T> {
    shared_ptr<ListBase<T>> _first;
    shared_ptr<ListBase<T>> _current;
public:
    ListIterator(shared_ptr<ListBase<T>> l);
    void First();
    T GetCurrentItem();
    void Next();
    bool Finished();
};

template <class T>
class AggregatedIterator: public Iterator<T> {
    shared_ptr<Iterator<T>> _current;
    shared_ptr<ListIterator<shared_ptr<Iterator<T>>>> _iterator;
    shared_ptr<ListBase<shared_ptr<Iterator<T>>>> _iterators;
//    shared_ptr<ListBase<shared_ptr<Iterator<T>>>> _first;
public:
    AggregatedIterator();
    void First();
    T GetCurrentItem();
    void Next();
    bool Finished();
    void AppendIterator(shared_ptr<Iterator<T>>);
};


template <class T>
ArrayIterator<T>::ArrayIterator(T *ptr, size_t size) {
    _arr = ptr;
    _size = size;
    _current = 0;
};

template <class T>
ArrayIterator<T>::~ArrayIterator() {
    _arr = nullptr;
};

template <class T>
void ArrayIterator<T>::First() {
    _current = 0;
};

template <class T>
T ArrayIterator<T>::GetCurrentItem() {
    return _arr[_current];
};

template <class T>
void ArrayIterator<T>::Next() {
    if (_current < _size) {
        _current++;
        return;
    }
    throw this;
};

template <class T>
bool ArrayIterator<T>::Finished() {
    return _current >= _size;
};



template <class T>
ListIterator<T>::ListIterator(shared_ptr<ListBase<T>> l) {
    _first = l;
    _current = _first;
}
template <class T>
void ListIterator<T>::First(){
    _current = _first;
}
template <class T>
T ListIterator<T>::GetCurrentItem() {
    return _current->GetItem();
}
template <class T>
void ListIterator<T>::Next() {
    _current = _current->Next();
}
template <class T>
bool ListIterator<T>::Finished() {
    return _current == nullptr;
}



template <class T>
AggregatedIterator<T>::AggregatedIterator() {
    _iterators = nullptr;
    _current = nullptr;
}
template <class T>
void AggregatedIterator<T>::First(){
    if (_iterator != nullptr) {
        _iterator->First();
        _current = _iterator->GetCurrentItem();

    }
}

template <class T>
void AggregatedIterator<T>::Next() {
    if (_current == nullptr) {
        return;
    }
    if (!_current->Finished()) {
        _current->Next();
        return;
    }

    shared_ptr<Iterator<T>> iter_item = _iterator->GetCurrentItem();
    iter_item->Next();
    while (!iter_item->Finished()) {
        iter_item = _iterator->GetCurrentItem();
        if (!iter_item->Finished()) break;
        _iterator->Next();
    }
    _current = iter_item;
}

template <class T>
bool AggregatedIterator<T>::Finished() {
    return _current == nullptr;
}
template <class T>
void AggregatedIterator<T>::AppendIterator(shared_ptr<Iterator<T>> iter) {
    if (_iterators == nullptr) {
        _iterators = make_shared<ListBase< shared_ptr<Iterator<T>> >>(iter);
        _iterator = make_shared<ListIterator<shared_ptr<Iterator<T>>>>(_iterators);
    }
    _iterators->Append(iter);
}

template <class T>
T AggregatedIterator<T>::GetCurrentItem() {
    if (_current != nullptr)
    return _current->GetCurrentItem();
}


#endif
