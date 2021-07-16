/*
  GeoRover Blackbox protocols

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

// Initialize system Blackbox
void InitBlackBox()
{
    if (SDReaderStatus())
    {
        File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
        if (file)
        {
            DEBUG_PRINTLN("Initializing Blackbox");
            blackBoxFile.println("GeoRover Blackbox");
            blackBoxFile.print("System Version:;");
            blackBoxFile.println(SystemVersion);
            blackBoxFile.println();
            blackBoxFile.close();

            SetStatus(MODULE_BLACKBOX, true);
        }
        else
            SetStatus(MODULE_BLACKBOX, false);
    }
    else
    {
        DEBUG_PRINTLN("SD connection error!");
        SetStatus(MODULE_BLACKBOX, false);
    }
}

bool BlackBoxStatus()
{
    if (SDReaderStatus())
    {
        if (!GetStatus(MODULE_BLACKBOX))
        {
            DEBUG_PRINTLN("Blackbox not initialized!");
            InitBlackBox();
        }
    }
    else
        SetStatus(MODULE_BLACKBOX, false);
    return GetStatus(MODULE_BLACKBOX);
}

void BlackBoxAppend(String blackBoxInput)
{
    if (BlackBoxStatus())
    {
        File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
        if (blackBoxFile)
        {
            DEBUG_PRINTLN("Writing to Blackbox");
            blackBoxFile.print(millis());
            blackBoxFile.print(";");
            blackBoxFile.println(blackBoxInput);
            blackBoxFile.close();
        }
        else
        {
            DEBUG_PRINTLN("Blackbox Error");

            SetStatus(MODULE_BLACKBOX, false);
        }
    }
}
