#include "mouse.h"

void printf(char *);

MouseDriver::MouseDriver(InterruptManager *manager) : InterruptHandler(0x2C, manager), dataport(0x60), commandport(0x64)
{
    uint16_t *VideoMemory = (uint16_t *)0xb8000;
    offset = 0;
    buttons = 0;
    x = 40;
    y = 12;
    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);

    commandport.Write(0xA8);              // activate the mouse commands
    commandport.Write(0x20);              // get current state
    uint8_t status = dataport.Read() | 2; // read the status and set the second rightmost bit to true.
    commandport.Write(0x60);              // Set state command
    dataport.Write(status);               // and write the new status

    commandport.Write(0xD4);
    dataport.Write(0xF4);
    dataport.Read(); // Activate the mouse
}

MouseDriver::~MouseDriver()
{
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandport.Read();
    if (!(status & 0x20))
        return esp;

    buffer[offset] = dataport.Read();
    offset = (offset + 1) % 3;

    if (offset == 0)
    {
        if (buffer[1] != 0 || buffer[2] != 0)
        {
            static uint16_t *VideoMemory = (uint16_t *)0xb8000;
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);

            x += buffer[1];
            if (x >= 80)
                x = 79;
            if (x < 0)
                x = 0;
            y -= buffer[2];
            if (y >= 25)
                y = 24;
            if (y < 0)
                y = 0;

            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);
        }

        for (uint8_t i = 0; i < 3; i++)
        {
            if ((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i)))
            {
                // ....
            }
        }
        buttons = buffer[0];
    }
    return esp;
}