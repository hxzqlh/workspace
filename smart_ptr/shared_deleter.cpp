#include <iostream>
#include <boost/shared_ptr.hpp>

// a verbose array deleter
struct D {
  void operator () (int* p) {
    std::cout << p << "\n";
    std::cout << "[deleter called]\n";
    delete [] p;
  }
};

int main () {
  boost::shared_ptr<int> foo (new int[10], D ());

  int * bar = new int[20];

  // use foo's deleter to delete bar (which is unmanaged):
  (*boost::get_deleter <D> (foo)) (bar);
  std::cout << "xxxxxxx\n";

  return 0;
  // foo's deleter called automatically
}
