enum enumtipo 
{
  AVVIO,
  INCALIBRAZIONE,
  FINECALIBRAZIONE,
  RICEVIMENTO,
  ESECUZIONE,
  FINE,
};

/*CONNESSO (riceve una s)
NONCONNESSO (invia una s)
CALIBRAZIONE (i motori si muovono x y )
-prima si deve raggiungere un angolo senza contare i passi
-poi si raggiungo l'angolo opposto contando i passi
ROTAZ_XMAX n. rotaz. max primo motore
ROTAZ_YMAX n. rotaz. secondo motore

INVIOCALIBRAZIONE (e comunico due numeri al computer x y ) 
RICEVE
DISEGNARE (motore)*/

int coc[2];
int DESTX,DESTY;
char input;
int ROTAZ_XMAX=5,ROTAZ_YMAX=5;
String certo;
enumtipo stato;
const int NMAX =10;
unsigned char tempcal[1];
byte index = 0;
void setup ()  
{
  Serial.begin(9600);
   stato=AVVIO;
}
void loop()  
{

  if (stato==AVVIO)
  {
    delay(1500);
    if (Serial.available() > 0){
      input=Serial.read();
    }
     if(input=='s')
     {
        stato=INCALIBRAZIONE; 
     }
     else
     {
        Serial.print('s');
     }
  }
if(stato==INCALIBRAZIONE)
{
  stato=FINECALIBRAZIONE;
}
if(stato==FINECALIBRAZIONE) 
{
  invia();
  stato=RICEVIMENTO;
}
if(stato==RICEVIMENTO)
{

  Serial.print('i');
      DESTX=0;
      DESTY=0;
      coc[1]=0;
      coc[2]=0;
   for(int i=0;i<3;i++){
    coc[i]=Serial.parseInt();
  }
  DESTX=coc[1];
  DESTY=coc[2];
   if(DESTX!=0&&DESTY!=0){
    stato=ESECUZIONE;
   }
  }

  

if(stato==ESECUZIONE){

stato=FINE;
}
if(stato==FINE){
}  
}
void invia()
{
  String x=String(ROTAZ_XMAX);
  String y=String(ROTAZ_YMAX);
  certo=String(","+x+";"+y+";");
  Serial.print(certo.length());
  Serial.print(certo);
}

