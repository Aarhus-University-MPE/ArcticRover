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
        if (blackBoxFile)
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
    return GetStatus(MODULE_BLACKBOX);
}


void BlackBoxAppend()
{
    if (BlackBoxStatus())
    {
        File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
        if (blackBoxFile)
        {
            DEBUG_PRINTLN("Writing to Blackbox");
            blackBoxFile.print(millis());
            blackBoxFile.print(";");
            blackBoxFile.println();
            blackBoxFile.close();
        }
        else
        {
            DEBUG_PRINTLN("Blackbox Error");

            SetStatus(MODULE_BLACKBOX, false);
        }
    }
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

void BlackBoxAppend(byte blackBoxInput)
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
void BlackBoxAppend(char blackBoxInput)
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

void BlackBoxAppend(int blackBoxInput)
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

void BlackBoxAppend(bool blackBoxInput)
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

void BlackBoxAppend(long int blackBoxInput)
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

void BlackBoxAppend(long int blackBoxInput, int Type)
{
    if (BlackBoxStatus())
    {
        File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
        if (blackBoxFile)
        {
            DEBUG_PRINTLN("Writing to Blackbox");
            blackBoxFile.print(millis());
            blackBoxFile.print(";");
            blackBoxFile.print(blackBoxInput, Type);
            blackBoxFile.println();
            blackBoxFile.close();
        }
        else
        {
            DEBUG_PRINTLN("Blackbox Error");

            SetStatus(MODULE_BLACKBOX, false);
        }
    }
}
