#include "bst_api.h"
#include "main.h"

/* Buffer used by HAL to compute control data for the Private Communication */
uint32_t aControlBuffer[0xF];

/* Context buffer related to Frame context, contain different buffer value for a
 * communication */
I3C_XferTypeDef aContextBuffers[2];

/* Buffer used for transmission */
uint8_t aTxBuffer[REG_BUFF_LEN];
/* Buffer used for reception */
uint8_t aRxBuffer[REG_BUFF_LEN];
/* Buffer used for register*/
uint8_t Regbuff[REG_BUFF_LEN];

/* Context buffer related to Frame context, contain different buffer value for a communication */
I3C_XferTypeDef aContextBuffers_CCC[5];
/* Buffer used for transmission */
uint8_t aTxBuffer_CCC[1];
/* Buffer used for Reception */
uint8_t aRxBuffer_CCC[5];

/* Initialize handle for timing modification of the bus */
LL_I3C_CtrlBusConfTypeDef CtrlBusConf;

/* Buffer used by HAL to compute control data for the Private Communication */
uint32_t aControlBuffer_CCC[0xF];


/**
  * @brief  Function for writing data to registers in BST sensors using I3C protocol
  * @param  hi3c1: I3C handle
  * @param  targ_add: Target device for writing data
  * @param  start_addr: Starting address of register(s), to write data
  * @param  data: pointer of data array to be written.
  * @param  len: number of bytes to be written
  * @retval None
  */
