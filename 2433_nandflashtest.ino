// Pin definitions for Arduino Leonardo
#define D0 2  // Pin 2 corresponds to P1_0 on MSP430
#define D1 3  // Pin 3 corresponds to P1_1 on MSP430
#define D2 4  // Pin 4 corresponds to P1_2 on MSP430
#define D3 5  // Pin 5 corresponds to P1_3 on MSP430
#define D4 6  // Pin 6 corresponds to P2_7 on MSP430
#define D5 7  // Pin 7 corresponds to P1_5 on MSP430
#define D6 8  // Pin 8 corresponds to P1_6 on MSP430
#define D7 9  // Pin 9 corresponds to P1_7 on MSP430

#define NCE1 10  // Pin 10 corresponds to P2_0 on MSP430
#define NCE2 11  // Pin 11 corresponds to P2_1 on MSP430
#define CLE 12  // Pin 12 corresponds to P3_1 on MSP430
#define ALE 13  // Pin 13 corresponds to P2_5 on MSP430
#define NRE A0  // Analog pin A0 corresponds to P2_6 on MSP430
#define NWE A1  // Analog pin A1 corresponds to P3_2 on MSP430
#define NWP A2  // Analog pin A2 corresponds to P2_2 on MSP430
#define RNB A3  // Analog pin A3 corresponds to P2_4 on MSP430

// NAND Flash Commands
#define NAND_CMD_PAGE_READ (0x00)
#define NAND_CMD_READ_CONFIRM (0x30)
#define NAND_CMD_READ_STATUS_REGISTER (0x70)
#define NAND_CMD_READ_ID (0x90)
#define NAND_CMD_READ_PARAMETER_PAGE (0xEC)
#define NAND_CMD_RESET (0xFF)

#define NAND_CMD_PAGE_READ (0x00)
#define NAND_CMD_READ_CONFIRM (0x30)
#define NAND_CMD_READ_STATUS_REGISTER (0x70)
#define NAND_CMD_READ_ID (0x90)
#define NAND_CMD_READ_PARAMETER_PAGE (0xEC)
#define NAND_CMD_RESET (0xFF)

void setDataBusRead (void) {
  pinMode(D7, INPUT);
  pinMode(D6, INPUT);
  pinMode(D5, INPUT);
  pinMode(D4, INPUT);
  pinMode(D3, INPUT);
  pinMode(D2, INPUT);
  pinMode(D1, INPUT);
  pinMode(D0, INPUT);
}

unsigned char readDataBus() {
  unsigned char dataBuffer = 0x00;
  setDataBusRead();
  if (digitalRead(D7) == HIGH)
    dataBuffer |= 0x80;
  if (digitalRead(D6) == HIGH)
    dataBuffer |= 0x40;
  if (digitalRead(D5) == HIGH)
    dataBuffer |= 0x20;
  if (digitalRead(D4) == HIGH)
    dataBuffer |= 0x10;
  if (digitalRead(D3) == HIGH)
    dataBuffer |= 0x08;
  if (digitalRead(D2) == HIGH)
    dataBuffer |= 0x04;
  if (digitalRead(D1) == HIGH)
    dataBuffer |= 0x02;
  if (digitalRead(D0) == HIGH)
    dataBuffer |= 0x01;

  return (dataBuffer);
}

void setDataBusWrite() {
  pinMode(D7, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D0, OUTPUT);
}

void writeDataBus(unsigned char dataBusState) {
  setDataBusWrite();
  if ((dataBusState & 0x80) >> 7)
    digitalWrite(D7, HIGH);
  else
    digitalWrite(D7, LOW);
  if ((dataBusState & 0x40) >> 6)
    digitalWrite(D6, HIGH);
  else
    digitalWrite(D6, LOW);
  if ((dataBusState & 0x20) >> 5)
    digitalWrite(D5, HIGH);
  else
    digitalWrite(D5, LOW);
  if ((dataBusState & 0x10) >> 4)
    digitalWrite(D4, HIGH);
  else
    digitalWrite(D4, LOW);
  if ((dataBusState & 0x08) >> 3)
    digitalWrite(D3, HIGH);
  else
    digitalWrite(D3, LOW);
  if ((dataBusState & 0x04) >> 2)
    digitalWrite(D2, HIGH);
  else
    digitalWrite(D2, LOW);
  if ((dataBusState & 0x02) >> 1)
    digitalWrite(D1, HIGH);
  else
    digitalWrite(D1, LOW);
  if ((dataBusState & 0x01))
    digitalWrite(D0, HIGH);
  else
    digitalWrite(D0, LOW);
}

void chip1Select(boolean isSelected) {
  if (isSelected)
  digitalWrite(NCE1, LOW);
  else
  digitalWrite(NCE1, HIGH);
}

void chip2Select(boolean isSelected) {
  if (isSelected)
  digitalWrite(NCE2, LOW);
  else
  digitalWrite(NCE2, HIGH);
}

boolean isChipBusy() {
  return !digitalRead(RNB);
}

