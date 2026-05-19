#include <boot/limine.hpp>
#include <limine.h>

namespace kernel::boot::limine {

Revision base_revision(lib::u64 n)
{
        return LIMINE_BASE_REVISION(n);
}

bool Revision::is_supported(this volatile Revision &self)
{
        lib::u64 _rev[] = {self.magic0, self.magic1, self.rev};
        return LIMINE_BASE_REVISION_SUPPORTED(_rev);
}

StartMarker requests_start_marker()
{
        return LIMINE_REQUESTS_START_MARKER;
}

EndMarker requests_end_marker()
{
        return LIMINE_REQUESTS_END_MARKER;
}

} /* namespace kernel::boot::limine */