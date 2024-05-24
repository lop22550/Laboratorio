# Import standard python modules.
import sys
import time
#import random
import serial


# This example uses the MQTTClient instead of the REST client
from Adafruit_IO import MQTTClient # type: ignore

# holds the count for the feed
global run_count

ADAFRUIT_IO_USERNAME = "blopezmoreira"
ADAFRUIT_IO_KEY      = "aio_FASs870z2IUeJ66XI0J4wmkoSZF7"

# Set to the ID of the feed to subscribe to for updates.
#feedContador = 'contador'
#feedLed1 = 'led1'
#feedLed2 = 'led2'
#feedTemperatura = 'temperatura'

feedServo1  = 'servo1'
feedServo2  = 'servo2'
feedServo3  = 'servo3'
feedServo4  = 'servo4'
feedPos1 = 'posicion-1'
feedPos2 = 'posicion-2'
feedPos3 = 'posicion-3'
feedPos4 = 'posicion-4'
feedGuardarLeer = 'guardar-leer'

servo1 = 0
servo2 = 0
servo3 = 0
servo4 = 0
pos1 = 0
pos2 = 0
pos3 = 0
pos4 = 0
EstadoSerial = 0
bufferAIO = []


mensaje = ''

# Define callback functions which will be called when certain events happen.
def connected(client):
    """Connected function will be called when the client is connected to
    Adafruit IO.This is a good place to subscribe to feed changes.  The client
    parameter passed to this function is the Adafruit IO MQTT client so you
    can make calls against it easily.
    """
    # Subscribe to changes on a feed named Counter.
    #print('Subscribing to Feed {0} and {1}'.format(feedLed, feedContador))
    client.subscribe(feedServo1)
    client.subscribe(feedServo2)
    client.subscribe(feedServo3)
    client.subscribe(feedServo4)
    client.subscribe(feedPos1)
    client.subscribe(feedPos2)
    client.subscribe(feedPos3)
    client.subscribe(feedPos4)
    client.subscribe(feedGuardarLeer)


    print('Waiting for feed data...')

def disconnected(client):
    """Disconnected function will be called when the client disconnects."""
    sys.exit(1)

def message(client, feed_id, payload):
    """Message function will be called when a subscribed feed has a new value.
    The feed_id parameter identifies the feed, and the payload parameter has
    the new value.
    """

    servo1 = 0
    servo2 = 0
    servo3 = 0
    servo4 = 0
    pos1 = 0
    pos2 = 0
    pos3 = 0
    pos4 = 0
    EstadoSerial = 0
    print('Feed {0} received new value: {1}'.format(feed_id, payload))
    
    if(feed_id == feedGuardarLeer):
        if(payload == 'G'):
            print('Estado: G')
            EstadoSerial = payload
            #print(EstadoSerial)
            arduino.write(bytes('G\n', 'utf-8'))
        if(payload == 'L'):
            print('Estado: L')
            EstadoSerial = payload
            arduino.write(bytes('L\n', 'utf-8'))
    if (feed_id == feedPos1):
        if (payload == 'a'):
            pos1 = payload
            arduino.write(bytes('a\n', 'utf-8'))
        if (payload == '0'):
            pos1 = payload
            arduino.write(bytes('0\n', 'utf-8'))
    if (feed_id == feedPos2):
        if (payload == 'b'):
            pos2 = payload
            arduino.write(bytes('b\n', 'utf-8'))
        if (payload == '0'):
            pos2 = payload
            arduino.write(bytes('0\n', 'utf-8'))
    if (feed_id == feedPos3):
        if (payload == 'c'):
            pos3 = payload
            arduino.write(bytes('c\n', 'utf-8'))
        if (payload == '0'):
            pos3 = payload
            arduino.write(bytes('0\n', 'utf-8'))
    if (feed_id == feedPos4):
        if (payload == 'd'):
            pos4 = payload
            arduino.write(bytes('d\n', 'utf-8'))
        if (payload == '0'):
            pos4 = payload
            arduino.write(bytes('0\n', 'utf-8'))
            #arduino.write(bytes('1\n', 'utf-8'))
    if (feed_id == feedServo1):
        servo1 = payload
        arduino.write (bytes('e\n', 'utf-8'))   #el caracter '1' es para que el arduino la detecte como una bandera de que se debe actualizar servo 1
        arduino.write (bytes('servo1', 'utf-8'))
    if (feed_id == feedServo2):
        servo2 = payload
        arduino.write (bytes('f\n', 'utf-8'))   
        arduino.write (bytes('servo2', 'utf-8'))
    if (feed_id == feedServo3):
        servo3 = payload
        arduino.write (bytes('g\n', 'utf-8'))   
        arduino.write (bytes('servo3', 'utf-8'))
    if (feed_id == feedServo4):
        servo4 = payload
        arduino.write (bytes('h\n', 'utf-8'))   
        arduino.write (bytes('servo4', 'utf-8'))

        print(servo1)
    

    
    arduino.write(bytes('bufferAIO', 'utf-8'))

try:
    client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

    # Setup the callback functions defined above.
    client.on_connect = connected
    client.on_disconnect = disconnected
    client.on_message = message

    # Connect to the Adafruit IO server.
    client.connect()
    client.loop_background()
              
    arduino = serial.Serial(port= 'COM5', baudrate =9600, timeout = 0.1)
    
    while True:    

        mensaje = arduino.readline().decode('utf-8')
        if(mensaje == 'Save\n'):
            print('Save\n')
            client.publish(feedGuardarLeer, 1)
        #if(mensaje == 'LED2\n'):
        #    print('LED 2\n')
        #    client.publish(feedLed2, 2)
        time.sleep(3)
        
        
except KeyboardInterrupt:
    print("Salimos del programa")
    if arduino.is_open:
        arduino.close()
    sys.exit(1)