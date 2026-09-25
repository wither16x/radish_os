#include <melon/string.hpp>
#include <melon/print.hpp>

using namespace Melon;

int main()
{
        String::String filename = "  example (2).txt  ";

        Print::println("Original filename: \"{}\"", filename);
        filename = filename.trim();
        Print::println("Trimmed filename: \"{}\"", filename);
}