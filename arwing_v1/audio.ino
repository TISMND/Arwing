void audio_setup() {
  pinMode(speakerPin, OUTPUT);

  //Boot up Sound
    for (int i = 150; i <= 400; i += 5) {  
    tone(speakerPin, i);
    delay(15);  
  }
  noTone(speakerPin);  
}

void tone_sat_nav_on(){
tone(speakerPin,800);
delay(200);
noTone(speakerPin);  
delay(100);  
tone(speakerPin,1200);
delay(200);
noTone(speakerPin); 
}

void tone_manual_con_on(){
tone(speakerPin,1200);
delay(200);
noTone(speakerPin);  
delay(100);  
tone(speakerPin,1000);
delay(200);
noTone(speakerPin); 
}

void tone_unknown_msg(){
tone(speakerPin,125);
delay(100);
noTone(speakerPin);  
delay(50);  
tone(speakerPin,125);
delay(100);
noTone(speakerPin); 
}

void tone_bt_connected(){
tone(speakerPin,1400);
delay(200);
noTone(speakerPin);  
delay(10);  
tone(speakerPin,1100);
delay(175);
noTone(speakerPin); 
delay(10);
tone(speakerPin,1700);
delay(250);
noTone(speakerPin); 
}

void tone_bt_disconnected(){
tone(speakerPin,1200);
delay(200);
noTone(speakerPin);  
delay(10);  
tone(speakerPin,1500);
delay(175);
noTone(speakerPin); 
delay(10);
tone(speakerPin,1000);
delay(250);
noTone(speakerPin);  
}