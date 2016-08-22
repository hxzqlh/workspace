#include <thread>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

void func_with_args (int i, double d, const string& s)
{
    cout << i << ", " << d << ", " << s << endl;
}

void func_ref (int& a)
{
    a++;
}

int main ()
{
    thread t2 (func_with_args, 1, 2.5, "hxz");
    t2.join ();

    int a = 42;
    thread t3 (func_ref, ref(a));
    t3.join ();

    cout << "a=" << a << endl;

    return 0;
}
