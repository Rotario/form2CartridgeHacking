/*

DS2431 example that dumps the whole memory and prints it to Serial.
Then a write operation is done and the memory is read again.

MIT License

Copyright (c) 2017 Tom Magnier

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#include <DS2431.h>
#include <OneWire.h>

const int ONE_WIRE_PIN = 1; // One Wire pin, change according to your needs. A 4.7k pull up resistor is needed.

OneWire oneWire(ONE_WIRE_PIN);
DS2431 eeprom(oneWire);

void setup()
{
  Serial.begin(115200);
  while (!Serial); // wait for Serial to come up on USB boards

  
}

void loop()
{
  if (Serial.available()){
    while(Serial.available())
      Serial.read();

    // Search the 1-Wire bus for a connected device.
  byte serialNb[8];
  oneWire.target_search(DS2431::ONE_WIRE_FAMILY_CODE);
  if (!oneWire.search(serialNb))
  {
    Serial.println("No DS2431 found on the 1-Wire bus.");
    return;
  }

  // Check serial number CRC
  if (oneWire.crc8(serialNb, 7) != serialNb[7])
  {
    Serial.println("A DS2431 was found but the serial number CRC is invalid.");
    return;
  }

  Serial.print("DS2431 found with serial number : ");
  printBuffer(serialNb, 8);
  Serial.println("");

  // Initialize DS2431 object
  eeprom.begin(serialNb);

  // Read all memory content
  byte data[128];
  eeprom.read(0, data, sizeof(data));

  Serial.println("Memory contents : ");
  printLargeBuffer(data, sizeof(data));
  Serial.println("");

  // Write a 8-byte row
  /*byte header1[] = {0x43,0x0,0xEB,0xF,0x62,0x41,0x7C,0x7};
  byte header2[] = {0x92,0x89,0x59,0xE2,0x1,0xE0,0x53,0x2F};
  byte header3[] = {0xEF,0xE2,0x47,0x74,0x4D,0x7E,0x7E,0x67};
  byte header4[] = {0x48,0x1A,0xAE,0xDC,0x8A,0x6F,0xEF,0x3D};
  byte header5[] = {0x60,0x2A,0xA2,0x91,0x69,0x48,0x4A,0xB3};
  byte header6[] = {0xA7,0x48,0x98, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  byte cartridge1[] = {0x1,0xB4,0xA,0x72,0xE1,0xF2,0x84,0x1B};
  byte cartridge2[] = {0x91,0xD2,0xB4,0x5C,0x1,0x0,0x8,0x0};

  /*byte header1[] = {0x43,0x0,0x38,0x82,0x86,0xDA,0x62,0x7};
  byte header2[] = {0x52,0x86,0x4A,0x78,0x2A,0x6C,0xF8,0x52};
  byte header3[] = {0xDB,0x99,0x5D,0xEE,0x8D,0x5A,0x91,0x94};
  byte header4[] = {0x4B,0xB0,0x3E,0xF7,0x9E,0x6B,0x71,0xF7};
  byte header5[] = {0x7C,0x69,0xEB,0xB,0xC5,0x19,0xB0,0xEE};
  byte header6[] = {0xF6,0xAD,0x23,0xFF,0xFF,0xFF,0xFF,0xFF};

  byte cartridge1[] = {0x1,0xDA,0xF2,0x76,0x82,0xDC,0xC1,0xDB};
  byte cartridge2[] = {0xAF,0x2C,0x72,0xF,0x22,0x2,0xED,0xA};*/

  eeprom.write(0, header1, sizeof(header1));
  eeprom.write(8, header2, sizeof(header2));
  eeprom.write(16, header3, sizeof(header3));
  eeprom.write(24, header4, sizeof(header4));
  eeprom.write(32, header5, sizeof(header5));
  eeprom.write(40, header6, sizeof(header6));

  eeprom.write(64, cartridge1, sizeof(cartridge1));
  eeprom.write(72, cartridge2, sizeof(cartridge2));
  

  // Read again memory content
  eeprom.read(0, data, sizeof(data));

  Serial.println("Memory contents : ");
  printLargeBuffer(data, sizeof(data));
  Serial.println("");

  }
}

void printBuffer(const uint8_t *buf, uint8_t len)
{
  for (int i = 0; i < len-1; i++)
  {
    Serial.print(buf[i], HEX);
    Serial.print(",");
  }
  Serial.println(buf[len-1], HEX);
}

void printLargeBuffer(const uint8_t *buf, uint16_t len)
{
  uint8_t bytesPerLine = 8;

  for (int i = 0; i < len / bytesPerLine; i++)
  {
    Serial.print(i * bytesPerLine);
    Serial.print("\t\t:");
    printBuffer(buf + i * bytesPerLine, bytesPerLine);
  }
}
