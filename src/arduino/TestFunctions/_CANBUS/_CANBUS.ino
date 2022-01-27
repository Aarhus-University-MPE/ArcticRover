#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
struct can_frame canTestMsg;
MCP2515 mcp2515(30);



void setup() {
  Serial.begin(115200);
  SPI.begin();
  mcp2515.reset();
  Serial.print("setBitrate: ");
  Serial.println(mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ));

  
  Serial.print("setNormalMode: ");
  Serial.println(mcp2515.setNormalMode());

  canTestMsg.can_id = 0x12;
  canTestMsg.can_dlc = 3;
  canTestMsg.data[0] = 0xE8;
  canTestMsg.data[1] = 0x03;
  canTestMsg.data[2] = 80;

  mcp2515.sendMessage(&canTestMsg);
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) { 
    // StreamData()
    ParseData();
  }
  else{
    
    mcp2515.sendMessage(&canTestMsg);
    delay(100);
  }
}

// Stream all data in raw HEX
void StreamData(){
  Serial.print(canMsg.can_id, HEX); // print ID
  Serial.print(" "); 
  Serial.print(canMsg.can_dlc, HEX); // print DLC
  Serial.print(" ");
  
  for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    Serial.print(canMsg.data[i],HEX);
    Serial.print(" ");
  }
  
  Serial.println();     
}

void ParseData(){
  char buffer[100];

  if(canMsg.can_id == 0x64){
    int control_value = (int)((canMsg.data[1] << 8) | canMsg.data[0]);
    int motor_state = (int)(canMsg.data[3] >> 6); 
    int rpm = (int)((canMsg.data[6] << 8) | canMsg.data[5]) / 10.0f;
    int temperature = (int)canMsg.data[7];
    
    Serial.print("Control value: ");
    Serial.print(control_value);
    Serial.print("\t Motor State: ");
    Serial.print(motor_state);
    Serial.print("\t rpm: ");
    Serial.print(rpm);
    Serial.print("\t Temperature: ");
    Serial.println(temperature);
  }
  
  if(canMsg.can_id == 0x65){
    // inv peak current
    // Motor power
  }
  
  // Warning
  if(canMsg.can_id == 0x66){
    bool warning[64];
    
    for (size_t i = 0; i < 8; i++)
    {
      for (size_t j = 0; j < 8; j++)
      {
        warning[i*8+j] = canMsg.data[i] >> j & 1;
      }
    }
    Serial.print("Warning: ");
    for (size_t i = 0; i < 64; i++)
    {
      if(warning[i] == 1){
        Serial.print(i);
        Serial.print("\t");
      }
    }

    // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    //   Serial.print(canMsg.data[i],HEX);
    //   Serial.print(" ");
    // }
    Serial.println();
  }
  
  // Error
  if(canMsg.can_id == 0x67){
    bool error[64];
    
    for (size_t i = 0; i < 8; i++)
    {
      for (size_t j = 0; j < 8; j++)
      {
        error[i*8+j] = canMsg.data[i] >> j & 1;
      }
    }
    Serial.print("Error: ");
    for (size_t i = 0; i < 64; i++)
    {
      if(error[i] == 1){
        Serial.print(i);
        Serial.print("\t");
      }
    }
    
    // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    //   Serial.print(canMsg.data[i],HEX);
    //   Serial.print(" ");
    // }

    Serial.println();
  }
}