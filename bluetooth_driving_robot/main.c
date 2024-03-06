#include <stm32f4xx.h>
#include <stdio.h>
#include <string.h>

volatile int speed=999;
char buffer[7];
volatile char receivedChar;
volatile int i=0;
volatile int bool=0;

// Function to initialize System clock
void HSEInit(void) 							// 8MHz
{
	RCC->CFGR = 0x01; 						//select HSE as system clock
	RCC->CR |= 1<<16; 						//HSE on: activation du HSE
	while (!(RCC->CR & (1<<17))); 			//wait until HSE ready
}
// Function to initialize USART2 for Bluetooth communication
void USART2_Init() {
    // Enable USART2 and GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure PA2 (USART2_TX) and PA3 (USART2_RX) as alternate function
    GPIOA->AFR[0] |= (0x7 << (4 * 2)) | (0x7 << (4 * 3));
    GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1 ;

    // Configure USART2
    USART2->CR1 &= ~USART_CR1_UE;  			// Disable USART2
    USART2->BRR = 8000000/9600 ;          	// Baud rate 9600
    USART2->CR1 |= USART_CR1_RE ;			// Enable receiver
    USART2->CR1 |= USART_CR1_UE ;			// Enable USART2
    USART2->CR1 |= USART_CR1_RXNEIE;		// Enable RXNE Flag
    USART2->CR1 |= USART_CR1_IDLEIE;  		// Enable IDLE flag

    // Enable USART2 interrupt in NVIC
    NVIC_EnableIRQ(USART2_IRQn);
}

// USART2 interrupt service routine
void USART2_IRQHandler() {
    if (USART2->SR & USART_SR_RXNE) {
        // RXNE flag is set, indicating data in the receive register
    	receivedChar = USART2->DR;               // Read the received character

    	if (receivedChar == '\n' || receivedChar == '\r') {
    		// End of line, terminate the string
    		receivedChar= '\0';
    	}
    	buffer[i] = receivedChar;				 // store the received character in buffer
    	i++;
        }

    if (USART2->SR & USART_SR_IDLE) {
    	USART2->DR;
    	USART2->SR;								 // reset IDLE Flag
        bool=1;
        buffer[i] ='\0';
        i=0;
    }
}
void TIM3_Init() {
    // Enable TIM3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Configure TIM2 for PWM
    TIM3->CR1 = 0;  	// Clear control register 1
    TIM3->CR2 = 0;  	// Clear control register 2
    TIM3->PSC = 19;  	// Set prescaler
    TIM3->ARR = 999 ;  	// Set auto-reload value

    // Configure Channel 1, 2, 3, and 4 on PWM Mode 1
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;					// Channel 1
    TIM3->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;  				// Channel 2
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;  				// Channel 3
    TIM3->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;  				// Channel 4

    // Link TIM3 channels with GPIO pins
    // GPIOB and GPIOA is used here
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;  // Enable GPIOB clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable GPIOA clock
    // Set alternate function AF2 (TIM3) for PB0, PB1, PA6 and PA7
    GPIOB->AFR[0] |= (0x2 << (4 * 0)) | (0x2 << (4 * 1));
    GPIOA->AFR[0] |= (0x2 << (4 * 6)) | (0x2 << (4 * 7));
    // Configure GPIO pins for alternate function mode
    GPIOB->MODER |= GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1;
    GPIOA->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
    // Enable capture/compare channel 1, 2, 3, and 4 outputs
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;
    //Counter enable
    TIM3->CR1|= 0x1;

}
void setMotorDirection(int in1,int in2,int in3,int in4){
	TIM3->CCR1 = (speed*in1);
	TIM3->CCR2 = (speed*in2);
	TIM3->CCR3 = (speed*in3);
	TIM3->CCR4 = (speed*in4);
}
int main() {
    HSEInit();   	// Initialize system clock
    USART2_Init();  // Initialize USART2 for Bluetooth communication
    TIM3_Init();	// Initialize TIM3 for motors PWMs
    while (1) {
    	while(bool){
    		if (strcmp(buffer, "Forward") == 0) {
    		    setMotorDirection(1, 0, 1, 0);
    		} else if (strcmp(buffer, "Backward") == 0) {
    		    setMotorDirection(0, 1, 0, 1);
    		} else if (strcmp(buffer, "Right") == 0) {
    		    setMotorDirection(1, 0, 0, 0);
    		} else if (strcmp(buffer, "Left") == 0) {
    		    setMotorDirection(0, 0, 1, 0);
    		} else if (strcmp(buffer, "PivotR") == 0) {
    		    setMotorDirection(1, 0, 0, 1);
    		} else if (strcmp(buffer, "PivotL") == 0) {
    		    setMotorDirection(0, 1, 1, 0);
    		} else {
    		    setMotorDirection(0, 0, 0, 0);
    		}


    		bool=0;
    		i=0;
    		while(strcmp(buffer, "\0")){
    			buffer[i]='\0';
    			i++;
    		}
    		i=0;


    	}

    }

}
