#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

// Use this class if a heap allocator is available and if you need
// dynamic strings.
// Otherwise, please keep it simple and use char pointers.
class String {
private:
        char *data;
        usize len;

        void cleanup(this String &self);

public:
        String();
        String(const char *buf);
        String(const String &other);
        String(String &&other);

        ~String();

        const char *raw(this const String &self);
        usize length(this const String &self);

        String operator +(this const String &self, const String &other);
        String operator +(this const String &self, char ch);
        String &operator =(this String &self, const String &other);
        String &operator =(this String &self, String &&other);
        String &operator +=(this String &self, const String &other);
        String &operator +=(this String &self, char ch);
        char operator [](this String &self, usize index);
        char operator [](this const String &self, usize index);
        bool operator ==(this const String &self, const String &other);
};

} /* namespace kernel::lib */