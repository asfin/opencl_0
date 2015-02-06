#ifndef _DECOR_HPP_
#define _DECOR_HPP_
#include <iostream>

using namespace std;

class BaseA {
public:
    virtual void say_ololo() = 0;
};

class A: public BaseA {
public:
    void say_ololo() { std::cout<< "OLOLO\n"; };
};

class BaseDecor: public BaseA {
    shared_ptr<BaseA> _component;
public:
    BaseDecor(shared_ptr<BaseA> component) {
        _component = component;
    }
    virtual void say_ololo() {
        _component->say_ololo();
    };
};

class TrololoDecor: public BaseDecor {
    using BaseDecor::BaseDecor;
public:
    void say_ololo() {
        BaseDecor::say_ololo();
        std::cout << "TROLOLO\n";
    };
};

class AzazaDecor: public BaseDecor {
using BaseDecor::BaseDecor;
public:
    void say_ololo() {
        std::cout << "AZAZA\n";
        BaseDecor::say_ololo();
    };
};

#endif