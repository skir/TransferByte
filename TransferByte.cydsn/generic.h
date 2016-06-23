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
#include <project.h>

#define DATA_NTF_DATA_LEN		1u
/* Client Characteristic Configuration descriptor data length. This is defined
* as per BLE spec. */
#define CCCD_DATA_LEN						2

/* Bit mask for notification bit in CCCD (Client Characteristic 
* Configuration Descriptor) written by Client device. */
#define CCCD_NTF_BIT_MASK					0x01

/* Minimum connection interval = CONN_PARAM_UPDATE_MIN_CONN_INTERVAL * 1.25 ms*/
#define CONN_PARAM_UPDATE_MIN_CONN_INTERVAL	50        	
/* Maximum connection interval = CONN_PARAM_UPDATE_MAX_CONN_INTERVAL * 1.25 ms */
#define CONN_PARAM_UPDATE_MAX_CONN_INTERVAL	52        	
/* Slave latency = Number of connection events */
#define CONN_PARAM_UPDATE_SLAVE_LATENCY		4          
/* Supervision timeout = CONN_PARAM_UPDATE_SUPRV_TIMEOUT * 10*/
#define CONN_PARAM_UPDATE_SUPRV_TIMEOUT		200   

void GenericAppEventHandler(uint32 event, void * eventParam);
/* [] END OF FILE */
