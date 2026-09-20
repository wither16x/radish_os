#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Boot::Bootloaders
{
        namespace LimineSpecific
        {
                struct Revision
                {
                        Lib::u64 magic0;
                        Lib::u64 magic1;
                        Lib::u64 rev;
                };

                struct RequestsStartMarker
                {
                        Lib::u64 magic0;
                        Lib::u64 magic1;
                        Lib::u64 magic2;
                        Lib::u64 magic3;
                };

                struct RequestsEndMarker
                {
                        Lib::u64 magic0;
                        Lib::u64 magic1;
                };
        } // namespace LimineSpecific

        class Limine
        {
                LimineSpecific::Revision base_revision;
                LimineSpecific::RequestsStartMarker requests_start;
                LimineSpecific::RequestsEndMarker requests_end;

        public:
                void init(this Limine &self);

                const LimineSpecific::RequestsStartMarker &requestsStart(this const Limine &self);
                const LimineSpecific::RequestsEndMarker &requestsEnd(this const Limine &self);
                const LimineSpecific::Revision &getVersionInfo(this const Limine &self);

                bool isBaseRevisionSupported(this const Limine &self);
        };
} // namespace Kiwi::Boot::Bootloaders