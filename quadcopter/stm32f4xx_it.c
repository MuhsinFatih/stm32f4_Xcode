#include "stm32f4xx_it.h"
#include "stm32f4xx_conf.h"

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void SysTick_Handler(void)
{

}

#define MAX_WORDLEN		255
volatile char receivedStr[MAX_WORDLEN + 1];
extern void usart_puts(USART_TypeDef *USARTx, volatile char *str);
// Interrupt request handler for all usart2 interrupts
// This interrupt handler will be executed each time a char is received in usart2
void USART2_IRQHandler(){
	// make sure it was usart2 and we didnt screw up things
	if(USART_GetITStatus(USART2, USART_IT_RXNE)){
		static int count = 0;
		char ch = USART2->DR;
		if((ch != '\n')){
			receivedStr[count++] = ch;
		} else {
			receivedStr[count] = '\n';
			count = 0;
			usart_puts(USART2, receivedStr);
		}
	}
	
}























