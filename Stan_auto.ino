/*
﻿#-------------------------------------------------------------------------------
# Name:        module_for_automation_blinds
# Purpose:
#
# Author:      Luka Kićinbaći
#
# Created:     19/06/2013
# Copyright:   (c) Luka Kicinbaci 2013
# Licence:     <your licence>
#-------------------------------------------------------------------------------
*/


#include <SPI.h> 
#include <Ethernet.h> 
#include <RCSwitch.h> 
#include <Wire.h>
#include "RTClib.h"

 RCSwitch mySwitch = RCSwitch();
 RTC_DS1307 RTC;
 int broj=0; // broj za buđenje
 int broj1=0; // broj za spavanje
 int broj2=1;
 int a=0, b, d=0, e=100; 
 int prvi,drugi,treci,cetvrti;
 
 byte mac[] = {0x50, 0xCC, 0xF8, 0x1B, 0xF7, 0x8D };
 IPAddress ip(192,168,0,12);
 byte gateway[] = { 192, 168, 1, 1 }; // internet access via router 
 byte subnet[] = { 255, 255, 255, 0 }; //subnet mask 
 
 char serverName[] = "192.168.0.12";
 String location = "/~demo_form_action.asp?quantity=1965 HTTP/1.0";
 EthernetClient client;
 
 EthernetServer server(80); //server port 
 
 String readString; 

 void setup(){  
 Ethernet.begin(mac, ip, gateway, subnet); 
 server.begin(); 
 Serial.begin(9600); 
    mySwitch.enableTransmit(8); 
    Wire.begin();
    RTC.begin();
 
      if (! RTC.isrunning()) {
          Serial.println("RTC is NOT running!");
      }
 Serial.println("server RCSwitch "); 
 client.println("GET http://192.168.0.12/demo_form_action.asp?quantity=XXXX HTTP/1.0");
 } 
 
 void loop(){ 
   
   DateTime now = RTC.now();
   /* Serial.print(now.year(), DEC);
   Serial.print('/');
   Serial.print(now.month(), DEC);
   Serial.print('/');
   Serial.print(now.day(), DEC);
   Serial.print(' ');
   Serial.print(now.hour(), DEC);
   Serial.print(':');
   Serial.print(now.minute(), DEC);
   Serial.print(':');
   Serial.print(now.second(), DEC);
   Serial.println();
   */
   
   broj2=((now.hour()*100)+(now.minute()));
   if(broj==broj2)//provjeri dali se pokalapaju
   {
      mySwitch.switchOff(1, 2);
      delay(10);
      mySwitch.switchOff(1, 4);
      delay(10);
      mySwitch.switchOn(1, 1);
      delay(10);
      mySwitch.switchOn(1, 3);
      Serial.println("Buđenje_pravo");
      Serial.print('/');
      Serial.println(broj);
      Serial.print('/');
      Serial.println(broj2);   
   }
   broj2=((now.hour()*100)+(now.minute()));
   if(broj1==broj2)//provjeri dali se pokalapaju
   {
      mySwitch.switchOff(1, 1);
      delay(10);
      mySwitch.switchOff(1, 3);
      delay(10);
      mySwitch.switchOn(1, 2);
      delay(10);
      mySwitch.switchOn(1, 4);
      Serial.println("Spavanje_pravo");
      Serial.print('/');
      Serial.println(broj1);
      Serial.print('/');
      Serial.println(broj2);   
   }
   
   broj2=((now.hour()*100)+(now.minute()));
   if(broj2==(broj+1) or broj2==(broj1+1))//provjeri dali se pokalapaju
        {
         mySwitch.switchOff(1, 1);
         delay(10);
         mySwitch.switchOff(1, 2);
         delay(10);
         mySwitch.switchOff(1, 3);
         delay(10);
         mySwitch.switchOff(1, 4);
         delay(10);
         Serial.println("pravo_gašenje");
         Serial.print('/');
         Serial.println(broj);
         Serial.print('/');
         Serial.println(broj1);
         Serial.print('/');
         Serial.println(broj2);
      }
   
   
 EthernetClient client = server.available(); 
 if (client) { 
  while (client.connected()) { 
    if (client.available()) { 
     char c = client.read(); 
      Serial.print(c);
      if (c==61) {  
        a=1;
        c = client.read();
        if (c==71 or c==103) //prvi znak koji je upisan je g ili G
         e=0; // znaci dok je e u nuli treba ga dizat buđenje
        if (c==68 or c==100) //prvi znak je d ili D
         e=1; // treba spustat spavanje 
         readString += c; 
      } 
        if (a==1){
         b=0; 
         a=0;
        }
         b++;
        if (b==2) {
         prvi=c-48;
          if (prvi==0)
            d=1; 
        }
        if (b==3) 
         drugi=c-48;
        if (b==4) 
         treci=c-48;
         if (b==5) 
         cetvrti=c-48;
         
         
     if (e==0 and d==0)
      broj=prvi*1000 + drugi*100 +treci*10 +cetvrti;
      
      if (e==0 and d==1)
      broj=drugi*100 +treci*10 +cetvrti;
      
      if (e==1 and d==0)
      broj1=prvi*1000 + drugi*100 +treci*10 +cetvrti;
      
      if (e==1 and d==1)
      broj1=drugi*100 +treci*10 +cetvrti;
      
     /* if (e==2){
      broj=100000;
      broj1=10000;
      }
      */
      
      Serial.print(broj);
      Serial.print('/');
      Serial.print(broj1);
      Serial.print('/');
      broj2=((now.hour()*100)+(now.minute()));
      Serial.print(broj2);
      
 //read char by char HTTP request 
  if (readString.length() < 100) { 
 
 //store characters to string 
 readString += c; 
 //Serial.print(c); 
 } 
 
 //if HTTP request has ended 
 if (c == '\n' ) { 
 
 /////////////// 
 Serial.println(readString); //print to serial monitor for debuging 
 client.println("HTTP/1.1 200 OK"); //send new page 
 client.println("Content-Type: text/html"); 
 client.println(); 
 client.println("<HTML>"); 
 client.println("<HEAD>"); 
 client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
 client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
 client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
 client.println("<TITLE>Stan Hecimovic</TITLE>"); 
 client.println("</HEAD>"); 
 client.println("<BODY>"); 
 client.println("<H1>Stan Hecimovic</H1>"); 
 client.println("<hr />"); 
 client.println("<br />"); 
 
 client.println("<H2>Lijeva roleta</H2>");
 client.println("<a href=\"/?lgore\"\">Gore</a>"); 
 client.println("<a href=\"/?lstop\"\">Stop</a>");
 client.println("<a href=\"/?ldole\"\">Dolje</a><br />"); 
 
 client.println("<H2>Desna roleta</H2>");
 client.println("<a href=\"/?dgore\"\">Gore</a>"); 
 client.println("<a href=\"/?dstop\"\">Stop</a>");
 client.println("<a href=\"/?ddole\"\">Dolje</a><br />"); 
 
 client.println("<H2>    </H2>");
 client.println("<form action=\"welcome_get.php\"  method=\"get\">"); 
 client.println("<input type=\"text\" name=\"quantity\"><BR>");
 client.println("<input type=\"submit\">");
 client.println("</form>");
 
 client.println("<a href=\"/?reset\"\">Reset</a>");
 
 client.println("<H3>U input box unesi 5 znamenki:</H3>");
 client.println("<H3>1. G dizanje, D spustanje</H3>");
 client.println("<H3>2. 3. Sati</H3>");
 client.println("<H3>4. 5. Minute</H3>");
 
 
 
 client.println("<H2>Dnevna soba svjetlo</H2>");
 client.println("<a href=\"/?dupali\"\">Svjetlo upali</a>");
 client.println("<a href=\"/?dugasi\"\">Svjetlo ugasi</a><br />"); 
 
 client.println("<H2>Kuhinja svjetlo</H2>");
 client.println("<a href=\"/?kupali\"\">Svjetlo upali</a>");
 client.println("<a href=\"/?kugasi\"\">Svjetlo ugasi</a><br />");
 
 client.println("<H3> </H3>");
 client.println("<a href=\"/?Senzori\"\">Senzori</a>");
 client.println("<H3>Temperatura</H3>");
 client.println("<H3>Tlak zraka</H3>");
 client.println("<H3>Vlaga zraka</H3>");
 
 
 
 
 client.println("</BODY>"); 
 client.println("</HTML>"); 
 
 delay(1); 
 //stopping client 
 client.stop(); 
 
 ///////////////////// control arduino pin 
 if(readString.indexOf("?lgore") >0)//checks for on 
 {  
 mySwitch.switchOff(1, 2);
 mySwitch.switchOff(1, 2);
 delay(10);
 mySwitch.switchOn(1,1 );
 mySwitch.switchOn(1,1 );
 Serial.println("Lijeva roleta se diže"); 
 }  
 if(readString.indexOf("?lstop") >0)//checks for off 
 { 
 mySwitch.switchOff(1, 1);
 mySwitch.switchOff(1, 1);
 delay(10);
 mySwitch.switchOff(1, 2);
 mySwitch.switchOff(1, 2);
  Serial.println("Zaustavlja lijevu roletu"); 
 }  
 if(readString.indexOf("?ldole") >0)//checks for off 
 { 
 mySwitch.switchOff(1, 1);
 mySwitch.switchOff(1, 1);
 delay(10);
 mySwitch.switchOn(1, 2);
 mySwitch.switchOn(1, 2);
 Serial.println("Lijeva roleta se spušta"); 
 } 
 
 
 if(readString.indexOf("?dgore") >0)//checks for on 
 {  
 mySwitch.switchOff(1, 4);
 mySwitch.switchOff(1, 4);
 delay(10);
 mySwitch.switchOn(1,3 );
 mySwitch.switchOn(1,3 );
 Serial.println("Desna roleta se diže"); 
 }  
 if(readString.indexOf("?dstop") >0)//checks for off 
 { 
 mySwitch.switchOff(1, 3);
 mySwitch.switchOff(1, 3);
 delay(10);
 mySwitch.switchOff(1, 4);
 mySwitch.switchOff(1, 4);
  Serial.println("Zaustavlja desnu roletu"); 
 }  
 if(readString.indexOf("?ddole") >0)//checks for off 
 { 
 mySwitch.switchOff(1, 3);
 mySwitch.switchOff(1, 3);
 delay(10);
 mySwitch.switchOn(1, 4);
 mySwitch.switchOn(1, 4);
 Serial.println("Desna roleta se spušta"); 
 } 
 
 if(readString.indexOf("?dupali") >0)//checks for off 
 { 
 mySwitch.switchOn(2, 1);
  Serial.println("Palim svjetlo u dnevnoj"); 
 }  
 if(readString.indexOf("?dugasi") >0)//checks for off 
 { 
 mySwitch.switchOff(2, 1);
 Serial.println("Gasim svjetlo u dnevnoj"); 
 } 
 
 if(readString.indexOf("?kupali") >0)//checks for off 
 { 
 mySwitch.switchOn(2, 2);
  Serial.println("Palim svjetlo u kuhinji"); 
 }  
 if(readString.indexOf("?kugasi") >0)//checks for off 
 { 
 mySwitch.switchOff(2, 2);
 Serial.println("Gasim svjetlo u kuhinji"); 
 } 
 if(readString.indexOf("?reset") >0)//checks for off 
 { 
 broj=2500;
 broj1=2501;
 Serial.print(broj);
 Serial.print('/');
      Serial.print(broj1);
      Serial.print('/');
      Serial.print(broj2);
  Serial.println("Resetirana su vremena dizanja i spustanja"); 
 }  
 
 broj2=(now.hour()*100)+(now.minute());
  

 readString=""; //clearing string for next read 
 
 } 
 } 
 } 
 } 
 } 
