/*
    USB communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

String command;
bool newCommand = false;


// Receive Commands
void RecvWithEndMarker() {
  while (Serial.available())
  {
    char c = Serial.read();
    //Serial.print(c);              // Uncomment to see command string flow
    if (c == '<')
    {
      newCommand = true;
    }
    else if (c == '>' && newCommand == true)
    {
      newCommand = false;
      parseCommand(command);
      command = "";
    }
    else if (newCommand == true)
    {
      command += c;
    }
  }
}

// Command handler
void parseCommand(String com)
{
  String part1;
  String part2;
  File file;

  part1 = com.substring(0, com.indexOf('.'));
  part2 = com.substring(com.indexOf('.') + 1);

//   if (part1.equalsIgnoreCase("files")) {
//     if (part2.equalsIgnoreCase("query")) {
//       if (SD.active) {
//         Serial.println("Files in system:");
//         file = SD.open("/");
//         file.rewindDirectory();
//         printFiles(file, 0);
//         file.rewindDirectory();
//         file.close();
//         Serial.println("End of storage");
//       }
//       else Serial.println("SD card connection Error!");
//     }
//     else {
//       Serial.println("NACK");
//     }
//   }

//   else if (part1.equalsIgnoreCase("size")) {
//     // part 2 contains file name
//     if (systemActive) {
//       Serial.println("Opening file: " + part2 + ".txt");
//       file = SD.open(part2 + ".txt");
//       if (file) {
//         Serial.println("File size: " + (String)file.size() + " bytes");
//         file.close();
//       }
//       else Serial.println("File not found!");
//     }
//     else Serial.println("SD card connection Error!");
//   }
//   else if (part1.equalsIgnoreCase("download")) {
//     if (systemActive) {
//       Serial.println("Downloading file: " + part2 + ".txt");
//       Serial.println();
//       file = SD.open(part2 + ".txt");
//       if (file) {
//         while (file.available()) {
//           Serial.write(file.read());
//         }
//         file.close();
//         Serial.println("End of File");
//       }
//       else Serial.println("File not found!");
//     }
//     else Serial.println("SD card connection Error!");
//   }
//   else if (part1.equalsIgnoreCase("delete")) {
//     if (systemActive) {
//       if (SD.exists(part2 + ".txt")) {
//         Serial.println("Deleting file: " + part2 + ".txt");
//         Serial.println();
//         SD.remove(part2 + ".txt");
//         Serial.println("File Removed");
//       }
//       else Serial.println("File not found");
//     }
//     else Serial.println("SD card connection Error!");
//   }

//   else if (part1.equalsIgnoreCase("create")) {
//     if (systemActive) {
//       if (SD.exists(part2 + ".txt")) {
//         Serial.println("File already exist");
//       }
//       else {
//         Serial.println("Creating file: " + part2 + ".txt");
//         file = SD.open(part2 + ".txt", FILE_WRITE);
//         file.close();
//       }
//     }
//     else Serial.println("SD card connection Error!");
//   }
//   else Serial.println("NACK");
}
