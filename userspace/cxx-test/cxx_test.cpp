#include <cstdio>

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

        return 0;
}