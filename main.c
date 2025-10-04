#include "Ifx_Types.h"
#include "IfxAsclin_Asc.h"
#include "Bsp.h"

IfxAsclin_Asc g_asc;         // UART handle
uint8 g_rxData;              // Received character
uint8 g_txData[] = "Hello Infineon!\r\n";  // Data to send

void initUART(void)
{
    IfxAsclin_Asc_Config ascConfig;
    IfxAsclin_Asc_initModuleConfig(&ascConfig, &MODULE_ASCLIN0);

    // UART Pin Configuration (change pins as per your board)
    const IfxAsclin_Asc_Pins pins = {
        .cts       = NULL_PTR,
        .ctsMode   = IfxPort_InputMode_pullUp,
        .rx        = &IfxAsclin0_RXB_P15_3_IN,    // RX Pin
        .rxMode    = IfxPort_InputMode_pullUp,
        .rts       = NULL_PTR,
        .rtsMode   = IfxPort_OutputMode_pushPull,
        .tx        = &IfxAsclin0_TX_P15_3_OUT,    // TX Pin
        .txMode    = IfxPort_OutputMode_pushPull,
        .pinDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    ascConfig.pins = &pins;

    ascConfig.baudrate.baudrate = 115200; // Baudrate
    ascConfig.baudrate.oversampling = IfxAsclin_OversamplingFactor_16;

    // Initialize UART Module
    IfxAsclin_Asc_initModule(&g_asc, &ascConfig);
}

void sendString(uint8 *data)
{
    while (*data != '\0')
    {
        IfxAsclin_Asc_blockingSend(&g_asc, *data++);
    }
}

int main(void)
{
    initUART();                       // Initialize UART
    sendString(g_txData);              // Send test string

    while (1)
    {
        // Receive 1 character (blocking)
        IfxAsclin_Asc_blockingReceive(&g_asc, &g_rxData);
        // Echo back the received character
        IfxAsclin_Asc_blockingSend(&g_asc, g_rxData);
    }
}
