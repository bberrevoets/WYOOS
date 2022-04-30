#ifndef __WYOOS__HARDWARECOMMUNICATION__PCI_H
#define __WYOOS__HARDWARECOMMUNICATION__PCI_H

#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecommunication/port.h>

using namespace wyoos::common;
using namespace wyoos::drivers;
using namespace wyoos::hardwarecommunication;

namespace wyoos
{
    namespace hardwarecommunication
    {
        class PeripheralComponentInterconnectDeviceDescriptor
        {
        public:
            uint32_t portBase;
            uint32_t interrupt;

            uint16_t bus;
            uint16_t device;
            uint16_t function;

            uint16_t vendor_id;
            uint16_t device_id;

            uint8_t class_id;
            uint8_t subclass_id;
            uint8_t interface_id;

            uint8_t revision;

            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();
        };

        class PeripheralComponentInterconnectController
        {
            Port32Bit dataPort;
            Port32Bit commandPort;

        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();

            uint32_t Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset);
            void Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value);
            bool DeviceHasFunctions(uint16_t bus, uint16_t device);

            void SelectDrivers(DriverManager *driverManager);
            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);
        };
    }
}

#endif
