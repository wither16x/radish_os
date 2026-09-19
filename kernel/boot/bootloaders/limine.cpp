#include <boot/bootloaders/limine.hpp>
#include <limine.h>

namespace Kiwi::Boot::Bootloaders
{
        namespace
        {
                LimineSpecific::Revision baseRevision(Lib::u64 n)
                {
                        Lib::u64 l_rev[] = LIMINE_BASE_REVISION(n);
                        LimineSpecific::Revision rev = {
                                .magic0 = l_rev[0],
                                .magic1 = l_rev[1],
                                .rev = l_rev[2]
                        };
                        
                        return rev;
                }

                LimineSpecific::RequestsStartMarker requestsStartMarker()
                {
                        Lib::u64 l_start_marker[] = LIMINE_REQUESTS_START_MARKER;
                        LimineSpecific::RequestsStartMarker start_marker = {
                                .magic0 = l_start_marker[0],
                                .magic1 = l_start_marker[1],
                                .magic2 = l_start_marker[2],
                                .magic3 = l_start_marker[3]
                        };
                        
                        return start_marker;
                }

                LimineSpecific::RequestsEndMarker requestsEndMarker()
                {
                        Lib::u64 l_end_marker[] = LIMINE_REQUESTS_END_MARKER;
                        LimineSpecific::RequestsEndMarker end_marker = {
                                .magic0 = l_end_marker[0],
                                .magic1 = l_end_marker[1]
                        };
                        
                        return end_marker;
                }

                [[gnu::used, gnu::section(".limine_requests")]]
                volatile LimineSpecific::Revision limine_base_revision = baseRevision(6);

                [[gnu::used, gnu::section(".limine_requests_start")]]
                volatile LimineSpecific::RequestsStartMarker limine_requests_start_marker = requestsStartMarker();

                [[gnu::used, gnu::section(".limine_requests_end")]]
                volatile LimineSpecific::RequestsEndMarker limine_requests_end_marker = requestsEndMarker();
        } // anonymous namespace

        void Limine::init(const Lib::String<NAME_SIZE> &name, const Lib::String<VERSION_SIZE> &version)
        {
                this->name = name;
                this->version = version;
        }

        const volatile LimineSpecific::RequestsStartMarker &Limine::requestsStart()
        {
                return limine_requests_start_marker;
        }

        const volatile LimineSpecific::RequestsEndMarker &Limine::requestsEnd()
        {
                return limine_requests_end_marker;
        }

        const volatile LimineSpecific::Revision &Limine::getVersionInfo()
        {
                return limine_base_revision;
        }

        bool Limine::isBaseRevisionSupported() const
        {
                Lib::u64 _rev[] = {
                        limine_base_revision.magic0,
                        limine_base_revision.magic1,
                        limine_base_revision.rev
                };
                
                return LIMINE_BASE_REVISION_SUPPORTED(_rev);
        }
} // namespace Kiwi::Boot::Bootloaders