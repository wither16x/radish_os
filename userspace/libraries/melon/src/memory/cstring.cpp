#include <internal/memory/cstring.hpp>
#include <internal/exceptions/out_of_range.hpp>
#include <typing.hpp>

#include <string.h>

namespace Melon::Memory
{
        CString::CString(const char *s)
        {
                this->data = new char[strlen(s) + 1];
                strncpy(data, s, strlen(s) + 1);
        }

        CString::~CString()
        {
                if (this->data)
                        delete[] this->data;
        }

        CString &CString::copy(this const CString &self, CString &dest, Typing::USize chars)
        {
                strncpy(dest.data, self.data, chars);
                return dest;
        }

        bool CString::has(this const CString &self, char ch)
        {
                for (auto &c : self) {
                        if (c == ch)
                                return true;
                }
                return false;
        }

        Typing::USize CString::length(this const CString &self)
        {
                return strlen(self.data);
        }

        const char *CString::begin(this const CString &self)
        {
                return self.data;
        }

        const char *CString::end(this const CString &self)
        {
                return self.data + strlen(self.data);
        }

        const char *CString::get(this const CString &self)
        {
                return self.data;
        }

        bool CString::operator ==(this const CString &self, const CString &other)
        {
                return strcmp(self.data, other.data) == 0;
        }

        bool CString::operator !=(this const CString &self, const CString &other)
        {
                return strcmp(self.data, other.data) != 0;
        }

        bool CString::operator <(this const CString &self, const CString &other)
        {
                return strcmp(self.data, other.data) < 0;
        }

        bool CString::operator >(this const CString &self, const CString &other)
        {
                return strcmp(self.data, other.data) > 0;
        }

        bool CString::operator <=(this const CString &self, const CString &other)
        {
                return self < other or self == other;
        }

        bool CString::operator >=(this const CString &self, const CString &other)
        {
                return self > other or self == other;
        }

        char &CString::operator [](this CString &self, Typing::USize index)
        {
                Typing::USize length = strlen(self.data);
                if (index >= length)
                        throw Exceptions::OutOfRange(index, length);

                return self.data[index];
        }
} // namespace Melon::Memory