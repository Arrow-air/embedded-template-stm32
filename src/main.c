/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include <stdio.h>
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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
  uint8_t MID[4] = {0x90,0x00,0x00,0x00};

  const uint8_t Read  = 0b10000000;
  const uint8_t Write = 0b00000000;

  int8_t readReg[1] = {0};
  int16_t dataFull = 0;
  uint16_t dataPrintLen = 0; 

  uint8_t USER_CTRL[1] = {Write|0x6A};
  uint8_t WHO_AM_I[1] = {Read|0x75};
  uint8_t disableI2C[1] = {Read|10};// 0b00010000

  uint8_t CONFIG[1] = {Write|0x1A};
  uint8_t DLFP_FSYNC_FIF0_CONFIG[1] = {Write|0x00};
  uint8_t GYRO_CONFIG[1] = {Write|0x1B}; 
   //uint8_t GYRO_CONFIG[1] = {Write|0x1B}; 

  uint8_t GYRO_FS_SEL_250[1] = {Write|0x00}; // 2g
  uint8_t GYRO_FS_SEL_500[1] = {Write|0x08}; // 4g
  uint8_t GYRO_FS_SEL_1000[1] = {Write|0x10}; // 8g
  uint8_t GYRO_FS_SEL_2000[1] = {Write|0xD8};//0x18}; // 16g

  uint8_t TEMP_H[1] = {Read|0x41};
  uint8_t TEMP_L[1] = {Read|0x42};

  uint8_t GYRO_XOUT_H[1] = {Read|0x43};
  uint8_t GYRO_XOUT_L[1] = {Read|0x44};

  uint8_t GYRO_YOUT_H[1] = {Read|0x45};
  uint8_t GYRO_YOUT_L[1] = {Read|0x46};

  uint8_t GYRO_ZOUT_H[1] = {Read|0x47};
  uint8_t GYRO_ZOUT_L[1] = {Read|0x48};

  uint8_t ACCEL_CONFIG[1] = {Write|0x1C}; 

  uint8_t ACCEL_FS_SEL_2g[1] = {Write|0x00}; // 2g
  uint8_t ACCEL_FS_SEL_4g[1] = {Write|0x08}; // 4g
  uint8_t ACCEL_FS_SEL_8g[1] = {Write|0x10}; // 8g
  uint8_t ACCEL_FS_SEL_16g[1] = {Write|0x18}; // 16g

  int  mult[4] = {16384, 8192, 4096, 2048};

  float  mult2[4] = {131.0, 65.2, 32.8, 16.4};

  uint8_t ACCEL_XOUT_H[1] = {Read|0x3B};
  uint8_t ACCEL_XOUT_L[1] = {Read|0x3C};

  uint8_t ACCEL_YOUT_H[1] = {Read|0x3D};
  uint8_t ACCEL_YOUT_L[1] = {Read|0x3E};

  uint8_t ACCEL_ZOUT_H[1] = {Read|0x3F};
  uint8_t ACCEL_ZOUT_L[1] = {Read|0x40};
  
  double TEMP;

  double ACCEL_X;
  double ACCEL_Y;
  double ACCEL_Z;

  double GYRO_X;
  double GYRO_Y;
  double GYRO_Z;

  double MAG_X;
  double MAG_Y;
  double MAG_Z;

  uint8_t data[2] = {0};
  
  uint8_t uart_buf[50];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
