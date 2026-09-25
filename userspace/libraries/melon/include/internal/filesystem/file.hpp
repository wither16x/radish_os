#pragma once

#include "../../string.hpp"
#include "../memory/buffer.hpp"

#include <stdio.h>

namespace Melon::FileSystem
{
        /// @brief All possible values for File::seek().
        enum class SeekOrigin
        {
                Begin           = SEEK_SET,     ///< Beginning of the file
                Curr            = SEEK_CUR,     ///< Current cursor position
                End             = SEEK_END      ///<End of the file
        };

        /// @brief Representation of a file stream.
        class File
        {
                FILE *stream;

        public:
                /// @brief Open constructor.
                /// @param path path to the file
                /// @param modes open modes
                File(const String::String &path, const String::String &modes);

                /// @brief Constructs the object from an existing stream.
                /// @param stream base stream
                File(FILE *stream);

                /// @brief Destructor.
                ~File();

                /// @brief Close the file if it is open.
                void close(this File &self);

                /// @brief Read bytes from the file.
                /// @param bytes bytes count
                /// @return buffer containing the read bytes
                Memory::Buffer<char> readBytes(this File &self, Typing::USize bytes);

                /// @brief Read until EOF.
                /// @return buffer containing the read bytes
                Memory::Buffer<char> read(this File &self);

                /// @brief Read a line from the file.
                ///
                /// The \\n is not preserved.
                /// @param lineno 0-based line number
                /// @return string containing the line
                String::String readLine(this File &self, Typing::USize lineno);

                /// @brief Gets the file descriptor of this stream.
                /// @return file descriptor
                int descriptor(this const File &self);

                /// @brief Seek to a certain position in the file.
                /// @param offset offset from origin
                /// @param origin seek from here
                void seek(this File &self, long offset, SeekOrigin origin);

                /// @brief Write bytes to a file.
                /// @param buf bytes to write
                /// @param bytes byte count
                void write(this File &self, const Memory::Buffer<char> &buf, Typing::USize bytes);

                /// @brief Checks if the file is open.
                ///
                /// If the stream is not null, then it is considered as open.
                /// @return boolean
                bool isOpen(this const File &self);
        };
} // namespace Melon::FileSystem