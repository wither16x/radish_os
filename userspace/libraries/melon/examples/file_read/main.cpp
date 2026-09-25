#include <melon/print.hpp>
#include <melon/filesystem.hpp>

using namespace Melon;

int main(int argc, char **argv)
{
        if (argc < 2) {
                Print::println("usage: file_read <pathname>");
                return 0;
        }

        FileSystem::File file(argv[1], "r");

        int line_count = 0;
        int word_count = 0;

        String::String data = file.read().get();
        for (auto ch : data) {
                if (ch == '\n') {
                        ++line_count;
                        ++word_count;
                }

                if (ch == ' ' or ch == '\t')
                        ++word_count;
        }

        if (data.length() != 0 and data[data.length() - 1] != '\n')
                ++line_count;

        Print::println("{} lines and {} words", line_count, word_count);
}