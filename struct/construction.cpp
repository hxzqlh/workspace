#include <iostream>
#include <string>
using namespace std;

class Foo {
public:
    Foo () : name() { cout << "Foo()..." << endl;}

private:
    string name;
};

struct Bar {
public:
    Bar (): age(0) { cout << "Bar()..." << age <<  endl;}
    Foo f;
private:
    int age;
};

int main () {
    Bar b;

    return 0;
}
