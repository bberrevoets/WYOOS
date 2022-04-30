#ifndef __WYOOS__GDT_H
#define __WYOOS__GDT_H

#include <common/types.h>
namespace wyoos
{

    class GlobalDescriptorTable
    {
    public:
        class SegmentDescriptor
        {
        private:
            wyoos::common::uint16_t limit_lo;
            wyoos::common::uint16_t base_lo;
            wyoos::common::uint8_t base_hi;
            wyoos::common::uint8_t type;
            wyoos::common::uint8_t limit_hi;
            wyoos::common::uint8_t base_vhi;

        public:
            SegmentDescriptor(wyoos::common::uint32_t base, wyoos::common::uint32_t limit, wyoos::common::uint8_t type);
            wyoos::common::uint32_t Base();
            wyoos::common::uint32_t Limit();
        } __attribute__((packed));

    private:
        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        wyoos::common::uint16_t CodeSegmentSelector();
        wyoos::common::uint16_t DataSegmentSelector();
    };
}
#endif