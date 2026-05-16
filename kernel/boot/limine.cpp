#include <boot/limine.hpp>
#include <limine.h>

namespace kernel::boot::limine {

revision base_revision(lib::u64 n)
{
        return LIMINE_BASE_REVISION(n);
}

bool revision::is_supported(this volatile revision& self)
{
        lib::u64 _rev[] = {self.magic0, self.magic1, self.rev};
        return LIMINE_BASE_REVISION_SUPPORTED(_rev);
}

start_marker requests_start_marker()
{
        return LIMINE_REQUESTS_START_MARKER;
}

end_marker requests_end_marker()
{
        return LIMINE_REQUESTS_END_MARKER;
}

} /* namespace kernel::boot::limine */