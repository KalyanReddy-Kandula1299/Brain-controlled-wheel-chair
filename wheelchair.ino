#define BAUDRATE 57600

#define LED 13

#define ATT_TH 40

int Mot_L_Plus = 3;

int Mot_L_Minus = 5;

int Mot_R_Plus = 6;

int Mot_R_Minus = 9;

byte payloadData[32] = {0};

byte checksum=0,generatedchecksum=0,Poorquality,Attention;

int Att_Avg=0,Plength,Temp;

int k=0,j=0;

void setup()

{

Serial.begin(BAUDRATE); // USB

pinMode(LED, OUTPUT);

pinMode(Mot_L_Plus, OUTPUT);

pinMode(Mot_L_Minus, OUTPUT);

pinMode(Mot_R_Plus, OUTPUT);

pinMode(Mot_R_Minus, OUTPUT);

}

byte ReadOneByte() // One Byte Read Function

{

int ByteRead;

while(!Serial.available());

ByteRead = Serial.read();

return ByteRead;

}

void loop() // Main Function

{

while (1)

{

if(ReadOneByte() == 170) // AA 1 st Sync data

{

if(ReadOneByte() == 170) // AA 2 st Sync data

{

Plength = ReadOneByte();

if(Plength == 32) // Big Packet

{

generatedchecksum = 0;

for(int i = 0; i < Plength; i++)

{

payloadData[i] = ReadOneByte(); //Read payload into memory

generatedchecksum += payloadData[i] ;

}

generatedchecksum = 255 - generatedchecksum;

checksum = ReadOneByte();

if(checksum == generatedchecksum) // Varify Checksum

{

Poorquality = payloadData[1];

Attention = payloadData[29];

Serial.print(Attention);

Serial.print("\n");

Attention_Fun(Attention);

}

}

}

}

}

}

void Robot_Forword()

{

digitalWrite(Mot_L_Plus, LOW);

digitalWrite(Mot_L_Minus, HIGH);

digitalWrite(Mot_R_Plus, LOW);

digitalWrite(Mot_R_Minus, HIGH);

}

void Robot_Stop()

{

digitalWrite(Mot_L_Plus, LOW);

digitalWrite(Mot_L_Minus, LOW);

digitalWrite(Mot_R_Plus, LOW);

digitalWrite(Mot_R_Minus, LOW);

}

void Attention_Fun(byte data1)

{

if (Poorquality == 0)

{

if (j<3)

{

Temp += data1;

j++;

}

else

{

Temp += data1;

Att_Avg = Temp/4;

Serial.print("Att_Avg");

Serial.print(Att_Avg);

Serial.print("\n");

if (Att_Avg >= ATT_TH)

{

digitalWrite(LED, HIGH);

Robot_Forword();

}

else

{

digitalWrite(LED, LOW);

Robot_Stop();

}

j=0;

Temp=0;

}

}

else

{

Robot_Stop();

Serial.print("Signal Poor Quality");

Serial.print("\n");

}

}
