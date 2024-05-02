/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    I3C/I3C_Controller_InBandInterrupt_IT/Src/main.c
 * @author  MCD Application Team
 * @brief This sample code shows how to use STM32H5xx I3C HAL API to
 *        manage an In-Band-Interrupt procedure between a Controller
 *        and Targets with a communication process based on Interrupt transfer.
 *        The communication is done using 2 or 3 Boards.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bma530.h"
#include "bst_api.h"
#include "desc_target1.h"
#include "desc_target2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I3C_HandleTypeDef hi3c1;

/* USER CODE BEGIN PV */

/* Index of Target to store the different Target capabilities */
uint32_t ubTargetIndex;

/* Descriptor that contains the bus devices configurations */
I3C_DeviceConfTypeDef DeviceConf[4] = {0};

/* Number of Targets detected during DAA procedure */
__IO uint32_t uwTargetCount = 0;

/* Variable to catch IBI event */
__IO uint32_t uwIBIRequested = 0;

/* CCC information updated after CCC event */
I3C_CCCInfoTypeDef CCCInfo;

/* Array contain targets descriptor */
TargetDesc_TypeDef *aTargetDesc[2] = {
    &TargetDesc1, /* DEVICE_ID1 */
    &TargetDesc2  /* DEVICE_ID2 */
};

/* Buffer that contain payload data, mean PID, BCR, DCR */
uint8_t aPayloadBuffer[64 * COUNTOF(aTargetDesc)];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I3C1_Init(void);
static void MX_ICACHE_Init(void);
static void MX_MEMORYMAP_Init(void);
/* USER CODE BEGIN PFP */
#if defined(__ICCARM__)
__ATTRIBUTES size_t __write(int, const unsigned char *, size_t);
#endif /* __ICCARM__ */

#if defined(__ICCARM__)
/* New definition from EWARM V9, compatible with EWARM8 */
int iar_fputc(int ch);
#define PUTCHAR_PROTOTYPE int iar_fputc(int ch)
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
/* ARM Compiler 5/6*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#endif /* __ICCARM__ */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I3C1_Init();
  MX_ICACHE_Init();
  MX_MEMORYMAP_Init();
  /* USER CODE BEGIN 2 */
  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /* Configure USER push-button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* Wait for USER push-button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET) {

  }

  /* Wait for USER push-button release before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET) {

  }


  // Set frequency to 500kHz for initialization of I3C protocol
  SET_I3C_BUS_FREQ(&hi3c1,OD_500kHz_PP_500kHz);

  HAL_Delay(1);

  // Enable I3C protocol
  BMA530_INIT_I3C(&hi3c1);

  HAL_Delay(1);

  /*##- Start the transmission process ###################################*/
  /* Assign dynamic address process */
  if (HAL_I3C_Ctrl_DynAddrAssign_IT(&hi3c1, I3C_ONLY_ENTDAA) != HAL_OK) {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }
  /*##- Wait for the end of the transfer #################################*/
  /*  Before starting a new communication transfer, you need to check the current
   state of the peripheral; if it�s busy you need to wait for the end of current
   transfer before starting a new one.
   For simplicity reasons, this example is just waiting till the end of the
   transfer, but application may perform other tasks while transfer operation
   is ongoing. */
  while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY) {
  }

  HAL_Delay(1);

  // Transmit ENEC broadcast message to Enable target initiated events(IBI).
  uint8_t aENEC_data[1] = {0x01};
//  CCC_BROADCAST(&hi3c1, Broadcast_ENEC,aENEC_data);
  CCC_DIRECT_WRITE(&hi3c1,TARGET1_DYN_ADDR,DIRECT_ENEC,aENEC_data,1);
  HAL_Delay(1);


  // Transmit Setxtime broadcast message to enable Async mode 0
//  0xDF -> Enter Async Mode 0, 0xFF ->Exit from enabled timing modes
  uint8_t aSETXTIME_data[1] = {0xDF};
  CCC_BROADCAST(&hi3c1, BROADCAST_SETXTIME,aSETXTIME_data);

  //CCC_DIRECT_WRITE(&hi3c1,DIRECT_SETXTIME,TARGET1_DYN_ADDR,aSETXTIME_data,1);
  HAL_Delay(1);

  /* (Optional) SETMWL direct write to test function DIRECT_WRITE. */
  uint8_t aSETMWL_data[2] = {0xF,0x00};
  CCC_DIRECT_WRITE(&hi3c1,TARGET1_DYN_ADDR,DIRECT_ENEC,aSETMWL_data,1);
  HAL_Delay(1);

  /* (Optional) GETXTIME direct read to test function DIRECT_READ.
   * The values received can be used for IBI analysis purposes */
  uint8_t aGETXTIME_data[4] ;
  CCC_DIRECT_READ(&hi3c1,TARGET1_DYN_ADDR,DIRECT_GETXTIME,aGETXTIME_data,4);
  HAL_Delay(1);

