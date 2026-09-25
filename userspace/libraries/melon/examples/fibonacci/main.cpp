#include <melon/vector.hpp>
#include <melon/print.hpp>

using namespace Melon;

Vector::Vector<int> fib(int n)
{
        Vector::Vector<int> vec;
        if (n == 0)
                return vec;

        vec.pushBack(0);
        if (n == 1)
                return vec;

        vec.pushBack(1);
        for (int i = 2; i < n; ++i)
                vec.pushBack(vec[i - 1] + vec[i - 2]);

        return vec;
}

int main()
{
        auto sequence = fib(15);

        for (auto n : sequence)
                Print::println("{}", n);
}