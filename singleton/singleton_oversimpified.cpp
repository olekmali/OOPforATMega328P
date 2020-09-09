//* singleton design pattern oversimplified *
#include <iostream>
using namespace std;

class singleton_oversimplified {
public:
    singleton_oversimplified();
    ~singleton_oversimplified();

    void doSomething();
protected:
    static size_t count;
    // any state variables should be static as there is only one device of this kind

private:
        singleton_oversimplified(const singleton_oversimplified&);
        void operator=(const singleton_oversimplified&);
};

size_t singleton_oversimplified::count = 0;

singleton_oversimplified::singleton_oversimplified() {
    ++count;
    if (1==count) {
        // do the actual initialization of a device
        cout << "Initializing" << endl;
    } // else ignore
}

singleton_oversimplified::~singleton_oversimplified() {
    --count;
    if (0==count) {
        // do the actual shut down of a device
        cout << "Shutting down" << endl;
    } // else ignore
}

void singleton_oversimplified::doSomething() {
    cout << "Something" << endl;
}


int main() {
    {
        cout << "1" << endl;
        singleton_oversimplified A;
        cout << "2" << endl;
        singleton_oversimplified B;
        cout << "3" << endl;
        A.doSomething();
        B.doSomething();
        cout << "4" << endl;
    }
    cout << "5" << endl;


    cin.ignore();
    return(0);
}
