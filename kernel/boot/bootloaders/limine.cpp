#include <boot/bootloaders/limine.hpp>
#include <lib/memory.hpp>
#include <panic_simple.hpp>
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

                [[gnu::used, gnu::section(".limine_requests")]]
                volatile limine_bootloader_info_request l_bootloader_info = {
                        .id = LIMINE_BOOTLOADER_INFO_REQUEST_ID,
                        .revision = 0,
                        .response = nullptr
                };

                [[gnu::used, gnu::section(".limine_requests")]]
                volatile limine_firmware_type_request l_firmware_type = {
                        .id = LIMINE_FIRMWARE_TYPE_REQUEST_ID,
                        .revision = 0,
                        .response = nullptr
                };

                [[gnu::used, gnu::section(".limine_requests")]]
                volatile limine_memmap_request l_memmap_info = {
                        .id = LIMINE_MEMMAP_REQUEST_ID,
                        .revision = 0,
                        .response = nullptr
                };

                [[gnu::used, gnu::section(".limine_requests")]]
                volatile limine_hhdm_request l_hhdm_info = {
                        .id = LIMINE_HHDM_REQUEST_ID,
                        .revision = 0,
                        .response = nullptr      
                };

                [[gnu::used, gnu::section(".limine_requests")]]
                volatile limine_executable_address_request l_executable_info = {
                        .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
                        .revision = 0,
                        .response = nullptr
                };

                [[gnu::used, gnu::section(".limine_requests")]]
                volatile limine_module_request l_module_info = {
                        .id = LIMINE_MODULE_REQUEST_ID,
                        .revision = 1,
                        .response = nullptr,
                        .internal_module_count = 1,
                        .internal_modules = nullptr
                };

                [[gnu::used, gnu::section(".limine_requests")]]
                volatile limine_framebuffer_request l_framebuffer_request = {
                        .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
                        .revision = 0,
                        .response = nullptr
                };

                [[gnu::used, gnu::section(".limine_requests_start")]]
                volatile LimineSpecific::RequestsStartMarker limine_requests_start_marker = requestsStartMarker();

                [[gnu::used, gnu::section(".limine_requests_end")]]
                volatile LimineSpecific::RequestsEndMarker limine_requests_end_marker = requestsEndMarker();
        } // anonymous namespace

        void Limine::init(this Limine &self)
        {
                self.base_revision.magic0 = limine_base_revision.magic0;
                self.base_revision.magic1 = limine_base_revision.magic1;
                self.base_revision.rev = limine_base_revision.rev;

                if (not self.isBaseRevisionSupported())
                        panic_simple("limine base revision not supported");

                self.requests_start.magic0 = limine_requests_start_marker.magic0;
                self.requests_start.magic1 = limine_requests_start_marker.magic1;
                self.requests_start.magic2 = limine_requests_start_marker.magic2;
                self.requests_start.magic3 = limine_requests_start_marker.magic3;

                self.requests_end.magic0 = limine_requests_end_marker.magic0;
                self.requests_end.magic1 = limine_requests_end_marker.magic1;

                self.req_bootloader_info.name = l_bootloader_info.response->name;
                self.req_bootloader_info.version = l_bootloader_info.response->version;

                self.req_firmware_type.type = static_cast<FirmwareType>(l_firmware_type.response->firmware_type);

                self.req_memmap.entry_count = l_memmap_info.response->entry_count;
                for (Lib::usize i = 0; i < self.req_memmap.entry_count; i++) {
                        const MemmapEntry &e = {
                                .base = l_memmap_info.response->entries[i]->base,
                                .length = l_memmap_info.response->entries[i]->length,
                                .type = static_cast<MemmapEntryType>(l_memmap_info.response->entries[i]->type)
                        };

                        self.req_memmap.entries[i] = e;
                }

                self.req_hhdm.offset = l_hhdm_info.response->offset;

                self.req_executable_address.phys_base = l_executable_info.response->physical_base;
                self.req_executable_address.virt_base = l_executable_info.response->virtual_base;

                self.req_module.module_count = l_module_info.response->module_count;
                for (Lib::usize i = 0; i < self.req_module.module_count; i++) {
                        const Module &m = {
                                .address = l_module_info.response->modules[i]->address,
                                .size = l_module_info.response->modules[i]->size,
                                .path = l_module_info.response->modules[i]->path
                        };

                        self.req_module.modules[i] = m;
                }

                limine_framebuffer *l_fb = l_framebuffer_request.response->framebuffers[0];
                self.req_framebuffer.address = l_fb->address;
                self.req_framebuffer.pitch = l_fb->pitch;
                self.req_framebuffer.width = l_fb->width;
                self.req_framebuffer.height = l_fb->height;
                self.req_framebuffer.bpp = l_fb->bpp;
                self.req_framebuffer.red_mask_size = l_fb->red_mask_size;
                self.req_framebuffer.red_mask_shift = l_fb->red_mask_shift;
                self.req_framebuffer.green_mask_size = l_fb->green_mask_size;
                self.req_framebuffer.green_mask_shift = l_fb->green_mask_shift;
                self.req_framebuffer.blue_mask_size = l_fb->blue_mask_size;
                self.req_framebuffer.blue_mask_shift = l_fb->blue_mask_shift;
        }

        const Request &Limine::requestRaw(this const Limine &self, RequestType type)
        {
                switch (type) {
                case RequestType::BootloaderInfo:
                        return self.req_bootloader_info;
                case RequestType::FirmwareType:
                        return self.req_firmware_type;
                case RequestType::Memmap:
                        return self.req_memmap;
                case RequestType::Hhdm:
                        return self.req_hhdm;
                case RequestType::ExecutableAddress:
                        return self.req_executable_address;
                case RequestType::Module:
                        return self.req_module;
                case RequestType::Framebuffer:
                        return self.req_framebuffer;
                }

                panic_simple("invalid request");
        }

        const LimineSpecific::RequestsStartMarker &Limine::requestsStart(this const Limine &self)
        {
                return self.requests_start;
        }

        const LimineSpecific::RequestsEndMarker &Limine::requestsEnd(this const Limine &self)
        {
                return self.requests_end;
        }

        const LimineSpecific::Revision &Limine::getVersionInfo(this const Limine &self)
        {
                return self.base_revision;
        }

        bool Limine::isBaseRevisionSupported(this const Limine &self)
        {
                Lib::u64 _rev[] = {
                        self.base_revision.magic0,
                        self.base_revision.magic1,
                        self.base_revision.rev
                };
                
                return LIMINE_BASE_REVISION_SUPPORTED(_rev);
        }
} // namespace Kiwi::Boot::Bootloaders