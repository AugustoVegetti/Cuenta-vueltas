// Este programa utiliza un solo motor, sin definición del sentido de giro, que se fija estable en el pin 5.
// Los pulsos del motor salen por el pin 6.
// El otro motor, no contemplado en este programa, tiene salida de pulsos por pin 4 y sentido por pin 3.
#include <LiquidCrystal.h> //Incluyo librería necesaria para la utilización del LCD.
LiquidCrystal lcd(12, 11, 10, 9, 8, 7); //(7, 8, 9, 10, 11, 12); //    ( RS, EN, d4, d5, d6, d7) //Defino los pines de dicho LCD.
unsigned long milli; //Variables utilizadas para retardos no bloqueantes.
unsigned long micro;
boolean giro; // Variable que establece si hay orden de giro.
boolean flag = 0; //Bandera para el antirebote del sensor.
boolean i1 = 0; //Bandera antirrebote de aumentar.
boolean i2 = 0; //Bandera antirrebote de desplazar blinqueo.
int cuenta = 0; //Variable que almacena la cantidad de vueltas en curso.
int retardo = 200;
byte dm = 0; //Vueltas en curso.
byte um = 0;
byte c = 0;
byte d = 0;
byte u = 0;
byte dm1 = 0; //Almacenamiento temporal para definir la variable "parada".
byte um1 = 0;
byte c1 = 0;
byte d1 = 0;
byte u1 = 0;
byte posicion = 5; //Variable utilizada en la configuración de las vueltas a dar.
int pedal; //Variable para la lectura de pulsador "pedal".
int aumentar; //Variable para la lectura del pulsador "incremento".
int desplazar; //Variable para la lectura del pulsador "blink".
int emergencia; //Variable para la lectura del pulsador "stop".
long parada = 0; //Variable que guarda el valor de vueltas a la que debe detenerse el motor.
boolean sensor; //Sensor encargado de medir las vueltas cuando se interrumpe el haz.

