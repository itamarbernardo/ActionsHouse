#include <SPI.h>
#include <Ethernet.h>
#include <IRremote.h>


EthernetServer server = EthernetServer(1000);
IRsend irsend;

int pinLuz = 7;
boolean verificaLuz;
boolean verificaTV;
boolean verificaAr;

void setup()
{
  Serial.begin(9600);

  uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
  IPAddress myIP(192, 168, 0, 6);

  Serial.println("Iniciando...");

  Ethernet.begin(mac, myIP);

  server.begin();

  Serial.println("Rodando!!!");
  pinMode(pinLuz, OUTPUT);
  digitalWrite(pinLuz, LOW);
  verificaLuz = false;
  verificaTV = false;
  verificaAr = false;
}

void loop()
{

  EthernetClient client = server.available();
  if (client.connected())
  {
    Serial.println("Cliente conectado!");
    if (client.available()) {
      char c = client.read();
      Serial.println(c);
      if (c == '2') {
        if (verificaLuz == false) {
          digitalWrite(pinLuz, HIGH);
          verificaLuz = true;
          Serial.println("Lâmpada ligada");
        } else if (verificaLuz == true) {
          digitalWrite(pinLuz, LOW);
          verificaLuz = false;
          Serial.println("Lâmpada desligada");
        }
        client.println("55555");
      }
      
      else if (c == '3') {
        //Ligar-Desligar Ar
        if (verificaAr == false) {
          ligarArCondicionado();
          verificaAr = true;
          Serial.println("Ar Ligado");
        } else if (verificaAr == true) {
          desligarArCondicionado();
          verificaAr = false;
          Serial.println("Ar Desligado");
        }
        client.println("22222");

      }
     
      else if (c == '4') {
        //Ligar-Desligar TV
        if (verificaTV == false) {
          ligarTV();
          verificaTV = true;
          Serial.println("TV Ligado");
        } else if (verificaTV == true) {
          desligarTV();
          verificaTV = false;
          Serial.println("TV Desligado");
        }
        client.println("33333");
      }
     
      else if (c == '5') {
        //Aumentar Volume
        aumentarVolume();
        Serial.println("Volume aumentado!");
      }
      else if (c == '6') {
        //Diminuir volume
        diminuirVolume();
        Serial.println("Volume diminuído!");
      }
      else if (c == '7') {
        //Aumentar Canal
        aumentarCanal();
        Serial.println("Canal aumentado");
      }
      else if (c == '8') {
        //Diminuir canal
        diminuirCanal();
        Serial.println("Canal diminuido");
      }
      else if (c == '9') {
        digitalWrite(pinLuz, HIGH);
        ligarArCondicionado();
        ligarTV();
        delay(2000);
        tecla3(); 
        //Função Chegar em casa
        //Ligar Luz, TV, Ar-Condicionado
      }

    }
  }
  client.stop();
  delay(1000);

}

void ligarArCondicionado() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int irSignal[] = {624, 17840, 2964, 9016, 536, 488, 456, 1516, 452, 568, 464, 532, 496, 500, 528, 468, 536, 464, 532, 464, 532, 464, 536, 1432, 560, 460, 536, 460, 536, 1460, 532, 464, 536, 460, 536, 1460, 532, 1460, 456, 1540, 512, 1480, 556, 1436, 564, 460, 532, 464, 536, 460, 536, 460, 536, 460, 536, 460, 536, 464, 532, 464, 536, 460, 536, 460, 536, 488, 508, 488, 428, 572, 428, 568, 460, 536, 492, 504, 528, 468, 532, 464, 536, 464, 532, 464, 536, 460, 536, 460, 536, 460, 536, 464, 532, 464, 536, 460, 532, 464, 536, 460, 536, 488, 508, 492, 424, 568, 432, 568, 460, 1504, 528, 1468, 560, 1432, 560, 1436, 560, 2940, 3040, 8920, 560, 1436, 556, 460, 540, 460, 536, 460, 536, 460, 536, 488, 508, 488, 428, 568, 432, 564, 464, 1504, 552, 472, 536, 460, 536, 1432, 564, 460, 536, 1432, 560, 1432, 560, 1436, 556, 1456, 540, 1460, 532, 1460, 452, 568, 436, 564, 464, 532, 496, 500, 532, 464, 536, 460, 540, 456, 540, 460, 536, 460, 536, 460, 536, 460, 536, 460, 540, 456, 540, 456, 540, 460, 536, 460, 536, 488, 452, 544, 428, 568, 436, 564, 460, 536, 524, 472, 532, 464, 536, 460, 536, 460, 540, 460, 536, 460, 536, 460, 536, 460, 536, 460, 540, 456, 540, 456, 540, 460, 536, 488, 508, 488, 452, 544, 536, 2916, 3064, 8920, 488, 1504, 556, 468, 536, 460, 540, 456, 540, 456, 540, 456, 540, 460, 536, 460, 540, 456, 540, 1452, 540, 456, 540, 456, 540, 1456, 540, 484, 428, 1540, 488, 1504, 556, 468, 536, 1428, 568, 1428, 564, 1428, 564, 1432, 560, 1436, 556, 1456, 540, 1456, 536, 1456, 452, 572, 436, 560, 464, 532, 496, 1472, 564, 1432, 564, 1428, 564, 456, 540, 456, 540, 460, 536, 460, 540, 456, 540, 456, 540, 456, 540, 460, 536, 488, 512, 1456, 452, 1540, 516, 504, 528, 1440, 564, 1432, 564, 456, 540, 456, 540, 456, 540, 460, 536, 460, 540, 456, 540, 456, 540, 1452, 540, 1456, 536, 1456, 460, 1536, 564}; //AnalysIR Batch Export (IRremote) - RAW
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.
}

