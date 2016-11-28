#include <iostream>
#include <string>
#include <thread>
#include <assert.h>

using namespace std;

class Foo
{
public:
	void bar (int id, string& name) {
        name += "+1s";
		cout << "id=" << id << ",name=" << name << endl;
	}
};

void func ()
{
	cout << "thread func" << endl;
}

void func_with_args (int& a)
{
    cout << "a=" << a <<  endl;
}

int main_thread ()
{
	Foo foo;
	int id = 1;
	string name("elder");

    thread th_1 (func); //thread func
    th_1.join ();
    
    thread th_2 (&Foo::bar, foo, id, ref(name));//id=1,name=elder+1s
    th_2.join ();
    
    thread th_3 (bind (&Foo::bar, foo, id, name));//id=1,name=elder+1s+1s
    th_3.join ();
    
    thread th_4 ([&]{ foo.bar (id, name); });//id=1,name=elder+1s+1s
    th_4.join ();

	return 0;
}
