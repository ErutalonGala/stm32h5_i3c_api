#include "bma530.h"
#include "bst_api.h"
#include "main.h"


/**
 * @brief Enable I3C protocol in BMA530
 * @note Writes 0x01 to register 0x3B by mimicking the I2C communication in the I3C way.
 * @note The first write is a dummy write.
 * @param hi3c1: I3C handle
 * @retval None
 */
void BMA530_INIT_I3C(I3C_HandleTypeDef* hi3c1) {
  /* */
  uint8_t I3C_Enable = 0x01;
  I3C_WRITE_REGS(hi3c1, BMA530_STATIC_ADD,0x3B, &I3C_Enable, 1);
  HAL_Delay(1);
  I3C_WRITE_REGS(hi3c1, BMA530_STATIC_ADD,0x3B, &I3C_Enable, 1);

}