uint8_t SPISend(uint8_t address, uint8_t* data);
uint8_t SPIReceive(uint8_t address, uint8_t* data);
uint8_t UARTPrint(uint8_t address, uint8_t* data);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */


  HAL_GPIO_WritePin(Flash_CS_GPIO_Port,Flash_CS_Pin,1);
  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);

  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
  HAL_SPI_Transmit(&hspi1,(uint8_t *)USER_CTRL,sizeof(USER_CTRL),10);
  HAL_SPI_Transmit(&hspi1,(uint8_t *)disableI2C,sizeof(disableI2C),10);
  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);

  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
  HAL_SPI_Transmit(&hspi1,(uint8_t *)CONFIG,sizeof(CONFIG),10);
  HAL_SPI_Transmit(&hspi1,(uint8_t *)DLFP_FSYNC_FIF0_CONFIG,sizeof(DLFP_FSYNC_FIF0_CONFIG),10);
  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);

  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
  HAL_SPI_Transmit(&hspi1,(uint8_t *)ACCEL_CONFIG,sizeof(ACCEL_CONFIG),10);
  HAL_SPI_Transmit(&hspi1,(uint8_t *)ACCEL_FS_SEL_2g,sizeof(ACCEL_FS_SEL_2g),10);
  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);

  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
  HAL_SPI_Transmit(&hspi1,(uint8_t *)GYRO_CONFIG,sizeof(GYRO_CONFIG),10);
  HAL_SPI_Transmit(&hspi1,(uint8_t *)GYRO_FS_SEL_2000,sizeof(GYRO_FS_SEL_2000),10);
  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
    HAL_SPI_Transmit(&hspi1,(uint8_t *)TEMP_H,sizeof(TEMP_H),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[0] = readReg[0];
    HAL_SPI_Transmit(&hspi1,(uint8_t *)TEMP_L,sizeof(TEMP_L),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[1] = readReg[0];
    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);

    dataFull = (data[0] << 8 )| data[1];
    TEMP = dataFull;

    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
    HAL_SPI_Transmit(&hspi1,(uint8_t *)GYRO_XOUT_H,sizeof(GYRO_XOUT_H),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[0] = readReg[0];
    HAL_SPI_Transmit(&hspi1,(uint8_t *)GYRO_XOUT_L,sizeof(GYRO_XOUT_L),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[1] = readReg[0];
    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);

    dataFull = (data[0] << 8 )| data[1];
    GYRO_X = dataFull/mult2[3];

    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
    HAL_SPI_Transmit(&hspi1,(uint8_t *)GYRO_YOUT_H,sizeof(GYRO_YOUT_H),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[0] = readReg[0];
    HAL_SPI_Transmit(&hspi1,(uint8_t *)GYRO_YOUT_L,sizeof(GYRO_YOUT_L),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[1] = readReg[0];
    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);

    dataFull = (data[0] << 8 )| data[1];
    GYRO_Y = dataFull/mult2[3];

    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
    HAL_SPI_Transmit(&hspi1,(uint8_t *)GYRO_ZOUT_H,sizeof(GYRO_ZOUT_H),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[0] = readReg[0];
    HAL_SPI_Transmit(&hspi1,(uint8_t *)GYRO_ZOUT_L,sizeof(GYRO_ZOUT_L),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[1] = readReg[0];
    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);

    dataFull = (data[0] << 8 )| data[1];
    GYRO_Z = dataFull/mult2[3];

    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
    HAL_SPI_Transmit(&hspi1,(uint8_t *)ACCEL_XOUT_H,sizeof(ACCEL_XOUT_H),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[0] = readReg[0];
    HAL_SPI_Transmit(&hspi1,(uint8_t *)ACCEL_XOUT_L,sizeof(ACCEL_XOUT_L),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[1] = readReg[0];
    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);

    dataFull = (data[0] << 8 )| data[1];
    ACCEL_X = dataFull*9.81/mult[0];

    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
    HAL_SPI_Transmit(&hspi1,(uint8_t *)ACCEL_YOUT_H,sizeof(ACCEL_YOUT_H),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[0] = readReg[0];
    HAL_SPI_Transmit(&hspi1,(uint8_t *)ACCEL_YOUT_L,sizeof(ACCEL_YOUT_L),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[1] = readReg[0];
    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);

    dataFull = (data[0] << 8 )| data[1];
    ACCEL_Y = dataFull*9.81/mult[0];

    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
    HAL_SPI_Transmit(&hspi1,(uint8_t *)ACCEL_ZOUT_H,sizeof(ACCEL_ZOUT_H),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[0] = readReg[0];
    HAL_SPI_Transmit(&hspi1,(uint8_t *)ACCEL_ZOUT_L,sizeof(ACCEL_ZOUT_L),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
    data[1] = readReg[0];
    HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);

    dataFull = (data[0] << 8 )| data[1];
    ACCEL_Z = dataFull*9.81/mult[0];

    uint8_t RegDataT[] = "\n\r TEMP = ";
    HAL_UART_Transmit(&huart1,RegDataT,sizeof(RegDataT),10);
    dataPrintLen = sprintf((char *)uart_buf,"%.2f",TEMP);  
    HAL_UART_Transmit(&huart1,uart_buf,dataPrintLen,10);

    uint8_t RegDataGx[] = " | GYRO_XOUT_Full = ";
    HAL_UART_Transmit(&huart1,RegDataGx,sizeof(RegDataGx),10);
    dataPrintLen = sprintf((char *)uart_buf,"%.2f",GYRO_X);  
    HAL_UART_Transmit(&huart1,uart_buf,dataPrintLen,10);

    uint8_t RegDataGy[] = " | GYRO_YOUT_Full = ";
    HAL_UART_Transmit(&huart1,RegDataGy,sizeof(RegDataGy),10);
    dataPrintLen = sprintf((char *)uart_buf,"%.2f",GYRO_Y);  
    HAL_UART_Transmit(&huart1,uart_buf,dataPrintLen,10);

    uint8_t RegDataGz[] = " | GYRO_ZOUT_Full = ";
    HAL_UART_Transmit(&huart1,RegDataGz,sizeof(RegDataGz),10);
    dataPrintLen = sprintf((char *)uart_buf,"%.2f",GYRO_Z);  
    HAL_UART_Transmit(&huart1,uart_buf,dataPrintLen,10);

    uint8_t RegDataAx[] = " | ACCEL_XOUT_Full = ";
    HAL_UART_Transmit(&huart1,RegDataAx,sizeof(RegDataAx),10);
    dataPrintLen = sprintf((char *)uart_buf,"%.2f",ACCEL_X);  
    HAL_UART_Transmit(&huart1,uart_buf,dataPrintLen,10);

    uint8_t RegDataAy[] = " | ACCEL_YOUT_Full = ";
    HAL_UART_Transmit(&huart1,RegDataAy,sizeof(RegDataAy),10);
    dataPrintLen = sprintf((char *)uart_buf,"%.2f",ACCEL_Y);  
    HAL_UART_Transmit(&huart1,uart_buf,dataPrintLen,10);

    uint8_t RegDataAz[] = " | ACCEL_ZOUT_Full = ";
    HAL_UART_Transmit(&huart1,RegDataAz,sizeof(RegDataAz),10);
    dataPrintLen = sprintf((char *)uart_buf,"%.2f",ACCEL_Z);  
    HAL_UART_Transmit(&huart1,uart_buf,dataPrintLen,10);

    /*
    uint8_t Hello[] = "Hello, World !\n\r";
    uint8_t Bye[] = "Bye, World !\n\r";

    HAL_GPIO_WritePin(Flash_CS_GPIO_Port,Flash_CS_Pin,0);
    HAL_SPI_Transmit(&hspi1,(uint8_t *)MID,sizeof(MID),10);
    HAL_SPI_Receive(&hspi1,(uint8_t *)data,sizeof(data),10);
    HAL_GPIO_WritePin(Flash_CS_GPIO_Port,Flash_CS_Pin,1);

    
    if(HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin) == 0)
    {
      HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin,0); 
      dataPrintLen = sprintf(uart_buf,"0x%X\n\r",(unsigned int)data[0]);
      HAL_UART_Transmit(&huart1,(uint8_t *)Hello,sizeof(Hello),10);
      HAL_UART_Transmit(&huart1,(uint8_t *)uart_buf,dataPrintLen,10);
    }
    else if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == 0)
    {
      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,0); 
      dataPrintLen = sprintf(uart_buf,"0x%X\n\r",(unsigned int)data[1]);
      HAL_UART_Transmit(&huart1,(uint8_t *)Bye,sizeof(Bye),10);
      HAL_UART_Transmit(&huart1,(uint8_t *)uart_buf,dataPrintLen,10);
    }
    else
    {
      HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin,1);
      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,1); 
      if(HAL_UART_Receive(&huart1,(uint8_t *)uart_buf2,sizeof(uart_buf2),10))
      {
         HAL_UART_Transmit(&huart1,(uint8_t *)uart_buf2,sizeof(uart_buf2),10);
      }
      //dataPrintLen = sprintf(uart_buf2,"%C",(unsigned int)uart_buf2);
     
    }*/
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Flash_CS_GPIO_Port, Flash_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MPU_CS_GPIO_Port, MPU_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : KEY1_Pin KEY0_Pin */
  GPIO_InitStruct.Pin = KEY1_Pin|KEY0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LED0_Pin LED1_Pin */
  GPIO_InitStruct.Pin = LED0_Pin|LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Flash_CS_Pin */
  GPIO_InitStruct.Pin = Flash_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(Flash_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MPU_CS_Pin */
  GPIO_InitStruct.Pin = MPU_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MPU_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

uint8_t SPISend(uint8_t address, uint8_t* data)
{
  const uint8_t Write = 0b00000000;
  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
  HAL_SPI_Transmit(&hspi1,(uint8_t *)(Write | address),sizeof((Write | address)),10);
  HAL_SPI_Transmit(&hspi1,(uint8_t *)data,sizeof(data),10);
  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);
  return(0);
}

uint8_t SPIReceive(uint8_t address, uint8_t* data)
{
  const uint8_t Read  = 0b10000000;
  uint8_t readReg[1];
  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,0);
  HAL_SPI_Transmit(&hspi1,(uint8_t *)(Read | address),sizeof((Read | address)),10);
  HAL_SPI_Receive(&hspi1,(uint8_t *)readReg,sizeof(readReg),10);
  data[0] = readReg[0];
  HAL_GPIO_WritePin(MPU_CS_GPIO_Port,MPU_CS_Pin,1);
  return(0);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
