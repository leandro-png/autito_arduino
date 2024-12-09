#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int botonArriba = 2;
const int botonAbajo = 3;

int jugadorY = 2;
const int maxY = 3;
const int calleInicio = 4;
const int calleFin = 19;
char calle[4][20];

unsigned long ultimaActualizacion = 0;
const unsigned long intervalo = 600;

int puntaje = 0;

byte autoautito[] = {
  B00000,
  B01010,
  B01110,
  B01111,
  B01111,
  B01110,
  B01010,
  B00000
};

void setup() {
  lcd.begin(20, 4);
  lcd.backlight();
  pinMode(botonArriba, INPUT_PULLUP);
  pinMode(botonAbajo, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("Juego: Esquivar");
  lcd.setCursor(0, 1);
  lcd.print("Autos!");
  lcd.setCursor(0, 3);
  lcd.print("Por Leandro");
  delay(2000);
  lcd.clear();

  lcd.createChar(0, autoautito);

  inicializarCalle();
}

void loop() {
  if (digitalRead(botonArriba) == LOW && jugadorY > 0) {
    jugadorY--;
  }
  if (digitalRead(botonAbajo) == LOW && jugadorY < maxY) {
    jugadorY++;
  }

  if (millis() - ultimaActualizacion > intervalo) {
    ultimaActualizacion = millis();
    actualizarCalle();
    if (verificarChoque()) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PERDISTE!");
      lcd.setCursor(0, 1);
      lcd.print("Puntaje: ");
      lcd.print(puntaje);
      delay(2000);
      reiniciarJuego();
    }
    puntaje++;
    dibujarCalle();
  }
}

void inicializarCalle() {
  for (int i = 0; i < 4; i++) {
    for (int j = calleInicio; j <= calleFin; j++) {
      calle[i][j] = ' ';
    }
  }
  calle[jugadorY][calleInicio] = 0;
}

void actualizarCalle() {
  for (int i = 0; i < 4; i++) {
    for (int j = calleInicio; j < calleFin; j++) {
      calle[i][j] = calle[i][j + 1];
    }
  }

  for (int i = 0; i < 4; i++) {
    if (random(0, 10) < 1.3) {
      calle[i][calleFin] = '#';
    } else {
      calle[i][calleFin] = ' ';
    }
  }

  calle[jugadorY][calleInicio] = 0;
}

bool verificarChoque() {
  return calle[jugadorY][calleInicio + 1] == '#';
}

void dibujarCalle() {
  lcd.clear();
  for (int i = 0; i < 4; i++) {
    for (int j = calleInicio; j <= calleFin; j++) {
      lcd.setCursor(j, i);
      if (calle[i][j] == 0) {
        lcd.write(0);
      } else {
        lcd.print(calle[i][j]);
      }
    }
  }
}

void reiniciarJuego() {
  jugadorY = 2;
  puntaje = 0;
  inicializarCalle();
}


