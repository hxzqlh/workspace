#include <iostream>
#include <dlfcn.h>
#include "test_base.hpp"
 
int main(int argc , char** argv) 
{
    if(argc!=2)
    {
        cout << argv[0]  << " so" << endl;
        return 1;
    }
 
    void* test_index = dlopen(argv[1], RTLD_NOW);
    if (!test_index) {
        cerr << "Cannot load library: " << dlerror() << '\n';
        return 1;
    }
 
    dlerror();
   
    create_t* create_test = (create_t*) dlsym(test_index, "create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }
   
    destroy_t* destroy_test = (destroy_t*) dlsym(test_index, "destroy");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
        return 1;
    }
 
    test_base* c_test = create_test();
 
    c_test->display();
 
    destroy_test(c_test);
 
    dlclose(test_index);
}