void desligarArCondicionado() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int irSignal[] = {632, 17832, 3000, 8988, 464, 556, 500, 1492, 500, 500, 536, 460, 544, 452, 548, 448, 548, 448, 548, 448, 548, 452, 544, 1448, 544, 452, 548, 448, 548, 1444, 548, 1476, 516, 480, 520, 1472, 468, 1524, 500, 1496, 540, 1452, 544, 1448, 544, 452, 548, 452, 544, 452, 544, 452, 544, 452, 544, 452, 544, 452, 548, 452, 544, 480, 516, 480, 464, 532, 464, 532, 492, 504, 500, 500, 536, 460, 544, 452, 544, 452, 544, 452, 544, 452, 544, 456, 540, 456, 540, 456, 544, 452, 544, 452, 548, 448, 544, 456, 540, 484, 516, 480, 420, 576, 436, 560, 464, 532, 496, 500, 536, 464, 540, 456, 540, 1452, 536, 1456, 540, 2940, 3048, 8908, 560, 1460, 532, 464, 536, 460, 536, 460, 540, 484, 512, 484, 432, 568, 432, 564, 464, 532, 496, 1500, 528, 464, 536, 464, 536, 1456, 532, 464, 536, 1456, 536, 1460, 532, 1460, 532, 1460, 532, 1488, 432, 1540, 480, 540, 464, 532, 496, 500, 532, 464, 536, 464, 532, 464, 536, 460, 536, 460, 536, 460, 536, 460, 536, 460, 540, 460, 532, 464, 532, 464, 536, 460, 536, 488, 508, 488, 432, 568, 460, 536, 464, 532, 496, 500, 532, 464, 536, 460, 536, 460, 536, 464, 536, 460, 536, 460, 536, 460, 536, 460, 540, 456, 540, 460, 536, 460, 536, 460, 536, 488, 512, 484, 432, 568, 536, 2916, 3072, 8908, 524, 1496, 536, 460, 544, 452, 544, 452, 544, 452, 544, 456, 544, 452, 544, 452, 544, 452, 544, 1448, 544, 456, 540, 456, 544, 1476, 436, 1556, 464, 1532, 528, 1464, 540, 456, 544, 1448, 544, 1448, 544, 1452, 544, 1448, 544, 1448, 544, 1452, 540, 1480, 440, 1552, 464, 532, 500, 496, 536, 464, 540, 1452, 544, 1448, 544, 1452, 540, 456, 544, 452, 544, 452, 544, 452, 544, 452, 548, 448, 548, 452, 544, 480, 516, 480, 464, 1528, 468, 1524, 536, 464, 540, 1452, 544, 1448, 544, 452, 544, 456, 544, 452, 544, 452, 544, 452, 544, 452, 544, 452, 544, 456, 544, 476, 520, 1476, 436, 1556, 544}; //AnalysIR Batch Export (IRremote) - RAW
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.
}

void ligarTV() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int irSignal[] = {2736, 848, 484, 852, 484, 412, 484, 412, 1376, 1300, 484, 412, 484, 412, 484, 412, 484, 412, 484, 412, 488, 408, 484, 412, 484, 416, 480, 416, 484, 412, 480, 416, 928, 408, 484, 852, 484, 412, 484, 17708, 2732, 848, 488, 848, 484, 416, 480, 416, 1372, 1300, 484, 412, 484, 412, 480, 416, 484, 412, 484, 412, 484, 416, 480, 416, 480, 416, 480, 416, 480, 416, 484, 412, 928, 408, 484, 852, 480, 416, 484}; //AnalysIR Batch Export (IRremote) - RAW
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.
}

