// -*-c++-*-

// --------------------------------------------------------------
//
// Mimi Stanimirova Vladeva 
// 2023-07-10
//
// --------------------------------------------------------------

// https://learn.sparkfun.com/tutorials/nrf52840-development-with-arduino-and-circuitpython

// https://stackoverflow.com/questions/29246805/can-an-ibeacon-have-a-data-payload

// --------------------------------------------------------------
// --------------------------------------------------------------
#include <bluefruit.h>
// Desdefinimos las macros min y max para evitar conflictos con la biblioteca estándar
#undef min 
#undef max 
// Definimos el pin que se usará para LED
#define PIN_O3 5

#include "LED.h"// Biblioteca para controlar el LED
#include "PuertoSerie.h"// Biblioteca para la comunicación serie

// --------------------------------------------------------------

// Biblioteca para la comunicación serie
namespace Globales {
  // Instanciamos un objeto LED en el pin 7
  LED elLED ( /* NUMERO DEL PIN LED = */ 7 );
  // Configuramos la comunicación serie a 115200 baudios
  PuertoSerie elPuerto ( /* velocidad = */ 115200 ); // 115200 o 9600 o ...
};

// --------------------------------------------------------------
// --------------------------------------------------------------
// Incluimos las bibliotecas personalizadas
#include "EmisoraBLE.h"
#include "Publicador.h"
#include "Medidor_fijo.h"


// --------------------------------------------------------------
// --------------------------------------------------------------
namespace Globales {
  Publicador elPublicador;// Instancia del publicador
  Medidor elMedidor;// Instancia del medidor 
}; // namespace

// --------------------------------------------------------------
// --------------------------------------------------------------
void inicializarPlaquita () {
  
} // ()

// --------------------------------------------------------------
// setup()
// --------------------------------------------------------------
void setup() {

  Globales::elPuerto.esperarDisponible(); // Espera a que el puerto serie esté disponible
  
  // Inicializa la placa
  inicializarPlaquita();

  // Suspend Loop() to save power
  // suspendLoop();

  Globales::elPublicador.encenderEmisora();  // Enciende la emisora BLE

  // Globales::elPublicador.laEmisora.pruebaEmision();
  
  esperar( 1000 );

 // Mensaje que indica que se ha finalizado la configuración
  Globales::elPuerto.escribir( "---- setup(): fin ---- \n " );


  //-----------------------------------------------------------------------
  //--------- Probar funcionamiento mandar beacon valores major minor -----

  // Muestra el UUID en el puerto serie
    Globales::elPuerto.escribir("\n");

    // Si tienes un valor mayor inicial, puedes usarlo aquí
    uint16_t major = 0; // O algún valor por defecto

    // Inicializa el valor de Major (puede ser un valor por defecto)
    Globales::elPuerto.escribir("Major: ");
    Globales::elPuerto.escribir(major);
    Globales::elPuerto.escribir("\n");

    // Muestra el valor Minor en el puerto serie (por defecto 0)
    Globales::elPuerto.escribir("Minor: ");
    Globales::elPuerto.escribir(0); // Coloca un valor por defecto o inicial aquí
    Globales::elPuerto.escribir("\n");



  

} // setup ()

// --------------------------------------------------------------
// --------------------------------------------------------------

// Función que controla el parpadeo de un LED
inline void lucecitas() {
  using namespace Globales;

// El LED se enciende y apaga con un retardo entre cada acció
  elLED.brillar( 100 ); // 100 encendido
  esperar ( 400 ); //  100 apagado
  elLED.brillar( 100 ); // 100 encendido
  esperar ( 400 ); //  100 apagado
  Globales::elLED.brillar( 100 ); // 100 encendido
  esperar ( 400 ); //  100 apagado
  Globales::elLED.brillar( 1000 ); // 1000 encendido
  esperar ( 1000 ); //  100 apagado

  // Inicia el medidor
  Globales::elMedidor.iniciarMedidor();
} // ()

// --------------------------------------------------------------
// loop ()
// --------------------------------------------------------------
namespace Loop {
  uint8_t cont = 0;// Contador para el loop
};

// ..............................................................
// ..............................................................
// Función principal que se ejecuta en un ciclo continuo
void loop () {

  using namespace Loop;
  using namespace Globales;

  cont++;// Incrementa el contador

  // Mensaje que indica el inicio de la iteración del loop
  elPuerto.escribir( "\n---- loop(): empieza " );
  elPuerto.escribir( cont );
  elPuerto.escribir( "\n" );

// Llama a la función que controla el parpadeo del LED
  lucecitas();

  // 
  // mido y publico
  // Mide el nivel de CO2
  
  float valorCO2 = elMedidor.medirCO2();
  // Publica el valor de CO2
  elPublicador.publicarCO2( valorCO2, cont, 1000 // intervalo de emisión
							);


//-----------------------------------------------------------------------------
// Código para mostrar el UUID, Major y Minor
    uint16_t major = (Publicador::MedicionesID::CO2 << 8) + cont; // Obtén el Major
    
    // Calcula el valor de Major y lo muestra
    Globales::elPuerto.escribir("\nMajor: ");
    Globales::elPuerto.escribir(major);
    Globales::elPuerto.escribir("\nMinor: ");
    Globales::elPuerto.escribir(valorCO2); // Usando el valor de CO2 como Minor
    Globales::elPuerto.escribir("\n");

  
  
  // 
  // prueba para emitir un iBeacon y poner
  // en la carga (21 bytes = uuid 16 major 2 minor 2 txPower 1 )
  // lo que queramos (sin seguir dicho formato)
  // 
  // Al terminar la prueba hay que hacer Publicador::laEmisora privado
  // 
  /*char datos[21] = {
	'H', 'o', 'l', 'a',
	'H', 'o', 'l', 'a',
	'H', 'o', 'l', 'a',
	'H', 'o', 'l', 'a',
	'H', 'o', 'l', 'a',
	'H'
  };*/

  char datos[21] = { 
    'H', 'o', 'y', 
    'N', 'o', 
    'H', 'a', 'c', 'e', 
    'U', 'n', 
    'D', 'i', 'a', 
    'S', 'o', 'l', 'e', 'a', 'd', 'o'};     //Mensaje que llega al movil --------- CAMBIAR

  // elPublicador.laEmisora.emitirAnuncioIBeaconLibre ( &datos[0], 21 );
  elPublicador.laEmisora.emitirAnuncioIBeaconLibre ( &datos[0], 21 );




 // Muestra el valor de CO2 en el puerto serie
  Serial.print("Gas: ");
  Serial.println(valorCO2);


  
 
  // Añade un retardo para evitar saturar el puerto serie
  delay(1000);
  
//------------------------------------------------------------------------


  esperar( 2000 );
  
  // Detiene el anuncio
  elPublicador.laEmisora.detenerAnuncio();

  // 
  // 
   // Mensaje que indica el final de la iteración del loop
  elPuerto.escribir( "---- loop(): acaba **** " );
  elPuerto.escribir( cont );
  elPuerto.escribir( "\n" );
  
  
} // loop ()
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
