#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "cmsis_os.h"


xTaskHandle xHandle_Led_1;
xTaskHandle xHandle_Led_2;
xTaskHandle xHandle_Led_3;

osThreadId defaultTaskHandle;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);


void tarea_led_1(void *p);
void tarea_led_2(void *p);
void tarea_led_3(void *p);
void TareaPrior(void *p);

unsigned long xor128(void);

int main(void)
{

	HAL_Init();

	SystemClock_Config();


	MX_GPIO_Init();

	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	xTaskCreate(TareaPrior,"tarea_prior",128,NULL,tskIDLE_PRIORITY + 3,NULL);
	xTaskCreate(tarea_led_1,"tarea_led_1",128,NULL,tskIDLE_PRIORITY + 1,&xHandle_Led_1);
	xTaskCreate(tarea_led_2,"tarea_led_2",128,NULL,tskIDLE_PRIORITY + 1,&xHandle_Led_2);
	xTaskCreate(tarea_led_3,"tarea_led_3",128,NULL,tskIDLE_PRIORITY + 1,&xHandle_Led_3);

	osKernelStart();

	while (1)
	{

	}
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);


  //Setup Leds
  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  /*Configuro Pulsador*/
  /*Configure GPIO pin : PB6 */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void tarea_led_1(void *p)
{
	while(1)
	{
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		//GPIOC -> ODR |= GPIO_PIN_13;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);//Prende con estado RESET por ser activo bajo?
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	}
}

void tarea_led_2(void *p)
{
	while(1)
	{
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		//GPIOC -> ODR &= GPIO_PIN_13;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	}
}

void tarea_led_3(void *p)
{
	while(1)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	}
}

void TareaPrior(void *p)
{
	static int ticks_start=0;
	static int ticks_end=0;
	int ticks=0;
	static int Prioridad=0;
	static int estado=1;
	unsigned long Random;
	static int set_flag=0;

	while(1)
	{
		switch(estado)
		{
		case 1:
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET)
			{

				ticks_start=xTaskGetTickCount();
				estado=2;
			}
			break;
		case 2:
			ticks_end=xTaskGetTickCount();
			ticks=ticks_end-ticks_start;
			if(ticks>100 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET )
			{
				estado=3;
			}
			break;
		case 3:
			if(Prioridad<4)
			{
				Prioridad++;
				if(Prioridad == 4)
				{
					Random=xor128();
					Random=Random & 0x00000003;
				}
			}
			else
			{
				Prioridad=0;
			}
			set_flag=0;
			estado=1;
			break;
		default:
			estado=1;
			break;
		}

		if (set_flag==0)
		{
			set_flag=1;
			switch(Prioridad)
			{
			case 0:
				vTaskPrioritySet( xHandle_Led_1, tskIDLE_PRIORITY + 1);
				vTaskPrioritySet( xHandle_Led_2, tskIDLE_PRIORITY + 1);
				vTaskPrioritySet( xHandle_Led_3, tskIDLE_PRIORITY + 1);
				break;
			case 1:
				vTaskPrioritySet( xHandle_Led_1, tskIDLE_PRIORITY + 2);
				vTaskPrioritySet( xHandle_Led_2, tskIDLE_PRIORITY + 1);
				vTaskPrioritySet( xHandle_Led_3, tskIDLE_PRIORITY + 1);
				break;
			case 2:
				vTaskPrioritySet( xHandle_Led_1, tskIDLE_PRIORITY + 1);
				vTaskPrioritySet( xHandle_Led_2, tskIDLE_PRIORITY + 2);
				vTaskPrioritySet( xHandle_Led_3, tskIDLE_PRIORITY + 1);
				break;
			case 3:
				vTaskPrioritySet( xHandle_Led_1, tskIDLE_PRIORITY + 1);
				vTaskPrioritySet( xHandle_Led_2, tskIDLE_PRIORITY + 1);
				vTaskPrioritySet( xHandle_Led_3, tskIDLE_PRIORITY + 2);
				break;
			case 4:
				if(Random == 1 || Random == 0)
				{
					vTaskPrioritySet( xHandle_Led_1, tskIDLE_PRIORITY + 2);
					vTaskPrioritySet( xHandle_Led_2, tskIDLE_PRIORITY + 1);
					vTaskPrioritySet( xHandle_Led_3, tskIDLE_PRIORITY + 1);
				}
				if(Random == 2)
				{
					vTaskPrioritySet( xHandle_Led_1, tskIDLE_PRIORITY + 1);
					vTaskPrioritySet( xHandle_Led_2, tskIDLE_PRIORITY + 2);
					vTaskPrioritySet( xHandle_Led_3, tskIDLE_PRIORITY + 1);
				}
				if(Random == 3)
				{
					vTaskPrioritySet( xHandle_Led_1, tskIDLE_PRIORITY + 1);
					vTaskPrioritySet( xHandle_Led_2, tskIDLE_PRIORITY + 1);
					vTaskPrioritySet( xHandle_Led_3, tskIDLE_PRIORITY + 2);
				}
				break;
			default:
				Prioridad=0;
				break;
			}
		}
	vTaskDelay(1);
	}
}

unsigned long xor128(void){
	static unsigned long x=0x5555AAAA,y=0x5555AAAA,z=0x5555AAAA,w=1536114;
	uint32_t t=(x^(x<<11));
	x=y;
	y=z;
	z=w;

	return w=(w^(w>>19))^(t^(t>>8));
}


void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }

}


void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT


void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
