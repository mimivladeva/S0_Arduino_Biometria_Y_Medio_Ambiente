// -*- mode: c++ -*-

#ifndef LED_H_INCLUIDO
#define LED_H_INCLUIDO

// ----------------------------------------------------------
// Mimi Stanimirova Vladeva
// 2024-07-10
// ----------------------------------------------------------

// ----------------------------------------------------------
// ----------------------------------------------------------
// Función para pausar la ejecución por un tiempo determinado.
// `delay()` es una función de Arduino que detiene el programa durante el número
// de milisegundos especificado por `tiempo`.
void esperar (long tiempo) {
  delay (tiempo);
}

// ----------------------------------------------------------
// ----------------------------------------------------------
class LED {
private:
  int numeroLED;
  bool encendido;// Estado del LED (true si está encendido, false si está apagado).
public:

  // .........................................................
  // .........................................................

  // Constructor que inicializa el LED en el pin especificado.
  // Configura el pin como salida y apaga el LED por defecto.
  LED (int numero)
	: numeroLED (numero), encendido(false)// Inicializa numeroLED con el número dado y encendido en false.
  {
	pinMode(numeroLED, OUTPUT); // Establece el pin como salida.
	apagar ();                  // Apaga el LED inicialmente.
  }

  // .........................................................
  // .........................................................
  void encender () {
	digitalWrite(numeroLED, HIGH); 
	encendido = true;
  }

  // .........................................................
  // .........................................................
  void apagar () {
	  digitalWrite(numeroLED, LOW);
	  encendido = false;
  }

  // .........................................................
  // .........................................................
    // Método para alternar el estado del LED.
  // Si el LED está encendido, lo apaga; si está apagado, lo enciende.
  void alternar () {
	if (encendido) {
	  apagar();
	} else {
	  encender ();
	}
  } // ()

  // .........................................................
  // .........................................................
  
    // Método para hacer que el LED brille durante un tiempo específico.
  // Enciende el LED, espera un periodo de tiempo, y luego lo apaga.
  void brillar (long tiempo) {
	encender ();
	esperar(tiempo); 
	apagar ();
  }
}; // class

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
#endif