void setup() {
  digitalWrite(13, LOW);
  lcd.begin(16, 2); // Fijar el numero de caracteres y de filas del lcd.
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);//Defino como entrada del sensor del cuentavuletas.
  Serial.begin(9600);//Inicializo la comunicación serial.
  pinMode(A2, INPUT);//Definido como entrada del pulsador de emergencia o "stop".
  pinMode(A3, INPUT);//Definido como entrada del pulsador de aumentar o "incremento".
  pinMode(A4, INPUT);//Definido como entrada del pulsador de desplazar o "blink".
  pinMode(A5, INPUT);//Definido como entrada del pulsador de "pedal".
  pinMode(3, OUTPUT);//Salida de dirección del motor 2 (no incluído).
  pinMode(4, OUTPUT);//Salida de pasos del motor 2 (no incluído).
  pinMode(5, OUTPUT);//Salida de dirección del motor 1.
  pinMode(6, OUTPUT);//Salida de pasos del motor 1.
  pinMode(7, OUTPUT);//D7 del LCD.
  pinMode(8, OUTPUT);//D6 del LCD.
  pinMode(9, OUTPUT);//D5 del LCD.
  pinMode(10, OUTPUT);//D4 del LCD.
  pinMode(11, OUTPUT);//Enable del LCD (EN).
  pinMode(12, OUTPUT);//Chip Select del LCD (CS).
  digitalWrite (3, LOW);
  digitalWrite (4, LOW);
  digitalWrite (5, LOW);
  lcd.setCursor(6, 0);  // Seteo del cursor. El numero de la izquierda corresponde a la columna.
  lcd.print(dm);
  lcd.print(um);
  lcd.print(c);
  lcd.print(d);
  lcd.print(u);
  giro = 0;
  flag = 0;
}
void loop() {
  sensor = digitalRead(2);

  if (sensor == 0 and flag == 0) { // Valor va a ser 1 cuando algo interrumpa el haz.
    flag = 1;
  }
  if (sensor == 1 and flag == 1) { // Valor va a ser 0 cuando el haz sea continuo.
    cuenta = cuenta + 1;
    u = u + 1;
    if (u == 10) {
      u = 0;
      d = d + 1;
      if (d == 10) {
        d = 0;
        c = c + 1;
        if (c == 10) {
          c = 0;
          um = um + 1;
          if (um == 10) {
            um = 0;
            dm = dm + 1;
            if (dm == 10) {
              dm = 0;
            }
          }
        }
      }
    }
    flag = 0;
    lcd.setCursor(6, 0);  // Seteo del cursor. El numero de la izquierda corresponde a la columna.
    lcd.print(dm);
    lcd.print(um);
    lcd.print(c);
    lcd.print(d);
    lcd.print(u);
  }
  if (giro == 0) {
    desplazar = analogRead(A4);
    if (desplazar > 900 and i2 == 0) {
      i2 = 1;
    }
    if (desplazar < 100 and i2 == 1) {
      posicion = posicion + 1;
      lcd.setCursor(posicion, 0);
      lcd.blink();
      if (posicion == 11) {
        parada = ((dm1 * 10000) + (um1 * 1000) + (c1 * 100) + (d1 * 10) + u1);
        posicion = 5;
        lcd.setCursor(6, 0);  // Seteo del cursor. El numero de la izquierda corresponde a la columna.
        lcd.print(dm);
        lcd.print(um);
        lcd.print(c);
        lcd.print(d);
        lcd.print(u);
        lcd.noBlink();
      }
      i2 = 0;
    }
    aumentar = analogRead(A3);
    if (aumentar > 900 and i1 == 0) {
      i1 = 1;
    }
    if (aumentar < 100 and i1 == 1) {
      if (posicion == 6) {
        dm1 = dm1 + 1;
        if (dm1 == 10) {
          dm1 = 0;
        }
      }
      if (posicion == 7) {
        um1 = um1 + 1;
        if (um1 == 10) {
          um1 = 0;
        }
      }
      if (posicion == 8) {
        c1 = c1 + 1;
        if (c1 == 10) {
          c1 = 0;
        }
      }
      if (posicion == 9) {
        d1 = d1 + 1;
        if (d1 == 10) {
          d1 = 0;
        }
      }
      if (posicion == 10) {
        u1 = u1 + 1;
        if (u1 == 10) {
          u1 = 0;
        }
      }
      lcd.setCursor(6, 0);       // Seteo del cursor. El numero de la izquierda corresponde a la columna.
      lcd.print(dm1);
      //  lcd.setCursor(7, 0);   // Seteo del cursor. El numero de la izquierda corresponde a la columna.
      lcd.print(um1);
      //  lcd.setCursor(8, 0);   // Seteo del cursor. El numero de la izquierda corresponde a la columna.
      lcd.print(c1);
      //  lcd.setCursor(9, 0);   // Seteo del cursor. El numero de la izquierda corresponde a la columna.
      lcd.print(d1);
      //  lcd.setCursor(10, 0);  // Seteo del cursor. El numero de la izquierda corresponde a la columna.
      lcd.print(u1);
      lcd.setCursor(posicion, 0);
      i1 = 0;
    }
  }
  emergencia = analogRead(A2);
  if (emergencia > 900) {
    giro = 0;
    digitalWrite (6, LOW);
    digitalWrite(13, LOW);
  }
  if (cuenta == parada) {
    giro = 0;                 //Instrucción para parar el motor.
    digitalWrite (6, LOW);
  }
  pedal = analogRead(A5);
  if (pedal > 900 or giro == 1) {
    digitalWrite(6, HIGH);
    micro = micros();
    while (micros() - micro < 10) {
      digitalWrite(6, LOW);
    }
    switch (retardo) {
      case 1: retardo = 2;
        giro = 1;
        digitalWrite(13, HIGH);
        break;
      case 2: retardo = 1;
        break;
      default:;
        retardo = retardo - 3;
        break;
    }
  }
  else {
    retardo = 200;
  }

  milli = millis();
  while (millis() - milli < retardo) {

  }

}
