#include <iostream>
using namespace std;
class Base
{
public:
    friend ostream &operator<<(ostream &os, Base &base)
    {
        os << "base";
        return os;
    }
};
class Derived : public Base
{
public:
    int m_a;
    friend ostream &operator<<(ostream &os, Derived &d)
    {
        os << "derived:" << d.m_a;
        return os;
    }
};
int main(int argc, char const *argv[])
{
    Base *b = new Derived();
    cout << b << endl;
    return 0;
}
