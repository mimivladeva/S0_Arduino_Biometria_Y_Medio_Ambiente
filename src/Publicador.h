// -*- mode: c++ -*-

// ----------------------------------------------------------
// Mimi Stanimirova Vladeva
// 2024-07-10
// ----------------------------------------------------------

#ifndef PUBLICADOR_H_INCLUIDO
#define PUBLICADOR_H_INCLUIDO

// --------------------------------------------------------------
// --------------------------------------------------------------
class Publicador {

  // ............................................................
  // ............................................................
private:

  // UUID del Beacon, un identificador único que representa al dispositivo beacon.
  // Se debe cambiar según el requerimiento específico del proyecto.
uint8_t beaconUUID[16] = { //UUID del Beacon // --------------------------------------------- CAMBIAR
  'E', 'S', 'T', 'O', '-', 'E', 'S', '-', 
  'U', 'N', '-', 'T', 'E', 'X', 'T', 'O'
  };
  

  // ............................................................
  // ............................................................
public:
  EmisoraBLE laEmisora {
	"Manolito", //  nombre emisora   //Nombre que aparece como dispositivo en nRFConnect ----------- CAMBIAR
	  0x004c, // fabricanteID (Apple)
	  4 //  Potencia de transmisión (txPower) del beaco
	  };
  
  const int RSSI = -53;// Valor de la señal recibida (RSSI) para el beacon

  // ............................................................
  // ............................................................
public:

  // ............................................................
  // ............................................................
  enum MedicionesID  {  // --------------------------------------------- CAMBIAR
	CO2 = 11,// Identificador para mediciones de CO2
	};

  // ............................................................
  // ............................................................
  Publicador( ) {
	// ATENCION: no hacerlo aquí. (*this).laEmisora.encenderEmisora();
	// Pondremos un método para llamarlo desde el setup() más tarde
  } // ()

  // ............................................................
  // ............................................................
    // Método para encender el beacon (emisora)
  void encenderEmisora() {
	(*this).laEmisora.encenderEmisora(); // Llama al método de la emisora para encenderla.
  } // ()

  // ............................................................
  // ............................................................
   // Método para publicar el valor de CO2 mediante un anuncio iBeacon.
  void publicarCO2( int16_t valorCO2, uint8_t contador, long tiempoEspera ) {
	
	// 1. empezamos anuncio
	// Genera el valor 'major' combinando el ID de medición y el contador.
	uint16_t major = (MedicionesID::CO2 << 8) + contador;
	// Envía el anuncio iBeacon con el UUID, el valor major, el valor CO2 (minor) y el RSSI.
	(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, 
											major,
											valorCO2, // minor
											(*this).RSSI // rssi
									);

	// 2. esperamos el tiempo que nos digan
	esperar( tiempoEspera );
	// 3. paramos anuncio
	(*this).laEmisora.detenerAnuncio();
  } // ()

 
}; // class

// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
#endif
