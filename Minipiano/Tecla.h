#ifndef TECLA_H
#define TECLA_H
#include <Arduino.h>

// Estructura para guardar cada nota en memoria cuando grabamos
struct NotaGrabada {
	byte numeroTecla;      // Tecla del 0 al 12
	unsigned int duracion; // Duración en milisegundos
};

class Tecla {
private:
	byte pin;               // Pin del Arduino Mega donde va el pulsador
	unsigned int frecuencia; // Frecuencia en Hz (ej: Do = 262 Hz)
	
public:
	Tecla(byte p, unsigned int frec); // Constructor
	void inicializar();               // Configura el pinMode
	bool estaPresionada();            // Lee si el botón está apretado
	unsigned int obtenerFrecuencia(); // Devuelve la nota para el buzzer
};

#endif
