#include "Tecla.h"

#define CANT_TECLAS 13
#define PIN_BUZZER 8
#define MAX_NOTAS 100

// Pines para los LEDs de estado
#define LED_LIBRE 9
#define LED_GRABANDO 10
#define LED_REPRODUCIENDO 11

#define ESTADO_LIBRE 0
#define ESTADO_GRABANDO 1
#define ESTADO_REPRODUCIENDO 2

byte estadoActual = ESTADO_LIBRE;

void tocarYGrabar();

// Arreglo de objetos Tecla: {pin, frecuencia en Hz}
Tecla piano[CANT_TECLAS] = {
  Tecla(22, 262), // 0: Do (C4)
  Tecla(23, 277), // 1: Do# (C#4)
  Tecla(24, 294), // 2: Re (D4)
  Tecla(25, 311), // 3: Re# (D#4)
  Tecla(26, 330), // 4: Mi (E4)
  Tecla(27, 349), // 5: Fa (F4)
  Tecla(28, 370), // 6: Fa# (F#4)
  Tecla(29, 392), // 7: Sol (G4)
  Tecla(30, 415), // 8: Sol# (G#4)
  Tecla(31, 440), // 9: La (A4)
  Tecla(32, 466), // 10: La# (A#4)
  Tecla(33, 494), // 11: Si (B4)
  Tecla(34, 523)  // 12: Do (C5)
};

NotaGrabada melodia[MAX_NOTAS];
int indiceNota = 0;

// Variables para detectar cambios en las teclas y medir el tiempo
int teclaSonando = -1; // -1: ninguna apretada
unsigned long tiempoInicio = 0;

void setup() {
  Serial.begin(9600); 
  pinMode(LED_LIBRE, OUTPUT);
  pinMode(LED_GRABANDO, OUTPUT);
  pinMode(LED_REPRODUCIENDO, OUTPUT);
  for (int i = 0; i < CANT_TECLAS; i++) {
    piano[i].inicializar(); // Inicializa la tecla en la posición i (de 0 a 12)
  }
}

// Lee los pulsadores, controla el buzzer y envía datos en vivo a Processing
void tocarYGrabar() {
  int teclaDetectada = -1;

  for (int i = 0; i < CANT_TECLAS; i++) {
    if (piano[i].estaPresionada()) {
      teclaDetectada = i;
      break;
    }
  }

  // 1. SE PRESIONÓ UNA TECLA NUEVA
  if (teclaDetectada != -1 && teclaDetectada != teclaSonando) {
    tone(PIN_BUZZER, piano[teclaDetectada].obtenerFrecuencia());
    tiempoInicio = millis();
    teclaSonando = teclaDetectada;
    Serial.println(teclaSonando); // Avisa a Processing en vivo qué tecla pintar
  }

  // 2. SE SOLTÓ LA TECLA QUE ESTABA SONANDO
  if (teclaDetectada == -1 && teclaSonando != -1) {
    noTone(PIN_BUZZER);
    unsigned int duracion = millis() - tiempoInicio;
    Serial.println(-1); // Avisa a Processing que apague la tecla en pantalla

    // Si estamos en modo GRABANDO, guardamos en el arreglo en memoria
    if (estadoActual == ESTADO_GRABANDO && indiceNota < MAX_NOTAS) {
      melodia[indiceNota].numeroTecla = teclaSonando;
      melodia[indiceNota].duracion = duracion;
      indiceNota++; // Avanzamos al siguiente casillero para la próxima nota
    }
    teclaSonando = -1;
  }
}

// Bucle principal con la máquina de estados
void loop() {
  switch (estadoActual) {
    case ESTADO_LIBRE:
      digitalWrite(LED_LIBRE, HIGH);
      digitalWrite(LED_GRABANDO, LOW);
      digitalWrite(LED_REPRODUCIENDO, LOW);
      tocarYGrabar();
      break;

    case ESTADO_GRABANDO:
      digitalWrite(LED_LIBRE, LOW);
      digitalWrite(LED_GRABANDO, HIGH);
      digitalWrite(LED_REPRODUCIENDO, LOW);
      tocarYGrabar();
      break;

    case ESTADO_REPRODUCIENDO:
      digitalWrite(LED_LIBRE, LOW);
      digitalWrite(LED_GRABANDO, LOW);
      digitalWrite(LED_REPRODUCIENDO, HIGH);
      // Aquí irá la lógica para reproducir la melodía
      break;
  }
}


