#include "Tecla.h"

// Constructor: guarda el pin y la frecuencia en los atributos de la clase
// Si los nombres son iguales, usamos this-> para diferenciarlos:
Tecla::Tecla(byte pin, unsigned int frecuencia) {
	this->pin = pin;
	this->frecuencia = frecuencia;
}

// Configura el pin usando la resistencia interna
void Tecla::inicializar() {
	pinMode(this->pin, INPUT_PULLUP); // <-- Espacio 1
}

// Devuelve true (verdadero) solo cuando el botón está apretado
bool Tecla::estaPresionada() {
	return (digitalRead(this->pin) == LOW); // <-- Espacio 2
}

unsigned int Tecla::obtenerFrecuencia() {
	return this->frecuencia;
}
