#include <boot/limine.hpp>
#include <limine.h>

namespace Kiwi::Boot::Limine
{
        namespace
        {
                [[gnu::used, gnu::section(".limine_requests")]]
                volatile Revision limine_base_revision = baseRevision(6);

                [[gnu::used, gnu::section(".limine_requests_start")]]
                volatile StartMarker limine_requests_start_marker = requestsStartMarker();

                [[gnu::used, gnu::section(".limine_requests_end")]]
                volatile EndMarker limine_requests_end_marker = requestsEndMarker();
        } // anonymous namespace

        Revision baseRevision(Lib::u64 n)
        {
                return LIMINE_BASE_REVISION(n);
        }

        bool Revision::isSupported(this volatile Revision &self)
        {
                Lib::u64 _rev[] = {
                        self.magic0,
                        self.magic1,
                        self.rev
                };
                
                return LIMINE_BASE_REVISION_SUPPORTED(_rev);
        }

        StartMarker requestsStartMarker()
        {
                return LIMINE_REQUESTS_START_MARKER;
        }

        EndMarker requestsEndMarker()
        {
                return LIMINE_REQUESTS_END_MARKER;
        }

        volatile Revision &getBaseRevision()
        {
                return limine_base_revision;
        }
} // namespace Kiwi::Boot::Limine