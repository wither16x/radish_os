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

String String::operator +(this String &self, const String &other)
{
        String s;
        s.len = self.len + other.len;
        s.data = new char[s.len + 1];
        strcpy(self.data, s.data);
        strcpy(other.data,s.data + self.len);
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
// -------------------------------------------------------------

} /* namespace kernel::lib */