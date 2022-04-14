//* singleton design pattern with global static variable *
#include <iostream>
using namespace std;

class singleton_global {
public:
    ~singleton_global();
    static singleton_global& getInstance();

public:
    void doSomething();

protected:
    static singleton_global onlyonce;
    // all state variables - no need for static

private:
    singleton_global();

private:
    singleton_global(const singleton_global&);
    void operator=(const singleton_global&);
};

singleton_global singleton_global::onlyonce;

singleton_global::singleton_global() {
    cout << "constructor called" << endl;
}

singleton_global::~singleton_global() {
    cout << "destructor is never called" << endl;
}

singleton_global& singleton_global::getInstance() {
    cout << "getInstance" << endl;
    return(onlyonce);
}

void singleton_global::doSomething() {
    cout << "Something" << endl;
}


int main() {
    {
        cout << "1" << endl;
        singleton_global & A = singleton_global::getInstance();
        cout << "2" << endl;
        singleton_global & B = singleton_global::getInstance();
        cout << "3" << endl;
        A.doSomething();
        B.doSomething();
        cout << "4" << endl;
    }
    cout << "5" << endl;


    cin.ignore();
    return(0);
}
