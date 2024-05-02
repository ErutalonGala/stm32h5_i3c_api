#include "bma530.h"
#include "main.h"
/* USER CODE BEGIN Includes */
#include "desc_target1.h"
#define I3C_IDX_FRAME_1 0U /* Index of Frame 1 */
#define I3C_IDX_FRAME_2 1U /* Index of Frame 2 */
#define REG_BUFF_LEN (65)
#define BMA530_STATIC_ADD 0x18
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

void bma530_init_i3c(I3C_HandleTypeDef *hi3c1) {
  // Enable I3C protocol in BMA530 by writing 01 to register 3B. First write is
  // dummy write.

  uint8_t I3C_Enable[] = {0x3B, 0x01};

  I3C_PrivateTypeDef bPrivateTypeDescriptorWrite[1] = {
      {BMA530_STATIC_ADD, {I3C_Enable, 2}, {NULL, 0}, HAL_I3C_DIRECTION_WRITE}};

  /* Prepare Transmit context buffer with the different parameters*/
  aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
  aContextBuffers[0].CtrlBuf.Size = 1;
  aContextBuffers[0].TxBuf.pBuffer = I3C_Enable;
  aContextBuffers[0].TxBuf.Size = 2;

  /* Prepare a dummy frame to send*/
  if (HAL_I3C_AddDescToFrame(&hi3c1, NULL, &bPrivateTypeDescriptorWrite[0],
                             &aContextBuffers[0],
                             aContextBuffers[0].CtrlBuf.Size,
                             I3C_PRIVATE_WITHOUT_ARB_STOP) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  if (HAL_I3C_Ctrl_Transmit_IT(&hi3c1, &aContextBuffers[0]) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  HAL_Delay(2);

  // Repeat the same procedure this time the write should work normally.

  aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
  aContextBuffers[0].CtrlBuf.Size = 1;
  aContextBuffers[0].TxBuf.pBuffer = I3C_Enable;
  aContextBuffers[0].TxBuf.Size = 2;

  /* Prepare a frame to send to enable the I3C bit*/
  if (HAL_I3C_AddDescToFrame(&hi3c1, NULL, &bPrivateTypeDescriptorWrite[0],
                             &aContextBuffers[0],
                             aContextBuffers[0].CtrlBuf.Size,
                             I3C_PRIVATE_WITHOUT_ARB_STOP) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  if (HAL_I3C_Ctrl_Transmit_IT(&hi3c1, &aContextBuffers[0]) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /*##- Start the transmission process ###################################*/
  /* Assign dynamic address processus */
  if (HAL_I3C_Ctrl_DynAddrAssign_IT(&hi3c1, I3C_ONLY_ENTDAA) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /*##- Wait for the end of the transfer #################################*/
  /*  Before starting a new communication transfer, you need to check the
  current state of the peripheral; if it�s busy you need to wait for the end of
  current transfer before starting a new one. For simplicity reasons, this
  example is just waiting till the end of the transfer, but application may
  perform other tasks while transfer operation is ongoing. */
  while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY) {
  }

  if (HAL_I3C_Ctrl_IsDeviceI3C_Ready(&hi3c1, TARGET1_DYN_ADDR, 300, 1000) !=
      HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }
}

void write_regs(uint8_t addr, uint8_t *data, uint32_t len,
                I3C_HandleTypeDef *hi3c1) {
  /* Descriptor for private data transmit */
  if (len > 64) {
    return;
  }
  Regbuff[0] = addr;
  for (uint32_t i = 0; i < len; i++) {
    Regbuff[i + 1] = data[i];
  }
  I3C_PrivateTypeDef aPrivateDescriptorConfig[1] = {
      {TARGET1_DYN_ADDR,
       {Regbuff, len + 1},
       {NULL, 0},
       HAL_I3C_DIRECTION_WRITE},
  };

  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.pBuffer = aControlBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size = 1;
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.pBuffer = Regbuff;
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.Size = len + 1;

  /*##- Add context buffer transmit in Frame context #####################*/
  if (HAL_I3C_AddDescToFrame(hi3c1, NULL,
                             &aPrivateDescriptorConfig[I3C_IDX_FRAME_1],
                             &aContextBuffers[I3C_IDX_FRAME_1],
                             aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size,
                             I3C_PRIVATE_WITH_ARB_STOP) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /*##- Start the transmission process ###################################*/
  /* Transmit private data processus */
  if (HAL_I3C_Ctrl_Transmit_IT(hi3c1, &aContextBuffers[I3C_IDX_FRAME_1]) !=
      HAL_OK) {
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
}

void read_regs(uint8_t addr, uint8_t *data, uint32_t len, uint32_t dummy_len,
               I3C_HandleTypeDef *hi3c1) {
  /* Descriptor for private data transmit */
  I3C_PrivateTypeDef aPrivateDescriptor[2] = {
      {TARGET1_DYN_ADDR, {&addr, 1}, {NULL, 0}, HAL_I3C_DIRECTION_WRITE},
      {TARGET1_DYN_ADDR,
       {NULL, 0},
       {data, len + dummy_len},
       HAL_I3C_DIRECTION_READ}};
  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.pBuffer = aControlBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size = 2;
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.pBuffer = aTxBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.Size = 1;
  aContextBuffers[I3C_IDX_FRAME_1].RxBuf.pBuffer = aRxBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].RxBuf.Size = RXBUFFERSIZE;

  /*##- Add context buffer transmit in Frame context #####################*/
  if (HAL_I3C_AddDescToFrame(hi3c1, NULL, &aPrivateDescriptor[I3C_IDX_FRAME_1],
                             &aContextBuffers[I3C_IDX_FRAME_1],
                             aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size,
                             I3C_PRIVATE_WITHOUT_ARB_RESTART) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /*##- Start the multiple transfer process
   * ###################################*/
  /* Transmit/receive private data processus */
  if (HAL_I3C_Ctrl_MultipleTransfer_IT(
          hi3c1, &aContextBuffers[I3C_IDX_FRAME_1]) != HAL_OK) {
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

  for (uint32_t i = 0; i < len; i++) {
    data[i] = aRxBuffer[i + dummy_len];
  }
}
