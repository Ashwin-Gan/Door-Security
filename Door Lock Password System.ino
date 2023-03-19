#include<Keypad.h>
#include<Servo.h>

#define IR_1 10
#define IR_2 11
#define BUZZ A2

Servo Gate1;
Servo Gate2;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char RIG_PIN[10] = "1234";
char ENT_PIN[10], NEW_PIN[10];
int i = 0;
char key = 0;
char keys[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//Create an object of keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
  Serial.begin(9600);
  Gate1.attach(12);
  Gate2.attach(13);
  pinMode(IR_1,INPUT);
  pinMode(IR_2,INPUT);
  pinMode(BUZZ,OUTPUT);
  Serial.println("To change password, press #"); 
  Serial.print("Enter Password : ");
  Gate1.write(0);
  Gate2.write(180);
}

void Open_Gate()
{
  int pos1,pos2;
  delay(2000);  
  bool Value_1 = digitalRead(IR_1);
  bool Value_2 = digitalRead(IR_2);
  if((Value_1 == HIGH && Value_2 == LOW)||(Value_1 == LOW && Value_2 == HIGH))
  {
    Serial.println("Half Gate Open");    
    for(pos1=0,pos2=180;pos1<=75,pos2>=105;pos1++,pos2--)
    {
      Gate1.write(pos1);
      Gate2.write(pos2);
      delay(35);
    }
    delay(4000);
  
    for(pos1=75,pos2=105;pos1>=0,pos2<=180;pos1--,pos2++)
    {
      Gate1.write(pos1);
      Gate2.write(pos2);
      delay(35);
    }
  }      
  else if(Value_1 == LOW && Value_2 == LOW)
  {
    Serial.println("Full Gate Open");
    for(pos1=0,pos2=180;pos1<=135,pos2>=45;pos1++,pos2--)
    {
      Gate1.write(pos1);
      Gate2.write(pos2);
      delay(35);
    }
    delay(6000);
  
    for(pos1=135,pos2=45;pos1>=0,pos2<=180;pos1--,pos2++)
    {
      Gate1.write(pos1);
      Gate2.write(pos2);
      delay(35);
    }
  }
  else
  {
    Serial.println("Entrant Can't be Identified.");
    Serial.println("Please try again");
  }  
}

void Beep()
{
  tone(BUZZ,1000);
  delay(500);
  noTone(BUZZ);
}

void Correct_Beep()
{
  delay(1000);
  tone(BUZZ,700);
  delay(200);
  noTone(BUZZ);
  delay(200);  
  tone(BUZZ,700);
  delay(200);
  noTone(BUZZ);
  tone(BUZZ,700);
  delay(200);
  noTone(BUZZ);
}

void Wrong_Beep()
{
  delay(1000);
  tone(BUZZ,1200);
  delay(600);
  noTone(BUZZ);
  delay(600);  
  tone(BUZZ,1200);
  delay(600);
  noTone(BUZZ);
  delay(600);
  tone(BUZZ,1200);
  delay(600);
  noTone(BUZZ);
}

void Change_Pass()
{
  int j=0;
  Serial.print("\nEnter your current Password : ");
  while(j<4)
  {
    char key = keypad.getKey();
    if(key)
    {
      Serial.print("*");
      ENT_PIN[j++] = key;
      Beep();
    }
    key=0;    
  }
  Serial.println("");
  delay(500);
  if(strncmp(RIG_PIN,ENT_PIN, 4) == 0)
  {
    j=0;
    Serial.print("Enter New Password:");
    while(j<4)
    {
      char key= keypad.getKey();
      if(key)
      {
        Serial.print("*");
        NEW_PIN[j++]=key;
        Beep();
      }
    }
    strncpy(RIG_PIN,NEW_PIN,4);
    Serial.print("\nPassword Successfully changed!");
    delay(1000);
    Correct_Beep();
    delay(1000);
  }
  else  
  {
    Serial.println("Wrong Password");
    Serial.println("Better Luck Again");
    delay(1000);
    Wrong_Beep();
    delay(1000);
  }
  key = 0;
  Serial.print("\nEnter Password : ");
}

void loop()
{
  char key = keypad.getKey();// Read the key
  if(key == '#')
  {
    Serial.print(key);
    Beep();
    Change_Pass();    
  }
  if(key)
  {
    Serial.print("*");
    ENT_PIN[i++] = key;
    Beep();
  }
  if(i==4)
  {
    if(strncmp(ENT_PIN,RIG_PIN,4)==0)
   {
      Serial.println("");
      Serial.println("Access Granted");
      Correct_Beep();
      Open_Gate();
      i=0;
      Serial.print("Enter Password : ");
   }
   else
   {
      Serial.println("");
      Serial.println("Access Denied");
      Wrong_Beep();
      i=0;
      Serial.print("Enter Password : ");
   }
  }
}