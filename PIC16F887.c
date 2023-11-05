/*
 * File:   PIC16F887.c
 * Author: ragab
 *
 * Created on November 2, 2023, 5:52 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 4000000 // Set the internal oscillator frequency

/***************************esternal interrupt*********************************/

void __interrupt() ISR(void) {

    if (INTF == 1) {
        // YOUR INTERRUPT

        INTF = 0;    // Clear the interrupt flag
    }
}
/************************************************************/
/**********************Gloable variable**********************/
#define NUMBEROFLOOP 6

#define LED(N) (1<<N)
unsigned char ledNumberArray[30]={
    LED(1),LED(2),LED(3),LED(4),LED(5),LED(6),LED(7),       //PORTA
    LED(1),LED(2),LED(3),LED(4),LED(5),LED(6),LED(7),       //PORTB
    LED(0),LED(1),LED(2),LED(3),LED(4),LED(5),LED(6),LED(7),//PORTC
    LED(0),LED(1),LED(2),LED(3),LED(4),LED(5),LED(6),LED(7),//PORTD
};
unsigned short delay_val=1024;
/**************************************************************/


#define NUMBER_OF_LED 29 // the end of the loop leds

/****************************PORT_LIMIT************************/
#define PORTA_START 0 //LED NUMBER 0 
#define PORTA_END   6 //LED NUMBER 6

#define PORTB_START 7 //LED NUMBER 7 
#define PORTB_END   13 //LED NUMBER 13

#define PORTC_START 14 //LED NUMBER 14 
#define PORTC_END   21 //LED NUMBER 21

#define PORTD_START 22 //LED NUMBER 14 
#define PORTD_END   29 //LED NUMBER 29

#define LIGHT 0
#define DARK  1

/*************************************************************/


/**************************MACROS****************************/
#define OUTPUT_BIT(P , B)  TRIS##P &=~(1<<B);
#define INPUT_BIT(P , B)   TRIS##P |=(1<<B);

#define OUTPUT_PORT(P)     TRIS##P =0X00;
#define INPUT_PORT(P)      TRIS##P =0XFF;

#define OUTPUT_PORTS       TRISA =0X00;TRISB=0X00;TRISC=0X00;TRISD=0X00;
#define INPUT_PORTS        TRISA =0XFF;TRISB=0XFF;TRISC=0XFF;TRISD=0XFF;
//FOR ALL PORT SKIP PORT A,B
#define OUTPUT_PORTA       TRISA &=~((1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
#define INPUT_PORTA        TRISA |= ((1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));

#define OUTPUT_PORTB       TRISB &=~((1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
#define INPUT_PORTB        TRISB |= ((1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));

#define SET_BIT(P , B)     PORT##P |=B
#define CLEAR_BIT(P , B)   PORT##P &=~B

#define SET_PORT(P)        PORT##P =0XFF;
#define CLEAR_PORT(P)      PORT##P =0X00;

#define SET_PORTS           PORTA =0XFF;PORTB =0XFF;PORTC =0XFF;PORTD =0XFF;
#define CLEAR_PORTS         PORTA =0X00;PORTB =0X00;PORTC =0X00;PORTD =0X00;

#define SET_PORTA           PORTA |=((1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
#define CLEAR_PORTA         PORTA &=~((1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));

#define SET_PORTB           PORTB |=((1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
#define CLEAR_PORTB         PORTB &=~((1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));

#define TOGALE_BIT(P , B)      PORT##P ^=B
#define TOGALE_PORTA           PORTA ^=((1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
#define TOGALE_PORTB           PORTB ^=((1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
#define TOGALE_PORT(P)         PORT##P ^=0xFF;
/*************************************************************/



/*************************FUNCTION LED***********************************/

void leds_init(){
    ANSEL =0x00;// for make PORTA DIGITAL OUTTPUT
    ANSELH=0X00;// for make PORTB DIGITAL OUTPUT 
    OPTION_REG =0X00;
    SSPEN =0;
    OUTPUT_PORTS;//make all ports output
    OUTPUT_PORTA;
    OUTPUT_PORTB;
    OUTPUT_PORT(C);
    OUTPUT_PORT(D);

    CLEAR_PORTS;//make all ports output off
    CLEAR_PORTA;
    CLEAR_PORTB;
    CLEAR_PORT(C);
    CLEAR_PORT(D);
}

