#include <math.h>

enum newType{AVVIO, CALIBRAZIONE, INVIO_CALIBRAZIONE, MOVIMENTO, FINE_RIAVVIO};

char risposta;
boolean risp;
char cestino;

const int nMAX=100;
int nCaratteri;
int lungVett;
int dati[nMAX];

int millimetriXMax=100;
int millimetriYMax=150;

newType stato;

void car_to_pc();
void bytes_to_pc();
void bytes_from_pc();
void leggi_cestina();
void ricomponiNum();
boolean controllaBuffer();

void setup()
{
  Serial.begin(9600);
  stato=AVVIO;
  risp=false;
}

void loop()
{
  while(stato == AVVIO)
  {
    car_to_pc('s');
    risp=controllaBuffer();
    if(risp==true)
    {
      cestino = Serial.read(); 
      stato = CALIBRAZIONE;
    }  
  }
  
  stato = INVIO_CALIBRAZIONE;
  nCaratteri=8;
  lungVett=2;
  dati[0]=millimetriXMax;
  dati[1]=millimetriYMax;
  bytes_to_pc();
  
  stato = MOVIMENTO;
  bytes_from_pc();
  Serial.write(nCaratteri);
  Serial.write(dati[0]);
  Serial.write(dati[1]);
  bytes_to_pc();
  stato = FINE_RIAVVIO;
}

void car_to_pc(char car)
{
  Serial.write(car);
  delay(500);
}

void bytes_to_pc()
{
  Serial.print(nCaratteri);
  car_to_pc(',');
  delay(500);
  for(int i=0;i<lungVett;i++)
  {
    Serial.print(dati[i]);
    delay(500);
    if(i==0)
      car_to_pc(';');
  }
}

void bytes_from_pc()
{
  while(Serial.available()==0)
    delay(50);
  nCaratteri = Serial.read();
  leggi_cestina();
  int y=0;
  for(int i=0;i<nCaratteri-1;i++)//controllare il funzionamento del for e la ricezione dei byte
  {
    if(i != (nCaratteri-1)/2)
    {
      while(Serial.available()==0)
      delay(50);
      dati[y] = Serial.read();
      y++;
    }
    else
      leggi_cestina();
  }
  ricomponiNum();
}

void leggi_cestina()
{
  while(Serial.available()==0)
    delay(50);
  cestino = Serial.read();
}

void ricomponiNum()
{
  lungVett = (nCaratteri-2)/2;
  int y,i,aus;
  i=0;
  for(int k=0;k<2;k++)
  {
    y=lungVett-1;
    aus=0;
    lungVett+=lungVett;
    for(i;i<lungVett;i++)
    {
      aus+= dati[i]*pow(10,y);
      y--;
    }
    dati[k];
  }
}

boolean controllaBuffer()
{
  if(Serial.available()==0)
    return false;
  risposta=Serial.read();
  return true;
}
