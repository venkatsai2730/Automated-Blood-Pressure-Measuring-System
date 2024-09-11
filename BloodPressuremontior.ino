int relayPin = 8;//Assign Pin 8 to the relay control/signal pin
float pmin=-15;
float pmax=15;
float vsupply=5;
float anvolt=0;
int i;
float maxvolt=0;
float volt=0;
float pressure=0;
float MAP=0;
float maxv=0;
float b;
float sys;
float dia;
float a;
int enA = 9;
int in1 = 13;
int in2 = 12;
/*Variable Declaration End*/
/*Setup Start*/
void setup(){
 Serial.begin(9600);
 pinMode(relayPin, OUTPUT);//Setting the Relay pin as an Output Pin
 if(digitalRead(3)==LOW){
 inflate();
 }
}
/*Setup End*/
/*Loop Start*/
void loop(){
 
}
/*Loop End*/
/*Motor Working Starts*/
void inflate(){ 
 //motor control
 pinMode(enA, OUTPUT);
 pinMode(in1, OUTPUT);
 pinMode(in2, OUTPUT);
 digitalWrite(in1, LOW);
 digitalWrite(in2, LOW);
 // Turn on motors
 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 delay(60000);
 digitalWrite(in1, LOW);
 digitalWrite(in2, LOW);
 //presssure calculate
 a=calculate();
 Serial.print(a);
 if(a>=160){
 solenoid_relay_1();
 bp();
 solenoid_relay_2();
 }
 else{
 inflate();
 }
 if(digitalRead(4)==LOW){
 solenoid_relay_2();
 }
 solenoid_relay_2();
}
/*Motor Working Ends*/
/*Pressure Calculation During Cuff Inflation & Deflation*/
float calculate(){
 //ADC Conversion 
 anvolt=analogRead(A0);
 volt=(anvolt*vsupply*1000)/(pow(2,10)-1);
 maxv=max(abs(volt-2.5),maxvolt);
 maxvolt=abs(maxv-2.5);
 //Pressure Calculation During Inflation
 pressure=((maxvolt)-(0.1*vsupply))/((0.8*vsupply)/(pmax-pmin))+pmin; //raw data into psi
 MAP=-1*(14.7-pressure*-1)*51.7-3.16/maxvolt; //psi into mmHg 
 return MAP;
}
/*Pressure Calculation During Cuff Inflation & Deflation*/
/*BP Measurement*/
void bp(){
 for(i=0;i<50;i++){
 anvolt=analogRead(A0);
 volt=(anvolt*vsupply)/(pow(2,10)-1);
 maxv=max(abs(volt-2.5),maxvolt);
 maxvolt=abs(maxv-2.5);
 delay(250);
 }
 //raw data into psi
 pressure=((maxvolt)-.1*vsupply)/((.8*vsupply)/(pmax-pmin))+pmin;
 //psi into mmHg
 MAP=-1*(14.7-pressure*-1)*51.7-3.16/maxvolt; 
 sys=MAP*1.1;
 dia=MAP*0.8;
 Serial.print(sys);
 Serial.print("/");
 Serial.print(dia);
}
/*BP Measurement*/
/*Valve Working Starts*/
void solenoid_relay_1(){
 //Turn the relay ON for 1 second
 digitalWrite(relayPin, HIGH);
 delay(1000);
 //Turn the relay OFF
 digitalWrite(relayPin,LOW);
 //Pressure Calculation
 b=calculate();
 //
 if((b<150)){
 bp(); 
 }
 else if(b>135){
 solenoid_relay_1();
 }
 else if(b<130){
 inflate();
 }
}
/*Valve Working Ends*/
/*Valve is opened*/
void solenoid_relay_2(){
 digitalWrite(relayPin, HIGH);
}
/*Valve is opened*/
/*Exit Program*/
void reset(){
 exit(0);
}