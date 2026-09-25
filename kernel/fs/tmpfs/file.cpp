#include <fs/tmpfs/file.hpp>
#include <fs/tmpfs/node.hpp>

namespace Kiwi::Fs::Tmpfs
{
        Vfs::Status File::read(void *buf, Lib::usize n)
        {
                if (not this->owner or not this->owner->storage)
                        return Vfs::Status::NullData;
                if (this->owner->storage->size >= n)
                        return Vfs::Status::OutOfBounds;

                Lib::memcpy(buf, this->owner->storage->data, this->owner->storage->size);

                return Vfs::Status::Success;
        }

        Vfs::Status File::write(const void *buf, Lib::usize n)
        {
                if (not this->owner or not this->owner->storage)
                        return Vfs::Status::NullData;

                Storage *storage = this->owner->storage;
                if (storage->data)
                        delete[] storage->data;

                storage->data = new char[n + 1];
                storage->size = n;

                Lib::memcpy(storage->data, buf, n);

                return Vfs::Status::Success;
        }

        Vfs::Status File::close()
        {
                if (this->owner)
                        this->owner->file_data = nullptr;

                return Vfs::Status::Success;
        }
} // namespace Kiwi::Fs::Tmpfs