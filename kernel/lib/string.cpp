#include <lib/memory.hpp>
#include <lib/string.hpp>

namespace kernel::lib {

// private
// -------------------------------------------------------------
void String::cleanup(this String &self)
{
        if (self.data)
                delete[] self.data;
        self.len = 0;
}
// -------------------------------------------------------------
//
// public
// -------------------------------------------------------------
String::String()
{
        this->len = 0;
        this->data = new char;
        this->data[0] = '\0';
}

String::String(const char *buf)
{
        this->len = strlen(buf);
        this->data = new char[this->len + 1];   // add 1 byte for the null character
        strcpy(buf, this->data);
}

String::String(const String &other)
{
        this->len = other.len;
        this->data = new char[this->len + 1];
        strcpy(other.data, this->data);
}

String::String(String &&other)
{
        this->cleanup();
        this->len = other.len;
        this->data = other.data;
        other.data = nullptr;
}

String::~String()
{
        this->cleanup();
}

const char *String::raw(this const String &self)
{
        return self.data;
}

usize String::length(this const String &self)
{
        return self.len;
}

String String::operator +(this const String &self, const String &other)
{
        String s;
        s.len = self.len + other.len;
        s.data = new char[s.len + 1];
        strcpy(self.data, s.data);
        strcpy(other.data, s.data + self.len);
        return s;
}

String String::operator +(this const String &self, char ch)
{
        String s;
        s.len = self.len + 1;
        s.data = new char[s.len + 1];
        if (self.data && self.len > 0)
                strcpy(self.data, s.data);
        s.data[self.len] = ch;
        s.data[self.len + 1] = '\0';
        return s;
}

String &String::operator =(this String &self, const String &other)
{
        self.cleanup();
        self.len = other.len;
        self.data = new char[self.len + 1];
        strcpy(other.data, self.data);
        return self;
}

String &String::operator =(this String &self, String &&other)
{
        self.cleanup();
        self.len = other.len;
        self.data = other.data;
        other.data = nullptr;
        return self;
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
        if (index >= self.len)
                return '\0';

        return self.data[index];
}

char String::operator [](this const String &self, usize index)
{
        if (index >= self.len)
                return '\0';

        return self.data[index];
}

bool String::operator ==(this const String &self, const String &other)
{
        return strcmp(self.data, other.data) == 0;
}

// -------------------------------------------------------------

} /* namespace kernel::lib */