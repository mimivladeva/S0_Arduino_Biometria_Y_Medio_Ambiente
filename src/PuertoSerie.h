
// -*- mode: c++ -*-

// ----------------------------------------------------------
// Mimi Stanimirova Vladeva
// 2024-07-10
// ----------------------------------------------------------

#ifndef PUERTO_SERIE_H_INCLUIDO
#define PUERTO_SERIE_H_INCLUIDO

// ----------------------------------------------------------
// ----------------------------------------------------------
class PuertoSerie  {

public:
  // .........................................................
  // .........................................................
  // Constructor de la clase PuertoSerie
  // Inicializa la comunicación serie a una velocidad especificada en baudios.
  PuertoSerie (long baudios) { // Inicia la comunicación serie con la velocidad especificada.
	Serial.begin( baudios );
	
  } // ()

  // .........................................................
  // .........................................................
  
  // Método para esperar a que el puerto serie esté disponible.
  void esperarDisponible() {

	delay(10);   

  } // ()

  // .........................................................
  // .........................................................
    // Método genérico para escribir un mensaje en el puerto serie.
  template<typename T>
  void escribir (T mensaje) {
	Serial.print( mensaje );// Envía el mensaje al puerto serie.
  } // ()
  
}; // class PuertoSerie

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
#endif
