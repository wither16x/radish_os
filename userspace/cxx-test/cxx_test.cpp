#include <cstdio>
#include <cstdlib>
#include <Melon/Exceptions.hpp>

class Person
{
        const char *name;
        unsigned int age;

public:
        Person(const char *name, unsigned int age)
                : name(name), age(age)
        {}

        void greet() const
        {
                std::printf("Hello, I am %s and I am %d!\n", this->name, this->age);
        }
};

int main()
{
        Person p("John", 42);
        p.greet();

        try {
                throw Melon::Exceptions::BufferOverflow("buffer overflow detected");
        } catch (const Melon::Exceptions::BufferOverflow &e) {
                std::printf("caught %s\n", e.what());
        }

        return 0;
}