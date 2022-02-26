#include <AccelStepper.h>
enum enumtipo 
{
  AVVIO,
  INCALIBRAZIONE,
  FINECALIBRAZIONE,
  RICEVIMENTO,
  ESECUZIONE,
  FINE,
};



int butAngoloX=12;  //pulsante angolo X
int butAngoloY=13;  //pulsante angolo Y

int butEndX=2;      //pulsante fine corsa X
int butEndY=3;      //pulsante fine corsa Y

long stepYMax;
long stepXMax;

int millimetriYMax;
int millimetriXMax;

AccelStepper stepper1(4, 8, 10, 9, 11);// (tipo di motore, IN1,IN3,IN2,IN4)
AccelStepper stepper2(4, 4, 6, 5, 7);  // (tipo di motore, IN1,IN3,IN2,IN4)

int speed=-500; //velocista che si dara ai motori

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
  stepper1.setMaxSpeed(1000);  //velocita massima che il motore 1 puo raggiungere
  stepper2.setMaxSpeed(1000);  //velocita massima che il motore 2 puo raggiungere
  stepper1.setCurrentPosition(0);  //imposta la posizione attuale del motore come cordinata 0
  stepper2.setCurrentPosition(0);

  // pin per l'angolo
  pinMode(butAngoloX,INPUT);
  pinMode(butAngoloY,INPUT);

  //pin per determinare lunghezza X && Y
  pinMode(butEndY,INPUT);
  pinMode(butEndX,INPUT);

  
  

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
    calibraMotore();
    spostaOrigine();
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
    disegna();
    stato=RICEVIMENTO;
  }
    if(stato==FINE){
  }  
}
void spostaOrigine()
{
  bool arrivati=false;
  stepper1.setSpeed(speed);           
  stepper2.setSpeed(-speed); 
  delay(100);
  while(!arrivati)  //questi ciclo, serve per poter riportare i motori all angolo, quindi alla posizione (0;0)
  {
    if(stepper1.currentPosition() !=  0)
      stepper1.runSpeed();
    if(stepper2.currentPosition() !=  0)
      stepper2.runSpeed();   
    arrivati=stepper1.currentPosition() ==  0 && stepper2.currentPosition()==0;             
  }
  delay(100); 
}

void calibraMotore()
{
  stepper1.setSpeed(speed);           
  stepper2.setSpeed(-speed); 
  
  bool arrivati=false;


  while(!arrivati)  //finche il pulsante per gli angolo non è premuto
  {
    if(digitalRead(butAngoloX)==LOW) 
      stepper1.runSpeed();      //vai avanti
    if(digitalRead(butAngoloY)==LOW)
      stepper2.runSpeed();      //vai avanti
    arrivati=digitalRead(butAngoloY)==HIGH &&  digitalRead(butAngoloX)==HIGH;//quando entrami i pulsante sono premuti, cambia valore ad arrivati per uscire dal ciclo                
  }
  delay(100);     
  stepper1.setCurrentPosition(0);       //imposta la posizione attuale del motore come cordinata 0
  stepper2.setCurrentPosition(0);

  arrivati=false;

  stepper1.setSpeed(-speed);      //dai u lna velocita al motore
  stepper2.setSpeed(speed);        
  while(!arrivati)  //finche il pulsante per la lunghezza non è premuto
  {
    if(digitalRead(butEndX)==LOW)
      stepper1.runSpeed();      //vai avanti nel verso opposto del movimento di prima
    if(digitalRead(butEndY)==LOW)
      stepper2.runSpeed();      //vai avanti nel verso opposto del movimento di prima
    arrivati=digitalRead(butEndY)==HIGH &&  digitalRead(butEndX)==HIGH;//quando entrami i pulsante sono premuti, cambia valore ad arrivati per uscire dal ciclo        
  }
  delay(100);

  stepXMax=stepper1.currentPosition();  //salva quanti passi ha percorso il motore "e quindi anche quanto è lungo il lato"
  stepYMax=stepper2.currentPosition(); 
  millimetriXMax=stepXMax/2048 ;
  millimetriYMax=stepYMax/2048;                                  
  //Serial.println(ROTAZ_XMAX);
  //Serial.println(ROTAZ_YMAX);
  //fine calibrazione  
}
void disegna()
{
  
  stepper1.setSpeed(speed);
  stepper2.setSpeed(-speed);
  while(1)
  {
    if(stepper1.currentPosition()==DESTX && stepper2.currentPosition()==DESTY)
    {
      return;
    }
    if(stepper1.currentPosition()>DESTX)
    { 
      stepper1.runSpeed();
    }
    else if(stepper1.currentPosition()<DESTX)
    {
      stepper1.setSpeed(-speed);
      stepper1.runSpeed();
    }
    if(stepper2.currentPosition()>DESTY)
    {
      stepper2.runSpeed();
    }
    else if(stepper2.currentPosition()<DESTY)
    {
      stepper1.setSpeed(speed);
      stepper1.runSpeed();
    }
  }
  delay(100);
}
void invia()
{
  String x=String(ROTAZ_XMAX);
  String y=String(ROTAZ_YMAX);
  certo=String(","+x+";"+y+";");
  Serial.print(certo.length());
  Serial.print(certo);
}
