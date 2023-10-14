/*
 * Pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      ESP32       
 *             Reader/PCD   C3 DevKit       
 * Signal      Pin          Pin           
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          7             
 * SPI SS      SDA(SS)      0           
 * SPI MOSI    MOSI         6  
 * SPI MISO    MISO         5   
 * SPI SCK     SCK          4   
 *
 */
//----------------------------------------------------
// INCLUDE SECTION
#include <SPI.h>
#include <MFRC522.h>

//----------------------------------------------------
// DEFINE SECTION
#define RST_PIN         7           // Configurable, not part of ESP32 SPI hardware pins
#define SS_PIN          0           // Configurable, not part of ESP32 SPI hardware pins

 //----------------------------------------------------
// GLOBALS SECTION
 MFRC522 mfrc522(SS_PIN, RST_PIN);              // Create MFRC522 instance
 byte red_tag[4] = {0x90, 0xD3, 0x11, 0x25};    // Creating the byte array holding the red RFID tags Unique ID
 byte green_tag[4] = {0x11, 0x7C, 0x5D, 0x26};  // Creating the byte array holding the red RFID tags Unique ID
 
 //----------------------------------------------------
 // SETUP SECTION
 void setup() {
  Serial.begin(9600);                 // Initialize serial communications with the PC
  while (!Serial);                    // Do nothing if no serial port is opened (usually added for Arduinos based on ATMEGA32U4)
  SPI.begin();                        // Initialize the SPI bus
  
  mfrc522.PCD_Init();                 // Init MFRC522
  delay(4);                           // Optional delay. Some board do need more time after init to be ready
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  neopixelWrite(RGB_BUILTIN,0,0,0);   // Set the built in ESP32 Dev board LED to Off / Black
}
//----------------------------------------------------
// MAIN LOOP 
void loop() {
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) {                     // Reset the loop if no new card present on the MFRC522 reader... 
    return;                                                     // ...This saves the entire process when idle.
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {                       // Read the tag that is present on the MFRC522 reader ...
    return;                                                     // ...(this updates the datafields of the global "mfrc522" instance from the GLOBALS section)
  }
 
    Serial.print(F(": Card UID:"));                             // Show some details of the tag that was found
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();

  if (memcmp(mfrc522.uid.uidByte, red_tag, 4) == 0){            // Compare the byte array for the red tag with the ID read from the tag present on the MFRC522 reader
    neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,0,0);              // Set the built in ESP32 Dev board LED to RED
    delay(1000);
    neopixelWrite(RGB_BUILTIN,0,0,0); // Off / black            // Set the built in ESP32 Dev board LED to Off / Black
    delay(1000);
  }

  if (memcmp(mfrc522.uid.uidByte, green_tag, 4) == 0){          // Compare the byte array for the red tag with the ID read from the tag present on the MFRC522 reader
    neopixelWrite(RGB_BUILTIN,0,RGB_BRIGHTNESS,0);              // Set the built in ESP32 Dev board LED to RED
    delay(1000);
    neopixelWrite(RGB_BUILTIN,0,0,0);                           // Set the built in ESP32 Dev board LED to Off / Black
    delay(1000);
  }
}
//----------------------------------------------------
// ADDITIONAL FUNCTIONS SECTION 

void dump_byte_array(byte * buffer, byte bufferSize) {          // Helper function which steps through each of the bytes in the tags ID and prints them to the terminal
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
