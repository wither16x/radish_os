#pragma once

namespace Kiwi::Fs::Vfs
{
        enum class Status
        {
                Success,
                FsMounted,
                FsNotMounted,
                NullNode,
                NoRefs,
                NullFile,
                PathTooShort,
                NullRoot,
                EmptyPath,
                ChildNotFound,
                IsADirectory,
                NullData,
                OutOfBounds,
                NotADirectory,
                NotImplemented,
                Eof
        };
} // namespace Kiwi::Fs::Vfs