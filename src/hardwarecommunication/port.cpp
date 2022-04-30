#include <hardwarecommunication/port.h>
using namespace wyoos::hardwarecommunication;

Port::Port(wyoos::common::uint16_t portnumber)
{
    this->portnumber = portnumber;
}

Port::~Port()
{
}





Port8Bit::Port8Bit(wyoos::common::uint16_t portnumber)
    : Port(portnumber)
{
}

Port8Bit::~Port8Bit()
{
}

void Port8Bit::Write(wyoos::common::uint8_t data)
{
    Write8(portnumber, data);
}

wyoos::common::uint8_t Port8Bit::Read()
{
    return Read8(portnumber);
}





Port8BitSlow::Port8BitSlow(wyoos::common::uint16_t portnumber)
    : Port8Bit(portnumber)
{
}

Port8BitSlow::~Port8BitSlow()
{
}

void Port8BitSlow::Write(wyoos::common::uint8_t data)
{
    Write8Slow(portnumber, data);
}





Port16Bit::Port16Bit(wyoos::common::uint16_t portnumber)
    : Port(portnumber)
{
}

Port16Bit::~Port16Bit()
{
}

void Port16Bit::Write(wyoos::common::uint16_t data)
{
    Write16(portnumber, data);
}

wyoos::common::uint16_t Port16Bit::Read()
{
    return Read16(portnumber);
}





Port32Bit::Port32Bit(wyoos::common::uint16_t portnumber)
    : Port(portnumber)
{
}

Port32Bit::~Port32Bit()
{
}

void Port32Bit::Write(wyoos::common::uint32_t data)
{
    Write32(portnumber, data);
}

wyoos::common::uint32_t Port32Bit::Read()
{
    return Read32(portnumber);
}