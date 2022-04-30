#ifndef __WYOOS__DRIVERS__MOUSE_H
#define __WYOOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>

namespace wyoos
{
    namespace drivers
    {
        class MouseEventHandler
        {
            wyoos::common::int8_t x, y;

        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseButtonPressed(wyoos::common::uint8_t button);
            virtual void OnMouseButtonReleased(wyoos::common::uint8_t button);
            virtual void OnMouseMove(int x, int y);
        };

        class MouseDriver : public wyoos::hardwarecommunication::InterruptHandler, public Driver
        {
            wyoos::hardwarecommunication::Port8Bit dataport;
            wyoos::hardwarecommunication::Port8Bit commandport;

            wyoos::common::uint8_t buffer[3];
            wyoos::common::uint8_t offset;
            wyoos::common::uint8_t buttons;

            MouseEventHandler *handler;

        public:
            MouseDriver(wyoos::hardwarecommunication::InterruptManager *manager, MouseEventHandler *handler);
            ~MouseDriver();
            virtual wyoos::common::uint32_t HandleInterrupt(wyoos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif