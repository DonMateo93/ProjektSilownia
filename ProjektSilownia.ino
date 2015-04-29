#define trig 4
#define echo 5

int obciazenie = 0;
int iloscPowtorzen = 0;
long debouncing_time = 1000;
volatile unsigned long last_micros = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  Serial.begin(115200);
  attachInterrupt(1,ObslugaSygnaluOdCzujnika,HIGH);

}

void loop() {
  int buff;
  buff = (int)WeryfikujObciazenie();
  
  if(buff != -1)
    obciazenie = buff;

  //Serial.println(obciazenie);
  delay(1000);
}

double WeryfikujObciazenie()
{
  boolean ok = true;
  int czas, czasBuff, ileRazySprawdzac = 3, coIleMlSprawdzac = 1000;
  double buff, dist, akceptowalnyPoziomBledu = 1,distBuff;
  
  digitalWrite(trig,HIGH);
  delayMicroseconds(25);
  digitalWrite(trig, LOW);
  czas = pulseIn(echo,HIGH);
  dist = (double)(czas/2)/29.1;
  //Serial.println(dist);
  delay(2000);
  
  for(int i = 0; i < ileRazySprawdzac; i++)
  {
    digitalWrite(trig,HIGH);
    delayMicroseconds(50);
    digitalWrite(trig, LOW);
    czasBuff = pulseIn(echo,HIGH);
    distBuff = (double)(czasBuff/2)/29.1;
    //Serial.println(distBuff);
    if(abs(distBuff - dist) > akceptowalnyPoziomBledu)
    {
      ok = false;
    }
    
    if(i < ileRazySprawdzac - 1)
    {
      delay(coIleMlSprawdzac);
    }

  }
  
  if(ok)
  {
    return (dist + 0.5);
  }else
  {
    return -1;
  }

}

void ObslugaSygnaluOdCzujnika()
{  
    if((long)(micros() - last_micros) >= debouncing_time * 1000) 
    {
    poFiltracji();
    last_micros = micros();
    }
}

void poFiltracji()
{
    iloscPowtorzen++;
    Serial.println(iloscPowtorzen);
    Serial.println(obciazenie);
}
