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

CYBLE_CONN_HANDLE_T  connectionHandle;

/*This flag is set when the Central device writes to CCCD of the 
* CapSense slider Characteristic to enable notifications */
uint8 sendNotifications = TRUE;						

/* This flag is used by application to know whether a Central 
* device has been connected. This is updated in BLE event callback 
* function*/
uint8 deviceConnected = FALSE;

/* Status flag for the Stack Busy state. This flag is used to notify the application 
* whether there is stack buffer free to puch more data or not */
uint8 busyStatus = 0;

/* Local variable to store the current CCCD value */
uint8 DataCCCDvalue[2];

/* Handle value to update the CCCD */
CYBLE_GATT_HANDLE_VALUE_PAIR_T DataNotificationCCCDhandle;

void GenericAppEventHandler(uint32 event, void *eventParam)
{
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
    CYBLE_GATTS_WRITE_REQ_PARAM_T *uartRxDataWrReq;
    switch(event)
	{
    /**********************************************************
    *                       General Events
    ***********************************************************/
	case CYBLE_EVT_STACK_ON: 
		/* Start BLE Advertisement after BLE Stack is ON */
		CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
		
		break;
        
    /**********************************************************
    *                       GAP Events
    ***********************************************************/
    case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
        /* Put the device to discoverable mode so that remote can search it. */
        CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
		
        break;
    /**********************************************************
    *                       GATT Events
    ***********************************************************/
    case CYBLE_EVT_GATT_CONNECT_IND:
        connectionHandle = *(CYBLE_CONN_HANDLE_T  *)eventParam;
			
			/* This flag is used in application to check connection status */
			deviceConnected = TRUE;	
                UART_1_UartPutString("\n\rConnection established");    
        break;

    case CYBLE_EVT_GATT_DISCONNECT_IND:
			/* This event is received when device is disconnected */

			/* Update deviceConnected flag*/
			deviceConnected = FALSE;
			
			/* Reset CapSense notification flag to prevent further notifications
			 * being sent to Central device after next connection. */
			sendNotifications = FALSE;
			
			/* Reset the CCCD value to disable notifications */
    		/* Write the present CapSense notification status to the local variable */
    	    DataCCCDvalue[0] = sendNotifications;
    		DataCCCDvalue[1] = 0x00;
    		
    		/* Update CCCD handle with notification status data*/
    		DataNotificationCCCDhandle.attrHandle = CYBLE_CUSTOM_SERVICE_CUSTOM_CHARACTERISTIC_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE;
    		DataNotificationCCCDhandle.value.val = DataCCCDvalue;
    		DataNotificationCCCDhandle.value.len = CCCD_DATA_LEN;
    		
    		/* Report data to BLE component for sending data when read by Central device */
    		CyBle_GattsWriteAttributeValue(&DataNotificationCCCDhandle, ZERO, &connectionHandle, CYBLE_GATT_DB_PEER_INITIATED);
            
            UART_1_Stop();
            UART_1_SpiUartClearTxBuffer();
            UART_1_SpiUartClearRxBuffer();
            UART_1_Start();
		    break;
        
        case CYBLE_EVT_GATTS_WRITE_CMD_REQ:
            
            uartRxDataWrReq = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
            //if(uartRxDataWrReq->handleValPair.attrHandle == CYBLE_SERVER_UART_SERVER_UART_RX_DATA_CHAR_HANDLE)
            //{
                UART_1_SpiUartPutArray(uartRxDataWrReq->handleValPair.value.val, \
                                    (uint32) uartRxDataWrReq->handleValPair.value.len);
            //}
            break;
            
        case CYBLE_EVT_GATTS_WRITE_REQ:
                UART_1_UartPutString("\n\rConnection established");  
			/* This event is received when Central device sends a Write command on an Attribute */
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;

			/* When this event is triggered, the peripheral has received a write command on the custom characteristic */
			/* Check if command is for correct attribute and update the flag for sending Notifications */
            //if(CYBLE_CUSTOM_SERVICE_CUSTOM_CHARACTERISTIC_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE == wrReqParam->handleValPair.attrHandle)
            //{
				/* Extract the Write value sent by the Client for CapSense Slider CCCD */
                if(wrReqParam->handleValPair.value.val[CYBLE_CUSTOM_SERVICE_CUSTOM_CHARACTERISTIC_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX] == TRUE)
                {
                    sendNotifications = TRUE;
                }
                else if(wrReqParam->handleValPair.value.val[CYBLE_CUSTOM_SERVICE_CUSTOM_CHARACTERISTIC_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX] == FALSE)
                {
                    sendNotifications = FALSE;
                }
				
        		/* Write the present CapSense notification status to the local variable */
        		DataCCCDvalue[0] = sendNotifications;
        		DataCCCDvalue[1] = 0x00;
        		
        		/* Update CCCD handle with notification status data*/
        		DataNotificationCCCDhandle.attrHandle = CYBLE_CUSTOM_SERVICE_CUSTOM_CHARACTERISTIC_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE;
        		DataNotificationCCCDhandle.value.val = DataCCCDvalue;
        		DataNotificationCCCDhandle.value.len = CCCD_DATA_LEN;
        		
        		/* Report data to BLE component for sending data when read by Central device */
        		CyBle_GattsWriteAttributeValue(&DataNotificationCCCDhandle, ZERO, &connectionHandle, CYBLE_GATT_DB_PEER_INITIATED);
            //}
						
			/* Send the response to the write request received. */
			CyBle_GattsWriteRsp(connectionHandle);
			break;
        
        case CYBLE_EVT_STACK_BUSY_STATUS:
			/* This event is generated when the internal stack buffer is full and no more
			* data can be accepted or the stack has buffer available and can accept data.
			* This event is used by application to prevent pushing lot of data to stack. */
			
			/* Extract the present stack status */
            busyStatus = * (uint8*)eventParam;
            break;
            
    default:
        break;
	}
}

void SendDataOverNotification(uint8 Data)
{
	/* 'notificationHandle' stores notification data parameters */
	CYBLE_GATTS_HANDLE_VALUE_NTF_T		DataNotificationHandle;	
	
	/* If stack is not busy, then send the notification */
	if(busyStatus == CYBLE_STACK_STATE_FREE)
	{
		/* Update notification handle will data*/
		DataNotificationHandle.attrHandle = CYBLE_CUSTOM_SERVICE_CUSTOM_CHARACTERISTIC_CHAR_HANDLE;				
		DataNotificationHandle.value.val = &Data;
		DataNotificationHandle.value.len = DATA_NTF_DATA_LEN;

		/* Send the updated handle as part of attribute for notifications */
		CyBle_GattsNotification(connectionHandle,&DataNotificationHandle);
	}
}

/* [] END OF FILE */