void led_on(unsigned char lNumber){
    if(lNumber >= PORTA_START && lNumber <= PORTA_END ){     //PORTA
        SET_BIT(A , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTB_START && lNumber <= PORTB_END ){//PORTB
        SET_BIT(B , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTC_START && lNumber <= PORTC_END ){//PORTC
        SET_BIT(C , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTD_START && lNumber <= PORTD_END ){//PORTD
        SET_BIT(D , ledNumberArray[lNumber]);
    }
}

void led_off(unsigned char lNumber){
    if(lNumber >= PORTA_START && lNumber <= PORTA_END ){     //PORTA
        CLEAR_BIT(A , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTB_START && lNumber <= PORTB_END ){//PORTB
        CLEAR_BIT(B , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTC_START && lNumber <= PORTC_END ){//PORTC
        CLEAR_BIT(C , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTD_START && lNumber <= PORTD_END ){//PORTD
        CLEAR_BIT(D , ledNumberArray[lNumber]);
    }
}
void led_off_test(unsigned char lNumber){
    if(lNumber >= PORTA_START && lNumber <= PORTA_END ){     //PORTA
        CLEAR_BIT(A , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTB_START && lNumber <= PORTB_END ){//PORTB
        CLEAR_BIT(B , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTC_START && lNumber <= PORTC_END ){//PORTC
        CLEAR_BIT(C , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTD_START && lNumber <= PORTD_END ){//PORTD
        CLEAR_BIT(D , ledNumberArray[lNumber]);
    }
}

void led_on_only(unsigned char lNumber){
    CLEAR_PORTA;
    CLEAR_PORTB;
    CLEAR_PORT(C);
    CLEAR_PORT(D);
    if((lNumber >= PORTA_START) && (lNumber <= PORTA_END) ){     //PORTA
        SET_BIT(A , ledNumberArray[lNumber]);
    }else if((lNumber >= PORTB_START) && (lNumber <= PORTB_END) ){//PORTB
        SET_BIT(B , ledNumberArray[lNumber]);
    }else if((lNumber >= PORTC_START) && (lNumber <= PORTC_END) ){//PORTC
        SET_BIT(C , ledNumberArray[lNumber]);
    }else if((lNumber >= PORTD_START) && (lNumber <= PORTD_END) ){//PORTD
        SET_BIT(D , ledNumberArray[lNumber]);
    }
}

void led_off_only(unsigned char lNumber){
    SET_PORTA;
    SET_PORTB;
    SET_PORT(C);
    SET_PORT(D);
    if(lNumber >= PORTA_START && lNumber <= PORTA_END ){     //PORTA
        CLEAR_BIT(A , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTB_START && lNumber <= PORTB_END ){//PORTB
        CLEAR_BIT(B , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTC_START && lNumber <= PORTC_END ){//PORTC
        CLEAR_BIT(C , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTD_START && lNumber <= PORTD_END ){//PORTD
        CLEAR_BIT(D , ledNumberArray[lNumber]);
    }
}

void leds_off(){
    CLEAR_PORTA;
    CLEAR_PORTB;
    CLEAR_PORT(C);
    CLEAR_PORT(D);
}

void leds_on(){
    SET_PORTA;
    SET_PORTB;
    SET_PORT(C);
    SET_PORT(D);
}

void led_togale(unsigned char lNumber){
    if(lNumber >= PORTA_START && lNumber <= PORTA_END ){     //PORTA
        TOGALE_BIT(A , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTB_START && lNumber <= PORTB_END ){//PORTB
        TOGALE_BIT(B , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTC_START && lNumber <= PORTC_END ){//PORTC
        TOGALE_BIT(C , ledNumberArray[lNumber]);
    }else if(lNumber >= PORTD_START && lNumber <= PORTD_END ){//PORTD
        TOGALE_BIT(D , ledNumberArray[lNumber]);
    }
}

void leds_togale(){
    TOGALE_PORTA;
    TOGALE_PORTB;
    TOGALE_PORT(C);
    TOGALE_PORT(D);
}
/*************************************************************************/


//#define CHECKDEF

/*********************prototype function FOR LED FLASHER **********************/
void my_delay_ms(unsigned short n);
void forwardBackStepByStepCounter();
void forwardBackwordLightOneOnlyAndReveseSwitch();
void forwardBackStepByStep(unsigned char  flage);
void forwardBackwordLightOneOnlyAndRevese(unsigned  char );

void forwardBackwordLightNledOnlyAndRevese(unsigned char flage ,unsigned char divid_number );
void forwardBackStepByStepNled(unsigned char flage ,unsigned char divid_number );
void forwardBackStepByStepCounterNled(unsigned char divid_number);
void forwardBackStepByStepCounterNledReverse(unsigned char divid_number);
void forwardBackStepByStepCounterReverse();
void ledsOnOffNled(unsigned char divid_number);
void ledsOnOff();
/******************************************************************************/

/********************prototype function FOR ADC*********************************/
void initializeADC();
unsigned short readADC();

/*******************************************************************************/



// main function 
int main(void) {
    leds_init();
    initializeADC();
    while(1){
        // forwardBackwordLightOneOnlyAndReveseSwitch();
        /****************************************************/
        ledsOnOff();
        ledsOnOffNled(2);
        ledsOnOffNled(3);
        ledsOnOffNled(5);
        ledsOnOffNled(6);
        ledsOnOffNled(10);
        ledsOnOffNled(15);
        ledsOnOffNled(30);
        /****************************************************/
        forwardBackwordLightOneOnlyAndRevese(DARK);
        forwardBackwordLightOneOnlyAndRevese(LIGHT);
        forwardBackwordLightNledOnlyAndRevese(DARK , 2);
        forwardBackwordLightNledOnlyAndRevese(DARK , 3);
        forwardBackwordLightNledOnlyAndRevese(DARK , 5);
        forwardBackwordLightNledOnlyAndRevese(LIGHT , 2);
        forwardBackwordLightNledOnlyAndRevese(LIGHT , 3);
        forwardBackwordLightNledOnlyAndRevese(LIGHT , 5);
        forwardBackwordLightNledOnlyAndRevese(LIGHT , 6);
        forwardBackwordLightNledOnlyAndRevese(LIGHT , 10);
        forwardBackwordLightNledOnlyAndRevese(LIGHT , 15);
        /****************************************************/
        forwardBackStepByStep(LIGHT);
        forwardBackStepByStep(DARK);
        forwardBackStepByStepNled(LIGHT , 2);
        forwardBackStepByStepNled(LIGHT , 3);
        forwardBackStepByStepNled(LIGHT , 5);
        forwardBackStepByStepNled(LIGHT , 6);
        forwardBackStepByStepNled(LIGHT , 10);
        forwardBackStepByStepNled(LIGHT , 15);
        forwardBackStepByStepNled(DARK , 2);
        forwardBackStepByStepNled(DARK , 3);
        forwardBackStepByStepNled(DARK , 5);
        forwardBackStepByStepNled(DARK , 6);
        forwardBackStepByStepNled(DARK , 10);
        forwardBackStepByStepNled(DARK , 15);
        /****************************************************/
        forwardBackStepByStepCounter();
        forwardBackStepByStepCounterNled(2);
        forwardBackStepByStepCounterNled(3);
        forwardBackStepByStepCounterNled(5);
        forwardBackStepByStepCounterNled(6);
        forwardBackStepByStepCounterNled(10);
        forwardBackStepByStepCounterNled(15);
        forwardBackStepByStepCounterNled(30);
        forwardBackStepByStepCounterReverse();
        forwardBackStepByStepCounterNledReverse(2);
        forwardBackStepByStepCounterNledReverse(3);
        forwardBackStepByStepCounterNledReverse(5);
        forwardBackStepByStepCounterNledReverse(6);
        forwardBackStepByStepCounterNledReverse(10);
        forwardBackStepByStepCounterNledReverse(30);
        /****************************************************/
    }

    return 0;
}





/*******************************ADC******************************/
void initializeADC() {
    // Configure ADCON0 for AN5, internal voltage reference, and left-justified result
    ADCON0bits.ADCS1 = 0;  // Set ADC conversion clock to FOSC/2
    ADCON0bits.ADCS0 = 1;
    ADCON0bits.CHS3 = 0;   // Select channel AN5
    ADCON0bits.CHS2 = 1;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 1;
    ADCON0bits.ADON = 1;   // Turn on the ADC

    // Configure ADCON1 for right-justified result and Vref- as reference
    ADCON1bits.ADFM = 1;   // Right-justified result
    ADCON1bits.VCFG1 = 0;  // VREF- is reference
    ADCON1bits.VCFG0 = 0;  // VDD is reference
    __delay_us(2);
}

unsigned short readADC() {
    // Start the conversion
    ADCON0bits.GO = 1;
    
    // Wait for the conversion to complete
    while (ADCON0bits.GO);
    
    // Read the result from ADRESH and ADRESL
    unsigned short result = (ADRESH << 8) + ADRESL;
    
    return result;
}
/************************************************ ********************/
/***************************DELAY ********************/
void my_delay_ms(unsigned short n) {
    while(n--) {
    __delay_ms(1);
    }
}
/****************************************************/

/*****************************LEDS FUNCTIONS********************************/

void forwardBackwordLightOneOnlyAndRevese(unsigned char flage){
    unsigned char count=NUMBEROFLOOP;
    while(count--){
        //REVERSE COUNT
        for( unsigned char led_number =0 ; led_number <=29;led_number++){
            if(flage ==LIGHT){
                led_on_only(NUMBER_OF_LED -led_number);
            }else{
                led_off_only(NUMBER_OF_LED-led_number);
            }
    #ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }
        
    #endif
            
            delay_val=readADC();
            my_delay_ms(delay_val);

        }
    //     leds_off();
    
        for(unsigned char  led_number =0 ; led_number<=29 ;led_number++){
            if(flage ==LIGHT){
                led_on_only(led_number);
            }else{
                led_off_only(led_number);
            }
    #ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }
    //CLEARPORT;            
    #endif
            delay_val=readADC();
            my_delay_ms(delay_val);
            
        }
        
        
    }
    // CLEARPORT;
    leds_off();
}

void forwardBackStepByStep(unsigned char  flage){
    unsigned char count=NUMBEROFLOOP;
    while(count--){
        for( unsigned char led_number =0 ; led_number <=29;led_number++){
            if(flage == LIGHT){
                led_on_only(NUMBER_OF_LED - led_number);
            }else{
                led_off_only(NUMBER_OF_LED - led_number);
            }
            
    #ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }      
    #endif            
            delay_val=readADC();
            my_delay_ms(delay_val);

        }
        leds_off();


    }
    count=NUMBEROFLOOP;
    while(count--){
        for( unsigned char led_number =0 ; led_number <=29;led_number++){
            if(flage == LIGHT){
                led_on_only( led_number);
            }else{
                led_off_only(led_number);
            }
            
    #ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }      
    #endif
            delay_val=readADC();
            my_delay_ms(delay_val);
            
        }
        leds_off();
        
    }  
    //CLEARPORT;
    leds_off();

}


void forwardBackStepByStepCounter(){
    unsigned char count=NUMBEROFLOOP;
    while(count--){
        for( unsigned char led_number =0 ; led_number <=29;led_number++){
                led_on(NUMBER_OF_LED - led_number);
#ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }        
#endif            
            delay_val=readADC();
            my_delay_ms(delay_val);

        }
        
        for( unsigned char led_number =0 ; led_number <=29;led_number++){
            led_off(NUMBER_OF_LED - led_number);

#ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }    
#endif            
            delay_val=readADC();
            my_delay_ms(delay_val);
            
        }
    }
    // CLEARPORT;
    leds_off();
    
}



void forwardBackStepByStepCounterReverse(){
    unsigned char count=NUMBEROFLOOP;
    while(count--){
        for( unsigned char led_number =0 ; led_number <=29;led_number++){
                led_on(NUMBER_OF_LED - led_number);
#ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }        
#endif            
            delay_val=readADC();
            my_delay_ms(delay_val);

        }
        
        for( unsigned char led_number =0 ; led_number <=29;led_number++){
            led_off(led_number);

#ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }    
#endif            
            delay_val=readADC();
            my_delay_ms(delay_val);
            
        }
    }
    // CLEARPORT;
    leds_off();
    
}


void forwardBackwordLightOneOnlyAndReveseSwitch(){
        unsigned char count=NUMBEROFLOOP;
    while(count--){
        for( unsigned char led_number =0 ; led_number <=29;led_number++){
            led_on_only(NUMBER_OF_LED - led_number);
            delay_val=readADC();
            my_delay_ms(delay_val);
        }
    }
    count=NUMBEROFLOOP;
    while(count--){
        for( unsigned char led_number =0 ; led_number <=29;led_number++){
            led_on_only(led_number);
            delay_val=readADC();
            my_delay_ms(delay_val);
            
        }
    }
    // CLEARPORT;
}


void forwardBackwordLightNledOnlyAndRevese(unsigned char flage ,unsigned char divid_number ){
    unsigned char count=NUMBEROFLOOP;
    unsigned char shift = 30/divid_number;
    while(count--){
        //REVERSE COUNT
        for( unsigned char led_number =0 ; led_number <shift;led_number++){
            if(flage == LIGHT){
                leds_off();//CLEAR ALL LEDS
            }else{
                leds_on();//SET ALL LEDS
            }
            for( unsigned char shift_led_number =0 ; shift_led_number <=NUMBER_OF_LED; shift_led_number+=shift){
                if(flage ==LIGHT){
                    led_on(NUMBER_OF_LED -led_number - shift_led_number);
                }else{
                    led_off(NUMBER_OF_LED-led_number - shift_led_number);
                }
            }

        #ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }
        
        #endif
            delay_val=readADC();
            my_delay_ms(delay_val);
            
        }
        leds_off();
        for( unsigned char led_number =0 ; led_number <shift;led_number++){
            if(flage == LIGHT){
                leds_off();//CLEAR ALL LEDS
            }else{
                leds_on();//SET ALL LEDS
            }
            for( unsigned char shift_led_number =0 ; shift_led_number <=NUMBER_OF_LED; shift_led_number+=shift){
                if(flage ==LIGHT){
                    led_on(led_number + shift_led_number);
                }else{
                    led_off(led_number + shift_led_number);
                }
            }
        #ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }
        
        #endif
            delay_val=readADC();
            my_delay_ms(delay_val);
            
        }
        leds_off();
    }
    // CLEARPORT;
    //leds_off();
}


void forwardBackStepByStepNled(unsigned char flage ,unsigned char divid_number ){
    unsigned char count=NUMBEROFLOOP;
    unsigned char shift = 30/divid_number;
    while(count--){
        //REVERSE COUNT
        for( unsigned char led_number =0 ; led_number <shift;led_number++){
            if(flage == LIGHT){
                leds_off();//CLEAR ALL LEDS
            }else{
                leds_on();//SET ALL LEDS
            }
            for( unsigned char shift_led_number =0 ; shift_led_number <=NUMBER_OF_LED; shift_led_number+=shift){
                if(flage ==LIGHT){
                    led_on(NUMBER_OF_LED -led_number - shift_led_number);
                }else{
                    led_off(NUMBER_OF_LED-led_number - shift_led_number);
                }
            }
        #ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }
        
        #endif
            delay_val=readADC();
            my_delay_ms(delay_val);
            
        }
       // leds_off();
    }
    count=NUMBEROFLOOP;
    leds_off();
    while(count--){
            for( unsigned char led_number =0 ; led_number <shift;led_number++){
            if(flage == LIGHT){
                leds_off();//CLEAR ALL LEDS
            }else{
                leds_on();//SET ALL LEDS
            }
            for( unsigned char shift_led_number =0 ; shift_led_number <=NUMBER_OF_LED; shift_led_number+=shift){
                if(flage ==LIGHT){
                    led_on(led_number + shift_led_number);
                }else{
                    led_off(led_number + shift_led_number);
                }
            }
        #ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }
        
        #endif
            delay_val=readADC();
            my_delay_ms(delay_val);
            
        }
       // leds_off();
    }
    leds_off();

}


void forwardBackStepByStepCounterNled(unsigned char divid_number){
    unsigned char count=NUMBEROFLOOP;
    unsigned char shift = 30/divid_number;
    while(count--){
        for( unsigned char led_number =0 ; led_number <shift;led_number++){
            for( unsigned char shift_led_number =0 ; shift_led_number <=NUMBER_OF_LED; shift_led_number+=shift){
                led_on(NUMBER_OF_LED -led_number - shift_led_number);
            }
#ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }        
#endif            
            delay_val=readADC();
            my_delay_ms(delay_val);

        }
        
        for( unsigned char led_number =0 ; led_number <shift;led_number++){
            for( unsigned char shift_led_number =0 ; shift_led_number <=NUMBER_OF_LED; shift_led_number+=shift){
                led_off(NUMBER_OF_LED -led_number - shift_led_number);
            }

#ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }    
#endif            
            delay_val=readADC();
            my_delay_ms(delay_val);
            
        }
    }
    // CLEARPORT;
    leds_off();
    
}


void forwardBackStepByStepCounterNledReverse(unsigned char divid_number){
    unsigned char count=NUMBEROFLOOP;
    unsigned char shift = 30/divid_number;
    while(count--){
        for( unsigned char led_number =0 ; led_number <shift;led_number++){
            for( unsigned char shift_led_number =0 ; shift_led_number <=NUMBER_OF_LED; shift_led_number+=shift){
                led_on(NUMBER_OF_LED -led_number - shift_led_number);
            }
#ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }        
#endif            
            delay_val=readADC();
            my_delay_ms(delay_val);

        }
        
        for( unsigned char led_number =0 ; led_number <shift;led_number++){
            for( unsigned char shift_led_number =0 ; shift_led_number <=NUMBER_OF_LED; shift_led_number+=shift){
                led_off(led_number + shift_led_number);
            }

#ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }    
#endif            
            delay_val=readADC();
            my_delay_ms(delay_val);
            
        }
    }
    // CLEARPORT;
    leds_off();
    
}

void ledsOnOff(){
    unsigned char count=NUMBEROFLOOP;
    while(count--){
        leds_on();

#ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }        
#endif            
        delay_val=readADC();
        my_delay_ms(delay_val);

        leds_off();

        delay_val=readADC();
        my_delay_ms(delay_val);
    }
    // CLEARPORT;
    leds_off();
    
}

void ledsOnOffNled(unsigned char divid_number){
    unsigned char count=NUMBEROFLOOP;
    unsigned char shift = 30/divid_number;
    leds_off();
    unsigned char led_flage=1 , counter=0;
    for( unsigned char led_number =0 ; led_number <=NUMBEROFLOOP;){
        if(led_flage ==1 && counter < shift){
            led_on(led_number);
            counter++;
            led_number++;
        }else if(led_flage ==0 && counter < shift){
            led_off(led_number);
            counter++;
            led_number++;
        }else{
            counter=0;
            led_flage ^=1;
        }
    }

    while(count--){
        //REVERSE COUNT
        for( unsigned char led_number =0 ; led_number <=NUMBER_OF_LED;led_number++){
            for( unsigned char led_number =0 ; led_number <=NUMBER_OF_LED;led_number++){
            led_togale(led_number);
        }
        #ifdef CHECKDEF
            while(!CHECKGPIO(BUTT)){
                forwardBackwordLightOneOnlyAndReveseSwitch();  
            }
        #endif
        delay_val=readADC();
        my_delay_ms(delay_val);
        }
        
    }
    leds_off();
}