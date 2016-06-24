/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <main.h>

extern uint8 sendNotifications;	

extern uint8 deviceConnected;

/* 'connectionHandle' stores connection parameters */
extern CYBLE_CONN_HANDLE_T  connectionHandle;

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    InitializeSystem();
    for(;;)
    {
        CyBle_ProcessEvents();
        if(TRUE == deviceConnected)
		{
			
			/* Send CapSense Slider data when respective notification is enabled */
			if(sendNotifications == TRUE)
			{
				//if(CYBLE_BLESS_STATE_ECO_STABLE ==CyBle_GetBleSsState())
				//{
					/* Check for CapSense slider swipe and send data accordingly */
					HandleData();
				//}
			}
		}
    }
}

void InitializeSystem(void)
{
	/* Enable global interrupt mask */
	CyGlobalIntEnable; 
    UART_1_Start();

	/* Start BLE component and register the CustomEventHandler function. This 
	* function exposes the events from BLE component for application use */
    CyBle_Start(GenericAppEventHandler);
    UART_1_UartPutString("started");
    CyBle_ProcessEvents();
}

void HandleData(void)
{
    uint32 size = UART_1_SpiUartGetRxBufferSize();    
	static uint8 lastData;	
	
	uint8 newData;// UART_1_rx_Read() + lastData;
    //newData = UART_1_UartGetChar();
    //newData = (data & 0xff000000);
    //newData += lastData;
    
    if (0 != size) 
    {
	    SendDataOverNotification((uint8)UART_1_UartGetByte());
    }
    lastData = newData;
}
/* [] END OF FILE */
