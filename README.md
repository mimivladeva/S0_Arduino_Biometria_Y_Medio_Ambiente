# S0_Arduino_Biometria_Y_Medio_Ambiente

# ----------------------------------------------------------------------------------------------------------------------------------------

###  Descripción del Problema que el proyecto resuelve
Este proyecto utiliza una placa de desarrollo basada en el microcontrolador con conectividad Bluetooth Low Energy (BLE) para emitir información de sensores de CO2 en formato iBeacon. La información incluye mediciones ambientales como los niveles de dióxido de carbono (CO2), y es enviada a dispositivos compatibles cercanos a través de anuncios BLE.

El proyecto es ideal para monitorear la calidad del aire en espacios cerrados, pudiendo enviar alertas o valores actualizados de CO2 en tiempo real. El dispositivo mide continuamente el CO2 y envía los datos a un dispositivo móvil o un receptor Bluetooth configurado para interpretar los beacons.

# ----------------------------------------------------------------------------------------------------------------------------------------

### Clases principales:

1. LED
La clase LED maneja la operación de encendido y apagado de un LED conectado a un pin digital de la placa. Permite controlar el estado del LED, alternarlo, y encenderlo por un tiempo determinado para indicar la actividad del sistema.

`Métodos`:

encender(): Enciende el LED.
apagar(): Apaga el LED.
alternar(): Cambia el estado actual del LED (encendido/apagado).
brillar(long tiempo): Enciende el LED durante un tiempo específico.

2. EmisoraBLE
La clase EmisoraBLE se encarga de la emisión de datos en formato BLE iBeacon. Permite configurar y manejar las transmisiones BLE, definiendo el nombre de la emisora, el identificador de fabricante, la potencia de transmisión, y los datos a enviar. También puede emitir un beacon libre donde se definen manualmente los bytes de carga del anuncio.

`Métodos`:

encenderEmisora(): Inicializa y enciende la emisora BLE.
emitirAnuncioIBeacon(uint8_t* beaconUUID, int16_t major, int16_t minor, uint8_t rssi): Emite un beacon BLE estándar.
emitirAnuncioIBeaconLibre(const char* carga, uint8_t tamanyoCarga): Emite un beacon libre con una carga personalizada.
detenerAnuncio(): Detiene la emisión del beacon BLE.
anyadirServicioConSusCaracteristicas(): Añade servicios y características BLE al anuncio.

Ejemplo de uso:
char datos[21] = { 'C', 'O', '2', ' ', 'M', 'e', 'd', 'i', 'c', 'i', 'o', 'n' };
elPublicador.laEmisora.emitirAnuncioIBeaconLibre(&datos[0], 21);

3. Medidor
Esta clase maneja la lógica para medir los niveles de CO2 en el entorno usando un sensor compatible. Cada vez que se mide el CO2, la clase interactúa con el sensor y devuelve los valores obtenidos.

`Métodos`:

iniciarMedidor(): Inicia el sensor de medición de CO2.
medirCO2(): Realiza una medición del nivel de CO2 y devuelve el valor.

4. PuertoSerie
Maneja la comunicación serie con el puerto UART. Permite enviar mensajes al puerto serie para realizar debug y monitorear el estado del sistema.

`Métodos`:

escribir(const char* mensaje): Escribe un mensaje al puerto serie.
esperarDisponible(): Espera hasta que el puerto esté disponible para escritura.

5. Publicador
Esta clase actúa como un mediador entre el Medidor y la EmisoraBLE. Se encarga de coordinar la medición del CO2 y la posterior publicación de los datos en forma de beacon.

`Métodos`:

publicarCO2(float valorCO2, uint8_t cont, uint16_t intervalo): Publica la medición de CO2 en un intervalo de tiempo determinado.

Ejemplo de uso:
float valorCO2 = Globales::elMedidor.medirCO2();
elPublicador.publicarCO2(valorCO2, 5, 1000);

# ----------------------------------------------------------------------------------------------------------------------------------------

### Preguntas Frecuentes (FAQ)
1. ¿Qué placas de desarrollo son compatibles?
El código está diseñado para trabajar con placas NRF52840 o cualquier otra compatible con el framework de Bluefruit para BLE.

2. ¿Qué sensores de CO2 puedo usar?
El proyecto está diseñado para ser compatible con sensores de CO2 que posean interfaces I2C o UART. Debes adaptar el código de Medidor para tu sensor específico.

3. ¿Cómo puedo modificar los datos enviados por el beacon?
Puedes editar la función emitirAnuncioIBeaconLibre en la clase EmisoraBLE para cambiar los datos enviados en cada beacon.

4. ¿Puedo extender el proyecto para medir otros gases?
Sí, puedes añadir nuevas clases similares a Medidor para otros sensores y seguir el mismo flujo de publicación de datos.

# ----------------------------------------------------------------------------------------------------------------------------------------

### Bibliotecas necesarias 
Asegúrate de tener las siguientes bibliotecas instaladas:

Adafruit Bluefruit nRF52

