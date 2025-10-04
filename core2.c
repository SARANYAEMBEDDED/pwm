#include "Ifx_Types.h"
#include "IfxAsclin_Asc.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

IfxAsclin_Asc g_asclin0;   // UART handle

void uart_init(void)
{
    // Disable watchdogs (for safety in test code)
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    // UART configuration
    IfxAsclin_Asc_Config config;
    IfxAsclin_Asc_initModuleConfig(&config, &MODULE_ASCLIN0); // Use ASCLIN0

    config.baudrate.baudrate = 115200;   // Set baud rate
    config.interrupt.txPriority = 12;    // TX interrupt priority
    config.interrupt.rxPriority = 14;    // RX interrupt priority
    config.interrupt.erPriority = 16;    // Error interrupt
    config.txBuffer = NULL_PTR;          // Use default
    config.rxBuffer = NULL_PTR;          // Use default

    // Assign pins (depends on board, here for TC275 LiteKit)
    config.pins = &IfxAsclin0_P15_2_OUT;   // TX Pin = P15.2
    config.pins->mode = IfxPort_OutputMode_pushPull;

    config.rxPin = &IfxAsclin0_P15_3_IN;   // RX Pin = P15.3
    config.rxPinMode = IfxPort_InputMode_pullUp;

    // Initialize module
    IfxAsclin_Asc_initModule(&g_asclin0, &config);
}

void uart_send_char(char c)
{
    IfxAsclin_Asc_blockingWrite(&g_asclin0, c);
}

char uart_receive_char(void)
{
    uint8 c;
    IfxAsclin_Asc_blockingRead(&g_asclin0, &c);
    return (char)c;
}

int core0_main(void)
{
    uart_init();

    while (1)
    {
        // Transmit message
        char *msg = "Hello from Infineon UART\r\n";
        while (*msg)
        {
            uart_send_char(*msg++);
        }

        // Echo received characters
        char c = uart_receive_char();
        uart_send_char(c);
    }

    return 0;
}
