#include <xc.h>
#pragma config FOSC = INTIO7
#define _XTAL_FREQ 16000000

#define X_1		PORTBbits.RB0
#define X_2		PORTBbits.RB1
#define X_3		PORTBbits.RB2
#define X_4		PORTBbits.RB3
#define Y_1		PORTBbits.RB4
#define Y_2		PORTBbits.RB5
#define Y_3		PORTBbits.RB6

#define Keypad_PORT PORTB
#define Keypad_PORT_Direction TRISB	

#define rs LATC3  
#define en LATC4  
#define lcd LATD  

float atand[] = {0,26.57,45,63.43,75.96,82.87,87.14,88.57,89.43};
float tand[] = {0,0.5,1,2,4,8,20,40,100};
float tani(float d){ //tan inverse approximation
    int j;
    for(j = 0; j < 9;++j){
        if(tand[j]>d) break;
    }
    if(j==9) return 90;
    float ans;
    ans = (d-tand[j-1])/(tand[j]-tand[j-1])*(atand[j]-atand[j-1])+atand[j-1];
    return ans;
}
void pulse(){
    en = 1;
    NOP();
	en = 0;
}
void command(unsigned char cmd ){
	lcd = cmd;  
	rs = 0;  
	pulse();
}
void data(unsigned char dt){
	lcd = dt;  
	rs = 1;  
	pulse();
}
PWM_Duty(unsigned int duty,char c){
      if(duty<=20000)
  {
    duty = (float)duty/51.2;
    if(c == 1){DC1B0 = duty & 1; //Store the 1st bit
    DC1B1 = duty & 2; //Store the 0th bit
    CCPR1L = duty>>2;}// Store the remaining 8 bit
    else{DC2B0 = duty & 1; //Store the 1st bit
    DC2B1 = duty & 2; //Store the 0th bit
    CCPR2L = duty>>2;}// Store the remaining 8 bit
  }
}
char keypad_scanner(void){
    while(1){
	    Y_1 = 1; Y_2 = 0; Y_3 = 0;
            if (X_1 == 1) {while (X_1 == 1);return '1';}
            if (X_2 == 1) {while (X_2 == 1);return '4';}
            if (X_3 == 1) {while (X_3 == 1);return '7';}
            if (X_4 == 1) {while (X_4 == 1);return '.';}
	    Y_1 = 0; Y_2 = 1;   
            if (X_1 == 1) {while (X_1 == 1);return '2';}
            if (X_2 == 1) {while (X_2 == 1);return '5';}
            if (X_3 == 1) {while (X_3 == 1);return '8';}
            if (X_4 == 1) {while (X_4 == 1);return '0';}
	    Y_2 = 0; Y_3 = 1;  
            if (X_1 == 1) {while (X_1 == 1);return '3';}
            if (X_2 == 1) {while (X_2 == 1);return '6';}
            if (X_3 == 1) {while (X_3 == 1);return '9';}
            if (X_4 == 1) {while (X_4 == 1);return 'E';}
    }}
float scanner(char c){
    command(0x01);
    command(0x0c);
      if(c == 2){
        data('y');data('(');data('m');data(')');data(':');}
      else {
        data('x');data('(');data('m');data(')');data(':');}
    command(0x85);
    command(0x0f);
      char ch;
      float x = 0;
      float s  = 1;
      ch = keypad_scanner();
      if(ch=='.'){
          data('-');
          ch = keypad_scanner();
          s = -1;}
      while(ch != 'E'){
      if(ch=='.'){
	 data('.');
	 float i = 1;
	 ch = keypad_scanner();
	 while(ch != 'E' && ch != '.'){
	 data(ch);
	 i /=10;
	 x += (ch-'0')*i;
	 ch = keypad_scanner();
	 }
	 return s*x;
      }
      data(ch);
      x = x*10 + ch - '0';
      ch = keypad_scanner();
      }
      return s*x;
      }
unsigned int dcCalcT(float x,float y){
    float rng;
    unsigned int dc;
    rng = x*x + y*y;
    float rng2 = rng*rng;
    float rng3 = rng*rng2;
    rng = 3.9939*rng3-4.7126*rng2 + 2.5891*rng + 0.0915; // square root approximation
    rng2 = rng*rng;
    rng3 = rng*rng2;
    rng = -43.5426*rng3 + 72.3475*rng2 - 3.9841*rng - 0.2815; // function approximation
    dc = rng*1116.6667;
    return dc;
}
unsigned int dcCalcP(float x,float y){
    float phi;
    unsigned int dc;
    phi = y/x;
    if(phi < 0)phi = -1*tani(-1*phi);
    else phi = tani(phi);
    if(x < 0 && y >= 0){
        phi = phi + 180;}
    else if (x < 0 && y < 0){
        phi = phi - 180;}
    dc = (180+phi)*56.1111;
    return dc;
}
void main(){
    OSCCON &= 0b10000100;
    OSCCON |= 0b00000010;
    ANSELB = 0;
    TRISD = 0;     
    TRISC &= 0b11100111;
    NOP();
    command(0x38);  
    command(0x01);  
    command(0x0c); 
    command(0x06); 	
    Keypad_PORT	    = 0x00;	
    Keypad_PORT_Direction = 0x0f;
    PR2 = 97; //Setting the PR2 formulae using Data sheet
    PR4 = 97;

    CCP1M3 = 1; CCP1M2 = 1;  //make CCP1 PWM
    CCP2M3 = 1; CCP2M2 = 1;  //make CCP2 PWM

    T2CKPS1 = 1; TMR2ON = 1; //Configure the Timer module
    T4CKPS1 = 1; TMR4ON = 1; 

    TRISC2 = 0; // make port pin on C2 (CCP1 output pin) as output
    TRISC1 = 0; // make pin C1 (CCP2 output pin) as output
    float x,y;
  do{
    x = scanner(1);
    y = scanner(2);
    PWM_Duty(dcCalcT(x,y),1);
    PWM_Duty(dcCalcP(x,y),2);
  }while(1);
}