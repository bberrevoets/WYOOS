#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace wyoos;

void printf(char *str)
{
    static wyoos::common::uint16_t *VideoMemory = (wyoos::common::uint16_t *)0xb8000;

    static wyoos::common::uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; ++i)
    {
        switch (str[i])
        {
        case '\n':
            x = 0;
            y++;
            break;
        default:
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
            x++;
            break;
        }

        if (x >= 80)
        {
            x = 0;
            y++;
        }

        if (y >= 25)
        {
            for (y = 0; y < 25; y++)
                for (x = 0; x < 80; x++)
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printfHex(wyoos::common::uint8_t b)
{
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(b >> 4) & 0x0F];
    foo[1] = hex[b & 0x0F];
    printf(foo);
}

class PrintfKeyboardEventHandler : public wyoos::drivers::KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char *foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public wyoos::drivers::MouseEventHandler
{
    wyoos::common::int8_t x, y;

public:
    void OnMouseMove(int xoffset, int yoffset)
    {
        static wyoos::common::uint16_t *VideoMemory = (wyoos::common::uint16_t *)0xb8000;
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);

        x += xoffset;
        if (x >= 80)
            x = 79;
        if (x < 0)
            x = 0;
        y += yoffset;
        if (y >= 25)
            y = 24;
        if (y < 0)
            y = 0;

        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);
    }

    void OnActivate()
    {
        wyoos::common::uint16_t *VideoMemory = (wyoos::common::uint16_t *)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);
    }
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for (constructor *i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(const void *multiboot_structure, wyoos::common::uint32_t /*multiboot_magic*/)
{
    printf("WYOOS - http://www.berrevoets.net\n");

    GlobalDescriptorTable gdt;
    wyoos::hardwarecommunication::InterruptManager interrupts(0x20, &gdt);

    printf("Initializing Hardware, Stage 1\n\n");

    wyoos::drivers::DriverManager driverManager;

    PrintfKeyboardEventHandler kbhandler;
    wyoos::drivers::KeyboardDriver keyboard(&interrupts, &kbhandler);
    driverManager.AddDriver(&keyboard);

    MouseToConsole mousehandler;
    wyoos::drivers::MouseDriver mouse(&interrupts, &mousehandler);
    driverManager.AddDriver(&mouse);

    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&driverManager);

    printf("\nInitializing Hardware, Stage 2\n");
    driverManager.ActivateAll();

    printf("Initializing Hardware, Stage 3\n");
    interrupts.Activate();

    while (1)
        ;
}