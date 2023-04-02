// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <EEPROM.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//sprite code
int sprite = 0;
uint8_t egg[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
};
uint8_t pet1[] = {
  B00000,
  B00100,
  B00111,
  B00111,
  B11110,
  B11110,
  B01010,
  B01010,
};
uint8_t pet2[] = {
  B01010,
  B11111,
  B01110,
  B00100,
  B11111,
  B11011,
  B10001,
  B10001,
};

//button buffer variables - makes each button press register as a single press, not multiple
unsigned long first_press = 0;
unsigned long press_wait = 200;

//starting save menu variables
bool save_present;
String options[] = {"LOAD","NEW"};
int j = 0;
unsigned long wait_start = 0;

//naming variables
char first_letter = 'A';
char second_letter = 'A';
char third_letter = 'A';
bool naming_menu = true;
int junior = 0;
int arrows[] = {4,3,3};
int index;

uint8_t arrow[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
  B00000,
};
uint8_t arrow_inv[] = {
  B11011,
  B11011,
  B11011,
  B11011,
  B00000,
  B10001,
  B11011,
  B11111,
};

//age variables
unsigned long SecTime = 0;
unsigned long S_interval = 1000;
int minutes = 0;
int seconds = 0;
int total_age = 0;

//development stages variable
int dev_stage = 0;
int dev_change = 0;

//pet stats variables
unsigned long happy_timer;
int happy_interval = 7000;
int happiness = 2;
int max_happiness = 2;
unsigned long full_timer;
int full_interval = 11000;
int fullness = 3;
int max_fullness = 4;
bool initialise = true;

//menu variables
bool menu_active = false;
int choice_index = 0;
String choices[] = {"Save Pet","Delete Save","New Pet","Exit Menu"};

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  lcd.createChar(0,egg);
  lcd.createChar(1,pet1);
  lcd.createChar(2,pet2);
  lcd.createChar(3,arrow);
  lcd.createChar(4,arrow_inv);

  if (EEPROM.read(0) == 0){
    save_present = false;
  }else{
    save_present = true;
  }
}

