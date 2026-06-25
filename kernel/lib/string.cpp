#include <lib/memory.hpp>
#include <lib/string.hpp>

namespace kernel::lib {

// --------------------------------------------------
String::String()
{
        this->data.push_back('\0');
}
// --------------------------------------------------

// --------------------------------------------------
String::String(const char *buf)
{
        while (*buf)
                this->data.push_back(*buf++);

        this->data.push_back('\0');
}
// --------------------------------------------------

// --------------------------------------------------
const char *String::raw(this const String &self)
{
        return self.data.get_data();
}
// --------------------------------------------------

// --------------------------------------------------
usize String::length(this const String &self)
{
        // avoid returning -1
        if (self.data.size() == 0)
                return 0;

        return self.data.size() - 1;
}
// --------------------------------------------------

// --------------------------------------------------
String String::sub(this const String &self, usize start)
{
        String s;

        s.data.pop_back();
        usize len = self.data.size();

        if (start >= len)
                return s;

        for (usize i = start; i < len; ++i)
                s.data.push_back(self.data[i]);
        s.data.push_back('\0');

        return s;
}
// --------------------------------------------------

// --------------------------------------------------
String String::operator +(this const String &self, const String &other)
{
        String s = self;

        s.data.pop_back();

        for (usize i = 0; i < other.data.size(); ++i)
                s.data.push_back(other.data[i]);
        s.data.push_back('\0');

        return s;
}
// --------------------------------------------------

// --------------------------------------------------
String String::operator +(this const String &self, char ch)
{
        String s = self;

        s.data.pop_back();
        s.data.push_back(ch);
        s.data.push_back('\0');

        return s;
}
// --------------------------------------------------

// --------------------------------------------------
String &String::operator +=(this String &self, const String &other)
{
        self = self + other;
        return self;
}
// --------------------------------------------------

String &String::operator +=(this String &self, char ch)
{
        self = self + ch;
        return self;
}
// --------------------------------------------------

// --------------------------------------------------
char String::operator [](this String &self, usize index)
{
        return self.data[index];
}
// --------------------------------------------------

// --------------------------------------------------
char String::operator [](this const String &self, usize index)
{
        return self.data[index];
}
// --------------------------------------------------

// --------------------------------------------------
bool String::operator ==(this const String &self, const String &other)
{
        return strcmp(self.data.get_data(), other.data.get_data()) == 0;
}
// --------------------------------------------------

} /* namespace kernel::lib */