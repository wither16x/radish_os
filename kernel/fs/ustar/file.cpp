#include <fs/vfs/status.hpp>
#include <fs/ustar/file.hpp>
#include <fs/ustar/node.hpp>

namespace Kiwi::Fs::Ustar
{
        Vfs::Status File::read(void *buf, Lib::usize n)
        {
                if (not this->owner or not this->owner->storage or not this->owner->storage->data)
                        return Vfs::Status::NullData;
                if (this->cursor >= this->owner->storage->size)
                        return Vfs::Status::Eof;

                Lib::usize rem = this->owner->storage->size - this->cursor;
                Lib::usize to_copy = n < rem ? n : rem;

                Lib::memcpy(buf, reinterpret_cast<Lib::u8 *>(this->owner->storage->data) + this->cursor, to_copy);
                this->cursor += to_copy;

                return Vfs::Status::Success;
        }

        Vfs::Status File::close()
        {
                return Vfs::Status::Success;
        }
} // namespace Kiwi::Fs::Ustar