#include <string.hpp>
#include <typing.hpp>

namespace Melon::String
{
        String::String()
        {
                this->chars.pushBack('\0');
        }

        String::String(const char *s)
        {
                while (*s)
                        this->chars.pushBack(*s++);
                this->chars.pushBack('\0');
        }

        String::String(const String &other)
        {
                this->chars = other.chars;
        }

        String::String(String &&other)
        {
                this->chars = other.chars;
                other.chars.clear();
                other.chars.pushBack('\0');
        }

        String String::substract(this String &self, Typing::USize chars)
        {
                String tmp = self;

                tmp.chars.popBack(); // remove null terminator
                for (Typing::USize i = 0; i < chars; i++)
                        tmp.chars.popBack();
                tmp.chars.pushBack('\0'); // add null terminator

                return tmp;
        }

        const char *String::raw(this const String &self)
        {
                return self.chars.data();
        }

        Typing::USize String::length(this const String &self)
        {
                return strlen(self.chars.data());
        }

        const char *String::begin(this const String &self)
        {
                return self.chars.begin();
        }

        const char *String::end(this const String &self)
        {
                return self.chars.end();
        }

        void String::appendChar(this String &self, char ch)
        {
                self.chars.popBack();
                self.chars.pushBack(ch);
                self.chars.pushBack('\0');
        }

        String String::reverse(this String &self)
        {
                String new_string = self;

                Typing::USize l = 0;
                Typing::USize r = new_string.length() - 1;
                char t;

                while (l < r) {
                        t = new_string[l];
                        new_string[l] = new_string[r];
                        new_string[r] = t;

                        ++l;
                        --r;
                }

                return new_string;
        }

        String String::ltrim(this String &self, char ch)
        {
                String new_string = self;
                Typing::USize i = 0;

                while (new_string[i] == ch)
                        new_string.chars.erase(i);

                return new_string;
        }

        String String::rtrim(this String &self, char ch)
        {
                String new_string = self;
                Typing::USize i = new_string.length() - 1;

                while (i > 0) {
                        if (new_string[i] == ch)
                                --i;
                        else
                                break;
                }

                new_string[i + 1] = '\0';

                return new_string;
        }

        String String::trim(this String &self, char ch)
        {
                String new_string = self;
                new_string = new_string.ltrim(ch);
                new_string = new_string.rtrim(ch);
                return new_string;
        }

        char &String::operator [](this String &self, Typing::USize index)
        {
                return self.chars[index];
        }

        const char &String::operator [](this const String &self, Typing::USize index)
        {
                return self.chars[index];
        }

        bool String::operator ==(this const String &self, const String &other)
        {
                return strcmp(self.chars.data(), other.chars.data()) == 0;
        }

        bool String::operator !=(this const String &self, const String &other)
        {
                return strcmp(self.chars.data(), other.chars.data()) != 0;
        }

        bool String::operator <(this const String &self, const String &other)
        {
                return strcmp(self.chars.data(), other.chars.data()) < 0;
        }

        bool String::operator >(this const String &self, const String &other)
        {
                return strcmp(self.chars.data(), other.chars.data()) > 0;
        }

        bool String::operator <=(this const String &self, const String &other)
        {
                return strcmp(self.chars.data(), other.chars.data()) <= 0;
        }

        bool String::operator >=(this const String &self, const String &other)
        {
                return strcmp(self.chars.data(), other.chars.data()) >= 0;
        }

        String String::operator +(this const String &self, const String &other)
        {
                String tmp = self;

                tmp.chars.popBack(); // remove null terminator
                for (Typing::USize i = 0; i < other.length(); ++i)
                        tmp.chars.pushBack(other.chars[i]);
                tmp.chars.pushBack('\0');

                return tmp;
        }

        String &String::operator =(this String &self, const String &other)
        {
                if (self != other)
                        self.chars = other.chars;

                return self;
        }

        String &String::operator =(this String &self, String &&other)
        {
                if (self != other) {
                        self.chars = other.chars;
                        other.chars.clear();
                        other.chars.pushBack('\0');
                }

                return self;
        }

        String &String::operator +=(this String &self, const String &other)
        {
                self = self + other;
                return self;
        }
} // namespace Melon::String