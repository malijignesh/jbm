/* line 737
    ipadd(stip);
    IPAddress setip(k1, k2, k3, k4);
    delay(50);
    Serial.println("");
    Serial.print("STIP EEPROM READ :\t : ");
    Serial.println(setip);

    ipadd(gateway);
    IPAddress gateip(k1, k2, k3, k4);
    Serial.print("GATE EEPROM READ :\t : ");
    Serial.println(gateip);
    delay(50);

    ipadd(subnet);
    IPAddress subip(k1, k2, k3, k4);
    Serial.print("SUBIP EEPROM READ :\t : ");
    Serial.println(subip);
    delay(50);

    ipadd(dnsserver);
    IPAddress dnsip(k1, k2, k3, k4);
    Serial.print("DNSIP EEPROM READ :\t : ");
    Serial.println(dnsip);
  */


//================================================================================

/*  
 *   sss = "";
 
  coun = 0;  k1 = 0; k2 = 0; k3 = 0; k4 = 0;;
  for ( int i = 0; i <= sizeof(subnet1); i++)
  {

    if ('.' == subnet1[i])
    { coun += 1;
      i = i + 1;
      Serial.print("....");
      Serial.println(sss);


      if (coun == 1) {
        k1 = sss.toInt();
      }
      else if (coun == 2) {
        k2 = sss.toInt();
      }
      else if (coun == 3) {
        k3 = sss.toInt();
      }
      else if (coun == 4) {
        k4 = sss.toInt();

      }

      sss = "";
    }


    sss += subnet1[i];
//    Serial.println(sss);
//    Serial.println("");
    delay(500);

  }
//  Serial.println(sss);
  Serial.print(k1);
  Serial.print("\t");
  Serial.print(k2);
  Serial.print("\t");
  Serial.print(k3);
  Serial.print("\t");
  Serial.print(k4);
  Serial.println("\t");
  }
  */
//===========================================================================




















//=========================================================================================================
//=========================================================================================================
//=========================================================================================================

int firstClosingBracket = 0;
int secondOpeningBracket = 0;
int  secondClosingBracket = 0;
int thirdClosingBracket = 0;
int thirdOpeningBracket = 0;
int forthClosingBracket=0;
String str = "";
String  value = "";
String stringOne = "";
int first = 0 ;
int second = 0;
int Three = 0;
int i = 0;
int  jy1,jy2,jy3,jy4;

String ipadd(  String stringOne) {


  firstClosingBracket = 0;
  secondOpeningBracket = 0;
  secondClosingBracket = 0;
  thirdClosingBracket = 0;
  thirdOpeningBracket = 0;
  forthClosingBracket=0;
  str = "";
  value = "";
  first = 0 ;
  second = 0;
  Three = 0;
  i = 0;

  // Serial.print(str);
  jy1 = stringOne.toInt();
//  Serial.print(j1);
//  Serial.print("\t");

 k1=jy1;
  firstClosingBracket = stringOne.indexOf('.');

 secondClosingBracket = stringOne.indexOf('.', firstClosingBracket + 1);

  for (i = firstClosingBracket + 1; i <= secondClosingBracket - 1; i++) {
    str += stringOne[i];
 }
  jy2 = str.toInt();
//  Serial.print(j2);
//  Serial.print("\t");
  k2=jy2;
str="";
 thirdClosingBracket = stringOne.indexOf('.', secondClosingBracket + 1);

   for (i = secondClosingBracket + 1; i <= thirdClosingBracket - 1; i++) {
    str += stringOne[i];
 }
  jy3 = str.toInt();
//  Serial.print(j3);
//  Serial.print("\t");
k3=jy3;
str="";

 forthClosingBracket = stringOne.indexOf('.', thirdClosingBracket + 1);

   for (i = thirdClosingBracket + 1; i <= forthClosingBracket - 1; i++) {
    str += stringOne[i];
 }
   jy4 = str.toInt();
//  Serial.print(j4);
//  Serial.println("\t");
  k4=jy4;

}

//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
/*

string convertToString(char* a, int size) 
{ 
    int i; 
    string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
} */
