 #include <SPI.h>
#include <Client.h>
#include <Ethernet.h>
#include <Server.h>
#include <Udp.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0, 10);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);
int PinLed1= 5; //esto sera actuador 1 en algun momento
int PinLed2= 4; //esto sera actuador 2 en algun momento
//int Entrada1= 5;
//int Entrada2=6;
String readString = String(30);


void setup()
{
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(PinLed1,OUTPUT);
  pinMode(PinLed2,OUTPUT);
  //pinMode(Entrada1,INPUT);
  //pinMode(Entrada2,INPUT);
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (readString.length()<30) //leer peticion HTTP caracter por caracter
        {
          readString += c;
        }
        if (c=='\n') //Si la peticion HTTP ha finalizado
        {
          //Determinar lo que se recibe mediante GET para encender el Led o apagarlo
          if(readString.indexOf("Led1=On")>0){
            digitalWrite(PinLed1,HIGH);
          }
          if(readString.indexOf("Led1=Off")>0){
            digitalWrite(PinLed1,LOW);
          }
          if(readString.indexOf("Led2=On")>0){
            digitalWrite(PinLed2,HIGH);
          }
          if(readString.indexOf("Led2=Off")>0){
            digitalWrite(PinLed2,LOW);
          }

          readString=""; //Vaciar el string que se uso para la lectura
          //Enviar cabecera HTTP estandar

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
         
        //   client.println("Connection: close");  // the connection will be closed after completion of the response
	//    client.println("Refresh: 1");  // refresh the page automatically every 5 sec
            client.println();  
          //Crear pagina web HTML

          client.println("<html>");
          client.println("<head>");
          client.println("<title>Centro de control domestico 1.0</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>Centro de control domestico 1.0</h1>");
          client.println("<hr><br>");
          client.println("<h3>Encendido/Apagado de instrumentos</h3>");
          client.println("<hr><br>");
          client.println("<table>");
          client.println("<tr><td>LED 1</td><td>LED 2</td></tr>");
          client.println("<tr><td><form method=get><input type=submit name=Led1 value=On></form></td><td><form method=get><input type=submit name=Led2 value=On></form></td></tr>");
          client.println("<tr><td><form method=get><input type=submit name=Led1 value=Off></form></td><td><form method=get><input type=submit name=Led2 value=Off></form></td></tr>");
          client.println("</table>");
          client.println("<hr><br>");
          client.println("<h3>Lectura de sensores</h3>");
          client.println("</body>");
          client.println("</html>");



          client.stop();

        }
      }
    }
  }
}
