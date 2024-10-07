// -*- mode: c++ -*-

// ----------------------------------------------------------
// Mimi Stanimirova Vladeva
// 2024-07-10
// ----------------------------------------------------------
#ifndef EMISORA_H_INCLUIDO
#define EMISORA_H_INCLUIDO

// Buena introducción: https://learn.adafruit.com/introduction-to-bluetooth-low-energy/gap
// https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/

// fuente: https://www.instructables.com/id/Beaconeddystone-and-Adafruit-NRF52-Advertise-Your-/
// https://github.com/nkolban/ESP32_BLE_Arduino/blob/master/src/BLEBeacon.h

// https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/
// https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/bleadvertising

// ----------------------------------------------------------
// ----------------------------------------------------------

#include "ServicioEnEmisora.h" //Incluye el servicio de la emisora BLE

// ----------------------------------------------------------
// ----------------------------------------------------------

class EmisoraBLE {
private:

  const char * nombreEmisora; //nombre de la emisora
  const uint16_t fabricanteID; //id fabricante del disp
  const int8_t txPower;        // Potencia de transmision del anuncio ble

public:

  // .........................................................
  // .........................................................

  // Definiciones de los tipos de callback para manejar conexiones BLE
  using CallbackConexionEstablecida = void ( uint16_t connHandle );
  using CallbackConexionTerminada = void ( uint16_t connHandle, uint8_t reason);

  // .........................................................
  // .........................................................

  // Constructor de la clase EmisoraBLE
  EmisoraBLE( const char * nombreEmisora_, const uint16_t fabricanteID_,
			  const int8_t txPower_ ) 
	:
	nombreEmisora( nombreEmisora_ ) ,
	fabricanteID( fabricanteID_ ) ,
	txPower( txPower_ )
  {
	
  } // ()

  // .........................................................
  // .........................................................
  
  void encenderEmisora() {

	 Bluefruit.begin(); 

	
	 (*this).detenerAnuncio();
  } // ()

  // .........................................................
  // .........................................................

  // Sobrecarga de encenderEmisora() con callbacks para conexión establecida y terminada
  void encenderEmisora( CallbackConexionEstablecida cbce,
						CallbackConexionTerminada cbct ) {

	encenderEmisora();

	instalarCallbackConexionEstablecida( cbce );
	instalarCallbackConexionTerminada( cbct );

  } // ()

  // .........................................................
  // .........................................................
  void detenerAnuncio() {

	if ( (*this).estaAnunciando() ) {
	
	  Bluefruit.Advertising.stop(); 
	}

  }  // ()
  
  // .........................................................

  // .........................................................

  // Método que verifica si la emisora está anunciando
  bool estaAnunciando() {
	return Bluefruit.Advertising.isRunning();
  } // ()

  // .........................................................
  // .........................................................
  
  // Emite un anuncio iBeacon con los datos de UUID, major, minor y rssi
  void emitirAnuncioIBeacon( uint8_t * beaconUUID, int16_t major, int16_t minor, uint8_t rssi ) {

	(*this).detenerAnuncio();// Detiene cualquier anuncio en ejecución
	
  // Crea el beacon con los parámetros UUID, major, minor y RSSI
	
	BLEBeacon elBeacon( beaconUUID, major, minor, rssi );
	elBeacon.setManufacturer( (*this).fabricanteID ); // Establece el ID del fabricante


	Bluefruit.setTxPower( (*this).txPower ); // Establece la potencia de transmisión
	Bluefruit.setName( (*this).nombreEmisora );
	Bluefruit.ScanResponse.addName()         // Añade el nombre en la respuesta de escaneo
 
	// Configura el beacon en la publicidad BLE
	Bluefruit.Advertising.setBeacon( elBeacon );

	// Configuración de la publicidad BLE (tiempo, intervalos)
	Bluefruit.Advertising.restartOnDisconnect(true); // Reinicia la publicidad si se desconecta
	Bluefruit.Advertising.setInterval(100, 100);    //  Intervalo de 0.625ms

	// Inicia la publicidad BLE indefinidamente
	Bluefruit.Advertising.start( 0 ); 
	
  } // ()

  // .........................................................
  // .........................................................
  
  // Emitir anuncio iBeacon personalizado con carga libre (21 bytes de carga)
  void emitirAnuncioIBeaconLibre( const char * carga, const uint8_t tamanyoCarga ) {

	(*this).detenerAnuncio(); 

	Bluefruit.Advertising.clearData();  // Limpia los datos de anuncio
	Bluefruit.ScanResponse.clearData(); // Limpia la respuesta de escaneo

	
	Bluefruit.setName( (*this).nombreEmisora );
	Bluefruit.ScanResponse.addName(); // Añade el nombre en la respuesta de escaneo

	Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);

  // Prepara el array que contendrá los 4 bytes de prefijo y los 21 bytes de la carga
	uint8_t restoPrefijoYCarga[4+21] = {
	  0x4c, 0x00, // companyID 2
	  0x02, // ibeacon type 1byte
	  21, // ibeacon length 1byte (dec=21)  longitud del resto // 0x15 // ibeacon length 1byte (dec=21)  longitud del resto
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-'
	};

	// Copia los datos de la carga en el array
	memcpy( &restoPrefijoYCarga[4], &carga[0], ( tamanyoCarga > 21 ? 21 : tamanyoCarga ) ); 

