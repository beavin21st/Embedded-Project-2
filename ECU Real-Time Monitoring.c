#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF

#include <xc.h>
#define _XTAL_FREQ 4000000

volatile int count=0;
volatile int sec=0,min=0,hour=0;
int fan_state=0;
char hour_d1,hour_d2=0;
char min_d1,min_d2=0;
char sec_d1,sec_d2=0;
int count1=0,button=0;

void enable()
{
    RE1=1;
    __delay_ms(10);    
    RE1=0;
    __delay_ms(10);
}
void lcd(char c,char data)
{
    RE0=c;
    PORTD=data&0xf0;
    enable();
    PORTD=(data&0x0f)<<4;
    enable();
}
void string(char *str)
{
    while(*str)
    {
        lcd(1,*str++);
    }       
}

void __interrupt() timer()
{
    if(TMR1IF==1)
	{
		count++;
	}
	TMR1IF=0;
}

void EEPROM_write(char ads,char data)
{
        
        EEADR=ads;
        EEDAT=data;
        EEPGD=0;
        WREN=1;
        EECON2=0X55;
        EECON2=0XAA;
        WR=1;
        while(EEIF==0);
        EEIF=0;
        
}
char EEPROM_read(char ads)
{
    EEADR=ads;
    EEPGD=0;
    RD=1;
    return EEDAT;
  
}

void wait()
{
    while(SSPIF==0);
    SSPIF=0;
}
void start()
{
    SEN=1;
    wait();
}
void data(char data)
{
    SSPBUF=data;
    wait();
    while(ACKSTAT==1);
    
}
void stop()
{
    PEN=1;
    wait();
}
void repeat_start()
{
    RSEN=1;
    wait();
}
void lcd_split(char data)
{
    lcd(1,((data & 0xf0)>>4)+48);  //sec
    lcd(1,(data & 0x0f)+48);
}
int read(int a)
{
    RCEN=1;
    ACKDT=a;
    wait();
    
    char data=SSPBUF;
    
    ACKEN=1;
    wait();
    return data;
}

char keypad()
{
    while(1)
    {
        RB5=0;RB6=0;RB4=1;
        if(RB0==1)
        {
            while(RB0==1);
            return '1';
            __delay_ms(1000);
            
        }
        if(RB1==1)
        {
            while(RB1==1);
            return '4';
            __delay_ms(1000);
            
        }
        if(RB2==1)
        {
            while(RB2==1);
            return '7';
            __delay_ms(1000);
            
        }
        if(RB3==1)
        {
            while(RB3==1);
            return '*';
            __delay_ms(1000);
           
        }
        
        RB4=0;RB6=0;RB5=1;
        if(RB0==1)
        {
            while(RB0==1);
            return '2';
            __delay_ms(1000);
            
        }
        if(RB1==1)
        {
            while(RB1==1);
            return '5';
            __delay_ms(1000);
            
        }
        if(RB2==1)
        {
            while(RB2==1);
            return '8';
            __delay_ms(1000);
           	
        }
        if(RB3==1)
        {
            while(RB3==1);
            return '0';
            __delay_ms(1000);
            
        }
        RB4=0;RB5=0;RB6=1;
        if(RB0==1)
        {
            while(RB0==1);
            return '3';
            __delay_ms(1000);
            
        }
        if(RB1==1)
        {
            while(RB1==1);
            return '6';
            __delay_ms(1000);
            
        }
        if(RB2==1)
        {
            while(RB2==1);
            return '9';
            __delay_ms(1000);
            
        }
        if(RB3==1)
        {
            while(RB3==1);
            return '#';
            __delay_ms(1000);
            
        }
    }
}

