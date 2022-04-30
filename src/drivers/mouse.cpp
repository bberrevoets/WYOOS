#include <drivers/mouse.h>

using namespace wyoos::drivers;

void printf(char *);

MouseEventHandler::MouseEventHandler()
{
}

void MouseEventHandler::OnActivate()
{
}

void MouseEventHandler::OnMouseButtonPressed(wyoos::common::uint8_t button)
{
}

void MouseEventHandler::OnMouseButtonReleased(wyoos::common::uint8_t button)
{
}

void MouseEventHandler::OnMouseMove(int x, int y)
{
}

MouseDriver::MouseDriver(wyoos::hardwarecommunication::InterruptManager *manager,MouseEventHandler *handler) : InterruptHandler(0x2C, manager), dataport(0x60), commandport(0x64)
{
    this->handler = handler;
}

MouseDriver::~MouseDriver()
{
}

void MouseDriver::Activate()
{
    offset = 0;
    buttons = 0;

    commandport.Write(0xA8);              // activate the mouse commands
    commandport.Write(0x20);              // get current state
    wyoos::common::uint8_t status = dataport.Read() | 2; // read the status and set the second rightmost bit to true.
    commandport.Write(0x60);              // Set state command
    dataport.Write(status);               // and write the new status

    commandport.Write(0xD4);
    dataport.Write(0xF4);
    dataport.Read(); // Activate the mouse
    handler->OnActivate();
}

wyoos::common::uint32_t MouseDriver::HandleInterrupt(wyoos::common::uint32_t esp)
{
    wyoos::common::uint8_t status = commandport.Read();
    if (!(status & 0x20))
        return esp;

    buffer[offset] = dataport.Read();
    offset = (offset + 1) % 3;

    if (handler == 0)
        return esp;

    if (offset == 0)
    {
        if (buffer[1] != 0 || buffer[2] != 0)
        {
            handler->OnMouseMove(buffer[1], -buffer[2]);
        }

        for (wyoos::common::uint8_t i = 0; i < 3; i++)
        {
            if ((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i)))
            {
                if (buttons & (0x1 << 1))
                    handler->OnMouseButtonReleased(i + 1);
                else
                    handler->OnMouseButtonPressed(i + 1);
            }
        }
        buttons = buffer[0];
    }
    return esp;
}