void desligarTV() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int irSignal[] = {2680, 900, 432, 904, 432, 464, 436, 460, 432, 904, 880, 468, 456, 440, 408, 488, 432, 464, 432, 464, 432, 464, 428, 468, 432, 464, 436, 464, 436, 460, 432, 464, 432, 464, 856, 480, 460, 876, 432, 464, 412, 17784, 2656, 924, 428, 908, 432, 464, 432, 464, 408, 928, 880, 464, 408, 488, 436, 464, 468, 428, 432, 464, 452, 444, 408, 488, 408, 488, 432, 464, 432, 464, 436, 464, 440, 452, 880, 460, 472, 864, 484, 412, 432, 17748, 2704, 872, 488, 852, 408, 488, 432, 464, 408, 928, 876, 468, 436, 460, 464, 432, 412, 484, 464, 436, 416, 480, 460, 436, 408, 488, 432, 464, 436, 460, 408, 488, 452, 444, 880, 460, 428, 904, 488, 412, 408}; //AnalysIR Batch Export (IRremote) - RAW
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.
}

void aumentarVolume() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int irSignal[] = {2652, 900, 432, 908, 432, 464, 432, 464, 432, 904, 876, 468, 432, 464, 456, 444, 428, 468, 428, 468, 432, 464, 432, 464, 432, 464, 432, 464, 432, 464, 432, 464, 900, 888, 428, 468, 432, 464, 432, 464, 432, 17772, 2652, 900, 436, 904, 432, 464, 436, 460, 456, 880, 904, 444, 428, 464, 432, 468, 432, 460, 432, 468, 432, 464, 432, 464, 428, 468, 464, 432, 432, 468, 432, 460, 876, 912, 432, 460, 436, 464, 460, 436, 432}; //AnalysIR Batch Export (IRremote) - RAW
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.
}

void diminuirVolume() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int irSignal[] = {2680, 896, 436, 900, 436, 436, 460, 388, 508, 904, 880, 372, 528, 420, 476, 432, 464, 432, 464, 436, 460, 436, 460, 404, 492, 392, 508, 436, 460, 408, 484, 388, 956, 904, 436, 424, 472, 440, 900, 18228, 2624, 932, 404, 932, 404, 492, 404, 408, 488, 932, 848, 280, 620, 408, 488, 480, 420, 332, 564, 492, 404, 492, 404, 492, 404, 480, 416, 456, 440, 464, 432, 380, 964, 332, 64, 540, 404, 488, 408, 312, 1028, 18224, 2684, 896, 436, 900, 436, 364, 532, 436, 460, 900, 880, 408, 492, 368, 528, 392, 508, 436, 460, 372, 524, 436, 460, 380, 516, 380, 516, 440, 456, 388, 508, 396, 948, 904, 436, 432, 464, 432, 908}; //AnalysIR Batch Export (IRremote) - RAW
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.
}

void aumentarCanal() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int irSignal[] = {2788, 848, 484, 852, 484, 412, 484, 412, 1372, 1304, 480, 416, 480, 416, 480, 416, 484, 412, 484, 412, 484, 412, 484, 416, 480, 412, 484, 416, 924, 860, 480, 416, 480, 416, 484, 412, 484, 412, 480, 17712, 2656, 900, 460, 876, 432, 464, 432, 464, 1320, 1352, 432, 464, 436, 464, 436, 460, 460, 436, 432, 464, 432, 464, 436, 460, 436, 460, 432, 468, 876, 908, 432, 464, 432, 464, 432, 464, 432, 464, 432, 17776, 2652, 900, 432, 904, 432, 464, 432, 468, 1320, 1352, 428, 468, 432, 464, 428, 468, 428, 468, 436, 460, 432, 468, 428, 464, 432, 468, 432, 464, 876, 908, 428, 468, 428, 468, 428, 468, 428, 472, 428}; //AnalysIR Batch Export (IRremote) - RAW
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.
}

void diminuirCanal() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int irSignal[] = {2684, 896, 432, 904, 464, 432, 412, 484, 412, 924, 880, 468, 432, 464, 456, 440, 508, 388, 484, 412, 484, 412, 468, 428, 436, 460, 436, 460, 460, 440, 856, 928, 436, 460, 456, 440, 412, 484, 912, 18172, 2684, 896, 412, 924, 432, 464, 436, 464, 456, 880, 856, 488, 412, 484, 436, 460, 428, 468, 412, 484, 412, 484, 468, 428, 412, 484, 464, 436, 432, 464, 908, 876, 412, 484, 460, 436, 436, 460, 908}; //AnalysIR Batch Export (IRremote) - RAW
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.
}

void tecla3() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int irSignal[] = {2764, 820, 436, 900, 480, 416, 436, 460, 1352, 1320, 440, 460, 480, 416, 492, 404, 436, 460, 436, 460, 464, 432, 464, 432, 436, 460, 508, 392, 432, 460, 464, 436, 516, 380, 408, 488, 908, 428, 460, 18176, 2684, 896, 428, 908, 436, 460, 436, 460, 1300, 1372, 440, 460, 432, 464, 412, 484, 432, 464, 436, 460, 436, 460, 436, 460, 436, 460, 436, 460, 464, 436, 432, 464, 432, 464, 460, 436, 904, 432, 464}; //AnalysIR Batch Export (IRremote) - RAW
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.
}

