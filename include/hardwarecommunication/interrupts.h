#ifndef __WYOOS__HARDWARECOMMUNICATION__INTERRUPTMANAGER_H
#define __WYOOS__HARDWARECOMMUNICATION__INTERRUPTMANAGER_H

#include <gdt.h>
#include <common/types.h>
#include <hardwarecommunication/port.h>

namespace wyoos
{
    namespace hardwarecommunication
    {

        class InterruptManager;

        class InterruptHandler
        {
        protected:
            wyoos::common::uint8_t interruptNumber;
            InterruptManager *interruptManager;

            InterruptHandler(wyoos::common::uint8_t interruptNumber, InterruptManager *interruptManager);
            ~InterruptHandler();

        public:
            virtual wyoos::common::uint32_t HandleInterrupt(wyoos::common::uint32_t esp);
        };

        class InterruptManager
        {
            friend class InterruptHandler;

        protected:
            InterruptHandler *handlers[256];

            struct GateDescriptor
            {
                wyoos::common::uint16_t handlerAddressLowBits;
                wyoos::common::uint16_t gdt_codeSegmentSelector;
                wyoos::common::uint8_t reserved;
                wyoos::common::uint8_t access;
                wyoos::common::uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer
            {
                wyoos::common::uint16_t size;
                wyoos::common::uint32_t base;
            } __attribute__((packed));

            wyoos::common::uint16_t hardwareInterruptOffset;
            static InterruptManager *ActiveInterruptManager;
            static void SetInterruptDescriptorTableEntry(wyoos::common::uint8_t interrupt,
                                                         wyoos::common::uint16_t codeSegmentSelectorOffset, void (*handler)(),
                                                         wyoos::common::uint8_t DescriptorPrivilegeLevel, wyoos::common::uint8_t DescriptorType);

            static void InterruptIgnore();

            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0C();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();

            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();

            static wyoos::common::uint32_t HandleInterrupt(wyoos::common::uint8_t interrupt, wyoos::common::uint32_t esp);
            wyoos::common::uint32_t DoHandleInterrupt(wyoos::common::uint8_t interrupt, wyoos::common::uint32_t esp);

            wyoos::hardwarecommunication::Port8BitSlow picMasterCommand;
            wyoos::hardwarecommunication::Port8BitSlow picMasterData;
            wyoos::hardwarecommunication::Port8BitSlow picSlaveCommand;
            wyoos::hardwarecommunication::Port8BitSlow picSlaveData;

        public:
            InterruptManager(wyoos::common::uint16_t hardwareInterruptOffset, GlobalDescriptorTable *globalDescriptorTable);
            ~InterruptManager();
            wyoos::common::uint16_t HardwareInterruptOffset();
            void Activate();
            void Deactivate();
        };
    }
}
#endif
