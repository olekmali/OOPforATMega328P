//* singleton design pattern on the heap     *
#include <iostream>
using namespace std;

class singleton_heap {
public:
    ~singleton_heap();
    static singleton_heap* getInstance();

public:
    void doSomething();

protected:
    static singleton_heap* onlyonce;
    // all state variables - no need for static

private:
    singleton_heap();

private:
    singleton_heap(const singleton_heap&);
    void operator=(const singleton_heap&);
};

singleton_heap* singleton_heap::onlyonce = 0;                           // lazy version
// singleton_heap* singleton_heap::onlyonce = new singleton_heap();     // eager version

singleton_heap::singleton_heap() {
    cout << "constructor called" << endl;
}

singleton_heap::~singleton_heap() {
    cout << "destructor is never called" << endl;
}

singleton_heap* singleton_heap::getInstance() {
    cout << "getInstance" << endl;
    if (onlyonce==0) onlyonce = new singleton_heap();
    return(onlyonce);
}

void singleton_heap::doSomething() {
    cout << "Something" << endl;
}


int main() {
    {
        cout << "1" << endl;
        singleton_heap * A = singleton_heap::getInstance();
        cout << "2" << endl;
        singleton_heap * B = singleton_heap::getInstance();
        cout << "3" << endl;
        A->doSomething();
        B->doSomething();
        cout << "4" << endl;
    }
    cout << "5" << endl;


    cin.ignore();
    return(0);
}