char keypad_digit()
{
    char d1, d2;
    d1 = keypad();
    lcd(1, d1);
    d2 = keypad();
    lcd(1, d2);
    return ((d1-48)*10)+(d2-48);
}
char bcd(char val)
{
    return ((val/10)<<4) | (val%10);
}
char merge_digits(char d1,char d2)
{
    return (d1 * 10) + d2;
}
void RTC_write(char h,char m,char s)
{
    start();
    data(0xD0);  
    data(0x00);  

    data(bcd(s));
    data(bcd(m));
    data(bcd(h));

    stop();
}
void set_rtc()
{
    char hh,mm,ss;
    char key;

    lcd(0,0x01);
    lcd(0,0x80);
    string("SETTIME HH*MM*SS");
    __delay_ms(2000);
    lcd(0,0x01);
    lcd(0,0x80);
    
    hh=keypad_digit();
    if(hh > 23) 
    {
        hh = 23;
    }
    key=keypad();
    while(key != '*')
    {
        key=keypad();       // Hours
    }

    lcd(1,':');

    mm=keypad_digit();
    if(mm > 59) 
    {
        mm = 59;
    }
    key=keypad();
    while(key != '*') 
    {
        key=keypad();    // Minutes
    }

    lcd(1,':');

    ss=keypad_digit();
    if(ss > 59) 
    {
        ss = 59;
    }
    key=keypad();
    while(key != '#') 
    {
        key=keypad();      // Seconds
    }

    RTC_write(hh,mm,ss); 
}



void main(void) {

    PORTD=PORTE=TRISD=TRISE=0X00; //LCD

    PORTB=0x00;   //keypad
    TRISB=0x0f;  
    
    PORTA=0X00;
    TRISA=0X07;
    
    PORTC=0X00;  //RTC
    TRISC=0X18;
    
    ANSEL=0X00;
	ANSELH=0X00;
    
    SSPCON=0X28;
    SSPSTAT=0X80;
 
    
    TRISE2 = 0;   
    RE2 = 0; 

    lcd(0,0x02);
    lcd(0,0x28);
    lcd(0,0x0c);
    
    TMR1=0;
	T1CON=0X31;
	PIE1=0X01;
    GIE=1;
	INTCON=0XC0;
    
    char RTC_sec,RTC_min,RTC_hour;
    char prev_fan_state = 0;

    while(1)
    {
        
        if(RA0 == 1)
        {
            while(RA0==1);
            button=!button;
            
        }
        if(button==1)
        {
            RC0 = 1;
            fan_state = 1;

            if(prev_fan_state == 0)
            {
                hour = EEPROM_read(0);
                min  = EEPROM_read(1);
                sec  = EEPROM_read(2);
                count1=0;
            }
            prev_fan_state = 1;

            if(count >= 2)   
            {
                
                count=0;                
                sec++;
                count1++;
                if(count1<4)
                {
                    RA4=1;RA5=1;
                }
                if(count1 >= 4)
                    {
                        if(count1%2==0)
                        {
                            RA4=1;RA5=0;
                        }
                        else
                        {
                            RA4=0;RA5=1;
                        }
                    }
                
                if(sec == 60)
                {
                    sec=0;
                    min++;
                }
                if(min == 60)
                {
                    min=0;
                    hour++;
                }
            }
            
            lcd(0,0xC0);
            string("FAN - ");
            lcd(1,(hour/10)+48);
            lcd(1,(hour%10)+48);
            lcd(1,':');
            lcd(1,(min/10)+48);
            lcd(1,(min%10)+48);
            lcd(1,':');
            lcd(1,(sec/10)+48);
            lcd(1,(sec%10)+48);
        }
        else
        {
            RC0 = 0;
            RA4=0;RA5=0;
            if(prev_fan_state == 1)
            {
                EEPROM_write(0, hour);
                EEPROM_write(1, min);   //EEPROM write
                EEPROM_write(2, sec);
            }
            prev_fan_state = 0;
        }
        
        if(RA1 == 1)  
        {
            while(RA1==1);
            set_rtc();  //time set
        }
        
        if(RA2 == 1)
        {
            while(RA2==1);
            EEPROM_write(0,0);  // hour
            EEPROM_write(1,0);  // min
            EEPROM_write(2,0);  // sec
        }
        
        

        start();
        data(0xD0);
        data(0x00);
        repeat_start();
        data(0xD1);

        lcd(0,0x80);
        string("RTC - ");

        RTC_sec = read(0);
        RTC_min = read(0);      
        RTC_hour = read(1);
        
        lcd_split(RTC_hour);
        lcd(1,':');
        lcd_split(RTC_min);
        lcd(1,':');
        lcd_split(RTC_sec);

        stop();
    }

    return; 
}