void loop() {

  uint8_t buttons = lcd.readButtons();

  if (save_present == true){
    lcd.setCursor(0,0);
    lcd.print("Save present:");

    lcd.setCursor(5,1);
    lcd.print(options[j]);

    if (buttons & BUTTON_UP && (first_press + press_wait < millis())) {
        first_press = millis();
        lcd.clear();
        if (j == 0){
          j = 1;
        }else if (j == 1){
          j = 0;
        }
    }
    if (buttons & BUTTON_DOWN && (first_press + press_wait < millis())) {
        first_press = millis();
        lcd.clear();
        if (j == 0){
          j = 1;
        }else if (j == 1){
          j = 0;
        }
    }
    if (buttons & BUTTON_SELECT && (first_press + press_wait < millis())) {
        first_press = millis();
        lcd.clear();
        if (j == 0){
          dev_stage = EEPROM.read(1);
          sprite = dev_stage;
          happiness = EEPROM.read(2);
          fullness = EEPROM.read(3);
          minutes = EEPROM.read(4);
          seconds = EEPROM.read(5);
          total_age = seconds + (minutes * 60);
          first_letter = EEPROM.read(6);
          second_letter = EEPROM.read(7);
          third_letter = EEPROM.read(8);
          junior = EEPROM.read(9);

          naming_menu = false;
          save_present = false;
        }else if (j == 1){
          //jumps to the code below if the user has selected to load a new pet
          naming_menu = true;
          save_present = false;
        }
    }
    
  }else{
    if (naming_menu == true){
      lcd.setCursor(0,0);
      lcd.print("Name:");

      lcd.setCursor(7,0);
      lcd.print(first_letter);lcd.print(second_letter);lcd.print(third_letter);

      lcd.setCursor(7,1);
      lcd.write(arrows[0]);lcd.write(arrows[1]);lcd.write(arrows[2]);

      if (buttons & BUTTON_RIGHT && (first_press + 100 < millis())) {
        first_press = millis();
        for (int k=0;k<3;k++){
          if (arrows[k] == 4){
            index = k;
          }
        }
        arrows[index]=3;
        if (index == 2){
          index = 0;
        }else{
          index ++;
        }
        arrows[index]=4;
      }
      if (buttons & BUTTON_LEFT && (first_press + 100 < millis())) {
        first_press = millis();
        for (int k=0;k<3;k++){
          if (arrows[k] == 4){
            index = k;
          }
        }
        arrows[index]=3;
        if (index == 0){
          index = 2;
        }else{
          index --;
        }
        arrows[index]=4;
      }
      if (buttons & BUTTON_DOWN && (first_press + 100 < millis())) {
        first_press = millis();
        if (index==0){
          first_letter ++;
          if (first_letter == '['){
            first_letter = 'A';
          }
        }else if (index==1){
          second_letter ++; 
          if (second_letter == '['){
            second_letter = 'A';
          }   
        }else if (index==2){
          third_letter ++;
          if (third_letter == '['){
            third_letter = 'A';
          }
        }
        
      }
      if (buttons & BUTTON_UP && (first_press + 100 < millis())) {
        first_press = millis();
        if (index==0){
          first_letter --;
          if (first_letter == '@'){
            first_letter = 'Z';
          }
        }else if (index==1){
          second_letter --;
          if (second_letter == '@'){
            second_letter = 'Z';
          }          
        }else if (index==2){
          third_letter --;
          if (third_letter == '@'){
            third_letter = 'Z';
          }
        }
      }
      if (buttons & BUTTON_SELECT && (first_press + press_wait < millis())) {
        first_press = millis();
        lcd.clear();
        naming_menu = false;
      }

      
    }else{
      //sets the timers to start now, so they are not working during the starting menu above
      if (initialise == true){
        happy_timer = millis();
        full_timer = millis();
        //stops it being repeated again
        initialise = false;
      }
  
      
      //simple counter that displays the minutes and seconds the pet has been alive
      if (SecTime + S_interval < millis() && menu_active == false){
        total_age += 1;
        if (seconds >= 59){
          lcd.clear();
          seconds = 0;
          minutes += 1;
        }else{
          seconds += 1;
        }
        SecTime = millis();
      }
    
      //updates the development stage after the first 5 seconds - hatches the egg
      if (total_age == 5){
        if (dev_stage == 0){
          dev_stage = 1;
          dev_change = 1;
          //update pet sprite from egg to pet//
          sprite = 1;      
        }
    
      }
      
      //development stages code
      if (dev_stage != 0){
        if (dev_stage == 2 && dev_change != 2){
          happy_interval = 3500;
          max_happiness = 3;
          full_interval = 9000;
          max_fullness = 5;
          //update sprite to be older/ bigger pet//
          sprite = 2;
          dev_change = 2;
        }
        if (fullness == 0 && happiness != 0){
          happiness = 0;
        }else{
          if (happy_timer + happy_interval < millis() && happiness != 0 && menu_active == false){
            happiness -=1;
            happy_timer = millis();
          }
          if (full_timer + full_interval < millis() && fullness != 0 && menu_active == false){
            fullness -=1;
            full_timer = millis();
          }
        }
      }
    
    
      //button controls
    
      //Below button commands only apply when the menu isnt open, they stop working when it is
      if (buttons && dev_stage > 0){
    
        //UP grows the pet
        if (buttons & BUTTON_UP && (first_press + press_wait < millis()) && menu_active == false) {
          first_press = millis();
          if (total_age >= 35 && happiness > 0 && fullness >= 3){
            dev_stage = 2;
          }
        }
        
        //DOWN feeds the pet
        if (buttons & BUTTON_DOWN && (first_press + press_wait < millis()) && menu_active == false) {
          first_press = millis();
          if (fullness < max_fullness -1){
            fullness += 1;
          }else if (fullness == max_fullness -1){
            fullness += 1;
            //pet is over fed, so its not happy
            happiness = 0;
          }
        }
        
        //RIGHT plays with the pet
        if (buttons & BUTTON_RIGHT && (first_press + press_wait < millis()) && menu_active == false) {
          first_press = millis();
          if (happiness < max_happiness && fullness >=2){
            happiness += 1;
          }
        }
  
        //LEFT feeds the pet fizzy pop drinks - only once they are an 'adult' pet at dev stage 2
        if (buttons & BUTTON_LEFT && dev_stage == 2 && (first_press + press_wait < millis()) && menu_active == false) {
          first_press = millis();
          if (happiness < max_happiness){
            happiness += 2;
            if (happiness > max_happiness){
              happiness = max_happiness;
            }
          }
        }
      }
    
      
      //SELECT opens the menu
      if (buttons & BUTTON_SELECT && dev_stage > 0 && (first_press + press_wait < millis())) {
        first_press = millis();
        if (menu_active == false){
          menu_active = true;
          lcd.clear();
            
          lcd.setCursor(0,0);
          lcd.print("MENU:");
    
          lcd.setCursor(3,1);
          lcd.print(choice_index + 1);lcd.print(".");lcd.print(choices[choice_index]);
          
          
        }else{
          if (choice_index == 0){
            int save[] = {1,dev_stage,happiness,fullness,minutes,seconds,first_letter,second_letter,third_letter,junior};
            int i;
            for (i=0;i<10;i++){
              EEPROM.write(i,save[i]);
            }
            lcd.setCursor(6,0);
            lcd.print("Data saved");
            
          }else if (choice_index == 1){
            EEPROM.write(0,0);
            lcd.setCursor(6,0);
            lcd.print("Data wiped");
          //resets the main variables of the pet to their default, essentially starting over
          }else if (choice_index == 2){
            lcd.clear();
            minutes = 0;
            seconds = 0;
            total_age = 0;
            happiness = 2;
            fullness = 3;
            dev_stage = 0;
            sprite = dev_stage;
            junior = 0;
            naming_menu = true;
            menu_active = false;
            
          //simply closes the menu and returns the buttons to their standard functions
          }else if (choice_index == 3){
            lcd.clear();
            choice_index = 0;
            menu_active = false;
            //resets the timers so they dont instantly remove 1 from fullness and happiness when exiting the menu
            happy_timer = millis();
            full_timer = millis();
          }
        }
      }
      if (buttons & BUTTON_UP && dev_stage > 0 && (first_press + press_wait < millis())) {
          first_press = millis();
          if (choice_index == 0){
            choice_index = 3;
          }else{
            choice_index -= 1;
          }      
          lcd.clear();
              
          lcd.setCursor(0,0);
          lcd.print("MENU:");
        
          lcd.setCursor(3,1);
          lcd.print(choice_index + 1);lcd.print(".");lcd.print(choices[choice_index]);
      }
    
      if (buttons & BUTTON_DOWN && dev_stage > 0 && (first_press + press_wait < millis())) {
          first_press = millis();
          if (choice_index == 3){
            choice_index = 0;
          }else{
            choice_index += 1;
          }
          lcd.clear();
              
          lcd.setCursor(0,0);
          lcd.print("MENU:");
        
          lcd.setCursor(3,1);
          lcd.print(choice_index + 1);lcd.print(".");lcd.print(choices[choice_index]);
      }
          
    
    
      
      //displays all data to the lcd
      if (menu_active == false && naming_menu == false){
        //elderly pet code
        if (minutes == 9 && seconds == 59){
          lcd.clear();
          //pet lays an egg and goes to live in the wild - new pet is saved as egg
          dev_stage = 0;
          sprite = dev_stage;
          happiness = 2;
          fullness = 3;
          minutes = 0;
          seconds = 0;
          total_age = 0;
          junior = 1;

          int save[] = {1,dev_stage,happiness,fullness,minutes,seconds,first_letter,second_letter,third_letter,junior};
          int i;
          for (i=0;i<10;i++){
            EEPROM.write(i,save[i]);
          }

          initialise = true;
        }
        
        lcd.setCursor(11,0);
        lcd.print(minutes);lcd.print("m");
      
        lcd.setCursor(13,0);
        lcd.print(seconds);lcd.print("s");
      
        lcd.setCursor(10,1);
        lcd.print("D");lcd.print(dev_stage);lcd.print("H");lcd.print(happiness);lcd.print("F");lcd.print(fullness);

        lcd.setCursor(1,0);
        lcd.print(first_letter);lcd.print(second_letter);lcd.print(third_letter);

        //adds the Jr. suffix to the name if the pet had a parent
        if (junior == 1){
          lcd.setCursor(5,0);
          lcd.print("Jr.");
        }
  
        lcd.setCursor(2,1);
        lcd.write(sprite);
      }
    }
  }
}