//  uint8_t aGETMRL_data[4] ;
//  CCC_DIRECT_READ(&hi3c1,TARGET1_DYN_ADDR,DIRECT_GETMRL,aGETMRL_data,4);
//  HAL_Delay(1);

  /* Since I3C is enabled, change bus frequency to a higher value.
  	  12.5Mhz is the highest supported frequency in SDR mode. */
  SET_I3C_BUS_FREQ(&hi3c1,OD_650kHz_PP_1000kHz);

  HAL_Delay(1);


  //--------------IBI Configuration START--------------------------------------------------------------

  // Disable accelerometer
  uint8_t acc_dis = 0x00;
  I3C_WRITE_REGS(&hi3c1, TARGET1_DYN_ADDR,0x30, &acc_dis, 1);

  uint8_t acc_dis_r= 0x00;
  I3C_READ_REGS(&hi3c1, TARGET1_DYN_ADDR,0x30,&acc_dis_r,1,1);

  // Set ODR
  uint8_t odr = 0xa3;
  I3C_WRITE_REGS(&hi3c1, TARGET1_DYN_ADDR,0x31, &odr, 1);

  // Map Acc_drdy interrupt to I3C_IBI
  uint8_t acc_drdy = 0x03;
  I3C_WRITE_REGS(&hi3c1, TARGET1_DYN_ADDR,0x36, &acc_drdy, 1);

  /*##- Store Devices capabilities #######################################*/
  /* Fill Device descriptor for all target detected during ENTDAA procedure */
  for (ubTargetIndex = 0; ubTargetIndex < uwTargetCount;
  		ubTargetIndex++) {
  	DeviceConf[ubTargetIndex].DeviceIndex = (ubTargetIndex + 1);
  	DeviceConf[ubTargetIndex].TargetDynamicAddr =
  			aTargetDesc[ubTargetIndex]->DYNAMIC_ADDR;
  	DeviceConf[ubTargetIndex].IBIAck =
  			__HAL_I3C_GET_IBI_CAPABLE(
  					__HAL_I3C_GET_BCR(aTargetDesc[ubTargetIndex]->TARGET_BCR_DCR_PID));
  	DeviceConf[ubTargetIndex].IBIPayload =
  			__HAL_I3C_GET_IBI_PAYLOAD(
  					__HAL_I3C_GET_BCR(aTargetDesc[ubTargetIndex]->TARGET_BCR_DCR_PID));
  	DeviceConf[ubTargetIndex].CtrlRoleReqAck =
  			__HAL_I3C_GET_CR_CAPABLE(
  					__HAL_I3C_GET_BCR(aTargetDesc[ubTargetIndex]->TARGET_BCR_DCR_PID));
  	DeviceConf[ubTargetIndex].CtrlStopTransfer = DISABLE;

  	if (HAL_I3C_Ctrl_ConfigBusDevices(&hi3c1,
  			&DeviceConf[ubTargetIndex], 1U) != HAL_OK) {
  		Error_Handler();
  	}
  }

  /*##- Start the listen mode process ####################################*/
  /* Activate notifications for specially for this example
   - In Band Interrupt requested by a Target. */
  if (HAL_I3C_ActivateNotification(&hi3c1, NULL, HAL_I3C_IT_IBIIE)
  		!= HAL_OK) {
  	/* Error_Handler() function is called when error occurs. */
  	Error_Handler();
  }

  // Enable accelerometer to start receiving the IBIs.
  uint8_t acc_en = 0x0F;
  I3C_WRITE_REGS(&hi3c1, TARGET1_DYN_ADDR,0x30, &acc_en, 1);


  // -------------IBI Configuration END----------------------------------------------------------------------------------

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
	// -------------IBI listening loop start------------------------------------------------------------------------------
    /*##- Start the listen mode process for IBI ##################################*/
	while (uwIBIRequested == 0U) {
	}

	/* Getting the information from the last IBI request */
	if (HAL_I3C_GetCCCInfo(&hi3c1, EVENT_ID_IBI, &CCCInfo) != HAL_OK) {
		/* Error_Handler() function is called when error occurs. */
		Error_Handler();
	} else {
		/* Turn LED2 on: Inform of IBI completion treatment */
		BSP_LED_Toggle(LED2);
		HAL_Delay(100);
		uint8_t acc_disen = 0x00;
		I3C_WRITE_REGS(&hi3c1, TARGET1_DYN_ADDR,0x30, &acc_disen, 1);
	}

	/* Reset Global variable */
	uwIBIRequested = 0U;
	// -------------IBI listening loop end--------------------------------------------------------------------------------
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
  }

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 62;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 1;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 4096;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    Error_Handler();
  }
}

