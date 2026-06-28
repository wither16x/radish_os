#include <boot/limine.hpp>
#include <limine.h>

using kernel::lib::u64;

namespace kernel::boot::limine {

namespace {

[[gnu::used, gnu::section(".limine_requests")]]
volatile Revision limine_base_revision = base_revision(6);

[[gnu::used, gnu::section(".limine_requests_start")]]
volatile StartMarker limine_requests_start_marker = requests_start_marker();

[[gnu::used, gnu::section(".limine_requests_end")]]
volatile EndMarker limine_requests_end_marker = requests_end_marker();

} /* anonymous namespace */

// --------------------------------------------------
Revision base_revision(u64 n)
{
        return LIMINE_BASE_REVISION(n);
}
// --------------------------------------------------

// --------------------------------------------------
bool Revision::is_supported(this volatile Revision &self)
{
        u64 _rev[] = {self.magic0, self.magic1, self.rev};
        return LIMINE_BASE_REVISION_SUPPORTED(_rev);
}
// --------------------------------------------------

// --------------------------------------------------
StartMarker requests_start_marker()
{
        return LIMINE_REQUESTS_START_MARKER;
}
// --------------------------------------------------

// --------------------------------------------------
EndMarker requests_end_marker()
{
        return LIMINE_REQUESTS_END_MARKER;
}
// --------------------------------------------------

volatile Revision &get_base_revision()
{
        return limine_base_revision;
}

} /* namespace kernel::boot::limine */