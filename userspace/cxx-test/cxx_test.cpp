#include <Melon/Print.hpp>

using namespace Melon;

int main() {
        String::String firstname = "John";
        String::String lastname = "Doe";
        int age = 42;
        Print::println("Hello! My name is {} {} and I am {} years old!", firstname, lastname, age);
}