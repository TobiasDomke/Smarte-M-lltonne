/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-send-email-smtp-server-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include "ESP32_MailClient.h"
#include <stdio.h>
#include <stdbool.h>

// Wlan Zugang
const char* ssid = "Imperator";
const char* password = "123456789";

//Email Details

#define emailSenderAccount "smartemulltonne@gmail.com"  //Smarter gehts leider nicht
#define emailSenderPassword "wzxh wzew zprz xfhn"       //ESP NEW
#define emailRecipient "tobidom01@gmail.com"
#define smtpServer "smtp.gmail.com"
#define smtpServerPort 465
#define emailSubject "Mülleimer voll"
#define RXp2 16
#define TXp2 17
#define value_ESP 34
// Die Datei zum Versenden per E-Mail enthält Konfiguration und Daten für den Versand.

SMTPData smtpData;

// Umkehrfunktion zum Abrufen des Versandstatus einer Mail
void sendCallback(SendStatus info);
void sending_mail() {

  // Angaben von Host und Port des SMTP-Servers, sowie Login und Passwort des Absender-E-Mail-Kontos
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // Die Verbindung erfolgt automatisch, wenn Port 587 verwendet wird, aber sie kann auch manuell mit der folgenden Funktion aktiviert werden:
  // smtpData.setSTARTTLS(true);

  // Geben Sie die E-Mail-Adresse des Senders an.
  smtpData.setSender("Smarte Mülltonne", emailSenderAccount);

  // Priorität der Mail (Hoch, Normal, Niedrig oder von 1 bis 5 (1 - höchste)).
  smtpData.setPriority("High");

  // Betreff
  smtpData.setSubject(emailSubject);

  // Email text in HTML
  //smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Achtung Mülleimer voll!!!</h1><p>Hi,</p><p>Meine Sensoren sagen, dass der Mülleimer voll ist.<br>Würdest du so lieb sein und diesen Fachgerecht enleeren?<br>Wenn du damit fertig bist, sende mir doch einfach eine <a href='mailto:smartemulltonne@gmail.com?subject=Antwort auf <Mülleimer voll>&body=Mülleimer wurde entleert'>Mail.</a><br>Gruß,<br>Deine Smarte Mülltonne</p>", true);
  // Mail Inhalt in Klartext möglich:
  //smtpData.setMessage("Mailtext", false);

  // Zusätzliche empfänger
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("Max.Mustermann@gmail.com");

  smtpData.setSendCallback(sendCallback);

  //  starten des Versandes der Mail, es kann eine Rückruffunktion zum Verfolgen des Status festgelegt werden.
  if (!MailClient.sendMail(smtpData))
    Serial.println("Fehler bei versenden, " + MailClient.smtpErrorReason());

  //Speicher leeren
  smtpData.empty();
};

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Serial.print("Verbinde");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println();
  Serial.println("Verbinde zu WIfi.");
  Serial.println();
  Serial.println("Vorbereitung zum absenden");
  Serial.println();
}

bool sended_Mail = false;

int sensorValue = 0;

void loop() {
  sensorValue = analogRead(value_ESP);
  Serial.println(sensorValue);
  Serial.println(sended_Mail);
  Serial.println(Serial2.readString());
  if ((Serial2.readString() == "Mülleimer voll") && !sended_Mail && sensorValue < 2500) {
    //Serial.println(Serial2.readString());
    sending_mail();
    sended_Mail = true;
  }
  if (sended_Mail && sensorValue > 2500) {
    sended_Mail = false;
  }
}

// Rückfunktion für Statusabfrage

void sendCallback(SendStatus msg) {
  // Statusausgabe des Vorgangs
  Serial.println(msg.info());

  // Abschluss ausgabe falls senden erfolgreich war
  if (msg.success()) {
    Serial.println("----------------");
  }
}
