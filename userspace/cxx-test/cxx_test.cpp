#include <cstdio>
#include <cstdlib>
#include <Melon/Vector.hpp>

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

        Melon::Vector::Vector<int> integers = {{0, 1, 2, 3, 4, 5}};

        std::printf("integers are:\n");
        for (auto &i : integers)
                std::printf("* %d\n", i);

        return 0;
}