void I3C_WRITE_REGS(I3C_HandleTypeDef* hi3c1, uint8_t targ_add,uint8_t start_addr, uint8_t* data, uint32_t len) {
  /* Check to ensure numbers of bytes don't exceed the limit*/
  if (len > 64) {
    return;
  }
  /* Prepare a Data buffer with starting address as the index followed by data to be written */
  Regbuff[0] = start_addr;
  for (uint32_t i = 0; i < len; i++) {
    Regbuff[i + 1] = data[i];
  }
  /* Descriptor for private data transmit */
  I3C_PrivateTypeDef aPrivateDescriptorConfig[1] = {
      {targ_add, {Regbuff, len + 1}, {NULL, 0}, HAL_I3C_DIRECTION_WRITE},
  };
  /* Prepare context Buffer*/
  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.pBuffer = aControlBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size = 1;
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.pBuffer = Regbuff;
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.Size = len + 1;

  /*##- Add context buffer transmit in Frame context #####################*/
  if (HAL_I3C_AddDescToFrame(hi3c1, NULL, &aPrivateDescriptorConfig[I3C_IDX_FRAME_1], &aContextBuffers[I3C_IDX_FRAME_1],
                             aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size, I3C_PRIVATE_WITHOUT_ARB_STOP) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /* Transmit data */
  if (HAL_I3C_Ctrl_Transmit_IT(hi3c1, &aContextBuffers[I3C_IDX_FRAME_1]) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /*##- Wait for the end of the transfer #################################*/
  /*  Before starting a new communication transfer, you need to check the
   current state of the peripheral; if it's busy you need to wait for the end of
   current transfer before starting a new one. For simplicity reasons, this
   example is just waiting till the end of the transfer, but application may
   perform other tasks while transfer operation is ongoing. */

  /*skip this checking step, otherwise the program will be blocked in this check loop,
   * once the IBI request from BMA530 is enabled */
  if ((start_addr==0x30)&(data[0]==0xF)){
	  return;
  }

  while ((HAL_I3C_GetState(hi3c1) != HAL_I3C_STATE_READY) ) {

  }
}



/**
  * @brief  Function for reading data from registers in BST sensors using I3C protocol
  * @param  hi3c1: I3C handle
  * @param  targ_add: Target device for reading data
  * @param  start_addr: Starting address of register(s) to read data.
  * @param  data: pointer of data array to store data.
  * @param  len: number of bytes to be read.
  * @param  dummy_len: number of dummy bytes returned by the sensor.
  * 		The byte(s) will not be stored in output data array.
  * @retval None
  */
void I3C_READ_REGS(I3C_HandleTypeDef* hi3c1, uint8_t targ_add,uint8_t start_addr, uint8_t* data, uint32_t len, uint32_t dummy_len) {
  /* Descriptor for private data reception */
  I3C_PrivateTypeDef aPrivateDescriptor[2] = {
      {targ_add, {&start_addr, 1}, {NULL, 0}, HAL_I3C_DIRECTION_WRITE},
      {targ_add, {NULL, 0}, {data, len + dummy_len}, HAL_I3C_DIRECTION_READ}};
  /* Prepare context Buffer*/
  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.pBuffer = aControlBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size = 2;
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.pBuffer = aTxBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.Size = 1;
  aContextBuffers[I3C_IDX_FRAME_1].RxBuf.pBuffer = aRxBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].RxBuf.Size = COUNTOF(aRxBuffer);

  /*##- Add context buffer receiving in Frame context #####################*/
  if (HAL_I3C_AddDescToFrame(hi3c1, NULL, &aPrivateDescriptor[I3C_IDX_FRAME_1], &aContextBuffers[I3C_IDX_FRAME_1],
                             aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size, I3C_PRIVATE_WITH_ARB_RESTART) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /*##- Start the multiple transfer process
   * ###################################*/
  /* Receive data */
  if (HAL_I3C_Ctrl_MultipleTransfer_IT(hi3c1, &aContextBuffers[I3C_IDX_FRAME_1]) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /*##- Wait for the end of the transfer #################################*/
  /*  Before starting a new communication transfer, you need to check the
   current state of the peripheral; if it's busy you need to wait for the end of
   current transfer before starting a new one. For simplicity reasons, this
   example is just waiting till the end of the transfer, but application may
   perform other tasks while transfer operation is ongoing. */
  while (HAL_I3C_GetState(hi3c1) != HAL_I3C_STATE_READY) {
  }
  /* Transfer the received data from buffer to array(input arg)*/
  for (uint32_t i = 0; i < len; i++) {
    data[i] = aRxBuffer[i + dummy_len];
  }
}



/**
  * @brief  Function for changing i3c bus frequency
  * @param  hi3c1: I3C handle
  * @param  freq: I3C bus frequency in both open drain and push-pull phases
  * 		This parameter can be set to one of the following values (OD means open-drain frequency; PP means push-pull frequency.):
  *           @arg OD_500kHz_PP_500kHz : lowest I3C frequency.(Recommended when initializing BMA5XY)
  *           @arg OD_650kHz_PP_1000kHz: intermediate I3C frequency.
  *           @arg OD_2600kHz_PP_12500kHz: highest I3C frequency allowed in SDR mode.
  * @retval None
  *
  * @comment There are certain limitations w.r.t duty cycle allows for limited combinations of OD & PP frequencies.
			 In this case 3 sets of frequencies are defined for user to select from.
  */
void SET_I3C_BUS_FREQ(I3C_HandleTypeDef* hi3c1,char freq) {

	uint8_t scl_l_pp, scl_h_i3c, scl_l_od;

	if (freq == 0){
		scl_l_od = 0xf8;
		scl_l_pp = 0xf8;
		scl_h_i3c = 0xf8;
	}
	else if (freq == 1) {
		scl_l_od = 0xff;
		scl_l_pp = 0x7c;
		scl_h_i3c = 0x7c;
	}
	else if (freq == 2){
		scl_l_od = 0x55;
		scl_l_pp = 0x8;
		scl_h_i3c = 0x9;
	}
	else{
		Error_Handler();
	}

  /* Update timing registers */
  CtrlBusConf.SCLPPLowDuration = scl_l_pp;
  CtrlBusConf.SCLODLowDuration = scl_l_od;
  CtrlBusConf.SCLI3CHighDuration = scl_h_i3c;
  CtrlBusConf.SDAHoldTime = HAL_I3C_SDA_HOLD_TIME_1_5;
  CtrlBusConf.WaitTime = HAL_I3C_OWN_ACTIVITY_STATE_0;
  CtrlBusConf.SCLI2CHighDuration = 0x00;
  CtrlBusConf.BusIdleDuration = 0x32;
  CtrlBusConf.BusFreeDuration = 0xf8;

  /* Update Controller Bus characteristic */
  HAL_I3C_Ctrl_BusCharacteristicConfig(hi3c1, &CtrlBusConf);
}


/**
  * @brief  Function to send Broadcast CCC
  * @param  hi3c1: I3C handle
  * @param  CCC_add: Broadcast CCC address
  * @param  CCC_data: Broadcast CCC value
  * @retval None
  *
  * @comment This function is tested with ENEC & SETXTIME commands
  */
void CCC_BROADCAST(I3C_HandleTypeDef* hi3c1,char CCC_add,uint8_t* CCC_data) {


  /* Define a CCC Descriptor */
  I3C_CCCTypeDef aBroadcast_CCC[] = {
      /*  Target Addr           CCC Value           CCC data + defbyte pointer  CCC size + defbyte         Direction */
      {0, CCC_add, {CCC_data, 1}, LL_I3C_DIRECTION_WRITE}};
  /* Assign values */
  aContextBuffers_CCC[I3C_IDX_FRAME_1].CtrlBuf.pBuffer = aControlBuffer_CCC;
  aContextBuffers_CCC[I3C_IDX_FRAME_1].CtrlBuf.Size = COUNTOF(aControlBuffer_CCC);
  aContextBuffers_CCC[I3C_IDX_FRAME_1].TxBuf.pBuffer = aTxBuffer_CCC;
  aContextBuffers_CCC[I3C_IDX_FRAME_1].TxBuf.Size = 1;

  /*##- Add context buffer Set CCC frame in Frame context ############################*/
  if (HAL_I3C_AddDescToFrame(hi3c1, aBroadcast_CCC, NULL, &aContextBuffers_CCC[I3C_IDX_FRAME_1],
                             COUNTOF(aBroadcast_CCC), I3C_BROADCAST_WITHOUT_DEFBYTE_RESTART) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }
  /* Transmit the Broadcast CCC*/
  if (HAL_I3C_Ctrl_TransmitCCC_IT(hi3c1, &aContextBuffers_CCC[I3C_IDX_FRAME_1]) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }
}



/**
  * @brief  Function to read DIRECT CCC to a specific target
  * @param  hi3c1: I3C handle
  * @param  TARGET_DYN_ADDR: Target device for Direct CCC
  * @param  CCC_add: Direct CCC address
  * @param  CCC_data: Pointer of an array to store Direct CCC value
  * @param  len: number of bytes to be read
  * @retval None
  *
  * @comment This function is tested with GETXTIME command
  */
void CCC_DIRECT_READ(I3C_HandleTypeDef *hi3c1, char TARGET_DYN_ADDR, char CCC_add, uint8_t *CCC_data, int len) {


  /* Define a CCC Descriptor */
  I3C_CCCTypeDef aDirect_CCC_READ[] = {
      /*  Target Addr           CCC Value           CCC data + defbyte pointer  CCC size + defbyte         Direction */
      {TARGET_DYN_ADDR, CCC_add, {CCC_data, len}, LL_I3C_DIRECTION_READ}};
  /* Assign values */
  aContextBuffers_CCC[I3C_IDX_FRAME_1].CtrlBuf.pBuffer = aControlBuffer_CCC;
  aContextBuffers_CCC[I3C_IDX_FRAME_1].CtrlBuf.Size = COUNTOF(aControlBuffer_CCC);
  aContextBuffers_CCC[I3C_IDX_FRAME_1].RxBuf.pBuffer = CCC_data;
  aContextBuffers_CCC[I3C_IDX_FRAME_1].RxBuf.Size = len;

  /*##- Add context buffer Set CCC frame in Frame context ############################*/
  if (HAL_I3C_AddDescToFrame(hi3c1, aDirect_CCC_READ, NULL, &aContextBuffers_CCC[I3C_IDX_FRAME_1],
                             COUNTOF(aDirect_CCC_READ), I3C_DIRECT_WITHOUT_DEFBYTE_RESTART) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }
  /* Transmit the Direct CCC*/
  if (HAL_I3C_Ctrl_ReceiveCCC_IT(hi3c1, &aContextBuffers_CCC[I3C_IDX_FRAME_1]) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }
}




/**
  * @brief  Function to write DIRECT CCC to a specific target
  * @param  hi3c1: I3C handle
  * @param  TARGET_DYN_ADDR: Target device for Direct CCC
  * @param  CCC_add: Direct CCC address
  * @param  CCC_data: Direct CCC value to be written
  * @param  len: number of bytes to be written
  * @retval None
  *
  * @comment This function is tested with GETXTIME command
  */
void CCC_DIRECT_WRITE(I3C_HandleTypeDef *hi3c1, char TARGET_DYN_ADDR, char CCC_add, uint8_t *CCC_data, int len) {
  /* Define a CCC Descriptor */
  I3C_CCCTypeDef aDirect_CCC_WRITE[] = {
      /*  Target Addr           CCC Value           CCC data + defbyte pointer  CCC size + defbyte         Direction */
      {TARGET_DYN_ADDR, CCC_add, {CCC_data, len}, LL_I3C_DIRECTION_WRITE}};
  /* Assign values */
  aContextBuffers_CCC[I3C_IDX_FRAME_1].CtrlBuf.pBuffer = aControlBuffer_CCC;
  aContextBuffers_CCC[I3C_IDX_FRAME_1].CtrlBuf.Size = COUNTOF(aControlBuffer_CCC);
  aContextBuffers_CCC[I3C_IDX_FRAME_1].TxBuf.pBuffer = CCC_data;
  aContextBuffers_CCC[I3C_IDX_FRAME_1].TxBuf.Size = len;

  /*##- Add context buffer Set CCC frame in Frame context ############################*/
  if (HAL_I3C_AddDescToFrame(hi3c1, aDirect_CCC_WRITE, NULL, &aContextBuffers_CCC[I3C_IDX_FRAME_1],
                             COUNTOF(aDirect_CCC_WRITE), I3C_DIRECT_WITHOUT_DEFBYTE_RESTART) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }
  /* Transmit the Direct CCC*/
  if (HAL_I3C_Ctrl_TransmitCCC_IT(hi3c1, &aContextBuffers_CCC[I3C_IDX_FRAME_1]) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }
}