	// Añade los datos de la carga al anuncio
	Bluefruit.Advertising.addData( BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
								   &restoPrefijoYCarga[0],
								   4+21 );

	// Configuración del anuncio BLE
	Bluefruit.Advertising.restartOnDisconnect(true); // Configuración del anuncio BLE
	Bluefruit.Advertising.setInterval(100, 100);     // Intervalo de 0.625ms

	Bluefruit.Advertising.setFastTimeout( 1 );      // Duración en modo rápido
	Bluefruit.Advertising.start( 0 );               // Duración en modo rápido

	Globales::elPuerto.escribir( "emitiriBeacon libre  Bluefruit.Advertising.start( 0 );  \n");
  } // ()

  // .........................................................
  // .........................................................
  // Añadir un servicio BLE a la emisor
  bool anyadirServicio( ServicioEnEmisora & servicio ) {

	Globales::elPuerto.escribir( " Bluefruit.Advertising.addService( servicio ); \n");
	bool r = Bluefruit.Advertising.addService( servicio );

	if ( ! r ) {
	  Serial.println( " SERVICION NO AÑADIDO \n");
	}

	return r;
  } // ()
  
  // .........................................................
  // .........................................................
  // Añadir servicio con características BLE
  bool anyadirServicioConSusCaracteristicas( ServicioEnEmisora & servicio ) { 
	return (*this).anyadirServicio( servicio ); // Añade el servicio con características
  } // 

  // .........................................................
  // Sobrecarga para añadir múltiples características a un servicio
  template <typename ... T>
  bool anyadirServicioConSusCaracteristicas( ServicioEnEmisora & servicio,
											 ServicioEnEmisora::Caracteristica & caracteristica,
											 T& ... restoCaracteristicas) {

	servicio.anyadirCaracteristica( caracteristica );// Añade una característica al servicio

	return anyadirServicioConSusCaracteristicas( servicio, restoCaracteristicas... );// Añade el resto
	
  } // ()

  // .........................................................

  // Añadir servicio con características y activarlo
  template <typename ... T>
  bool anyadirServicioConSusCaracteristicasYActivar( ServicioEnEmisora & servicio,
													 // ServicioEnEmisora::Caracteristica & caracteristica,
													 T& ... restoCaracteristicas) {

	bool r = anyadirServicioConSusCaracteristicas( servicio, restoCaracteristicas... );

	servicio.activarServicio();

	return r;
	
  } // ()

  // .........................................................
  // .........................................................
  // Instala el callback para manejar conexiones BLE
  void instalarCallbackConexionEstablecida( CallbackConexionEstablecida cb ) {
	Bluefruit.Periph.setConnectCallback( cb );
  } // ()

  // .........................................................
  // .........................................................
  void instalarCallbackConexionTerminada( CallbackConexionTerminada cb ) {
	Bluefruit.Periph.setDisconnectCallback( cb );
  } // ()

  // .........................................................
  // .........................................................
  BLEConnection * getConexion( uint16_t connHandle ) {
	return Bluefruit.Connection( connHandle );
  } // ()

}; // class

#endif

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------

// .........................................................
  //
  // Ejemplo de Beacon (31 bytes)
  //
  // https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/
  //
  // The iBeacon Prefix contains the hex data : 0x0201061AFF004C0215. This breaks down as follows:
  //
  // 0x020106 defines the advertising packet as BLE General Discoverable and BR/EDR high-speed incompatible.
  // Effectively it says this is only broadcasting, not connecting.
  //
  // 0x1AFF says the following data is 26 bytes long and is Manufacturer Specific Data.
  //
  // 0x004C is Apple’s Bluetooth Sig ID and is the part of this spec that makes it Apple-dependent.
  //
  // 0x02 is a secondary ID that denotes a proximity beacon, which is used by all iBeacons.
  //
  // 0x15 defines the remaining length to be 21 bytes (16+2+2+1).
  //
  // Por ejemmplo:
  //
  // 1. prefijo: 9bytes
  //       0x02, 0x01, 0x06,       // advFlags 3bytes
  //       0x1a, 0xff,             // advHeader 2 (0x1a = 26 = 25(lenght de 0x4c a 0xca)+1)   0xFF -> BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA
  //       0x4c, 0x00,             // companyID 2bytes
  //       0x02,                   // ibeacon type 1 byte
  //       0x15,                   // ibeacon length 1 byte (dec=21 lo que va a continuación: desde  la 'f' hasta 0x01)
  // 
  // 2. uuid: 16bytes
  // 'f', 'i', 's', 't', 'r', 'o', 'f', 'i', 's', 't', 'r', 'o', 0xa7, 0x10, 0x96, 0xe0
  // 
  // 2 major: 2bytes
  // 0x04, 0xd2,
  // 
  // minor: 2bytes
  // 0x10, 0xe1,
  // 
  // 0xca, // tx power : 1bytes
  //
  // 0x01, // este es el byte 31 = BLE_GAP_ADV_SET_DATA_SIZE_MAX, parece que sobra
  //
  // .........................................................
  // Para enviar como carga libre los últimos 21 bytes de un iBeacon (lo que normalmente sería uuid-16 major-2 minor-2 txPower-1)
  // .........................................................
  /*
  void emitirAnuncioIBeaconLibre( const char * carga ) {

	const uint8_t tamanyoCarga = strlen( carga );
  */