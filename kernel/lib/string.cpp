#include <lib/memory.hpp>
#include <lib/string.hpp>

namespace Kiwi::Lib
{
        String::String()
        {
                this->data.pushBack('\0');
        }

        String::String(const char *buf)
        {
                while (*buf)
                        this->data.pushBack(*buf++);

                this->data.pushBack('\0');
        }

        const char *String::raw(this const String &self)
        {
                return self.data.getData();
        }

        usize String::length(this const String &self)
        {
                // avoid returning -1
                if (self.data.empty())
                        return 0;

                return self.data.size() - 1;
        }

        String String::sub(this const String &self, usize start)
        {
                String s;

                s.data.popBack();
                usize len = self.data.size();

                if (start >= len)
                        return s;

                for (usize i = start; i < len; ++i)
                        s.data.pushBack(self.data[i]);
                s.data.pushBack('\0');

                return s;
        }

        String String::operator +(this const String &self, const String &other)
        {
                String s = self;

                s.data.popBack();

                for (usize i = 0; i < other.data.size(); ++i)
                        s.data.pushBack(other.data[i]);
                s.data.pushBack('\0');

                return s;
        }

        String String::operator +(this const String &self, char ch)
        {
                String s = self;

                s.data.popBack();
                s.data.pushBack(ch);
                s.data.pushBack('\0');

                return s;
        }

        String &String::operator +=(this String &self, const String &other)
        {
                self = self + other;
                return self;
        }

        String &String::operator +=(this String &self, char ch)
        {
                self = self + ch;
                return self;
        }

        char String::operator [](this String &self, usize index)
        {
                return self.data[index];
        }

        char String::operator [](this const String &self, usize index)
        {
                return self.data[index];
        }

        bool String::operator ==(this const String &self, const String &other)
        {
                return strcmp(self.data.getData(), other.data.getData()) == 0;
        }
} // namespace Kiwi::Lib