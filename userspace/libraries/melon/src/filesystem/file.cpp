#include <internal/filesystem/file.hpp>
#include <internal/exceptions/file_not_found.hpp>
#include <internal/exceptions/null_stream.hpp>
#include <string.hpp>

#include <utility>

#include <stdio.h>
#include <unistd.h>

namespace Melon::FileSystem
{
        File::File(const String::String &path, const String::String &modes)
        {
                this->stream = fopen(path.raw(), modes.raw());
                if (not this->stream)
                        throw Exceptions::FileNotFound(path);
        }

        File::File(FILE *stream)
                : stream(stream)
        {
                if (not this->stream)
                        throw Exceptions::NullStream();
        }

        File::~File()
        {
                this->close();
        }

        void File::close(this File &self)
        {
                if (self.isOpen()) {
                        fclose(self.stream);
                        self.stream = nullptr;
                }
        }

        Memory::Buffer<char> File::readBytes(this File &self, Typing::USize bytes)
        {
                char *p = new char[bytes];

                self.seek(0, SeekOrigin::Begin);
                ::read(fileno(self.stream), p, bytes);
                Memory::Buffer<char> buf(p, bytes);
                delete[] p;
                return buf;
        }

        Memory::Buffer<char> File::read(this File &self)
        {
                Vector::Vector<char> bytes;
                char ch;

                self.seek(0, SeekOrigin::Begin);

                while ((ch = fgetc(self.stream)) != EOF)
                        bytes.pushBack(ch);

                return bytes.toBuffer();
        }

        String::String File::readLine(this File &self, Typing::USize lineno)
        {
                int ch = '\0';
                Typing::USize lf_count = 0;
                String::String str;

                self.seek(0, SeekOrigin::Begin);

                while (lf_count < lineno) {
                        ch = fgetc(self.stream);

                        if (ch == EOF)
                                return str;

                        if (ch == '\n')
                                ++lf_count;
                }

                while ((ch = fgetc(self.stream)) != EOF and ch != '\n')
                        str.appendChar(ch);

                return str;
        }

        int File::descriptor(this const File &self)
        {
                return fileno(self.stream);
        }

        void File::seek(this File &self, long offset, SeekOrigin origin)
        {
                fseek(self.stream, offset, std::to_underlying(origin));
        }

        void File::write(this File &self, const Memory::Buffer<char> &buf, Typing::USize bytes)
        {
                ::write(fileno(self.stream), buf.get(), bytes);
        }

        bool File::isOpen(this const File &self)
        {
                return self.stream != nullptr;
        }
} // namespace Melon::FileSystem