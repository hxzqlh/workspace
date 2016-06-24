#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class Good: public boost::enable_shared_from_this <Good> 
{
public:
    boost::shared_ptr<Good> getptr() {
        return shared_from_this();
    }
};
 
class Bad
{
public:
    boost::shared_ptr<Bad> getptr() {
        return boost::shared_ptr<Bad>(this);
    }
    ~Bad() { std::cout << "Bad::~Bad() called\n"; }
};
 
int main()
{
    // Good: the two shared_ptr's share the same object
    boost::shared_ptr<Good> gp1(new Good);
    boost::shared_ptr<Good> gp2 = gp1->getptr();
    std::cout << "gp2.use_count() = " << gp2.use_count() << '\n';
 
    // Bad, each shared_ptr thinks it's the only owner of the object
    boost::shared_ptr<Bad> bp1 (new Bad);
    //boost::shared_ptr<Bad> bp2 = bp1->getptr();
    boost::shared_ptr<Bad> bp2 = bp1;
    std::cout << "bp1.use_count() = " << bp1.use_count() << '\n';
    std::cout << "bp2.use_count() = " << bp2.use_count() << '\n';

    return 0;
} // UB: double-delete of Bad
