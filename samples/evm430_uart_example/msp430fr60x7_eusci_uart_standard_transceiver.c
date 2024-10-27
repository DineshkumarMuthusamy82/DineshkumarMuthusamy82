//******************************************************************************
//   MSP430FR60xx Demo - eUSCI_A0, UART Echo received character
//                     (ACLK 9600/SMCLK 9600/SMCLK 115200)
//
//   Description: The device will wait in LPM0/LPM3 (based on clock source)
//   until a UART character is received.
//   Then the device will echo the received character.
//   The UART can operate using ACLK at 9600, SMCLK at 115200 or SMCLK at 9600.
//   To configure the UART mode, change the following line:
//
//      #define UART_MODE       SMCLK_115200
//      to any of:
//      #define UART_MODE       SMCLK_115200
//      #define UART_MODE       SMCLK_9600
//      #define UART_MODE       ACLK_9600
//
//   UART RX ISR is used to handle communication.
//   ACLK = 32.768kHz, MCLK = SMCLK = DCO 16MHz.
//
//
//
//                   MSP430FR6047
//                 -----------------
//            /|\ |             P2.1|<-- Receive Data (UCA1RXD)
//             |  |                 |
//             ---|RST          P2.0|--> Transmit Data (UCA1TXD)
//                |                 |
//                |             PJ.5|--- LFXOUT
//                |                 |   |
//                |                 |  32kHz
//                |                 |   |
//                |             PJ.4|--- LFXIN
//
//   Nima Eskandari and Ryan Meredith
//   Texas Instruments Inc.
//   November 2017
//   Built with CCS V7.3
//******************************************************************************

#include <msp430.h>

//******************************************************************************
// Pin Config ******************************************************************
//******************************************************************************

#define LED_OUT     P1OUT
#define LED_DIR     P1DIR
#define LED0_PIN    BIT0
#define LED1_PIN    BIT1

//******************************************************************************
// UART Initialization *********************************************************
//******************************************************************************

#define SMCLK_115200     0
#define SMCLK_9600      1
#define ACLK_9600       2

#define UART_MODE       ACLK_9600//SMCLK_115200//
#if 0
void initUART()
{
    // Configure USCI_A0 for UART mode
    UCA1CTLW0 = UCSWRST;                      // Put eUSCI in reset

#if UART_MODE == SMCLK_115200

    UCA1CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK

    // Baud Rate Setting
    // Use Table 30-5 in Family User Guide
    UCA1BR0 = 8;
    UCA1BR1 = 0;
    UCA1MCTLW |= UCOS16 | UCBRF_10 | 0xF700;   //0xF700 is UCBRSx = 0xF7

#elif UART_MODE == SMCLK_9600

    UCA1CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK

    // Baud Rate Setting
    // Use Table 30-5 in Family User Guide
    UCA1BR0 = 104;
    UCA1BR1 = 0;
    UCA1MCTLW |= UCOS16 | UCBRF_2 | 0xD600;   //0xD600 is UCBRSx = 0xD6

#elif UART_MODE == ACLK_9600

    UCA1CTLW0 |= UCSSEL__ACLK;               // CLK = ACLK
    // Baud Rate calculation
    // 32768/(9600) = 3.4133
    // Fractional portion = 0.4133
    // Use Table 24-5 in Family User Guide
    UCA1BR0 = 3;                             // 32768/9600
    UCA1BR1 = 0;
    UCA1MCTLW |= 0x9200;    //0x9200 is UCBRSx = 0x92


#else
    # error "Please specify baud rate to 115200 or 9600"
#endif

    UCA1CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}
#else
void initUART()
{
    // Configure USCI_A3 for UART mode
    UCA3CTLW0 = UCSWRST;                      // Put eUSCI in reset

#if UART_MODE == SMCLK_115200

    UCA3CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK

    // Baud Rate Setting
    // Use Table 30-5 in Family User Guide
    UCA3BR0 = 8;
    UCA3BR1 = 0;
    UCA3MCTLW |= UCOS16 | UCBRF_10 | 0xF700;   //0xF700 is UCBRSx = 0xF7

#elif UART_MODE == SMCLK_9600

    UCA3CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK

    // Baud Rate Setting
    // Use Table 30-5 in Family User Guide
    UCA3BR0 = 104;
    UCA3BR1 = 0;
    UCA3MCTLW |= UCOS16 | UCBRF_2 | 0xD600;   //0xD600 is UCBRSx = 0xD6

#elif UART_MODE == ACLK_9600

    UCA3CTLW0 |= UCSSEL__ACLK;               // CLK = ACLK
    // Baud Rate calculation
    // 32768/(9600) = 3.4133
    // Fractional portion = 0.4133
    // Use Table 24-5 in Family User Guide
    UCA3BR0 = 3;                             // 32768/9600
    UCA3BR1 = 0;
    UCA3MCTLW |= 0x9200;    //0x9200 is UCBRSx = 0x92


#else
    # error "Please specify baud rate to 115200 or 9600"
#endif

    UCA3CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA3IE |= UCRXIE;                         // Enable USCI_A3 RX interrupt
}
#endif
//******************************************************************************
// Device Initialization *******************************************************
//******************************************************************************

