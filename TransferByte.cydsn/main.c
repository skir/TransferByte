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

	/* Start BLE component and register the CustomEventHandler function. This 
	* function exposes the events from BLE component for application use */
    CyBle_Start(GenericAppEventHandler);	
}

void HandleData(void)
{    
	static uint8 lastData;	
	
	uint8 newData = 1;
	
    lastData += newData;
	SendDataOverNotification(lastData);
}
/* [] END OF FILE */