/**
 * @brief I3C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I3C1_Init(void) {
  /* USER CODE BEGIN I3C1_Init 0 */

  /* USER CODE END I3C1_Init 0 */

  I3C_FifoConfTypeDef sFifoConfig = {0};
  I3C_CtrlConfTypeDef sCtrlConfig = {0};

  /* USER CODE BEGIN I3C1_Init 1 */

  /* USER CODE END I3C1_Init 1 */
  hi3c1.Instance = I3C1;
  hi3c1.Mode = HAL_I3C_MODE_CONTROLLER;
  hi3c1.Init.CtrlBusCharacteristic.SDAHoldTime = HAL_I3C_SDA_HOLD_TIME_1_5;
  hi3c1.Init.CtrlBusCharacteristic.WaitTime = HAL_I3C_OWN_ACTIVITY_STATE_0;
  hi3c1.Init.CtrlBusCharacteristic.SCLPPLowDuration = 0x09;
  hi3c1.Init.CtrlBusCharacteristic.SCLI3CHighDuration = 0x09;
  hi3c1.Init.CtrlBusCharacteristic.SCLODLowDuration = 0x55;
  hi3c1.Init.CtrlBusCharacteristic.SCLI2CHighDuration = 0x55;
  hi3c1.Init.CtrlBusCharacteristic.BusFreeDuration = 0x32;
  hi3c1.Init.CtrlBusCharacteristic.BusIdleDuration = 0xf8;
  if (HAL_I3C_Init(&hi3c1) != HAL_OK) {
    Error_Handler();
  }

  /** Configure FIFO
   */
  sFifoConfig.RxFifoThreshold = HAL_I3C_RXFIFO_THRESHOLD_1_4;
  sFifoConfig.TxFifoThreshold = HAL_I3C_TXFIFO_THRESHOLD_1_4;
  sFifoConfig.ControlFifo = HAL_I3C_CONTROLFIFO_DISABLE;
  sFifoConfig.StatusFifo = HAL_I3C_STATUSFIFO_DISABLE;
  if (HAL_I3C_SetConfigFifo(&hi3c1, &sFifoConfig) != HAL_OK) {
    Error_Handler();
  }

  /** Configure controller
   */
  sCtrlConfig.DynamicAddr = 0;
  sCtrlConfig.StallTime = 0x00;
  sCtrlConfig.HotJoinAllowed = DISABLE;
  sCtrlConfig.ACKStallState = DISABLE;
  sCtrlConfig.CCCStallState = DISABLE;
  sCtrlConfig.TxStallState = DISABLE;
  sCtrlConfig.RxStallState = DISABLE;
  sCtrlConfig.HighKeeperSDA = DISABLE;


  if (HAL_I3C_Ctrl_Config(&hi3c1, &sCtrlConfig) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN I3C1_Init 2 */

  /* USER CODE END I3C1_Init 2 */
}

/**
 * @brief ICACHE Initialization Function
 * @param None
 * @retval None
 */
static void MX_ICACHE_Init(void) {
  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache (default 2-ways set associative cache)
   */
  if (HAL_ICACHE_Enable() != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */
}

/**
 * @brief MEMORYMAP Initialization Function
 * @param None
 * @retval None
 */
static void MX_MEMORYMAP_Init(void) {
  /* USER CODE BEGIN MEMORYMAP_Init 0 */

  /* USER CODE END MEMORYMAP_Init 0 */

  /* USER CODE BEGIN MEMORYMAP_Init 1 */

  /* USER CODE END MEMORYMAP_Init 1 */
  /* USER CODE BEGIN MEMORYMAP_Init 2 */

  /* USER CODE END MEMORYMAP_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
 * @brief I3C target request a dynamic address callback.
 *        The main objective of this user function is to check if a target request a dynamic address.
 *        if the case we should assign a dynamic address to the target.
 * @par Called functions
 * - HAL_I3C_TgtReqDynamicAddrCallback()
 * - HAL_I3C_Ctrl_SetDynamicAddress()
 * @retval None
 */
void HAL_I3C_TgtReqDynamicAddrCallback(I3C_HandleTypeDef *hi3c, uint64_t targetPayload) {
  /* Update Payload on aTargetDesc */
  aTargetDesc[uwTargetCount]->TARGET_BCR_DCR_PID = targetPayload;

  /* Send associated dynamic address */
  HAL_I3C_Ctrl_SetDynAddr(hi3c, aTargetDesc[uwTargetCount++]->DYNAMIC_ADDR);
}

/**
 * @brief  Controller dynamic address assignment Complete callback.
 * @param  hi3c : [IN] Pointer to an I3C_HandleTypeDef structure that contains the configuration information
 *                     for the specified I3C.
 * @retval None
 */
void HAL_I3C_CtrlDAACpltCallback(I3C_HandleTypeDef *hi3c) {
  /* Turn LED2 on: Transfer in Transmission process is correct */
  BSP_LED_On(LED2);
}

/**
 * @brief I3C notify callback after receiving a notification.
 *        The main objective of this user function is to check on the notification ID and assign 1 to the global
 *        variable used to indicate that the event is well finished.
 * @par Called functions
 * - HAL_I3C_NotifyCallback()
 * @retval None
 */
void HAL_I3C_NotifyCallback(I3C_HandleTypeDef *hi3c, uint32_t eventId) {
  if ((eventId & EVENT_ID_IBI) == EVENT_ID_IBI) {
    uwIBIRequested = 1;
  } else {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* Error if LED2 is slowly blinking (1 sec. period) */
  while (1) {
    BSP_LED_Toggle(LED2);
    HAL_Delay(100);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
