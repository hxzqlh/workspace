#include <stdio.h>
#include <assert.h>
#include <memory>
#include <exception>
#include <stdexcept>

using namespace std;

class foo: public std::enable_shared_from_this <foo>
{
public:
    foo () {throw bad_alloc ();}
};

int main (int argc, char * argv[]) 
{
    try {
        shared_ptr<foo> sp_foo;
        assert (!sp_foo);
        printf ("%p\n", sp_foo.get ());
        sp_foo.reset (new foo);
        assert (!sp_foo);
        printf ("%p\n", sp_foo.get ());
    } catch (exception & ex) {
        printf ("%s\n", ex.what());
    }
} 