void writeCommand(unsigned char target, unsigned char command) {
  digitalWrite(CLE, HIGH);
  digitalWrite(ALE, LOW);
  if (target == 0x00) {
    chip1Select(1);
    chip2Select(0);
  }
  else if (target == 0x01) {
    chip1Select(0);
    chip2Select(1);    
  }
  else {
    chip1Select(0);
    chip2Select(0);
    Serial.println("Error at writeCommand(): Invalid chip selection (must be 0 for 1st target, 1 for 2nd target)!");
  }
  digitalWrite(NWE, LOW);
  writeDataBus(command);
  digitalWrite(NWE, HIGH);
  digitalWrite(CLE, LOW);
}

void writeAddress(unsigned char target, unsigned char address) {
  digitalWrite(CLE, LOW);
  digitalWrite(ALE, HIGH);
  if (target == 0x00) {
    chip1Select(1);
    chip2Select(0);
  }
  else if (target == 0x01) {
    chip1Select(0);
    chip2Select(1);    
  }
  else {
    chip1Select(0);
    chip2Select(0);
    Serial.println("Error at writeAddress(): Invalid chip selection (must be 0 for 1st target, 1 for 2nd target)!");
  }
  digitalWrite(NWE, LOW);
  writeDataBus(address);
  digitalWrite(NWE, HIGH);
  digitalWrite(ALE, LOW);
}

void writeData(unsigned char target, unsigned char data) {
  digitalWrite(CLE, LOW);
  digitalWrite(ALE, LOW);
  if (target == 0x00) {
    chip1Select(1);
    chip2Select(0);
  }
  else if (target == 0x01) {
    chip1Select(0);
    chip2Select(1);    
  }
  else {
    chip1Select(0);
    chip2Select(0);
    Serial.println("Error at writeData(): Invalid chip selection (must be 0 for 1st target, 1 for 2nd target)!");
  }
  digitalWrite(NWE, LOW);
  writeDataBus(data);
  digitalWrite(NWE, HIGH);
}


unsigned char readData(unsigned char target) {
  unsigned char dataOut = 0x00;
  digitalWrite(CLE, LOW);
  digitalWrite(ALE, LOW);
  if (target == 0x00) {
    chip1Select(1);
    chip2Select(0);
  }
  else if (target == 0x01) {
    chip1Select(0);
    chip2Select(1);    
  }
  else {
    chip1Select(0);
    chip2Select(0);
    Serial.println("Error at readData(): Invalid chip selection (must be 0 for 1st target, 1 for 2nd target)!");
  }
  digitalWrite(NRE, LOW);
  dataOut = readDataBus();
  digitalWrite(NRE, HIGH);
  return (dataOut);    
}



void setup() {
  Serial.begin(9600);
  Serial.println("NAND Flash Test Program, by Jason Gin. Written November 17, 2018.");
  Serial.println("Initializing interface...");
  
  // Set up R/B as input, pull-up
  pinMode(RNB, INPUT_PULLUP);
  
  // Set up control lines to idle
  pinMode(NCE1, OUTPUT);
  digitalWrite(NCE1, HIGH);
  pinMode(NCE2, OUTPUT);
  digitalWrite(NCE2, HIGH);
  
  pinMode(CLE, OUTPUT);
  digitalWrite(CLE, LOW);
  pinMode(ALE, OUTPUT);
  digitalWrite(ALE, LOW);
  
  pinMode(NRE, OUTPUT);
  digitalWrite(NRE, HIGH);
  pinMode(NWE, OUTPUT);
  digitalWrite(NWE, HIGH);
  
  pinMode(NWP, OUTPUT);
  digitalWrite(NWP, LOW);

  // Set up data lines to 0x00
  writeDataBus(0x00);
  Serial.println("Interface initialization complete.");
  
  // Reset NAND device
  Serial.println("Resetting NAND chips...");
  writeCommand(0,NAND_CMD_RESET);
  writeCommand(1,NAND_CMD_RESET);
  chip1Select(0);
  chip2Select(0);
}


