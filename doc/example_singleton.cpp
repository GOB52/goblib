#include <cstdio>
#include <gob_singleton.hpp>

class Foo : public goblib::Singleton<Foo>
{
  public:
    virtual void print() const { printf("I am Foo.\n"); }
    virtual ~Foo(){}

  protected:
    friend class goblib::Singleton<Foo>;
    Foo() : Singleton<Foo>() { printf("%s:\n", __func__);}

};

class Bar : public Foo, public goblib::Singleton<Bar>
{
  public:
    // Resolve ambiguities.
    using PointerType = std::unique_ptr<Bar>;
    using goblib::Singleton<Bar>::instance;
    using goblib::Singleton<Bar>::create;

    virtual void print() const override { printf("I am Bar.\n"); }
    virtual ~Bar(){}

  protected:
    friend class goblib::Singleton<Bar>;
    Bar() : Foo() { printf("%s:\n", __func__);}

};

int main()
{
    // Create instance and print
    Foo::instance().print(); // Foo\n I am Foo\n
    Bar::instance().print(); // Foo\n Bar\n I am Bar\n

    // Already created
    Foo::instance().print(); // I am Foo\n
    Bar::instance().print(); // I am Bar\n

    return 0;
}

