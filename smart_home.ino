#include <Keypad.h> //  biblioteka za matrix 4x4 tipkovnicu
#include <DHT.h>    //  biblioteka za senzor DHT


#define DHTPIN 13       // pin na koji je spojen DHT22
#define DHTTYPE DHT22   // DHT22
DHT dht (DHTPIN, DHTTYPE);  // Inicijalizacija DHT senzora

int chk;
float hum;      //Pohranjuje vrijednost vlage  
float temp;     //Pohranjuje vrijednost temperature

int smokeA0 = A1;
int sensorThres = 250;  // vrijednost dima za koju se pali alarm, prvobitno 400, ali smanjena radi brže detekcije

int sensorPIR = 2;         // PIR senzor

int sensorFlame = 5;          // Senzor plamena

// Alarm
int redLed = 53;
int greenLed = 52;
int buzzer = 12;

const byte ROWS = 4;    // 4 retka
const byte COLS = 4;    // 4 stupca
char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

byte rowPins[ROWS] = {23,25,27,29}; //  spajanje na pinove redaka tipkovnice
byte colPins[COLS] = {31,33,35,37}; //  spajanje na pinove stupaca tipkovnice  

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
    Serial.begin(9600);
    dht.begin();

    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(buzzer, OUTPUT);

    pinMode(smokeA0, INPUT);
    pinMode(sensorPIR, INPUT_PULLUP);       // PIR, zasad tipkalo
    pinMode(sensorFlame, INPUT_PULLUP);    // IR Flame, zasad tipkalo
}

void loop(){

    // Vlaga i temperatura
    hum = dht.readHumidity();
    temp = dht.readTemperature();
    Serial.print("Vlaga: ");
    Serial.print(hum);
    Serial.print(" %    Temperatura: ");
    Serial.print(temp);
    Serial.print(" C   ");

    // Dim
    int analogSmoke = analogRead(smokeA0);
    int digitalSmoke;
    if(analogSmoke > sensorThres)
        digitalSmoke = 1;
    else 
        digitalSmoke = 0;
    Serial.print("Dim: ");
    Serial.print(digitalSmoke);
    Serial.print(" (");
    Serial.print(analogSmoke);
    Serial.print(")");

    // Pokret
    int motion = !digitalRead(sensorPIR);
    Serial.print("  Pokret: ");
    Serial.print(motion);

    // Plamen
    int flame = !digitalRead(sensorFlame);
    Serial.print("  Pozar: ");
    Serial.print(flame);

    //Alarm
    if(digitalSmoke == 1 || flame == 1 || motion == 1){
        digitalWrite(redLed, HIGH);
        digitalWrite(greenLed, LOW);
        tone(buzzer, 4500, 1000);
    }
    else{
        digitalWrite(redLed, LOW);
        digitalWrite(greenLed, HIGH);
        noTone(buzzer);
    }

    char key = keypad.getKey();

    if(key != NO_KEY){
        Serial.println(key);
    }

    
    Serial.println("");
    delay(2000);
}           