void loop() {

  while(true)
  {

  unsigned char byteBuffer = 0x00;
  int i;
  // Reset NAND device
  Serial.println("Resetting NAND chip 1...");
  writeCommand(0,NAND_CMD_RESET);

  // Get status
  Serial.println("Reading chip status...");
  writeCommand(0,NAND_CMD_READ_STATUS_REGISTER);
  Serial.print ("Status = 0x");
  Serial.print(readData(0),HEX);
  Serial.println("");

  // Read ID
  Serial.println("Getting NAND ID for 1st target...");
  writeCommand(0,NAND_CMD_READ_ID);
  writeAddress(0,0x00);

  Serial.print("ID = ");
  
  for (int i = 0; i < 8; i++) {
    Serial.print(readData(0),HEX);
    Serial.print(" ");
  }
  Serial.println("");

  Serial.println("Checking for JEDEC signature in ID...");
  writeCommand(0,NAND_CMD_READ_ID);
  writeAddress(0,0x40);
  for (int i = 0; i < 5; i++) {
    Serial.write(readData(0));
  }
  Serial.println("");
  Serial.print("Interface byte = ");
  Serial.print(readData(0),HEX);
  Serial.println("");

  delay(1000);
  }

  Serial.println("Loading JEDEC Parameter page for target 1...");
  writeCommand(0,NAND_CMD_READ_PARAMETER_PAGE);
  writeAddress(0,0x40);
  delay(10);
  int tableRow, tableColumn, count = 0;
  unsigned char nibbleBuffer;
  count = 0;
  for (tableRow = 0; tableRow < 96; tableRow++) {
    Serial.print("Byte ");
    Serial.print(count);
    Serial.print(": ");
    for (tableColumn = 0; tableColumn < 16; tableColumn++) {
      nibbleBuffer = readData(0);

      Serial.print((nibbleBuffer >> 4),HEX); // high nibble
      Serial.print(nibbleBuffer & 0x0F,HEX); // low nibble
      Serial.print(" ");
      count++;
    }
    Serial.println("");
  }
  Serial.println("Done, halting now.");



  delay(1000);
  
  // Reset NAND device
  Serial.println("Resetting NAND chip 2...");
  writeCommand(1,NAND_CMD_RESET);


  // Read ID
  Serial.println("Getting NAND ID for 2nd target...");
  writeCommand(1,NAND_CMD_READ_ID);
  writeAddress(1,0x00);

  Serial.print("ID = ");
  
  for (int i = 0; i < 8; i++) {
    Serial.print(readData(1),HEX);
    Serial.print(" ");
  }
  Serial.println("");

  Serial.println("Checking for JEDEC signature in ID...");
  writeCommand(1,NAND_CMD_READ_ID);
  writeAddress(1,0x40);
  for (int i = 0; i < 5; i++) {
    Serial.write(readData(1));
  }
  Serial.println("");
  Serial.print("Interface byte = ");
  Serial.print(readData(1),HEX);
  Serial.println("");
  
  
  Serial.println("Loading JEDEC Parameter page for target 2...");
  writeCommand(1,NAND_CMD_READ_PARAMETER_PAGE);
  writeAddress(1,0x40);
  delay(10);
  tableRow, tableColumn, count = 0;
  nibbleBuffer;
  count = 0;
  for (tableRow = 0; tableRow < 96; tableRow++) {
    Serial.print("Byte ");
    Serial.print(count);
    Serial.print(": ");
    for (tableColumn = 0; tableColumn < 16; tableColumn++) {
      nibbleBuffer = readData(1);
      Serial.print((nibbleBuffer >> 4),HEX); // high nibble
      Serial.print(nibbleBuffer & 0x0F,HEX); // low nibble
      Serial.print(" ");
      count++;
    }
    Serial.println("");
  }

  delay(1000);

  Serial.println("Reading page 0, block 0 on target 1...");
  writeCommand(0,NAND_CMD_PAGE_READ);

  // Column
  writeAddress(0,0x00); // Unable to determine if THGBM4G9D8GBAII needs 2 or 3 COL addresses
  writeAddress(0,0x00);
  writeAddress(0,0x00);
  // Row
  writeAddress(0,0x00);
  writeAddress(0,0x00);
  writeAddress(0,0x00);
  
  writeCommand(0,NAND_CMD_READ_CONFIRM);
  delay(10);

  count = 0;
  for (tableRow = 0; tableRow < 552; tableRow++) {
      Serial.print("Byte ");
      Serial.print(count);
      Serial.print(": ");
      for (tableColumn = 0; tableColumn < 16; tableColumn++) {
        nibbleBuffer = readData(0);
        Serial.print((nibbleBuffer >> 4),HEX); // high nibble
        Serial.print(nibbleBuffer & 0x0F,HEX); // low nibble
        Serial.print(" ");
        count++;
      }
      Serial.println("");
    }

delay(1000);

Serial.println("Reading page 0, block 0 on target 2...");
  writeCommand(1,NAND_CMD_PAGE_READ);


  // Column
  writeAddress(1,0x00);
  writeAddress(1,0x00);
  writeAddress(1,0x00);
  // Row
  writeAddress(1,0x00);
  writeAddress(1,0x00);
  writeAddress(1,0x00);
  
  writeCommand(1,NAND_CMD_READ_CONFIRM);
  delay(10);


  count = 0;
  for (tableRow = 0; tableRow < 552; tableRow++) {
      Serial.print("Byte ");
      Serial.print(count);
      Serial.print(": ");
      for (tableColumn = 0; tableColumn < 16; tableColumn++) {
        nibbleBuffer = readData(1);
        Serial.print((nibbleBuffer >> 4),HEX); // high nibble
        Serial.print(nibbleBuffer & 0x0F,HEX); // low nibble
        Serial.print(" ");
        count++;
      }
      Serial.println("");
    }

  Serial.println("All done!");


  while(1)
  {}
}
