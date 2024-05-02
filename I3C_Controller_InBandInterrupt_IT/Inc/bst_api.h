#include <stdint.h>
//#include "stm32h5xx_util_i3c.h"
#include "stm32h5xx_hal.h"

/* Define dynamic address for target(s) */
#ifndef TARGET1_DYN_ADDR
#define TARGET1_DYN_ADDR 0x08
#endif

#ifndef TARGET2_DYN_ADDR
#define TARGET2_DYN_ADDR 0x09
#endif

/* Define some macros for further use in the functions */
#define I3C_IDX_FRAME_1 0U /* Index of Frame 1 */
#define I3C_IDX_FRAME_2 1U /* Index of Frame 2 */
#define I3C_IDX_FRAME_3 2U
#define REG_BUFF_LEN (65)

/* Standard addresses from MIPI docs*/
#define BROADCAST_ENEC 0x00
#define BROADCAST_SETXTIME 0x28
#define BROADCAST_SETMRL 0x0A
#define DIRECT_ENEC 0x80
#define DIRECT_SETXTIME 0x98
#define DIRECT_GETXTIME 0x99
#define DIRECT_SETMRL 0x8A
#define DIRECT_GETMRL 0x8C

#define DIRECT_GETPID 0x8D
#define DIRECT_SETMWL 0x89

/* Define frequency macros */
#define OD_500kHz_PP_500kHz 0
#define OD_650kHz_PP_1000kHz 1
#define OD_2600kHz_PP_12500kHz 2

/*General read function for BST sensors*/
void I3C_READ_REGS(I3C_HandleTypeDef *hi3c1,uint8_t targ_add,uint8_t start_addr, uint8_t *data, uint32_t len_in_bytes, uint32_t dummy_len);

/*General write function for BST sensors*/
void I3C_WRITE_REGS(I3C_HandleTypeDef *hi3c1,uint8_t targ_add,uint8_t start_addr, uint8_t *data, uint32_t len_in_bytes);

/* Function to set frequency of I3C bus*/
void SET_I3C_BUS_FREQ(I3C_HandleTypeDef *hi3c1,char freq);

/* Function to Broadcast CCCs over the bus */
void CCC_BROADCAST(I3C_HandleTypeDef* hi3c1,char CCC_add,uint8_t* CCC_data);

/* Function to transmit DIRECT(read) CCC to a specific target*/
void CCC_DIRECT_READ(I3C_HandleTypeDef *hi3c1, char TARGET_DYN_ADDR, char CCC_add, uint8_t *CCC_data, int len_in_bytes);

/* Function to transmit DIRECT(write) CCC to a specific target*/
void CCC_DIRECT_WRITE(I3C_HandleTypeDef *hi3c1, char TARGET_DYN_ADDR, char CCC_add, uint8_t *CCC_data, int len_in_bytes);
