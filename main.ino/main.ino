int Led_Red = 2;
int Led_Green = 4;
int Led_Blue = 6;
  
void setup ()
{
  // Output pin initialization for the LEDs
  pinMode (Led_Red, OUTPUT); 
  pinMode (Led_Green, OUTPUT);
  pinMode (Led_Blue, OUTPUT); 
}
  
void loop () //main program loop
{
  //analogWrite (Led_Red, 5);   // LED will be switched on
  //analogWrite (Led_Green, 5);  // LED will be switched off
  //analogWrite (Led_Blue, 5);   // LED will be switched off
}
