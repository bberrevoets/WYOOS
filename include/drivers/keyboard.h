#ifndef __WYOOS__DRIVERS__KEYBOARD_H
#define __WYOOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

namespace wyoos
{
    namespace drivers
    {
        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();

            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };

        class KeyboardDriver : public wyoos::hardwarecommunication::InterruptHandler,
                               public Driver
        {
            wyoos::hardwarecommunication::Port8Bit dataport;
            wyoos::hardwarecommunication::Port8Bit commandport;

            KeyboardEventHandler *handler;

        public:
            KeyboardDriver(wyoos::hardwarecommunication::InterruptManager *manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual wyoos::common::uint32_t HandleInterrupt(wyoos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}

#endif