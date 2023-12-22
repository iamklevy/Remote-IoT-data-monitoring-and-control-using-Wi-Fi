#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define LED1 3
#define LED2 8
#define smokePin A0    // Analog pin0
#define tempPin A1     // Analog pin1
#define motionPin 2   // Digital interrupt pin2

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

SoftwareSerial ESP(9, 10);  // RX , TX

uint16_t smokeValue;
uint16_t tempValue;
bool IsPageSent;
String IP;
//int motionValue;

volatile bool readSensors = false;
volatile bool motionDetected = false;

void setup() {
  
  ESP.begin(115200);
  Serial.begin(115200);
  delay(100);
  ConfigModule();
  delay(1000);
  lcd.begin(16 , 2);

  // Set the pinMode for LED1 and LED2
  pinMode(LED1 , OUTPUT);
  pinMode(LED2 , OUTPUT);

  
  cli(); // Disable global interrupts
  // Set up enable external interrupt
  EICRA |= (1 << ISC01); // Trigger INT0 on falling edge
  EIMSK |= (1 << INT0); // Enable INT0

  // Set up ADC for analog pins
  Init_ADC();

  // Set up timer and interrupt
  TCCR1A = 0; // Clear Timer/Counter1 Control Register A
  TCCR1B = 0; // Clear Timer/Counter1 Control Register B
  TCNT1L = 0xED;     //Initial timer value = 65535 - 31250 = 34285 (in decimal)
  TCNT1H = 0x85;     // Decimal 34285 = Hexadecimal 0x85ED
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set timer prescaler to 1024
  TIMSK1 |= (1 << TOIE1); // Enable timer overflow interrupt

  
  sei(); // Enable global interrupts
  
}

void ConfigModule() {
  
  SendCommand("AT+RST");
  SendCommand("AT+CWLAP");
  SendCommand("AT+CWJAP=\"Fifi\",\"o12w11h1a8s2\"");
  SendCommand("AT+CWMODE=1");
  SendCommand("AT+CIPMUX=1");   // Enable Multiple Connections
  SendCommand("AT+CIPSERVER=1,80");
  IP = SendCommand("AT+CIFSR");
  Serial.print(IP);
}

ISR(TIMER1_OVF_vect) {
  
  readSensors = true;
  
  if (readSensors) {
    
    smokeValue = readAnalog(smokePin);
    tempValue = readAnalog(tempPin);
    motionDetected  = false;

    float temperature = (tempValue * (5.0 / 1024.0)) * 100.0;

    // ADMUX &= ~3;    // Clear Mux bits
    // ADMUX &= ~3;    // Clear Mux bits
    
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("S:");
      lcd.print(smokeValue);
      lcd.print("M:");
      lcd.print(motionDetected ? "Y" : "N");
      lcd.print("T:");
      lcd.print(temperature);
      lcd.print(" C");
  
    readSensors = false;
  }
  
  TCNT1L = 0xEE;      // Reintialize timer value for 2 seconds
  TCNT1H = 0x85;
}

ISR(INT0_vect) {
  
  motionDetected = true;
  lcd.clear();
  lcd.print("Motion Detected!");

}

void Init_ADC(){
  
  ADCSRA |= (1<<ADEN);  // Enable the ADC
  ADMUX |= (1<<REFS0);  // Set reference voltage to AVcc (5V)
  ADCSRA |= (7<<ADPS0);  // Set ADC prescaler to 128

}

uint16_t readAnalog(uint8_t pinNum){
  
  ADMUX |= pinNum;    // Select analog pin
  ADCSRA |= (1<<ADSC);    // Start conversion
  while(ADCSRA & (1<<ADSC));    // Wait for conversion to complete
  return ADC;
  
}  


void loop() {

  if (ESP.available() > 0) {
    String InputData = ESP.readString();
    Serial.println(InputData);
    if (InputData.indexOf("GBut1=ON") > 0)
      TurnLedOn(LED1);  
    else if (InputData.indexOf("GBut1=OFF") > 0)
      TurnLedOff(LED1);
    else if (InputData.indexOf("GBut2=ON") > 0)
      TurnLedOn(LED2);  
    else if (InputData.indexOf("GBut2=OFF") > 0)
      TurnLedOff(LED2);
    else if (InputData.indexOf("SEND OK") > 0  && !IsPageSent) {
        String WebPage = "<style>@import url('https://fonts.googleapis.com/css2?family=Cairo');form{text-align:center;}button{background-color:#006E7F;margin:10px;border:none;font-size:1.2rem;border-radius:4px;padding:1px 28px;color:white;font-weight:bold}*{font-family:'Cairo', sans-serif;}</style>";
        char command[30];
        sprintf(command, "AT+CIPSEND=0,%d", WebPage.length());
        SendCommand(command);
        ESP.print(WebPage);
        IsPageSent = true;
        delay(200);
        SendCommand("AT+CIPCLOSE=0");
    }
    else if (!IsPageSent) {
        SendWebPage();
        String WebPage = "<style>@import url('https://fonts.googleapis.com/css2?family=Cairo');form{text-align:center;}button{background-color:#006E7F;margin:10px;border:none;font-size:1.2rem;border-radius:4px;padding:1px 28px;color:white;font-weight:bold}*{font-family:'Cairo', sans-serif;}</style>";
        char command[30];
        sprintf(command, "AT+CIPSEND=0,%d", WebPage.length());
        SendCommand(command);
        ESP.print(WebPage);
        IsPageSent = true;
    }
  }
}


String SendCommand(String command) {
  ESP.print(command);
  ESP.print("\r\n");
  delay(100);
  return ESP.readString();
}

void SendWebPage() {
  String WebPage = "<html><body><h1 style='color:#4CAF50;padding:3px;font-weight:bold;font-size:3rem;text-align:center;'>LEDS Control</h1><form method='get'><button class='GBut'name='GBut1'value='ON'>LED1 ON</button><button class='SBut'name='GBut1'value='OFF'>LED1 OFF</button><button class='GBut'name='GBut2'value='ON'>LED2 ON</button><button class='SBut'name='GBut2'value='OFF'>LED2 OFF</button></form></body>";
   char command[30];
    sprintf(command, "AT+CIPSEND=0,%d", WebPage.length());
    SendCommand(command);
    ESP.print(WebPage);
    delay(500);
}

void TurnLedOn(int led) {
  Serial.print("LED");
  Serial.print(led);
  Serial.println(" is Turned ON");
  digitalWrite(led , HIGH);
}

void TurnLedOff(int led) {
  Serial.print("LED");
  Serial.print(led);
  Serial.println(" is Turned OFF");
  digitalWrite(led , LOW);
}


void AbnormalConditions(){
  if(smokeValue > 220){
    SendCommand("AT+CIPSEND=0,42");
    ESP.print("<script>alert('There is Smoke!!')</script>");
    }
  
}




