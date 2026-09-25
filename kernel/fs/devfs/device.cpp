#include <fs/devfs/device.hpp>
#include <fs/devfs/node.hpp>
#include <fs/vfs/file.hpp>

namespace Kiwi::Fs::Devfs
{
        void registerDevice(DeviceType type, const Lib::String<> &path)
        {
                Vfs::mkfile(path);
                Lib::String<> devname = path.subString(3);
                Node *dev = reinterpret_cast<Node *>(root->lookup(devname));
                if (dev)
                        dev->devtype = type;
        }
} // namespace Kiwi::Fs::Devfs