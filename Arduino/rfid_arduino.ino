#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_R 2//LED Vermelho
#define LED_G 3 //LED Verde
char st[20];
char last_char;
 
MFRC522 mfrc522(SS_PIN, RST_PIN);
 
void setup()
{
  // Inicia a serial
  Serial.begin(9600);
  // Inicia  SPI bus 
  SPI.begin();
  // Inicia MFRC522    
  mfrc522.PCD_Init();
}
 
void loop()
{
  // Busca novos cartões 
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Seleciona um catão a ser lido
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Mostra ID na serial
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  conteudo.toUpperCase();
 
   if (conteudo.substring(1) == "6C 9A 62 9B") //ID CARTÃO MACIEL
  {
    if(last_char != '0'){
      Serial.println("0");
      last_char = '0';
    }
  }
  if (conteudo.substring(1) == "6A A5 8A 36") //ID CARTÃO LUCCA
  {
    if(last_char != '1'){
      Serial.println("1");
      last_char = '1';
  }
}
}