void initGPIO()
{
    // LEDs
    LED_DIR |= LED0_PIN | LED1_PIN;
    LED_OUT &= ~(LED0_PIN | LED1_PIN);         // turn off LEDs


    // Configure UART
   // P1SEL0 |= (BIT2 | BIT3);
   // P1SEL1 &= ~(BIT2 | BIT3);                  // USCI_A1 UART operation

    /* For Port4 pin 3 an 4 */
 /*   P4SEL0 |= (BIT3 | BIT4);
    P4SEL1 &= ~(BIT3 | BIT4);                  // USCI_A0 UART operation
    LCDCPCTL0 &= ~(LCDS13|LCDS12);*/

    /* For port 8 pin 2 and 3*/
    P8SEL0 |= (BIT2 | BIT3);
    P8SEL1 &= ~(BIT2 | BIT3);                  // USCI_A3 UART operation

    // Configure PJ.5 PJ.4 for external crystal oscillator
    PJSEL0 |= BIT4 | BIT5;                    // For XT1

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
}

void initClockTo16MHz()
{
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    // Clock System Setup
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_0;                     // Set DCO to 1MHz
    // Set SMCLK = MCLK = DCO, ACLK = LFXTCLK (VLOCLK if unavailable)
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    // Per Device Errata set divider to 4 before changing frequency to
    // prevent out of spec operation from overshoot transient
    CSCTL3 = DIVA__4 | DIVS__4 | DIVM__4;   // Set all corresponding clk sources to divide by 4 for errata
    CSCTL1 = DCOFSEL_4 | DCORSEL;           // Set DCO to 16MHz
    // Delay by ~10us to let DCO settle. 60 cycles = 20 cycles buffer + (10us / (1/4MHz))
    __delay_cycles(60);
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1 for 16MHz operation

    CSCTL4 &= ~LFXTOFF;
    do
    {
    CSCTL5 &= ~LFXTOFFG;                      // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

    CSCTL0_H = 0;                             // Lock CS registerss
}

//******************************************************************************
// Main ************************************************************************
// Enters LPM0 if SMCLK is used and waits for UART interrupts. If ACLK is used *
// then the device will enter LPM3 mode instead. The UART RX interrupt handles *
// the received character and echoes it.                                       *
//******************************************************************************
int cnt = 0;
int rst_state=0;
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog
  rst_state=SFRRPCR;


  initGPIO();
  initClockTo16MHz();
  initUART();

  rst_state=rst_state;
#if UART_MODE == ACLK_9600
    // __bis_SR_register(LPM3_bits + GIE);       // Since ACLK is source, enter LPM3, interrupts enabled
#else
    __bis_SR_register(LPM0_bits + GIE);       // Since SMCLK is source, enter LPM0, interrupts enabled
#endif

    while(1)
    {
       UCA3TXBUF = 0x46;
       // UCA3TXBUF = 0x41;
        // Delay by ~10us to let DCO settle. 60 cycles = 20 cycles buffer + (10us / (1/4MHz))
           //__delay_cycles(60);
        LED_OUT &= ~(LED0_PIN );
           __delay_cycles(500000);

        UCA3TXBUF = 0x47;
       // UCA3TXBUF = 0x42;
        //__delay_cycles(60);
        LED_OUT |= (LED0_PIN );
        __delay_cycles(500000);

    }
  __no_operation();                         // For debugger
}

//******************************************************************************
// UART Interrupt ***********************************************************
//******************************************************************************

#if 0
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA1IFG&UCTXIFG));
      UCA1TXBUF = UCA1RXBUF;
      ++cnt;
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}

#else


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A3_VECTOR))) USCI_A3_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA3IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA3IFG&UCTXIFG));
      UCA3TXBUF = UCA3RXBUF;
      ++cnt;
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}
#endif
