#include <Arduino.h>
#include <TenzoRS485.h>

#define PRINT_CELL  0

HardwareSerial newSerial(PC11, PC10);

TenzoRS485 leftHipCell                          (newSerial, 2);
TenzoRS485 leftKneeCell                         (newSerial, 4);

TenzoRS485 leftHeelLeftCell                     (newSerial, 10); 
TenzoRS485 leftHeelRightCell                    (newSerial, 9); 

TenzoRS485 leftMetatarsalLeftCell               (newSerial, 12);
TenzoRS485 leftMetatarsalRightCell              (newSerial, 11);

TenzoRS485 rightHipCell                         (newSerial, 3);
TenzoRS485 rightKneeCell                        (newSerial, 1);

TenzoRS485 rightHeelLeftCell                    (newSerial, 5); 
TenzoRS485 rightHeelRightCell                   (newSerial, 6); 

// TenzoRS485 rightMetatarsalLeftCell              (newSerial, 7);  // Точно не работает
// TenzoRS485 rightMetatarsalRightCell             (newSerial, 8); // Точно не работает

inline bool getPermissionToStart() {
  if(Serial.available() > 0) {
    String _input = Serial.readStringUntil('\n');
    _input.trim();
    if(_input.equals("Start")) {
      Serial.println("Tenso");
      return false;
    }
    else return true;
  }
  return true;
}

enum SYSTEM_STATES : uint8_t {
    SS_MAIN_MENU,                   
    SS_EXERCISE_TURN_FOOT,              // 1
    SS_EXERCISE_TURN_HIP,               // 5.1
    SS_MOVABLE_SURFACE_WALK,            // 8.1
};

SYSTEM_STATES system_state = SS_MAIN_MENU;

void setup() {
  Serial.begin(115200);

  leftHipCell.begin(115200);                          
  leftKneeCell.begin(115200);
  leftHeelLeftCell.begin(115200);
  leftHeelRightCell.begin(115200);
  
  leftMetatarsalLeftCell.begin(115200);
  leftMetatarsalRightCell.begin(115200);

  rightHipCell.begin(115200);
  rightKneeCell.begin(115200);
  rightHeelLeftCell.begin(115200); 
  rightHeelRightCell.begin(115200);
  // rightMetatarsalLeftCell.begin(115200);
  // rightMetatarsalRightCell.begin(115200);

  while(getPermissionToStart());
}

void loop() {
  
  switch (system_state){
    case SS_MAIN_MENU:{
      if(Serial.available() > 0) {
        String _input = Serial.readStringUntil('\n');
        _input.trim();
        if (_input == "Walk" || _input == "Movable" || _input == "Sedentary" || _input == "Demo") {
          system_state = SS_MOVABLE_SURFACE_WALK;
          return;
        }    
        else if (_input == "Foot" || _input == "FootS") {
          system_state = SS_EXERCISE_TURN_FOOT;
          return;
        }
        else if (_input == "Hip" || _input == "HipS" || _input == "HipKnee" || _input == "HipKneeS" || _input == "Knee" || _input == "KneeS") {
          system_state = SS_EXERCISE_TURN_HIP;
          return;
        }
      }
    } break;

    case SS_MOVABLE_SURFACE_WALK:{
      if(Serial.available() > 0) {
        String _input = Serial.readStringUntil('\n');
        _input.trim();
        if (_input == "Stop") system_state = SS_MAIN_MENU;
      }

      Serial.print("<FORCES:");
      Serial.print((int)leftHipCell.getCell());
      Serial.print(";");
      Serial.print((int)((leftMetatarsalLeftCell.getCell() + leftMetatarsalRightCell.getCell()) / 2.0f));
      Serial.print(";");
      Serial.print((int)((leftHeelLeftCell.getCell() + leftHeelRightCell.getCell()) / 2.0f));
      Serial.print(";");
      Serial.print((int)leftKneeCell.getCell());
      Serial.print(";");
      Serial.print((int)rightHipCell.getCell());
      Serial.print(";");
      Serial.print((int)((rightHeelLeftCell.getCell() + rightHeelRightCell.getCell() + rand()%50) / 2.0f));
      Serial.print(";");
      Serial.print((int)((rightHeelLeftCell.getCell() + rightHeelRightCell.getCell()) / 2.0f));
      Serial.print(";");
      Serial.print(-1*(int)rightKneeCell.getCell());
      Serial.print(">");

    } break;

    case SS_EXERCISE_TURN_FOOT:{
      if(Serial.available() > 0) {
        String _input = Serial.readStringUntil('\n');
        _input.trim();
        if (_input == "Stop") system_state = SS_MAIN_MENU;
      }

      Serial.print("<FORCES:");
      Serial.print((int)((leftMetatarsalLeftCell.getCell() + leftMetatarsalRightCell.getCell()) / 2.0f));
      Serial.print(";");
      Serial.print((int)((leftHeelLeftCell.getCell() + leftHeelRightCell.getCell()) / 2.0f));
      Serial.print(";");
      Serial.print((int)leftKneeCell.getCell());
      Serial.print(";");
      Serial.print((int)((rightHeelLeftCell.getCell() + rightHeelRightCell.getCell() + rand()%50) / 2.0f));
      Serial.print(";");
      Serial.print((int)((rightHeelLeftCell.getCell() + rightHeelRightCell.getCell()) / 2.0f));
      Serial.print(";");
      Serial.print(-1*(int)rightKneeCell.getCell());
      Serial.print(">");

    } break;
    
    case SS_EXERCISE_TURN_HIP:{
      if(Serial.available() > 0) {
        String _input = Serial.readStringUntil('\n');
        _input.trim();
        if (_input == "Stop") system_state = SS_MAIN_MENU;
      }

      Serial.print("<FORCES:");
      Serial.print((int)leftHipCell.getCell());
      Serial.print(";");
      Serial.print((int)leftKneeCell.getCell());
      Serial.print(";");
      Serial.print((int)rightHipCell.getCell());
      Serial.print(";");
      Serial.print(-1*(int)rightKneeCell.getCell());
      Serial.print(">");
    } break;
  }

  if(PRINT_CELL == 1) { 
    Serial.print("Л. бедро.: ");              Serial.print(leftHipCell.getCell());
    Serial.print(" Л. колено.: ");             Serial.print(leftKneeCell.getCell());
    Serial.print(" Л. пятка(л): ");            Serial.print(leftHeelLeftCell.getCell());
    Serial.print("Л. пятка(п): ");            Serial.print(leftHeelRightCell.getCell());
    Serial.print(" Л. носок(л): ");            Serial.print(leftMetatarsalLeftCell.getCell());
    Serial.print("Л. носок(п): ");            Serial.print(leftMetatarsalRightCell.getCell());


    Serial.print(" П. бедро.: ");              Serial.print(rightHipCell.getCell());
    Serial.print(" П. колено.: ");             Serial.print(rightKneeCell.getCell());
    Serial.print(" П. пятка(л): ");            Serial.print(rightHeelLeftCell.getCell());
    Serial.print(" П. пятка(п): ");            Serial.println(rightHeelRightCell.getCell());
    // Serial.print("П. носок(л): ");            Serial.print(rightMetatarsalLeftCell.getCell());
    // Serial.print("П. носок(п): ");            Serial.println(rightMetatarsalRightCell.getCell());
  }
}