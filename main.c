
#include "msp430fr5969.h"

#include "InfUserStream.h"
#include "HwAbUart.h"

#include "MemPool/MemPoolExample.h"

//****************************************************************************************

void Init_GPIO(void);
void Init_Clock(void);
void Init_UART(void);

static void HardwareInit( void );
static void SpinUntilTxBufferIsClear( InfUserStream_t *userInterface );

//****************************************************************************************
int _system_pre_init(void) // called
{
    // Stop Watchdog timer
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    /*==================================*/
    /* Choose if segment initialization */
    /* should be done or not. */
    /* Return: 0 to omit initialization */
    /* 1 to run initialization */
    /*==================================*/
    return 1;
}

//****************************************************************************************
void main(void) {
	InfUserStream_t userInterface;

    HardwareInit();


	HwAbUart_Init();
    InfUserStream_InitDefaults(&userInterface);
    userInterface.sendBytes = &HwAbUart_Send;
    userInterface.sendString = &HwAbUart_SendString;
    userInterface.isTxBufferClear = &HwAbUart_IsDoneTransmitting;
    userInterface.getRxByte = &HwAbUart_GetRxByte;
    userInterface.getRxContent = &HwAbUart_GetRxContent;


    SpinUntilTxBufferIsClear( &userInterface );
    (*userInterface.sendString)("Begin MemPool Example\r\n");
    SpinUntilTxBufferIsClear( &userInterface );


	MemPoolExample( &userInterface );


    SpinUntilTxBufferIsClear( &userInterface );
	(*userInterface.sendString)("Completed MemPool Example");
	(*userInterface.sendString)("\r\n\r\n");


	__no_operation();
    for(;;)
    {
        __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3
        __no_operation();
    }
}

//****************************************************************************************
static void SpinUntilTxBufferIsClear( InfUserStream_t *userInterface )
{
	while( ! (*userInterface->isTxBufferClear)() ); // BUSY WAIT
}

//****************************************************************************************
// Board initializations
static void HardwareInit( void )
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    Init_GPIO();
    Init_Clock();

    Init_UART();

    __enable_interrupt();     // Enable global interrupt
}

//****************************************************************************************
void Init_GPIO()
{
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    // Set all GPIO pins to output low for low power
    P1OUT = 0;
    P2OUT = 0;
    P3OUT = 0;
    P4OUT = 0;
    PJOUT = 0;

    // Set output pins
    P1DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
    P2DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
    P3DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
    P4DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
    PJDIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9 | BITA | BITB | BITC | BITD | BITE | BITF;

	// Configure P2.0 - UCA0TXD and P2.1 - UCA0RXD
    P2SEL1 |= (BIT0 | BIT1);
    P2SEL0 &= ~(BIT0 | BIT1);

    // Set PJ.4 and PJ.5 as Primary Module Function Input, LFXT.
    PJSEL1 &= ~BIT4;
    PJSEL0 |= BIT4;
}

//****************************************************************************************
void Init_Clock()
{
    //Unlock CS control register
    CSCTL0 = CSKEY;

    // Set DCO frequency to 8 MHz (DCORSEL=0b, DCOFSEL=110b)
    CSCTL1 = DCOFSEL_6;

    //Set ACLK=LFXT (SELA=000b) divide by 1 (DIVA=000b)
    CSCTL2 = (CSCTL2 & ~(SELA0 | SELA1 | SELA2)) | SELA__LFXTCLK;
    CSCTL3 = (CSCTL3 & ~(DIVA0 | DIVA1 | DIVA2)) | DIVA__1;

    // Set SMCLK = DCO (SELS=011b) divide by 1 (DIVS=000b)
    CSCTL2 = (CSCTL2 & ~(SELS0 | SELS1 | SELS2)) | SELS__DCOCLK;
    CSCTL3 = (CSCTL3 & ~(DIVS0 | DIVS1 | DIVS2)) | DIVS__1;

    // Set MCLK = DCO (SELM=011b) divide by 1 (DIVM=000b)
    CSCTL2 = (CSCTL2 & ~(SELM0 | SELM1 | SELM2)) | SELM__DCOCLK;
    CSCTL3 = (CSCTL3 & ~(DIVM0 | DIVM1 | DIVM2)) | DIVM__1;


    //Start LFXT (32.7638 kHz)
    CSCTL4 |= LFXTDRIVE_3; // Max drive strength
    CSCTL4 &= ~LFXTBYPASS;
    while( CSCTL5 & LFXTOFFG )
    {	// Busy-wait until crystal is stable
    	CSCTL5 &= ~(LFXTOFFG);	// LFXT oscillator fault flag
    	SFRIFG1 &= ~OFIFG;     	//Clear OFIFG fault flag
    }

    CSCTL4 = (CSCTL4 & ~(LFXTDRIVE0 | LFXTDRIVE1)) | LFXTDRIVE_0; // Min drive strength

    //Switch ON LFXT oscillator
    CSCTL4 &= ~LFXTOFF;

    // Lock CS control register
    CSCTL0_H = 0;
}

//****************************************************************************************
void Init_UART()
{
    UCA0CTLW0 |= UCSWRST; // disable uart module
    UCA0CTLW0 = (UCA0CTLW0 & ~(UCSWRST)) |
    		// UCPEN |		// Uncomment to enable parity
    		// UCPAR |		// Uncomment for even parity, otherwise odd
    		// UCMSB |		// Uncomment for MSB first, otherwise LSB first
    		// UC7BIT |		// Uncomment for 7bit data, otherwise 8bit
    		// UCSPB |		// Uncomment for 2 stop bits, otherwise 1 stop bit
    		// UCSYNC |		// Uncomment for synchronous mode, otherwise asynchronous
    		UCMODE_0 | // UART mode
    		UCSSEL__SMCLK; // Set BRCLK = SMCLK

#if 0
    // 115200 baud with oversampling enabled
    UCA0BRW = 4;
	UCA0MCTLW = (uint16_t) UCBRF0 * 5 |
				(uint16_t) UCBRS0 * 0x55 |
				(uint16_t) UCOS16; // Enable oversampling
#else
    // 57600 baud with oversampling enabled
    UCA0BRW = 8;
	UCA0MCTLW = (uint16_t) UCBRF0 * 10 |
				(uint16_t) UCBRS0 * 0xF7 |
				(uint16_t) UCOS16; // Enable oversampling
#endif


	UCA0STATW = 0;  // clear all error flags
	UCA0ABCTL = 0;	// disable auto-baud
	UCA0IRCTL = 0;	// disable IrDA

    UCA0CTLW0 &= ~UCSWRST; // enable uart module

	UCA0IFG	= 0; 	// clear all interrupt flags
	UCA0IE  = // Enable interrupts
			//UCTXCPTIE	| // Transmit complete interrupt enable
			//UCSTTIE 	| // Start bit interrupt enable
			//UCTXIE 		| // Transmit interrupt enable
			UCRXIE		; // Receive interrupt enable

}

