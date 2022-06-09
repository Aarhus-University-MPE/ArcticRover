#include <Arduino.h>
#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\_Mainframe.ino"
/*
  GeoRover mainframe - Manages primary systems as the Rover backbone.

  Mads Rosenhøj Jeppesen
  Aarhus University
  2022
*/

#include "_constants.h"
#include "_shared.h"
#include "_pinout.h"

// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
#line 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\_Mainframe.ino"
void setup();
#line 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\_Mainframe.ino"
void loop();
#line 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
bool InitializeCanBus();
#line 46 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
void TerminateCanBus();
#line 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
void CanBusPrint();
#line 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
bool CanBusProcess();
#line 109 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
bool CanBusStatus();
#line 113 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
bool ParseData();
#line 133 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
bool CanBusTest();
#line 177 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
void StreamData();
#line 191 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
void ParseCanMsg(bool motor);
#line 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
bool InitializeIridium();
#line 30 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
void TerminateIridium();
#line 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
bool IridiumStatus();
#line 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
void IridiumProcess();
#line 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
void IridiumReceive();
#line 91 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
void IridiumSend();
#line 95 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
bool IridiumTest();
#line 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
bool InitializeSBUS();
#line 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
void TerminateSBUS();
#line 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
bool SBusStatus();
#line 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
bool SBusTest();
#line 71 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
float getChannelFloat(int channel);
#line 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
int getChannel(int channel);
#line 102 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
float getChannelFloatFull(int channel);
#line 111 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
bool SBusProcess();
#line 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
void SBusController();
#line 172 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
void SBusPrint();
#line 181 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
void printChannels();
#line 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
void initializeDebugComm();
#line 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
void recvWithStartEndMarkers();
#line 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
void PerformCommand();
#line 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
bool DebugCommStatus();
#line 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
void parseCommand();
#line 111 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
void parseCommandFiles();
#line 143 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
void parseCommandStrategy();
#line 165 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
void parseCommandBackup();
#line 191 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
void parseCommandModule();
#line 272 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
void parseCommandRoute();
#line 311 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
void parseData();
#line 316 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
void CountDownPrint();
#line 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
bool InitializeAccel();
#line 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
void TerminateAccel();
#line 39 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
void ReadAccel();
#line 45 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
bool AccelStatus();
#line 51 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
bool AccelTest();
#line 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
bool InitializeGnss();
#line 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
bool GnssStatus();
#line 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
bool GnssUpdate();
#line 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
bool GnssTest();
#line 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
bool GnssTime(bool print);
#line 105 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
void TerminateGnss();
#line 110 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
long GnssGetLat();
#line 115 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
long GnssGetLong();
#line 120 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
long GnssGetHeading();
#line 125 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
void QueryGnss();
#line 154 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
bool GnssSignal();
#line 167 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
double DistanceBetween(double lat1, double long1, double lat2, double long2);
#line 190 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
double CourseTo(double lat1, double long1, double lat2, double long2);
#line 205 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
double DistanceBetweenLong(long lat1, long long1, long lat2, long long2);
#line 210 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
double CourseToLong(long lat1, long long1, long lat2, long long2);
#line 14 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
int MeanThermTemp();
#line 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
int ThermTemp(int thermistor);
#line 46 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
bool HeatingStart();
#line 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
bool HeatingStop();
#line 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
bool HeatingStatus();
#line 66 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
bool TemperatureStatus();
#line 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
bool TemperatureTest();
#line 116 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
void HeatingProcess();
#line 127 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
void HeatingCheck();
#line 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
void StartStrategyAutonomous();
#line 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
void RunStrategyAutonomous();
#line 36 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
void FinishStrategyAutonomous();
#line 51 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
void SelectFunctionAutonomous();
#line 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
void StartStrategyEmergency();
#line 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
void RunStrategyEmergency();
#line 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
void FinishStrategyEmergency();
#line 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
void SelectFunctionEmergency();
#line 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
bool EmergencyStopStatus();
#line 7 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
void StartStrategyExample();
#line 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
void RunStrategyExample();
#line 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
void FinishStrategyExample();
#line 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
void SelectFunctionExample();
#line 7 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
void StartStrategyIdle();
#line 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
void RunStrategyIdle();
#line 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
void FinishStrategyIdle();
#line 36 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
void SelectFunctionIdle();
#line 7 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
void StartStrategyModeLibrary();
#line 26 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
void RunStrategyModeLibrary();
#line 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
void FinishStrategyModeLibrary();
#line 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
void SelectFunctionModeLibrary();
#line 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
void ModeFunctionModeLibrary();
#line 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
void StartStrategyRemote();
#line 25 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
void RunStrategyRemote();
#line 71 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
void FinishStrategyRemote();
#line 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
void SelectFunctionRemote();
#line 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
void RemoteHalt();
#line 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
void StartStrategySystemTest();
#line 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
void RunStrategySystemTest();
#line 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
void FinishStrategySystemTest();
#line 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
void SelectFunctionSystemTest();
#line 13 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
void AutonomousProcess();
#line 26 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
bool AutonomyPreRun();
#line 50 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
bool AutonomyPowerCycle();
#line 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
bool AutonomyPreCheck();
#line 91 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
void AutonomyToggle();
#line 98 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
void AutonomyReset();
#line 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
void AutonomyStandby();
#line 116 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
bool StandbyRestart();
#line 12 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
bool InitBlackBox();
#line 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxPrint();
#line 48 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxClear();
#line 58 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
bool BlackBoxStatus();
#line 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppend(String blackBoxInput);
#line 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppendln();
#line 86 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppendln(String blackBoxInput);
#line 91 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppend(int blackBoxInput);
#line 95 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppendln(int blackBoxInput);
#line 100 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppend(long int blackBoxInput, int type);
#line 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppendln(long int blackBoxInput, int type);
#line 124 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppendln(float blackBoxInput);
#line 129 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppend(float blackBoxInput);
#line 133 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppend(long int blackBoxInput);
#line 137 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppendln(long int blackBoxInput);
#line 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppendln(unsigned long blackBoxInput);
#line 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
void BlackBoxAppend(unsigned long blackBoxInput);
#line 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
bool SystemEnable(int module);
#line 97 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
void SystemEnablePrimary();
#line 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
bool SystemSignalEnable(byte module);
#line 117 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
bool SystemEnableMode();
#line 155 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
bool SystemEnableStatus(bool status);
#line 165 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
void SystemDisable(int module);
#line 259 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
void SystemDisable();
#line 274 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
bool SystemCheckMode(int mode);
#line 315 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
bool SystemTest();
#line 420 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
bool SystemTestModule(byte module);
#line 522 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
void SystemCheck();
#line 536 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
bool SystemCheckModule(byte module);
#line 17 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
void HeartBeat();
#line 25 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
void HeartBeatOut();
#line 36 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
bool HeartBeatStatus();
#line 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
void HeartBeatTimeout();
#line 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
void HeartBeatIn();
#line 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
void HeartBeatInInterrupt();
#line 98 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
void ResetBackupCPU();
#line 104 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
void HeartbeatBlackBox();
#line 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
bool InitializeMotors();
#line 25 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
void TerminateMotors();
#line 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
void MotorUpdate(float dir, float speed);
#line 46 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
void MotorUpdateTorque(float dir, float speed);
#line 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
void TorqueCalculation(float dir, float speed, float &torqueLeft, float &torqueRight);
#line 78 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
void SpeedCalculation(float dir, float speed, float &velocityLeft, float &velocityRight);
#line 111 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
bool MotorCycle();
#line 135 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
bool MotorState();
#line 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
bool MotorStatus();
#line 148 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
bool MotorCanStatus();
#line 152 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
bool MotorStatusLeft();
#line 157 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
bool MotorStatusRight();
#line 162 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
float SteerFactor(float dir);
#line 178 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
bool MotorTest();
#line 216 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
bool MotorTestRamp();
#line 303 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
bool MotorTestSteer();
#line 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
bool Navigate();
#line 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
void NavigationMotorUpdate();
#line 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
void AutonomousTopSpeedUpdate();
#line 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
void NavigationPreCheckReset();
#line 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
void ResetNavigation();
#line 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
bool NavigationPreCheck();
#line 104 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
void PreCheckStatus(bool status);
#line 115 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
bool NavigationRunCheck();
#line 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool SDRoute();
#line 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
void ClearRoute();
#line 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool LoadRoute();
#line 58 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool RouteTest();
#line 88 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool CompareEepromSdRoute();
#line 129 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool RouteSDValid();
#line 169 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool RouteCheck();
#line 173 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool PathingProcess();
#line 184 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
void WaypointUpdate();
#line 191 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
void BearingUpdate();
#line 215 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
float BearingDirection();
#line 220 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool WaypointWithinRange();
#line 226 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
void IncrementWaypoint();
#line 237 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
void UpdateWaypoint();
#line 243 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool RouteSD(long &lat, long &lon, int index);
#line 277 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool RouteEe(long &lat, long &lon, int index);
#line 294 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool FlashRouteEeprom();
#line 323 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool PopulateLatLon(String waypoint, long &lat, long &lon);
#line 355 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool PopulateLatLon(String waypoint);
#line 359 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
void PopulateWaypointData(String waypoint);
#line 386 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
void ParseWaypoint(String waypoint, int index);
#line 404 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
void ResetRouteStatus();
#line 410 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
void EEPROMWriteLatLon(int index);
#line 416 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
void EEPROMReadLatLon(int index);
#line 422 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool CoordinateValidityFormat(long lat, long lon);
#line 450 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool CoordinateValidityRange(long lat1, long lon1, long lat2, long lon2);
#line 2 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
void InitAllPins();
#line 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
void InitButtons();
#line 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
void EstopButtonInterruptHandler();
#line 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
void ModeButtonInterruptHandler();
#line 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
bool VoltageCheck();
#line 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
bool BatteryStatus();
#line 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
bool BatteryStatus(bool print);
#line 40 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
int BatteryLevel();
#line 52 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
float BatteryVoltage();
#line 63 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
bool PowerCycle();
#line 86 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
void ResetPowerCycle();
#line 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
bool InitializeSDReader();
#line 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
void TerminateSDReader();
#line 26 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
bool SDReaderStatus();
#line 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
void SDQuery();
#line 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
void printFiles(File dir);
#line 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
void SDSize(char fileName[]);
#line 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
void SDDownload(char fileName[]);
#line 100 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
void SDDelete(char fileName[]);
#line 115 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
void SDCreate(char fileName[]);
#line 7 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void InitStatusLed();
#line 12 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
bool LedStatus();
#line 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedSet(byte color);
#line 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedSetSignal(LED_SIGNAL signal);
#line 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedSetSignal();
#line 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
bool LedBlink(byte color, unsigned int onDuration, unsigned int offDuration);
#line 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedBlink(byte colorOne, byte colorTwo, unsigned int onDuration, unsigned int offDuration);
#line 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void ResetLed();
#line 99 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedBlinkHalt(byte color, unsigned int duration);
#line 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedBlinkHalt(byte color, unsigned int duration, unsigned int afterHalt);
#line 114 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedBlinkSingleShort(byte color);
#line 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedBlinkDoubleShort(byte color);
#line 125 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedBlinkDoubleShort(byte color1, byte color2);
#line 131 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedBlinkTripleShort(byte color);
#line 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedBlinkTripleShort(byte color1, byte color2, byte color3);
#line 144 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void StrategyStartLed();
#line 170 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void StrategyRunLed(byte mode);
#line 193 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void StrategyRunLed();
#line 198 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void StatusRunLed(LED_SIGNAL signal);
#line 218 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void StatusHaltLed(LED_SIGNAL signal);
#line 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void StrategyFinishLed(int strategy);
#line 253 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void ModuleStartLed(int module);
#line 256 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void ModuleStopLed(int module);
#line 259 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
void LedTest();
#line 2 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
void InitStrategyMethods();
#line 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
void InitMode();
#line 48 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
void ModeUpdater();
#line 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
void AttachSelectButton();
#line 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
void AttachModeButton();
#line 71 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
void DetachModeButton();
#line 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
void DetachSelectButton();
#line 80 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
boolean SetMode(byte newMode);
#line 95 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
void IncrementModeCycle();
#line 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\_Mainframe.ino"
void setup() {
  // System initialization
  InitAllPins();
  InitButtons();
  InitStatusLed();
  
  // Debug
  DBG_ONLY(initializeDebugComm());
  
  SetStatus(false);

  SystemEnablePrimary();

  // Strategy initialization
  InitStrategyMethods();
  InitMode();
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {
  ModeUpdater();

  strategyMethods[1][mode]();

  HeartBeat();

  // IridiumProcess();

  DBG_ONLY(recvWithStartEndMarkers());
  DBG_ONLY(PerformCommand());
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
/*
  GeoRover CanBUS communication protocols for openCAN communication (Motors)
  https://github.com/autowp/arduino-mcp2515

  Mads Rosenhøj Jeppesen
  Aarhus University
  2022
*/

#include <mcp2515.h>

struct can_frame canMsg;
struct can_frame canTestMsg;

long CanBusTxLast;

int canTestState        = 0;
long millisCanTestStart = 0;
long millisLastCanPrint = 0;

MCP2515 mcp2515(PO_SPISS_CANBUS);

bool InitializeCanBus() {
  int err;

  digitalWrite(PO_MOTOR_EN, true);
  delay(5);

  SPI.begin();  // Begins SPI communication
  mcp2515.reset();

  err = mcp2515.setBitrate(CANBBUS_SPEED, MCP_8MHZ);
  if (err != MCP2515::ERROR_OK) return false;

  err = mcp2515.setNormalMode();
  if (err != MCP2515::ERROR_OK) return false;

  motorRight.ResetCanStatus();
  motorLeft.SetCanTxStatus();

  bool status = true;

  return status;
}

void TerminateCanBus() {
  SPI.end();
  digitalWrite(PO_MOTOR_EN, false);
}

/*
  TX Period:
    MotorL TX -> RX
    ->
    MotorR TX -> RX
    ->
    Repeat
*/

void CanBusPrint() {
  if (millis() - millisLastCanPrint < 1000) {
    return;
  }
  millisLastCanPrint = millis();

  bool motorLeftStatus  = motorLeft.PrintStatus();
  bool motorRightStatus = motorRight.PrintStatus();

  // status = motorLeftStatus || motorRightStatus;

  // printChannels();
  Serial.println(F("-------------------------------"));
}

// Process CANbus commands, returns false if MCP2515 throws error
bool CanBusProcess() {
  if (millis() - CanBusTxLast < CANBUS_TX_PERIOD) {
    return true;
  }

  int err;
  if (motorLeft.GetCanTxStatus()) {
    err = mcp2515.sendMessage(motorLeft.GetCanMsg());
    if (err != MCP2515::ERROR_OK) {
      DEBUG_PRINT(F("Can Error Left: "));
      DEBUG_PRINTLN(err);
      return false;
    }
  } else if (motorRight.GetCanTxStatus()) {
    err = mcp2515.sendMessage(motorRight.GetCanMsg());
    if (err != MCP2515::ERROR_OK) {
      DEBUG_PRINT(F("Can Error Right: "));
      DEBUG_PRINTLN(err);
      return false;
    }
  }

  if (motorLeft.GetCanRxStatus() || motorRight.GetCanRxStatus()) {
    if (motorLeft.CheckCanRxTimeout() == GemMotor::CAN_ERROR_TIMEOUT) motorRight.SetCanTxStatus();
    if (motorRight.CheckCanRxTimeout() == GemMotor::CAN_ERROR_TIMEOUT) motorLeft.SetCanTxStatus();

    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
      ParseData();
    }
  }
  return true;
}

bool CanBusStatus() {
  return GetStatus(MODULE_CANBUS);
}

bool ParseData() {
  char buffer[100];
  bool status = false;

  // Check ID against left motor
  if (canMsg.can_id == CANBUS_RX_MOTOR_LEFT || canMsg.can_id == CANBUS_RX_MOTOR_LEFT + 1 || canMsg.can_id == CANBUS_RX_MOTOR_LEFT + 2 || canMsg.can_id == CANBUS_RX_MOTOR_LEFT + 3) {
    status = motorLeft.ParseCanMsg(canMsg);
    motorRight.SetCanTxStatus();  // Indicate motor Right to send next msg
  }

  // Check ID against right motor
  else if (canMsg.can_id == CANBUS_RX_MOTOR_RIGHT || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 1 || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 2 || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 3) {
    status = motorRight.ParseCanMsg(canMsg);
    motorLeft.SetCanTxStatus();  // Indicate motor Left to send next msg
  }

  CanBusTxLast = millis();
  return status;
}

bool CanBusTest() {
  bool testDone = false;

  switch (canTestState) {
    case 0:
      DEBUG_PRINT(F("GNSS feed starting for: "));
      DEBUG_PRINT(SYS_TEST_DURATION_LONG);
      DEBUG_PRINTLN(F(" ms"));
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("------- CAN Read ----------"));
      DEBUG_PRINTLN(F("ID  DLC   DATA"));
      canTestMsg.can_id  = 0x12;
      canTestMsg.can_dlc = 3;
      canTestMsg.data[0] = 0;
      canTestMsg.data[1] = 0;
      canTestMsg.data[2] = 0;
      millisCanTestStart = millis();
      canTestState++;
      break;
    case 1:
      if (millis() - millisLastCanPrint > CANBUS_TX_PERIOD) {
        millisLastCanPrint = millis();
        if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
          StreamData();
          ParseData();
          DEBUG_PRINTLN();
        }
        mcp2515.sendMessage(&canTestMsg);
      }

      if (millis() - millisCanTestStart > SYS_TEST_DURATION_LONG) canTestState++;
      break;
    case 2:
      canTestState = 0;
      testDone     = true;
      SetStatus(MODULE_CANBUS, CanBusStatus());  //<---- Update to reflect Motor CAN Errors
    default:
      break;
  }

  return testDone;
}

// Stream all data in raw HEX
void StreamData() {
  Serial.print(canMsg.can_id, HEX);  // print ID
  Serial.print(F(" "));
  Serial.print(canMsg.can_dlc, HEX);  // print DLC
  Serial.print(F(" "));

  for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data
    Serial.print(canMsg.data[i], HEX);
    Serial.print(F(" "));
  }

  Serial.println();
}

void ParseCanMsg(bool motor) {
  if (canMsg.can_id == 0x64) {
    int control_value = (int)((canMsg.data[1] << 8) | canMsg.data[0]);
    int motor_state   = (int)(canMsg.data[3] >> 6);
    int rpm           = (int)((canMsg.data[6] << 8) | canMsg.data[5]) / 10.0f;
    int temperature   = (int)canMsg.data[7];

    Serial.print(F("Control value: "));
    Serial.print(control_value);
    Serial.print(F("\t Motor State: "));
    Serial.print(motor_state);
    Serial.print(F("\t rpm: "));
    Serial.print(rpm);
    Serial.print(F("\t Temperature: "));
    Serial.println(temperature);
  }

  if (canMsg.can_id == 0x65) {
    // inv peak current
    // Motor power
  }

  // Warning
  if (canMsg.can_id == 0x66) {
    bool warning[64];

    for (size_t i = 0; i < 8; i++) {
      for (size_t j = 0; j < 8; j++) {
        warning[i * 8 + j] = canMsg.data[i] >> j & 1;
      }
    }
    Serial.print(F("Warning: "));
    for (size_t i = 0; i < 64; i++) {
      if (warning[i] == 1) {
        Serial.print(i);
        Serial.print(F("\t"));
      }
    }

    // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    //   Serial.print(canMsg.data[i],HEX);
    //   Serial.print(" ");
    // }
    Serial.println();
  }

  // Error
  if (canMsg.can_id == 0x67) {
    bool error[64];

    for (size_t i = 0; i < 8; i++) {
      for (size_t j = 0; j < 8; j++) {
        error[i * 8 + j] = canMsg.data[i] >> j & 1;
      }
    }
    Serial.print(F("Error: "));
    for (size_t i = 0; i < 64; i++) {
      if (error[i] == 1) {
        Serial.print(i);
        Serial.print(F("\t"));
      }
    }

    // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    //   Serial.print(canMsg.data[i],HEX);
    //   Serial.print(" ");
    // }

    Serial.println();
  }
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
/*
  GeoRover Iridium communication protocols for long range communication
  using: https://github.com/sparkfun/SparkFun_IridiumSBD_I2C_Arduino_Library

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#include <IridiumSBD.h>
#define DIAGNOSTICS false  // Change this to see diagnostics

IridiumSBD modem(COM_SERIAL_IRID);

uint8_t buffer[200];
int signalQuality = -1;
bool iridiumTxStatus;
unsigned long millisLastIridiumProcess;

bool InitializeIridium() {
  bool status = true;
  COM_SERIAL_IRID.begin(IRID_BAUDRATE);

  // modem.setPowerProfile(IridiumSBD::USB_POWER_PROFILE);

  //bool status = (modem.begin() == ISBD_SUCCESS);
  return status;
}

void TerminateIridium() {
  COM_SERIAL_IRID.end();
}

bool IridiumStatus() {
  return modem.isConnected();
}

void IridiumProcess() {
  if (!GetStatus(MODULE_IRIDIUM)) {
    return;
  }

  if (signalQuality < 1) {
    return;
  }

  if (!(millis() - millisLastIridiumProcess > IRID_PROCESS_PERIOD)) {
    return;
  }

  millisLastIridiumProcess = millis();
  IridiumReceive();
  IridiumSend();
}

// Check modem for waiting messages
void IridiumReceive() {
  if (!modem.getWaitingMessageCount() > 0) {
    return;
  }

  int err;
  size_t bufferSize = sizeof(buffer);

  err = modem.sendReceiveSBDText(NULL, buffer, bufferSize);
  if (err != ISBD_SUCCESS) {
    DEBUG_PRINT(F("Receive msg failed, error: "));
    DEBUG_PRINTLN(err);
  } else {
    for (int i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i], HEX);
      if (isprint(buffer[i])) {
        Serial.print(F("("));
        Serial.write(buffer[i]);
        Serial.print(F(")"));
      }
      Serial.print(F(" "));
    }
    Serial.println();
  }
  // Clear the Mobile Originated message buffer to avoid re-sending the message during subsequent loops
  Serial.println(F("Clearing the MO buffer."));
  err = modem.clearBuffers(ISBD_CLEAR_MO);  // Clear MO buffer
  if (err != ISBD_SUCCESS) {
    Serial.print(F("clearBuffers failed, error: "));
    Serial.println(err);
  }
}

// Send iridium message awaiting to be sent
void IridiumSend() {
}

// Full Iridium Test
bool IridiumTest() {
  bool status = true;

  int err = modem.getSignalQuality(signalQuality);
  if (err != ISBD_SUCCESS) {
    status = false;
  }

  DEBUG_PRINT(F("Iridium: "));
  if (status) {
    DEBUG_PRINT(F("Signal Quality: "));
    DEBUG_PRINTLN(signalQuality);
  } else {
    DEBUG_PRINTLN(F("ERROR"));
  }

  // Example: Print the firmware revision
  char version[12];
  err = modem.getFirmwareVersion(version, sizeof(version));
  if (err != ISBD_SUCCESS) {
    DEBUG_PRINT(F("Firmware Version failed: error "));
    DEBUG_PRINTLN(err);
    status = false;
  }
  DEBUG_PRINT(F("Firmware Version is "));
  DEBUG_PRINT(version);
  DEBUG_PRINTLN(F("."));

  // Example: Print the IMEI
  char IMEI[16];
  err = modem.getIMEI(IMEI, sizeof(IMEI));
  if (err != ISBD_SUCCESS) {
    DEBUG_PRINT(F("getIMEI failed: error "));
    DEBUG_PRINTLN(err);
    status = false;
  }
  DEBUG_PRINT(F("IMEI is "));
  DEBUG_PRINT(IMEI);
  DEBUG_PRINTLN(F("."));

  // Example: Test the signal quality.
  // This returns a number between 0 and 5.
  // 2 or better is preferred.
  err = modem.getSignalQuality(signalQuality);
  if (err != ISBD_SUCCESS) {
    DEBUG_PRINT(F("SignalQuality failed: error "));
    DEBUG_PRINTLN(err);
    status = false;
  }

  DEBUG_PRINT(F("On a scale of 0 to 5, signal quality is currently "));
  DEBUG_PRINT(signalQuality);
  DEBUG_PRINTLN(F("."));

  DEBUG_PRINTLINE();

  return status;
}

#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c) {
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c) {
  Serial.write(c);
}
#endif

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
/*
  GeoRover SBUS communication protocols for RF communication
  using: https://github.com/zendes/SBUS

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#include <SBUS.h>

SBUS sbus(COM_SERIAL_RF);
bool sBusStatus;
int sbusTestState;
long millisSbusTestStart;
long millisLastSbusPrint;
long millisLastSBusUpdate;

// Initialize RF Communication
bool InitializeSBUS() {
  digitalWrite(PO_POWER_RF, HIGH);
  sBusStatus = true;

  sbus.begin(false);

  return COM_SERIAL_RF;
}

void TerminateSBUS() {
  COM_SERIAL_RF.end();
}

bool SBusStatus() {
  return GetStatus(MODULE_RF);
}

bool SBusTest() {
  bool testDone = false;

  switch (sbusTestState) {
    case 0:
      DEBUG_PRINT(F("SBUS feed starting for: "));
      DEBUG_PRINT(SYS_TEST_DURATION);
      DEBUG_PRINTLN(F(" ms"));
      millisSbusTestStart = millis();
      sbusTestState++;
      break;
    case 1:
      sbus.process();

      if (millis() - millisLastSbusPrint > SYS_PRINT_PERIOD_SHORT) {
        millisLastSbusPrint = millis();
        printChannels();
      }

      if (millis() - millisSbusTestStart > SYS_TEST_DURATION) sbusTestState++;
      break;
    case 2:
      sbusTestState = 0;
      testDone      = true;
      SetStatus(MODULE_RF, sbus.getGoodFrames() > 0);
      DEBUG_PRINTLINE();
    default:
      break;
  }

  return testDone;
}

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float getChannelFloat(int channel) {
  int value = getChannel(channel);

  float valueFloat = value / 128.0 - 1;

  return valueFloat;
}

const int minChannel = -83;
const int maxChannel = 83;
// Scale the S.BUS channel values into the range [0, 256]
int getChannel(int channel) {
  int value = sbus.getNormalizedChannel(channel);

  // if (value < minChannel) {
  //   minChannel = value;
  // }
  // if (value > maxChannel) {
  //   maxChannel = value;
  // }

  float result = value;

  result -= minChannel;
  result /= (maxChannel - minChannel);
  result *= 256;

  return (int)result;
}

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float getChannelFloatFull(int channel) {
  int value = getChannel(channel);

  float valueFloat = value / 256.0;

  return valueFloat;
}

// Read RF signal and update motors accordingly
bool SBusProcess() {
  sbus.process();
  
  if (millis() - millisLastSBusUpdate < REMOTE_PROCESS_DT) {
    return sBusStatus;
  }
  millisLastSBusUpdate = millis();

  if (getChannel(6) < REMOTE_CHANNEL_HIGH) {  // Enable (SF)
    MotorUpdate(0, 0);
    return sBusStatus;
  }

  SBusController();

  // First timeout error returns true (catches large first read getLastTime)
  if (millis() - (unsigned long)sbus.getLastTime() < SBUS_TIMEOUT && sBusStatus) {
    sBusStatus = false;
    return true;
  } else {
    sBusStatus = true;
  }

  return sBusStatus;
}

// Reads controller input and navigates motors
void SBusController(){
  float throttle1 = getChannelFloatFull(1);  // Left stick Vertical
  float dir       = getChannelFloat(2);      // Right stick Horisontal
  float throttle2 = getChannelFloat(3);      // Right stick Vertical
  int gear        = getChannel(5);           // Gear select (SA)

  int forwardDir;
  float speed;

  // Primary input (Left Stick)
  if (throttle1 > CONTROLLER_DEADZONE_FLOAT) {
    if (gear < REMOTE_CHANNEL_LOW) {
      speed = throttle1 * -1.0f;
    } else if (gear > REMOTE_CHANNEL_HIGH) {
      speed = throttle1;
    } else {
      speed = 0;
    }
    MotorUpdate(dir, speed);
  }
  // Secondary input (Right Stick)
  else if (abs(throttle2) > CONTROLLER_DEADZONE_FLOAT && gear < REMOTE_CHANNEL_HIGH && gear > REMOTE_CHANNEL_LOW) {
    if (throttle2 > 0) {
      speed = throttle2 / 2.0f;
    } else {
      speed = throttle2 / 2.0f;
    }
    MotorUpdateTorque(dir, speed);

  } else {
    MotorUpdate(0, 0);
  }
}

void SBusPrint() {
  if (millis() - millisLastSbusPrint < 1000) {
    return;
  }
  millisLastSbusPrint = millis();
  printChannels();
  Serial.println(F("-------------------------------"));
}

void printChannels() {
  // for (int i = 1; i < 8; i++) {
  //   Serial.print("CH ");
  //   Serial.print(i);
  //   Serial.print(": ");
  //   Serial.print(getChannelFloat(i));
  //   Serial.print("\t");
  // }
  // Serial.println();

  Serial.print("Good frames: ");
  Serial.print(sbus.getGoodFrames());

  Serial.print(" \tData loss on connection: ");
  Serial.print(sbus.getFrameLoss());
  Serial.print("% \t");

  Serial.print("Time diff: ");
  Serial.println(millis() - (unsigned long)sbus.getLastTime());
}

// Timer2 triggers ever 1ms and processes the incoming SBUS datastream
// ISR(TIMER2_COMPA_vect)
// {
//   if(GetStatus(MODULE_RF)) sbus.process();
// }

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
/*
    USB communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

const byte numChars = 32;
char receivedCMD[numChars];
int moduleSlct;

void initializeDebugComm() {
  Serial.begin(DEBUG_BAUDRATE);
  if (COM_SERIAL_DBG)
    SetStatus(MODULE_DBGCOMM, true);
  else
    SetStatus(MODULE_DBGCOMM, false);

  DEBUG_PRINTLN(F("#---------------------------#"));
  DEBUG_PRINTLN(F("#     GeoRover Firmware     #"));
  DBG_ONLY(DEBUG_PRINTLN(F("#       #DEBUG MODE#        #")));
  DEBUG_PRINT(F("#  System Version: "));
  DEBUG_PRINT(SystemVersion);
  DEBUG_PRINTLN(F(" #"));
  DEBUG_PRINTLN(F("#---------------------------#"));
  DEBUG_PRINTLN();
}

// Receive Commands
void recvWithStartEndMarkers() {
  if (Serial.available() <= 0) {
    return;
  }

  static boolean recvInProgress = false;
  static byte ndx               = 0;
  char startMarker              = '<';
  char endMarker                = '>';
  char rc;

  while (Serial.available() > 0) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedCMD[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedCMD[ndx] = '\0';  // terminate the string
        recvInProgress   = false;
        ndx              = 0;
        parseCommand();
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

bool activeCommand = false;
void PerformCommand() {
  if (!activeCommand) {
    return;
  }

  SystemTestModule(moduleSlct);
}

bool DebugCommStatus() {
  return COM_SERIAL_DBG;
}

// Parse read Command
void parseCommand() {
  DEBUG_PRINT(F("Received command (DBG_PRT): \""));
  DEBUG_PRINT(receivedCMD);
  DEBUG_PRINTLN(F("\""));

  switch (receivedCMD[0]) {
    case CMD_FILES:
      parseCommandFiles();
      break;
    case CMD_STRATEGY:
      parseCommandStrategy();
      break;
    case CMD_BACKUP:
      parseCommandBackup();
      break;
    case CMD_MODULE:
      parseCommandModule();
      break;
    case CMD_ROUTE:
      parseCommandRoute();
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandFiles() {
  char *fileNamePtr       = receivedCMD + 2;
  char fileName[numChars] = {0};
  strcpy(fileName, fileNamePtr);

  switch (receivedCMD[1]) {
    case CMD_FILES_LIST:
      SDQuery();
      break;
    case CMD_FILES_SIZE:
      SDSize(fileName);
      break;
    case CMD_FILES_DOWNLOAD:
      SDDownload(fileName);
      break;
    case CMD_FILES_DELETE:
      SDDelete(fileName);
      break;
    case CMD_FILES_BLCKBOX:
      BlackBoxPrint();
      break;
    case CMD_FILES_BLCKBOXCLEAR:
      BlackBoxClear();
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandStrategy() {
  switch (receivedCMD[1]) {
    case CMD_STRATEGY_SET:
      DEBUG_PRINT(F("Manual Strategy Set to: "));
      DEBUG_PRINTLN((int)(receivedCMD[2] - '0'));
      if (!SetMode((int)(receivedCMD[2] - '0')))
        DEBUG_PRINTLN(F("Mode not found!"));
      break;
    case CMD_STRATEGY_FUNCTION:
      strategyMethods[3][mode]();
      break;
    case CMD_STRATEGY_OVERRIDE:
      // Override
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandBackup() {
  switch (receivedCMD[1]) {
    case CMD_BACKUP_RST:
      DEBUG_PRINTLN(F("Manual Reset of Backup System."));
      ResetBackupCPU();
      break;
    case CMD_BACKUP_PRIMSTATUS:
      DEBUG_PRINT(F("Backup System Status: "));
      DEBUG_PRINTLN(GetStatus(MODULE_BACKUPCPU));
      break;
    case CMD_BACKUP_HB:
      DEBUG_PRINTLN(F("Virtual Heartbeat"));
      HeartBeatInInterrupt();
      break;
    case CMD_BACKUP_FREEZE:
      DEBUG_PRINTLN(F("Simulating System Halt for 60 sec"));
      delay(60000);
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandModule() {
  char *modulePtr           = receivedCMD + 2;
  char moduleChar[numChars] = {0};
  strcpy(moduleChar, modulePtr);

  moduleSlct = atoi(moduleChar);

  switch (receivedCMD[1]) {
    case CMD_MODULE_ENABLE:
      DEBUG_PRINT(F("Manual Enable of Module: "));
      switch (receivedCMD[2]) {
        case '\0':
          DEBUG_PRINTLN(F("ALL SYSTEMS"));
          break;
        default:
          DEBUG_PRINTLN(moduleSlct);
          SystemEnable(moduleSlct);
          break;
      }
      break;
    case CMD_MODULE_DISABLE:
      DEBUG_PRINT(F("Manual Disable of Module: "));
      switch (receivedCMD[2]) {
        case '\0':
          DEBUG_PRINTLN(F("ALL SYSTEMS"));
          SystemDisable();
          break;
        default:
          DEBUG_PRINTLN(moduleSlct);
          SystemDisable(moduleSlct);
          break;
      }
      break;
    case CMD_MODULE_OVERRIDE:
      DEBUG_PRINT(F("Manual Override of Module: "));
      switch (receivedCMD[2]) {
        case '\0':
          DEBUG_PRINTLN(F("NACK"));
          // SystemDisable();
          break;
        default:
          DEBUG_PRINT(moduleSlct);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINTLN(ToBoolString(!GetStatus(moduleSlct)));
          SetStatus(moduleSlct, !GetStatus(moduleSlct));
          break;
      }
      break;
    case CMD_MODULE_STATUS:
      DEBUG_PRINTLN(F("Manual System Status Check"));
      // GetStatus(true);
      DEBUG_PRINT(F("System Status: "));
      DEBUG_PRINTLN(String(ToLong(SystemStatus)));
      break;
    case CMD_MODULE_RESET:
      DEBUG_PRINT(F("Manual System Reset in: "));
      CountDownPrint();
      systemReset();
      break;
    case CMD_MODULE_TEST:
      DEBUG_PRINT(F("Testing Module: "));
      DEBUG_PRINTLN(ModuleToString(moduleSlct));
      DEBUG_PRINTLINE();
      activeCommand = true;
      break;
    case CMD_MODULE_STOPTEST:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Manual System Test Stop"));
      SystemDisable(moduleSlct);
      DEBUG_PRINTLINE();
      activeCommand   = false;
      systemTestState = -1;
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandRoute() {
  char tempChars[numChars];
  char *strtokIndx;
  strcpy(tempChars, receivedCMD);

  long latLong;
  long lonLong;

  strtokIndx = strtok(tempChars, ",");
  strtokIndx = strtok(NULL, ",");
  latLong     = atol(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  lonLong     = atol(strtokIndx);

  switch (receivedCMD[1]) {
    case CMD_ROUTE_SET:
      // write coordinate
      DEBUG_PRINT(F("Lat: "));
      DEBUG_PRINT(latLong);
      DEBUG_PRINT(F(" Lon: "));
      DEBUG_PRINTLN(lonLong);

      EEPROM_WRITE_LAT((long)(receivedCMD[2] - '0'), latLong);
      EEPROM_WRITE_LON((long)(receivedCMD[2] - '0'), lonLong);
      break;
    case CMD_ROUTE_PRINT:
      EEPROM_READ_LAT((long)(receivedCMD[2] - '0'), latLong);
      EEPROM_READ_LON((long)(receivedCMD[2] - '0'), lonLong);

      DEBUG_PRINT(F("Lat: "));
      DEBUG_PRINT(latLong);
      DEBUG_PRINT(F(" Lon: "));
      DEBUG_PRINTLN(lonLong);
      break;
    default:
      break;
  }
}

void parseData() {  // split the data into its parts

  char *strtokIndx;  // this is used by strtok() as an index
}

void CountDownPrint() {
  DEBUG_PRINT(F("3"));
  delay(333);
  DEBUG_PRINT(F("."));
  delay(333);
  DEBUG_PRINT(F(". "));
  delay(333);
  DEBUG_PRINT(F("2"));
  delay(333);
  DEBUG_PRINT(F("."));
  delay(333);
  DEBUG_PRINT(F(". "));
  delay(333);
  DEBUG_PRINT(F("1"));
  delay(333);
  DEBUG_PRINT(F("."));
  delay(333);
  DEBUG_PRINTLN(F(". "));
  delay(333);
}
#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
/*
  GeoRover Accelerometer functionalities, utilizing the XXXX
  (SparkFun Triple Axis Accelerometer Breakout - MMA8452Q https://www.sparkfun.com/products/12756)

  Primary library used:
    https://github.com/sparkfun/SparkFun_MMA8452Q_Arduino_Library/tree/V_1.1.0

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#include <SparkFun_MMA8452Q.h>

MMA8452Q accel;

int accTestState = 0;
long millisAccTestStart = 0;
long millisLastAccPrint = 0;

bool InitializeAccel() {
  bool status = accel.begin();  // scale +/- 2g and frequency of 1.56 Hz
  accel.setScale(SCALE_2G);
  accel.setDataRate(ODR_12);
  SetStatus(MODULE_ACCEL, status);
  return status;
}

void TerminateAccel() {
  //
}

// accel.read() will update two sets of variables.
// * int's x, y, and z will store the signed 12-bit values
//   read out of the accelerometer.
// * floats cx, cy, and cz will store the calculated
//   acceleration from those 12-bit values. These variables
//   are in units of g's.
void ReadAccel() {
  if (accel.available()) {
    accel.read();
  }
}

bool AccelStatus() {
  if(!GetStatus(MODULE_ACCEL)) return false;
  
  return accel.cz > MIN_ACCEL_TILT;
}

bool AccelTest() {
  bool testDone = false;
  switch (accTestState) {
    case 0:
      DEBUG_PRINT(F("Accelerometer feed starting for: "));
      DEBUG_PRINT(SYS_TEST_DURATION);
      DEBUG_PRINTLN(F(" ms"));
      millisAccTestStart = millis();
      accTestState++;
      break;
    case 1:
      if (millis() - millisLastAccPrint > SYS_PRINT_PERIOD) {
        millisLastAccPrint = millis();
        DEBUG_PRINT(F("Accelerometer: "));
        if (AccelStatus()) {
          ReadAccel();
          DEBUG_PRINT(F("x: "));
          DEBUG_PRINT((float)accel.cx);
          DEBUG_PRINT(F("\t y: "));
          DEBUG_PRINT((float)accel.cy);
          DEBUG_PRINT(F("\t z: "));
          DEBUG_PRINTLN((float)accel.cz);
        } else {
          DEBUG_PRINTLN(F("ERROR"));
        }
      }

      if (millis() - millisAccTestStart > SYS_TEST_DURATION) accTestState++;
      break;
    case 2:
      testDone = true;
      accTestState = 0;
      DEBUG_PRINTLINE();
    default:
      break;
  }

  return testDone;
}
#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
/*
  GeoRover GNSS functionalities, utilizing the ZED-F9P
  (SparkFun GPS-RTK-SMA Breakout https://www.sparkfun.com/products/16481)

  Primary library used:
    https://github.com/sparkfun/SparkFun_u-blox_GNSS_Arduino_Library

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

SFE_UBLOX_GNSS gnss;

long lastTimeGNSS = 0;  // Local timer, limits I2C traffic to u-blox module.

int gnssTestState        = 0;
long millisGnssTestStart = 0;
long millisLastGnssPrint = 0;

bool InitializeGnss() {
  Wire.begin();
  bool status = gnss.begin();
  if (status) {
    gnss.setI2COutput(COM_TYPE_UBX);                                // Set the I2C port to output UBX only (turn off NMEA noise)
    gnss.setVal(UBLOX_CFG_RATE_MEAS, GNSS_QUERY_UPDATE_FREQUENCY);  // Set measurement rate to 1000ms (1Hz update rate)
  }
  return status;
}

bool GnssStatus() {
  return gnss.isConnected();
}

// Updates current position and Gnss status
bool GnssUpdate(){
  latCurrent = GnssGetLat();
  lonCurrent = GnssGetLong();
}

bool GnssTest() {
  bool testDone = false;
  switch (gnssTestState) {
    case 0:
      DEBUG_PRINT(F("GNSS feed starting for: "));
      DEBUG_PRINT(SYS_TEST_DURATION);
      DEBUG_PRINTLN(F(" ms"));
      millisGnssTestStart = millis();
      gnssTestState++;
      break;
    case 1:
      if (millis() - millisLastGnssPrint > SYS_PRINT_PERIOD_LONG) {
        millisLastGnssPrint = millis();
        QueryGnss();
      }

      if (millis() - millisGnssTestStart > SYS_TEST_DURATION) gnssTestState++;
      break;
    case 2:
      SetStatus(MODULE_GNSS, GnssTime(true));
      gnssTestState = 0;
      testDone      = true;
      DEBUG_PRINTLINE();
    default:
      break;
  }

  return testDone;
}

bool GnssTime(bool print) {
  bool status;
  if (print) DEBUG_PRINT(F("Time and Date is: "));
  if (!gnss.getTimeValid() || !gnss.getDateValid()) {
    if (print) DEBUG_PRINTLN(F("not valid"));
    status = false;
  } else {
    if (print) {
      DEBUG_PRINTLN(F("valid"));
      int year   = gnss.getYear();
      int month  = gnss.getMonth();
      int day    = gnss.getDay();
      int hour   = gnss.getHour();
      int minute = gnss.getMinute();

      DEBUG_PRINT(F("Current time: "));
      DEBUG_PRINT(year);
      DEBUG_PRINT(F("-"));
      DEBUG_PRINT(month);
      DEBUG_PRINT(F("-"));
      DEBUG_PRINT(day);
      DEBUG_PRINT(F("-"));
      DEBUG_PRINT(hour);
      DEBUG_PRINT(F(":"));
      DEBUG_PRINTLN(minute);
    }
    status = true;
  }

  return status;
}

void TerminateGnss() {
  gnss.end();
}

// Gets positional data in Latitude in degrees * 10^-7
long GnssGetLat() {
  return gnss.getLatitude();
}

// Gets positional data in Longitude in degrees * 10^-7
long GnssGetLong() {
  return gnss.getLongitude();
}

// Returns heading in degrees * 10^-5
long GnssGetHeading(){
  return gnss.getHeading();
}

// Query module and prints Lat, Long, Alt, Acc
void QueryGnss() {
  DEBUG_PRINT(F("GNSS: "));

  if (GnssStatus()) {
    long latitude = gnss.getLatitude();
    DEBUG_PRINT(F("Lat: "));
    DEBUG_PRINT(latitude);
    DEBUG_PRINT(F(" (deg * 10^-7)"));

    long longitude = gnss.getLongitude();
    DEBUG_PRINT(F("\tLong: "));
    DEBUG_PRINT(longitude);
    DEBUG_PRINT(F(" (deg * 10^-7)"));

    long altitude = gnss.getAltitude();
    DEBUG_PRINT(F("\tAlt: "));
    DEBUG_PRINT(altitude);
    DEBUG_PRINT(F(" (mm)"));

    long accuracy = gnss.getPositionAccuracy();
    DEBUG_PRINT(F("\t3D Positional Accuracy: "));
    DEBUG_PRINT(accuracy);
    DEBUG_PRINTLN(F(" (mm)"));
  } else {
    DEBUG_PRINTLN("ERROR");
  }
}

// Returns current signal status
bool GnssSignal(){
  if(!GetStatus(MODULE_GNSS)){
    return false;
  }

  return (gnss.getTimeValid() && gnss.getDateValid() && gnss.getGnssFixOk());
}

// returns distance in meters between two positions, both specified
// as signed decimal-degrees latitude and longitude. Uses great-circle
// distance computation for hypothetical sphere of radius 6372795 meters.
// Because Earth is no exact sphere, rounding errors may be up to 0.5%.
// Courtesy of Maarten Lamers
double DistanceBetween(double lat1, double long1, double lat2, double long2) {
  double delta  = radians(long1 - long2);
  double sdlong = sin(delta);
  double cdlong = cos(delta);
  lat1          = radians(lat1);
  lat2          = radians(lat2);
  double slat1  = sin(lat1);
  double clat1  = cos(lat1);
  double slat2  = sin(lat2);
  double clat2  = cos(lat2);
  delta         = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta         = sq(delta);
  delta += sq(clat2 * sdlong);
  delta        = sqrt(delta);
  double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta        = atan2(delta, denom);
  return delta * 6372795;
}

// returns course in degrees (North=0, West=270) from position 1 to position 2,
// both specified as signed decimal-degrees latitude and longitude.
// Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
// Courtesy of Maarten Lamers
double CourseTo(double lat1, double long1, double lat2, double long2) {
  double dlon = radians(long2 - long1);
  lat1        = radians(lat1);
  lat2        = radians(lat2);
  double a1   = sin(dlon) * cos(lat2);
  double a2   = sin(lat1) * cos(lat2) * cos(dlon);
  a2          = cos(lat1) * sin(lat2) - a2;
  a2          = atan2(a1, a2);
  if (a2 < 0.0) {
    a2 += TWO_PI;
  }
  return degrees(a2);
}

// Returns Distance between two sets of coordinates
double DistanceBetweenLong(long lat1, long long1, long lat2, long long2){
  return DistanceBetween(CoordLong2Double(lat1),CoordLong2Double(long1),CoordLong2Double(lat2),CoordLong2Double(long2));
}

// Returns Distance between two sets of coordinates
double CourseToLong(long lat1, long long1, long lat2, long long2){
  return CourseTo(CoordLong2Double(lat1),CoordLong2Double(long1),CoordLong2Double(lat2),CoordLong2Double(long2));
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
/*
  GeoRover Temperature Sensor functionalities, utilizing a 10kOhm NTC thermistor
  (https://dk.rs-online.com/web/p/termistor-ic/7062759)

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

int tempTestState        = 0;
long millisTempTestStart = 0;
long millisLastTempPrint = 0;

int MeanThermTemp() {
  int tempCount, total;

  for (int i = 0; i < 3; i++) {
    int therm = ThermTemp(i);
    if (therm != 0) {
      total += therm;
      tempCount++;
    }
  }

  return total / tempCount;
}

int ThermTemp(int thermistor) {
  int temp;
  switch (thermistor) {
    case 0:
      temp = analogRead(PA_SENSOR_TEMP1);
      break;
    case 1:
      temp = analogRead(PA_SENSOR_TEMP2);
      break;
    case 2:
      temp = analogRead(PA_SENSOR_TEMP3);
      break;
    default:
      break;
  }
  return temp;
}

bool HeatingStart() {
  if (millis() - lastMillistHeatingOff < HEATING_TIMEOUT) {
    return false;
  }

  digitalWrite(PO_POWER_HEATING, HIGH);
  lastMillistHeatingOn = millis();

  return true;
}

bool HeatingStop() {
  digitalWrite(PO_POWER_HEATING, LOW);
  lastMillistHeatingOff = millis();
}

bool HeatingStatus() {
  return GetStatus(MODULE_HEATING);
}

bool TemperatureStatus() {
  bool status = MeanThermTemp() > TEMP_SYSTEM_MIN;

  SetStatus(MODULE_TEMP, status);
  return status;
}

// Runs temperature test for a duration, returns true once test is complete
bool TemperatureTest() {
  bool testDone = false;
  int meanTemp;
  switch (tempTestState) {
    case 0:
      DEBUG_PRINT(F("Thermistor feed starting for: "));
      DEBUG_PRINT(SYS_TEST_DURATION);
      DEBUG_PRINTLN(F(" ms"));
      millisTempTestStart = millis();
      tempTestState++;
      break;
    case 1:
      if (millis() - millisLastTempPrint > SYS_PRINT_PERIOD_LONG) {
        millisLastTempPrint = millis();
        meanTemp            = MeanThermTemp();
        DEBUG_PRINT(F("Temperatures: "));
        DEBUG_PRINT(F("\tT1: "));
        DEBUG_PRINT(ThermTemp(0));
        DEBUG_PRINT(F("\tT2: "));
        DEBUG_PRINT(ThermTemp(1));
        DEBUG_PRINT(F("\tT3: "));
        DEBUG_PRINT(ThermTemp(2));
        DEBUG_PRINT(F("\tMean: "));
        DEBUG_PRINTLN(meanTemp);

        SetStatus(MODULE_TEMP, meanTemp > TEMP_SYSTEM_MIN);
      }

      if (millis() - millisTempTestStart > SYS_TEST_DURATION) tempTestState++;
      break;
    case 2:
      testDone      = true;
      tempTestState = 0;
      break;
    default:
      break;
  }

  return testDone;
}

// If temp < minimum temp runs heating period
void HeatingProcess() {
  if (TemperatureStatus()) {
    SystemDisable(MODULE_HEATING);
    return;
  }

  HeatingCheck();
}

// If currently heating checks for maximum heating duration
// If not currently heating checks for timeout to reactivate heating
void HeatingCheck() {
  if (GetStatus(MODULE_HEATING)) {
    if (millis() - lastMillistHeatingOn > HEATING_DURATION) {
      SystemDisable(MODULE_HEATING);
    }
  } else {
    if (millis() - lastMillistHeatingOff > HEATING_TIMEOUT) {
      SystemEnable(MODULE_HEATING);
    }
  }
}
#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
/*  Strategy - Autonomous

    Primary Strategy for Autonomous Navigation
*/


// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Starting."));

  SystemDisable();

  // TODO: Will reset autonomy upon reboot
  // - Add bool in EEPROM autonomous active, if true skip reset. Unset bool upon in FinishStrategyAutonomous
  AutonomyReset(); 

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Autonomous): Initialized."));
  DEBUG_PRINTLINE();

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyAutonomous() {

  AutonomousProcess();

  StrategyRunLed(); 

}

// End sequence of strategy
void FinishStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Ending."));

  DetachSelectButton();

  // TODO: Add unset autonomy bool in EEPROM once implemented
  
  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (Autonomous): Finished."));
  DEBUG_PRINTLINE();
}

// Select button function, starts autonomous driving
void SelectFunctionAutonomous() {
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    lastMillisSelect = millis();
    AutonomyToggle();    
  }
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
/*  Strategy - Emergency mode

    Disable all outputs and activate light.
    Detatch interrupts from input buttons, to not exit mode unless emergency reset.
    No actions until mode end.
*/

// Start sequence of strategy
void StartStrategyEmergency() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Emergency): Starting"));

  // Disable all outputs
  SystemDisable();
  SetStatus(MODULE_ESTOP, false);

  // Disable input buttons
  detachInterrupt(PI_BUTTON_MODE);
  detachInterrupt(PI_BUTTON_SELECT);
  DEBUG_PRINTLN(F("Strategy (Emergency): Systems Disabled"));

  DEBUG_PRINTLN(F("Strategy (Emergency): Initialized"));
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyEmergency() {
  StrategyRunLed(MODE_EMERGENCY);

  // Exit strategy, if emergency not set -> Idle strategy
  if (EmergencyStopStatus()) {
    LedBlink(BINARY_CODE_LED_GRN, ESTOP_DEBOUNCE_TIME / 2, ESTOP_DEBOUNCE_TIME / 2);
    if (EmergencyStopStatus()) {
      LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
      SetMode(MODE_IDLE);
    }
  }
}

// End sequence of strategy
void FinishStrategyEmergency() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Emergency): Ending"));

  lastMillisMode = millis();
  lastMillisEstop = millis();

  // Enable input buttons
  attachInterrupt(PI_BUTTON_MODE, ModeButtonInterruptHandler, FALLING);

  // Assign Emergency Stop button interrupt
  attachInterrupt(PI_INT_BUTTON_ESTOP, EstopButtonInterruptHandler, FALLING);
  SetStatus(MODULE_ESTOP, true);

  DEBUG_PRINTLN(F("Strategy (Emergency): Finished"));

  // Disable light
}

void SelectFunctionEmergency() {
}

// Returns TRUE if Emergency Button is NOT pressed
// Returns FALSE if Emergency Button IS pressed
bool EmergencyStopStatus() {
  return digitalRead(PI_BUTTON_ESTOP);
}
#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
/*  Strategy - Example

    Description...
*/

// Start sequence of strategy
void StartStrategyExample() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Example): Starting"));
  SystemDisable();

  StrategyStartLed();

  SystemEnableMode();

  DEBUG_PRINTLN(F("Strategy (Example): Initialized"));
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyExample() {
  StrategyRunLed(); // Non-blocking LED blink
  
  // Add code which runs every loop for current strategy

}

// End sequence of strategy
void FinishStrategyExample() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Example): Ending"));

  DetachSelectButton();

  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (Example): Finished"));
}

// Select button function for current strategy
void SelectFunctionExample() {
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    lastMillisSelect = millis();
    // Add functionality here
  }
}


#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
/*  Idle mode

    Description...
*/

// Start sequence of strategy
void StartStrategyIdle() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Idle): Starting"));

  SystemDisable();

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Idle): Initialized"));
  DEBUG_PRINTLINE();

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyIdle() {
  StrategyRunLed();
}

// End sequence of strategy
void FinishStrategyIdle() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Idle): Ending"));
  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (Idle): Finished"));
}

// Selecet button function
void SelectFunctionIdle() {
}
#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
/*  Mode Library mode

    Cycles through modes, activates mode with Select Button.
*/

// Start sequence of strategy
void StartStrategyModeLibrary() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Mode Library): Starting"));

  lastMillisMode = millis();
  lastMillisEstop = millis();

  // Assign mode button interrupt
  attachInterrupt(PI_INT_BUTTON_MODE, ModeFunctionModeLibrary, FALLING);

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Mode Library): Initialized"));
  DEBUG_PRINTLINE();
  DEBUG_PRINT(F("Mode Cycle: "));
  DEBUG_PRINTLN(ModeToString(modeCycle));
}

// Main sequence of strategy
void RunStrategyModeLibrary() {
  StrategyRunLed(modeCycle);
}

// End sequence of strategy
void FinishStrategyModeLibrary() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Mode Library): Ending"));
  DEBUG_PRINTLN(F("Strategy (Mode Library): Finished"));

  DetachModeButton();
  DetachSelectButton();

  // Assign mode button interrupt
  AttachModeButton();
}

// Select button function
void SelectFunctionModeLibrary() {
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    lastMillisSelect = millis();

    delay(1);
    if (digitalRead(PI_BUTTON_SELECT)) {
      return;
    }

    // Select Mode
    DEBUG_PRINTLN(F("Mode Selected."));
    SetMode(modeCycle);
  }
}

// Mode button function
void ModeFunctionModeLibrary() {
  if (millis() - lastMillisMode > BTN_DEBOUNCE_TIME) {
    lastMillisMode = millis();

    lastMillistModeBlink = 0;

    delay(1);
    if (digitalRead(PI_BUTTON_MODE)) {
      return;
    }


    // Cycle Mode
    if (modeCycle + 1 < MODES_MIN_BROWSABLE || modeCycle + 1 >= MODES_MAX) {
      modeCycle = MODES_MIN_BROWSABLE;
    } else {
      modeCycle = modeCycle + 1;
    }

    DEBUG_PRINT(F("Mode Cycle: "));
    DEBUG_PRINTLN(ModeToString(modeCycle));
    
    ResetLed();
  }
}
#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
/*  Strategy - Remote control

    Allows manual control of motors
*/
bool remoteActive, remoteStart;

// Start sequence of strategy
void StartStrategyRemote() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Remote): Starting"));
  SystemDisable();

  remoteActive = false;
  remoteStart  = false;

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Remote): Initialized"));
  DEBUG_PRINTLINE();

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyRemote() {
  if (!remoteActive) {
    SystemDisable();
    StrategyRunLed(MODE_IDLE);
    return;
  }

  if (remoteStart) {
    remoteStart = false;
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Starting Remote Control"));
    DEBUG_PRINTLINE();
    if(!SystemEnableMode()){
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Module Start Error"));
      DEBUG_PRINTLINE();
      RemoteHalt();
      return;
    }
  }

  if (!SystemCheckMode(MODE_REMOTECONTROL)) {
    RemoteHalt();
    return;
  }

  StrategyRunLed(MODE_AUTONOMOUS);

  // Read RF signal
  SBusProcess();
  if (!SBusProcess()) {
    RemoteHalt();
    return;
  }
  SBusPrint();

  // Transmit via CAN
  CanBusProcess();
  if (!CanBusProcess()) {
    RemoteHalt();
    return;
  }
  CanBusPrint();
}

// End sequence of strategy
void FinishStrategyRemote() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Remote): Ending"));

  DetachSelectButton();

  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (Remote): Finished"));
}

// Select button function
void SelectFunctionRemote() {
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    lastMillisSelect = millis();
    lastSystemCheck  = millis() - SYSTEM_CHECK_DT;
    remoteActive     = !remoteActive;
    remoteStart      = true;
  }
}

void RemoteHalt() {
  remoteActive = false;
  SystemDisable();
  StatusHaltLed(LED_SIGNAL_ERROR);
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
/*  Strategy - System Test mode

    Runs full system test ...
*/
bool runTest = false;

// Start sequence of strategy
void StartStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (System Test): Starting"));
  SystemDisable();

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (System Test): Initialized"));
  DEBUG_PRINTLINE();
  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategySystemTest() {
  // Blink light
  StrategyRunLed();

  if (runTest) {
    runTest = !SystemTest();
  }
}

// End sequence of strategy
void FinishStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (System Test): Ending"));

  DetachSelectButton();
  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (System Test): Finished"));
}


// Selecet button function
void SelectFunctionSystemTest() {
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    systemTestState = 0;
    lastMillisSelect = millis();
    runTest = true;
    DetachSelectButton();
  }
}


#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
/*
  GeoRover Autonomy Stack

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

bool autonomyStart, autonomyRestart, autonomyActive, standbyMode;
unsigned long millisLastAutonomyCycle;

// Primary Autonomous Algorithm
void AutonomousProcess() {
  if (!AutonomyPreRun()) return;

  if (!AutonomyPowerCycle()) return;

  if (!StandbyRestart()) return;

  if (!CanBusProcess()) return;  // TODO: Handle CAN Error

  if (!Navigate()) return;  // TODO: Handle run-check Error
}

// Runs necessary autonomy pre-runs (Power, Communication, Navigation)
bool AutonomyPreRun() {
  if (autonomyActive) return true;

  if (!AutonomyPreCheck()) return false;

  // System ready to start, waiting idle
  if (!autonomyActive) {
    StatusRunLed(LED_SIGNAL_LOADING);
    return false;
  }

  // Select button pressed, start navigation
  if (autonomyStart) {
    autonomyStart       = false;
    millisAutonomyStart = millis();
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Starting Autonomous Driving"));
    DEBUG_PRINTLINE();
  }

  return true;
}

// Run power cycle, check if levels below thresholds or restart autonomy.
bool AutonomyPowerCycle() {
  if (millis() - millisLastAutonomyCycle < AUTONOMY_PWR_CYCLE_DT) return standbyMode;

  millisLastAutonomyCycle = millis();

  // Check power levels
  if (!PowerCycle()) {
    if (standbyMode) return false;
    AutonomyStandby();
    return false;
  }

  if (!standbyMode) {
    return true;
  }

  // Power levels sufficient, restart systems if previously powered down
  if (!StandbyRestart()) return false;

  standbyMode = false;
  return true;
}

// Runs necessary system pre-checks (enables systems, run Navigation pre-check)
bool AutonomyPreCheck() {
  // Enables primary and secondary systems
  if (!SystemEnableMode()) {
    autonomyActive = false;
    StatusHaltLed(LED_SIGNAL_ERROR);
    return false;
  }

  // Runs navigation route checks
  if (!NavigationPreCheck()) {
    return false;
  }
  return true;
}


// Inverts current autonomy state
void AutonomyToggle() {
  autonomyActive = !autonomyActive;
  autonomyStart  = autonomyActive;
  NavigationPreCheckReset();
}

// Full autonomy reset
void AutonomyReset() {
  NavigationPreCheckReset();
  ResetNavigation();
  ResetPowerCycle();
  autonomyActive = false;
  autonomyStart  = false;
}

// Power down system
void AutonomyStandby() {
  standbyMode     = true;
  autonomyRestart = true;
  MotorUpdate(0, 0);
  MotorCycle();
  SystemDisable();
}

// Power levels acceptable, if coming out of standby, reboot systems and await PreCheck flag
bool StandbyRestart() {
  if (autonomyRestart) {
    autonomyRestart = false;
    NavigationPreCheckReset();
  }

  if (!AutonomyPreCheck()) {
    return false;
  }
  return true;
}


#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
/*
  GeoRover Blackbox protocols

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

bool newLine = true;

// Initialize system Blackbox
bool InitBlackBox() {
  if (!SDReaderStatus()) {
    return false;
  }

  File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
  if (blackBoxFile) {
    SetStatus(MODULE_BLACKBOX, true);
  } else {
    SetStatus(MODULE_BLACKBOX, false);
  }
  blackBoxFile.close();

  BlackBoxAppend("GeoRover Blackbox ");
  BlackBoxAppend("System Version: ");
  BlackBoxAppendln(SystemVersion);

  return BlackBoxStatus;
}

void BlackBoxPrint() {
  if (SDReaderStatus()) {
    DEBUG_PRINTLN(F("Printing Blackbox: "));
    File file = SD.open("Blackbox.csv");
    if (file) {
      while (file.available()) {
        DEBUG_WRITE(file.read());
      }
      file.close();
      DEBUG_PRINTLN(F("End of File"));
    } else {
      DEBUG_PRINTLN(F("File not found!"));
    }
  }
}

void BlackBoxClear() {
  if (SD.exists("Blackbox.csv")) {
    DEBUG_PRINTLN(F("BLACKBOX CLEARED"));
    SD.remove("Blackbox.csv");
    InitBlackBox();
  } else {
    DEBUG_PRINTLN(F("BLACKBOX ERROR"));
  }
}

bool BlackBoxStatus() {
  return GetStatus(MODULE_BLACKBOX);
}

void BlackBoxAppend(String blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print((String)millis());
        blackBoxFile.print(F(";"));
        newLine = false;
      }
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN(F("Blackbox Error"));

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln() {
  BlackBoxAppend("\r\n");
  newLine = true;
}

void BlackBoxAppendln(String blackBoxInput) {
  BlackBoxAppend(blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppendln(int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(long int blackBoxInput, int type) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(F(";"));
        newLine = false;
      }
      blackBoxFile.print(blackBoxInput, type);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN(F("Blackbox Error"));

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(long int blackBoxInput, int type) {
  BlackBoxAppend(blackBoxInput, type);
  BlackBoxAppendln();
}

void BlackBoxAppendln(float blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(float blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppend(long int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppendln(long int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppendln(unsigned long blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(unsigned long blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
/*
  GeoRover System Control

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

bool SystemEnable(int module) {
  if (GetStatus(module)) return true;
  DEBUG_PRINT(F("SYS Enable: "));
  DEBUG_PRINT(ModuleToString(module));

  bool status = true;

  switch (module) {
    case MODULE_PWR:
      status = BatteryStatus();
      break;
    case MODULE_PWR_5V:
      digitalWrite(PO_POWER_5V, HIGH);
      break;
    case MODULE_PWR_12V:
      digitalWrite(PO_POWER_12V, HIGH);
      break;
    case MODULE_PWR_24V:
      digitalWrite(PO_POWER_24V, HIGH);
      break;
    case MODULE_PWR_MOTOR:
      digitalWrite(PO_POWER_MOTOR, HIGH);
      break;
    case MODULE_MOTORS:
      status = InitializeMotors();
      break;
    case MODULE_MOTOR_L:
      status = MotorStatusLeft();
      break;
    case MODULE_MOTOR_R:
      status = MotorStatusRight();
      break;
    case MODULE_MOTOR_ACT:
      status = MotorState();
      break;
    case MODULE_CANBUS:
      status = InitializeCanBus();
      break;
    case MODULE_RF:
      status = InitializeSBUS();
      break;
    case MODULE_IRIDIUM:
      digitalWrite(PO_POWER_IRIDIUM, HIGH);
      status = InitializeIridium();
      break;
    case MODULE_ACCEL:
      status = InitializeAccel();
      break;
    case MODULE_GNSS:
      status = InitializeGnss();
      break;
    case MODULE_SD:
      status = InitializeSDReader();
      break;
    case MODULE_BLACKBOX:
      status = InitBlackBox();
      break;
    case MODULE_ROUTE:
      status = RouteCheck();
      break;
    case MODULE_HEATING:
      status = HeatingStart();
      break;
    case MODULE_TEMP:
      status = TemperatureStatus();
      break;
    case MODULE_BACKUPCPU:
      status = GetStatus(MODULE_BACKUPCPU);
      break;
    case MODULE_LED:
      status = true;
      break;
    default:
      DEBUG_PRINT(F("- UNKNOWN Case"));
      break;
  }

  if (status) {
    DEBUG_PRINTLN(F(": Enabled"));
  } else {
    DEBUG_PRINTLN(F(": ERROR"));
  }
  SetStatus(module, status);
  delay(5);
  return status;
}

// Enables Primary Systems
void SystemEnablePrimary() {
  SystemEnable(MODULE_PWR);
  SystemEnable(MODULE_SD);
  SystemEnable(MODULE_BLACKBOX);
  SystemEnable(MODULE_GNSS);
  SystemEnable(MODULE_IRIDIUM);
}

// Enables module then flash LED indicating success or error
bool SystemSignalEnable(byte module) {
  bool status = SystemEnable(module);
  if (status) {
    StatusHaltLed(LED_SIGNAL_OK_SHORT_HALT);
  } else {
    StatusHaltLed(LED_SIGNAL_ERROR_SHORT_HALT);
  }
  return status;
}

// Enable systems for current mode
bool SystemEnableMode() {
  int modules = 0;
  bool status = true;
  bool enableStatus[10];
  switch (mode) {
    case MODE_REMOTECONTROL:
      enableStatus[0] = SystemEnable(MODULE_PWR);
      enableStatus[1] = SystemEnable(MODULE_CANBUS);
      enableStatus[2] = SystemEnable(MODULE_PWR_MOTOR);
      enableStatus[3] = SystemEnable(MODULE_MOTORS);
      enableStatus[4] = SystemEnable(MODULE_RF);
      modules         = 5;
      break;

    case MODE_AUTONOMOUS:
      enableStatus[0] = SystemEnable(MODULE_PWR);
      enableStatus[1] = SystemEnable(MODULE_CANBUS);
      enableStatus[2] = SystemEnable(MODULE_PWR_MOTOR);
      enableStatus[3] = SystemEnable(MODULE_MOTORS);
      enableStatus[4] = SystemEnable(MODULE_ACCEL);
      enableStatus[5] = SystemEnable(MODULE_RF);
      modules         = 6;
      break;

    default:
      status = false;
      break;
  }

  for (int i = 0; i < modules; i++) {
    if (!SystemEnableStatus(enableStatus[i])) {
      status = false;
    }
  }
  return status;
}

// Sets navigationPreCheck flag to false and signals
bool SystemEnableStatus(bool status) {
  if (status) {
    StatusHaltLed(LED_SIGNAL_OK_SHORT_HALT);
  } else {
    StatusHaltLed(LED_SIGNAL_ERROR_SHORT_HALT);
  }
  return status;
}

// Disables specified module
void SystemDisable(int module) {
  if (!GetStatus(module)) return;
  bool status = false;
  DEBUG_PRINT(F("SYS Disable: "));
  DEBUG_PRINT(ModuleToString(module));

  switch (module) {
    case MODULE_PWR:
      status = BatteryStatus();
      break;
    case MODULE_PWR_5V:
      digitalWrite(PO_POWER_5V, LOW);
      break;
    case MODULE_PWR_12V:
      digitalWrite(PO_POWER_12V, LOW);
      break;
    case MODULE_PWR_24V:
      digitalWrite(PO_POWER_24V, LOW);
      break;
    case MODULE_PWR_MOTOR:
      digitalWrite(PO_POWER_MOTOR, LOW);
      break;
    case MODULE_MOTORS:
      TerminateMotors();
      break;
    case MODULE_MOTOR_L:
      status = MotorStatusLeft();
      break;
    case MODULE_MOTOR_R:
      status = MotorStatusRight();
      break;
    case MODULE_MOTOR_ACT:
      status = MotorState();
      break;
    case MODULE_CANBUS:
      TerminateCanBus();
      break;
    case MODULE_RF:
      TerminateSBUS();
      digitalWrite(PO_POWER_RF, LOW);
      break;
    case MODULE_IRIDIUM:
      TerminateIridium();
      digitalWrite(PO_POWER_IRIDIUM, LOW);
      break;
    case MODULE_GNSS:
      TerminateGnss();
      break;
    case MODULE_ACCEL:
      TerminateAccel();
      break;
    case MODULE_SD:
      status = SDReaderStatus();
      break;
    case MODULE_BLACKBOX:
      status = BlackBoxStatus();
      break;
    case MODULE_DBGCOMM:
      status = DebugCommStatus();
      break;
    case MODULE_LED:
      status = LedStatus();
      break;
    case MODULE_HEATING:
      HeatingStop();
      break;
    case MODULE_TEMP:
      status = TemperatureStatus();
      break;
    case MODULE_BACKUPCPU:
      status = HeartBeatStatus();
      break;
    case MODULE_ESTOP:
      status = EmergencyStopStatus();
      break;
    case MODULE_RESERVED:
      status = true;
      break;
    default:
      DEBUG_PRINT(F(" - Unknown Case"));
      DEBUG_PRINTLN(ModuleToString(module));
      break;
  }

  if (status) {
    DEBUG_PRINTLN(F("Unable to Disable"));
  } else {
    DEBUG_PRINTLN(F("- Disabled"));
  }

  SetStatus(module, status);
}

// Disable all secondary systems
void SystemDisable() {
  SystemDisable(MODULE_RF);
  // SystemDisable(MODULE_IRIDIUM);
  // SystemDisable(MODULE_GNSS);
  SystemDisable(MODULE_ACCEL);
  SystemDisable(MODULE_PWR_MOTOR);
  SystemDisable(MODULE_MOTORS);
  SystemDisable(MODULE_CANBUS);
  SystemDisable(MODULE_PWR_MOTOR);
  SystemDisable(MODULE_PWR_5V);
  SystemDisable(MODULE_PWR_12V);
  SystemDisable(MODULE_PWR_24V);
}

// Runs system check if time since last check > timeout, returns comparison between required modules to current
bool SystemCheckMode(int mode) {
  static bool status = true;

  if (millis() - lastSystemCheck < SYSTEM_CHECK_DT) {
    return status;
  }

  lastSystemCheck = millis();
  SystemCheck();

  switch (mode) {
    case MODE_REMOTECONTROL:
      status = ((((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) &
                  SYSREQ_REMOTE_CONTROL) |
                 (1L << MODULE_RESERVED)) == (1L << MODULE_RESERVED));
      if (!status) {
        DEBUG_PRINT(F("ERROR Code: "));
        DEBUG_PRINTLN(String(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) &
                              SYSREQ_REMOTE_CONTROL) |
                             (1L << MODULE_RESERVED)));
      }
      break;
    case MODE_AUTONOMOUS:
      status =
          ((((ToLong(SystemStatus) ^ SYSREQ_AUTONOMOUS) & SYSREQ_AUTONOMOUS) |
            (1L << MODULE_RESERVED)) == (1L << MODULE_RESERVED));
      if (!status) {
        DEBUG_PRINT(F("ERROR Code: "));
        DEBUG_PRINTLN(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) &
                       SYSREQ_REMOTE_CONTROL) |
                      (1L << MODULE_RESERVED));
      }
      break;
    default:
      break;
  }

  return status;
}

// Run full system test
bool SystemTest() {
  unsigned long testResults;
  bool testDone = false;
  switch (systemTestState) {
    case 0:
      DEBUG_PRINTLN(F("Running Full System Test"));
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 1:
      DEBUG_PRINTLN(F("Running test (1/4) - Power Systems"));
      SystemTestModule(MODULE_PWR);
      SystemTestModule(MODULE_PWR_5V);
      SystemTestModule(MODULE_PWR_12V);
      SystemTestModule(MODULE_PWR_24V);
      SystemTestModule(MODULE_PWR_MOTOR);
      DEBUG_PRINTLN(F("Test (1/4) - Power Systems (Complete)"));
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 2:
      DEBUG_PRINTLN(F("Running test (2/4) - Communication"));
      systemTestState++;
      break;
    case 3:
      if (SystemTestModule(MODULE_RF)) systemTestState++;
      break;
    case 4:
      if (SystemTestModule(MODULE_IRIDIUM)) systemTestState++;
      break;
    case 5:
      if (SystemTestModule(MODULE_GNSS)) systemTestState++;
      break;
    case 6:
      if (SystemTestModule(MODULE_BACKUPCPU)) systemTestState++;
      break;
    case 7:
      DEBUG_PRINTLN(F("Test (2/4) - Communication (Complete)"));
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 8:
      DEBUG_PRINTLN(F("Running test (3/4) - Subsystems"));
      systemTestState++;
      break;
    case 9:
      if (SystemTestModule(MODULE_ACCEL)) systemTestState++;
      break;
    case 10:
      if (SystemTestModule(MODULE_SD)) systemTestState++;
      break;
    case 11:
      if (SystemTestModule(MODULE_BLACKBOX)) systemTestState++;
      break;
    case 12:
      if (SystemTestModule(MODULE_TEMP)) systemTestState++;
      break;
    case 13:
      if (SystemTestModule(MODULE_LED)) systemTestState++;
      break;
    case 14:
      if (SystemTestModule(MODULE_ROUTE)) systemTestState++;
      break;
    case 15:
      DEBUG_PRINTLN(F("Test (3/4) - Subsystems (Complete)"));
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 16:
      DEBUG_PRINTLN(F("Running test (4/4) - Motors"));
      systemTestState++;
      break;
    case 17:
      if (SystemTestModule(MODULE_MOTORS)) systemTestState++;
      break;
    case 18:
      DEBUG_PRINTLN(F("Test (4/4) - Motors (Complete)"));
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 19:
      DEBUG_PRINTLN(F("Disabeling all systems"));
      DEBUG_PRINTLINE();
      testResults = ToLong(SystemStatus);
      SystemDisable();
      AttachSelectButton();
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("System Test Complete"));
      DEBUG_PRINT(F("  Results: "));
      DEBUG_PRINTLN(String(testResults));
      DEBUG_PRINTLINE();
      testDone        = true;
      systemTestState = 0;
      break;
    default:
      DEBUG_PRINTLN(F("System Test Error: Stopping"));
      systemTestState = 0;
      testDone        = true;
      break;
  }

  return testDone;
}

// Runs module test
bool SystemTestModule(byte module) {
  SystemEnable(module);
  bool status   = false;
  bool testDone = true;

  if (GetStatus(module)) {
    switch (module) {
      case MODULE_PWR:
        status = BatteryStatus(false);
        break;
      case MODULE_PWR_5V:
        status = digitalRead(PO_POWER_5V);
        break;
      case MODULE_PWR_12V:
        status = digitalRead(PO_POWER_12V);
        break;
      case MODULE_PWR_24V:
        status = digitalRead(PO_POWER_24V);
        break;
      case MODULE_PWR_MOTOR:
        status = digitalRead(PO_POWER_MOTOR);
        break;
      case MODULE_MOTORS:
        testDone = MotorTest();
        status   = MotorStatus();
        break;
      case MODULE_MOTOR_L:
        status = MotorStatusLeft();
        break;
      case MODULE_MOTOR_R:
        status = MotorStatusRight();
        break;
      case MODULE_MOTOR_ACT:
        status = MotorState();
        break;
      case MODULE_CANBUS:
        testDone = CanBusTest();
        status   = CanBusStatus();
        break;
      case MODULE_RF:
        testDone = SBusTest();
        status   = SBusStatus();
        break;
      case MODULE_IRIDIUM:
        status = IridiumTest();
        break;
      case MODULE_GNSS:
        testDone = GnssTest();
        status   = GnssStatus();
        break;
      case MODULE_ACCEL:
        testDone = AccelTest();
        status   = AccelStatus();
        break;
      case MODULE_SD:
        status = SDReaderStatus();
        break;
      case MODULE_BLACKBOX:
        status = BlackBoxStatus();
        break;
      case MODULE_ROUTE:
        status = RouteTest();
        break;
      case MODULE_BACKUPCPU:
        status = HeartBeatStatus();
        break;
      case MODULE_DBGCOMM:
        status = DebugCommStatus();
        break;
      case MODULE_LED:
        LedTest();
        status = true;
        break;
      case MODULE_HEATING:
        HeatingStart();
        delay(2000);
        HeatingStop();
        status = true;
        break;
      case MODULE_TEMP:
        status   = TemperatureStatus();
        testDone = TemperatureTest();
        break;
      case MODULE_ESTOP:
        status = EmergencyStopStatus();
        break;
      case MODULE_RESERVED:
        status = true;
        break;
      default:
        DEBUG_PRINT(F("MODULE TEST: Unknown System Module: "));
        DEBUG_PRINTLN(ModuleToString(module));
        break;
    }
  }

  SetStatus(module, status);

  return testDone;
}

// Checks all Systems
void SystemCheck() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Running full system check"));
  BatteryStatus(true);
  DEBUG_PRINTLINE();
  for (int i = 0; i < MODULE_COUNT - 2; i++) {
    SystemCheckModule(i);
  }

  SetStatus(MODULE_ESTOP, EmergencyStopStatus());
  SetStatus(MODULE_RESERVED, true);
}

// Runs module check
bool SystemCheckModule(byte module) {
  bool status = false;

  switch (module) {
    case MODULE_PWR:
      status = BatteryStatus(false);
      break;
    case MODULE_PWR_5V:
      status = digitalRead(PO_POWER_5V);
      break;
    case MODULE_PWR_12V:
      status = digitalRead(PO_POWER_12V);
      break;
    case MODULE_PWR_24V:
      status = digitalRead(PO_POWER_24V);
      break;
    case MODULE_PWR_MOTOR:
      status = digitalRead(PO_POWER_MOTOR);
      break;
    case MODULE_MOTORS:
      status = MotorStatus();  // Left && Right motor
      break;
    case MODULE_MOTOR_L:
      status = MotorStatusLeft();
      break;
    case MODULE_MOTOR_R:
      status = MotorStatusRight();
      break;
    case MODULE_MOTOR_ACT:
      status = MotorState();  // <-- Active or idle?
      break;
    case MODULE_CANBUS:
      status = CanBusStatus();
      break;
    case MODULE_RF:
      status = SBusStatus();
      break;
    case MODULE_IRIDIUM:
      status = IridiumStatus();
      break;
    case MODULE_GNSS:
      status = GnssStatus();
      break;
    case MODULE_ACCEL:
      status = AccelStatus();
      break;
    case MODULE_SD:
      status = SDReaderStatus();
      break;
    case MODULE_BLACKBOX:
      status = BlackBoxStatus();
      break;
    case MODULE_ROUTE:
      status = RouteCheck();
      break;
    case MODULE_DBGCOMM:
      status = DebugCommStatus();
      break;
    case MODULE_LED:
      status = LedStatus();
      break;
    case MODULE_HEATING:
      status = HeatingStatus();
      break;
    case MODULE_TEMP:
      status = TemperatureStatus();
      break;
    case MODULE_BACKUPCPU:
      status = HeartBeatStatus();
      break;
    default:
      DEBUG_PRINT(F("MODULE CHECK: Unknown System Module: "));
      DEBUG_PRINTLN(module);
      break;
  }

  SetStatus(module, status);
  return status;
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
/*
  GeoRover Heartbeat functions.
  Send and receive heartbeat to backup MCU.

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

unsigned long lastMillisResetBackup       = 0;
unsigned long lastMillisHeartbeatIn       = 0;
unsigned long lastMillisHeartbeatOut      = 0;
unsigned long lastMillisHeartbeatBlackbox = HRTBEAT_DT_LOG;
bool receivedHeartBeat                    = false;

// Runs heartbeat in and out
void HeartBeat() {
  HeartBeatOut();
  HeartBeatIn();
  HeartBeatTimeout();
  HeartbeatBlackBox();
}

// Sends heartbeat with with frequency of HRTBEAT_FRQ
void HeartBeatOut() {
  if (millis() - lastMillisHeartbeatOut < HRTBEAT_DT_OUT) {
    return;
  }
  // DEBUG_PRINTLN("Sending Heartbeat!");
  lastMillisHeartbeatOut = millis();
  digitalWrite(PO_BACKUP_HRTBEAT, true);
  delay(5);
  digitalWrite(PO_BACKUP_HRTBEAT, false);
}

bool HeartBeatStatus() {
  HeartBeatTimeout();

  return GetStatus(MODULE_BACKUPCPU);
}

// Checks if time since last heartbeat received > maximum treshold
// Will attempt to reset backup CPU with a frequency of  BACKUP_RST_FRQ
void HeartBeatTimeout() {
  if (millis() - lastMillisHeartbeatIn < HRTBEAT_TRESHOLD) {
    return;
  }
  
  // Reset backup CPU
  if (GetStatus(MODULE_BACKUPCPU)) {
    lastMillisResetBackup = millis();
    SetStatus(MODULE_BACKUPCPU, false);
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Error: Backup CPU Heartbeat Timeout, attempting to Reset Backup System."));
    ResetBackupCPU();
    DEBUG_PRINTLINE();
  } else {
    if (millis() - lastMillisResetBackup > BACKUP_RST_DT) {
      lastMillisResetBackup = millis();
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Error: Backup CPU offline, attempting to Reset."));
      DEBUG_PRINTLINE();
      ResetBackupCPU();
    }
  }
}

// Resets Heartbeatflag if set and resets heartbeat in timer
void HeartBeatIn() {
  if (!receivedHeartBeat) {
    return;
  }
  receivedHeartBeat = false;

  if (millis() - lastMillisHeartbeatIn < BTN_DEBOUNCE_TIME) {
    return;
  }

  lastMillisHeartbeatIn = millis();
  // DEBUG_PRINTLN("Heartbeat Received!");
  if (!GetStatus(MODULE_BACKUPCPU)) {
    SetStatus(MODULE_BACKUPCPU, true);
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Backup CPU online"));
    DEBUG_PRINTLINE();
  }
}

// Set Heartbeat flag
void HeartBeatInInterrupt() {
  if (receivedHeartBeat) {
    return;
  }
  receivedHeartBeat = true;
}

// Attempt to reset backup CPU
void ResetBackupCPU() {
  digitalWrite(PO_BACKUP_RST, false);
  delay(5);
  digitalWrite(PO_BACKUP_RST, true);
}

void HeartbeatBlackBox() {
  if (millis() - lastMillisHeartbeatBlackbox > HRTBEAT_DT_LOG) {
    lastMillisHeartbeatBlackbox = millis();
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Heartbeat System Check: "));
    SystemCheck();
    DEBUG_PRINT(F("System Status: "));
    DEBUG_PRINTLN(String(ToLong(SystemStatus)));
    DEBUG_PRINTLINE();
  }
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
/*
  GeoRover Motor control

  Mads Rosenhøj Jeppesen
  Aarhus University
  2022
*/
unsigned long millisLastMotorStep;

// Initialize motor power
bool InitializeMotors() {
  // if (!GetStatus(MODULE_PWR_MOTOR)) {
  //   return false;
  // }

  digitalWrite(PO_MOTOR_EN, true);

  if (!MotorCycle()) return false;

  // return MotorStatus();
  return true;
}

// Disable Motor HW Enable
void TerminateMotors() {
  digitalWrite(PO_MOTOR_EN, false);
}

// Moves motors based on direction and speed input within the range of [-1 and 1]
// -1 full left, 1 full right (dir)
// -1 full reverse, 1 full forward (speed)
void MotorUpdate(float dir, float speed) {
  float velocityLeft;
  float velocityRight;

  SpeedCalculation(dir, speed, velocityLeft, velocityRight);

  // Update CAN messages
  motorLeft.Update(velocityLeft);
  motorRight.Update(velocityRight);
}

// Moves motors based on direction and speed input within the range of [-1 and 1]
// -1 full left, 1 full right (dir)
// -1 full reverse, 1 full forward (speed)
void MotorUpdateTorque(float dir, float speed) {
  float torqueLeft;
  float torqueRight;

  TorqueCalculation(dir, speed, torqueLeft, torqueRight);

  // Update CAN messages
  motorLeft.UpdateTorque(torqueLeft);
  motorRight.UpdateTorque(torqueRight);
}

void TorqueCalculation(float dir, float speed, float &torqueLeft, float &torqueRight) {
  float steerFactorLeft  = 1.0f;
  float steerFactorRight = 1.0f;

  // turn right, right wheel moves slower
  if (dir > 0) {
    steerFactorRight = SteerFactor(dir);
  }
  // turn left, left wheel moves slower
  else if (dir < 0) {
    steerFactorLeft = SteerFactor(dir);
  }

  torqueLeft  = speed * steerFactorLeft;
  torqueRight = speed * steerFactorRight;

  if (abs(torqueLeft) < MIN_TORQUE) torqueLeft = 0;
  if (abs(torqueRight) < MIN_TORQUE) torqueRight = 0;
}

// Calculates skid steering based on direction and speed
void SpeedCalculation(float dir, float speed, float &velocityLeft, float &velocityRight) {
  float steerFactorLeft  = 1.0f;
  float steerFactorRight = 1.0f;

  // turn right, right wheel moves slower
  if (dir > 0) {
    steerFactorRight = SteerFactor(dir);
  }
  // turn left, left wheel moves slower
  else if (dir < 0) {
    steerFactorLeft = SteerFactor(dir);
  }

  float speedLeft  = speed * steerFactorLeft;
  float speedRight = speed * steerFactorRight;

  if (speedLeft < 0) {
    velocityLeft = MOTOR_MAX_SPEED_BWD * speedLeft;
  } else {
    velocityLeft = MOTOR_MAX_SPEED_FWD * speedLeft;
  }

  if (speedRight < 0) {
    velocityRight = MOTOR_MAX_SPEED_BWD * speedRight;
  } else {
    velocityRight = MOTOR_MAX_SPEED_FWD * speedRight;
  }

  if (abs(velocityLeft) < MIN_VELOCITY) velocityLeft = 0;
  if (abs(velocityRight) < MIN_VELOCITY) velocityRight = 0;
}

// Run CAN cycle until both motors report NO ERROR or times out
bool MotorCycle() {
  if (!GetStatus(MODULE_CANBUS)) {
    return false;
  }
  if (!GetStatus(MODULE_PWR_MOTOR)) {
    return false;
  }

  MotorUpdate(0, 0);
  unsigned long millisMotorStart = millis();

  // (!motorLeft.CanStatus() || !motorRight.CanStatus()) && 1
  
  // Run canbus for a while to get motor states and test data connection
  while ((!motorLeft.CanStatus() || !motorRight.CanStatus()) && (millis() - millisMotorStart) < MOTOR_STARTUP_TIMEOUT) {
    if (!CanBusProcess()) {
      return false;
    }
  }

  return MotorCanStatus();
}

// Returns true if both motors are operational
bool MotorState() {
  return motorLeft.GetState() || motorRight.GetState();
}

// Motors operational?
bool MotorStatus() {
  // bool status = (MotorStatusLeft() && MotorStatusRight());
  bool status = GetStatus(MODULE_MOTORS);

  return status;
}

// Returns true if both motors have a valid can signal
bool MotorCanStatus(){
  return motorLeft.CanStatus() && motorRight.CanStatus();
}
// Left motor status
bool MotorStatusLeft() {
  return motorLeft.Status();
}

// Right motor status
bool MotorStatusRight() {
  return motorRight.Status();
}

// Calculates steering factor from 2nd order function (-2x^2 + 1), used in skid steering
float SteerFactor(float dir) {
  float scale;

  if (dir >= 1.0f)
    scale = -1.0f;

  else if (dir <= -1.0f)
    scale = -1.0f;

  else
    scale = -2.0f * (dir * dir) + 1.0f;

  return scale;
}

// Full motor test
bool MotorTest() {
  if (!GetStatus(MODULE_MOTORS)) {
    return true;
  }
  bool testDone = false;
  static int motorTestState;

  switch (motorTestState) {
    case 0:
      DEBUG_PRINTLN(F("Motor Test 1 - Linear Ramp"));
      DEBUG_PRINTLINE();
      motorTestState++;
      break;
    case 1:
      if (MotorTestRamp()) motorTestState++;
      break;
    case 2:
      DEBUG_PRINTLN(F("Motor Test 2 - Steering"));
      DEBUG_PRINTLINE();
      motorTestState++;
      break;
    case 3:
      if (MotorTestSteer()) motorTestState++;
      break;
    case 4:
      motorTestState = 0;
      testDone       = true;
      MotorUpdate(0, 0);
    default:
      break;
  }

  CanBusProcess();

  return testDone;
}

// Runs Motor Ramp Test, ramps up to maximum speed FWD -> Ramps down to 0 -> Ramps up to maximum speed BWD -> Ramps down to 0
bool MotorTestRamp() {
  if (!GetStatus(MODULE_MOTORS)) {
    return true;
  }

  static int motorTestRampState;
  static float speed;

  bool testDone = false;
  switch (motorTestRampState) {
    case 0:
      DEBUG_PRINTLN(F("Ramping up - FWD"));
      motorTestRampState++;
      break;
    case 1:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(0, speed);
        speed += 0.01;
        millisLastMotorStep = millis();
      }
      if (speed >= 1) motorTestRampState++;
      break;
    case 2:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Ramping down"));
      motorTestRampState++;
      break;
    case 3:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(0, speed);
        speed -= 0.01;
        millisLastMotorStep = millis();
      }
      if (speed <= 0) {
        speed = 0;
        MotorUpdate(0, speed);
        motorTestRampState++;
      }
      break;
    case 4:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Ramping up - BWD"));
      motorTestRampState++;
      break;
    case 5:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(0, speed);
        speed -= 0.01;
        millisLastMotorStep = millis();
      }
      if (speed <= -1) motorTestRampState++;
      break;
    case 6:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Ramping down"));
      motorTestRampState++;
      break;
    case 7:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(0, speed);
        speed += 0.01;
        millisLastMotorStep = millis();
      }
      if (speed >= 0) {
        speed = 0;
        motorTestRampState++;
        MotorUpdate(0, speed);
      }
      break;
    case 8:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Motor Test 2 - Steering"));
      motorTestRampState++;
      break;
    case 9:
      motorTestRampState = 0;
      testDone           = true;
      MotorUpdate(0, 0);
    default:
      testDone = true;
      break;
  }

  return testDone;
}

// Runs Motor Steering Test, Runs at lower speed, turns max steer right -> Max Steer Left -> Centered
bool MotorTestSteer() {
  if (!GetStatus(MODULE_MOTORS)) {
    return true;
  }
  static int motorTestSteerState;
  static float dir;
  float speed   = MOTOR_MAX_SPEED_FWD * 0.25;
  bool testDone = false;

  switch (motorTestSteerState) {
    case 0:
      DEBUG_PRINTLN("Turning Right -->");
      motorTestSteerState++;
      break;
    case 1:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(dir, speed);
        dir += 0.02;
        millisLastMotorStep = millis();
      }
      if (dir >= 1) motorTestSteerState++;
      break;
    case 2:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN("Turning Left <--");
      motorTestSteerState++;
      break;
    case 3:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(dir, speed);
        dir -= 0.02;
        millisLastMotorStep = millis();
      }
      if (dir <= -1) motorTestSteerState++;
      break;
    case 4:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN("Centering --> || <--");
      motorTestSteerState++;
      break;
    case 5:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(dir, speed);
        dir += 0.02;
        millisLastMotorStep = millis();
      }
      if (dir >= 0) motorTestSteerState++;
      break;
    case 6:
      motorTestSteerState = 0;
      testDone            = true;
      MotorUpdate(0, 0);
    default:
      testDone = true;
      break;
  }

  return testDone;
}
#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
/*
  GeoRover Navigation Stack

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

/* Algorithm:
  1. Check coordinates valid
    1.1 Format correct?
    1.2 Distance < min distance?
        -> Valid gps fix? (time since last)
    else break
  2. Calculate current bearing
    2.1 Distance between previous waypoints > Min Distance?
        -> Calculate using Forward Azimuth
    2.2 Else skip (Set bearing: 0)
  3. Calculate target bearing (Difference between current position and target waypoint)
    3.1 Is bearing valid?
        -> Calculate using forward Azimuth
    3.2 Else skip (Set target bearing: 0)
  4. Calculate change bearing
    4.1 difference between current and target

*/

#include "GeneralFunctions.h"
#include "Math.h"

bool navigationPreCheck = false;
long millisLastNavCheck;
float maxSpeedScale;

bool Navigate() {
  if (millis() - millisLastNavCheck < NAVIGATION_CYCLE_DT) {
    return false;
  }

  millisLastNavCheck = millis();

  if (!NavigationRunCheck()) {
    MotorUpdate(0, 0);
    return false;
  }

  PathingProcess();

  NavigationMotorUpdate();
}

// Autonomous Motor Controller, updates motor targets based on 
void NavigationMotorUpdate(){
  AutonomousTopSpeedUpdate();

  MotorUpdate(BearingDirection(), MAX_AUTONOMOUS_SPEED * maxSpeedScale);
}

// Increases top speed based on time since autonomy start to avoid high speed start up
void AutonomousTopSpeedUpdate(){
  float timeScaledSpeed = millisAutonomyStart * AUTONOMY_SPEED_SCALE;
  maxSpeedScale = min(1.0f,  timeScaledSpeed);
}

void NavigationPreCheckReset() {
  navigationPreCheck = false;
  ResetRouteStatus();

  millisLastNavCheck = millis() - NAVIGATION_CYCLE_DT;
}

// Resets current waypoint index and restarts route
void ResetNavigation() {
  waypointIndex = 0;
  UpdateWaypoint();
}

//  Checks to run before starting autonomous navigation
bool NavigationPreCheck() {
  if (navigationPreCheck) return true;

  if (millis() - millisLastNavCheck < NAVIGATION_CYCLE_DT) {
    return navigationPreCheck;
  }

  LedSetSignal(LED_SIGNAL_IDLE);

  millisLastNavCheck = millis();
  navigationPreCheck = true;

  bool routeValid = RouteTest();
  bool gnssValid  = GnssSignal();
  bool motorValid = MotorCycle();

  PreCheckStatus(routeValid);
  PreCheckStatus(gnssValid);
  PreCheckStatus(motorValid);

  LedSetSignal();
  return navigationPreCheck;
}

// Sets navigationPreCheck flag to false and signals
void PreCheckStatus(bool status) {
  if (!status) {
    navigationPreCheck = false;
    StatusHaltLed(LED_SIGNAL_ERROR_SHORT_HALT);
  } else {
    StatusHaltLed(LED_SIGNAL_OK_SHORT_HALT);
  }
}

// Check system status while navigation is running.
// GNSS status, Accelerometer output (tilt?), Battery Status
bool NavigationRunCheck() {
  return SystemCheckMode(MODE_AUTONOMOUS);
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
/*
  GeoRover pathing protocols

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

long latRoute, lonRoute;  // Latest position buffer (degrees *10^-7)
int idxRoute;             //
int lengthRoute;
int indexRoute;

bool routeStatus  = false;
bool routeTestRun = false;

char operatorName[numChars];

// Target coordinate (read from EEPROM)
long latTarget      = 0;
long lonTarget      = 0;
float navigationDir = 0;

// Current coordinate (From GNSS)
long headingError = 0;  // deg * 10^-5

// returns true if waypoint file exists
bool SDRoute() {
  return SD.exists("Waypoints.csv");
}

// Clears current route data
void ClearRoute() {
  latRoute = 0;
  lonRoute = 0;

  memset(operatorName, '\0', sizeof(operatorName));
  lengthRoute = 0;
  indexRoute  = 0;
}

// Loads Waypoint file data and first waypoint into buffer
bool LoadRoute() {
  DEBUG_PRINTLN(F("Loading Waypoint file"));

  ClearRoute();

  // Load first waypoint into buffer
  if (!RouteSD(latRoute, lonRoute, 0)) {
    return false;
  }

  DEBUG_PRINTLN(F("Done Loading"));
  return true;
}

// Test full route, only run once (used in Navigation pre-check). To perform new test, run ResetRouteStatus()
bool RouteTest() {
  if (routeTestRun) routeStatus;

  routeTestRun = true;  // Avoid several route tests being run

  DEBUG_PRINTLN(F("Running full route test"));

  routeStatus = false;
  if (!LoadRoute()) {
    return routeStatus;
  }
  if (!RouteSDValid()) {
    return routeStatus;
  }
  if (!CompareEepromSdRoute()) {
    if (!FlashRouteEeprom()) {
      return routeStatus;
    }
    if (!CompareEepromSdRoute()) {
      return routeStatus;
    }
  }

  routeStatus = true;

  return routeStatus;
}

// Compares current EEPROM route data to SD data
// TODO: Check full route, currently only checks route index, route length and 1st waypoint
bool CompareEepromSdRoute() {
  bool validity = true;

  int routeIndexEeprom  = EEPROM_READ_INT(MEMADDR_ROUTEIDX_START);
  int routeLengthEeprom = EEPROM_READ_INT(MEMADDR_ROUTELEN_START);

  DEBUG_PRINTLINE();
  DEBUG_PRINT(F("EEPROM route Index: "));
  DEBUG_PRINT(routeIndexEeprom);
  DEBUG_PRINT(F("\t EEPROM route Length: "));

  DEBUG_PRINT(F("Waypoint.csv route Index: "));
  DEBUG_PRINTLN(indexRoute);
  DEBUG_PRINT(F("\t Waypoint.csv route Length: "));
  DEBUG_PRINTLN(lengthRoute);
  DEBUG_PRINTLINE();

  if (indexRoute != routeIndexEeprom) return false;
  if (lengthRoute != routeLengthEeprom) return false;

  long latEE, lonEE;

  if (!RouteEe(latEE, lonEE, 0)) return false;
  RouteEe(latEE, lonEE, 0);

  DEBUG_PRINT(routeLengthEeprom);
  DEBUG_PRINT(F("\t - EEPROM 1st waypoint: Lat: "));
  DEBUG_PRINT(latEE);
  DEBUG_PRINT(F("\t lon: "));
  DEBUG_PRINTLN(lonEE);

  DEBUG_PRINT(F("\t - Waypoint.csv 1st waypoint: Lat: "));
  DEBUG_PRINT(latRoute);
  DEBUG_PRINT(F("\t lon: "));
  DEBUG_PRINTLN(lonRoute);

  if ((latRoute != latEE) && (lonRoute != lonEE)) return false;

  return validity;
}

bool RouteSDValid(){
  if (!SDRoute) {
    return false;
  }

  // DEBUG_PRINTLN(F("Validating Waypoint file"));

  File waypoints = SD.open("Waypoints.csv", FILE_READ);

  if (!waypoints) {
    // DEBUG_PRINTLN(F("Error opening waypoints file"));
    return false;
  }

  long latRouteOld, lonRouteOld;

  // Read waypoint file
  for (int i = 0; i < lengthRoute + 3; i++) {
    ParseWaypoint(waypoints.readStringUntil('\n'), i);
    // Skip first 3 (header, data, header)
    if (i > 2) {
      if (!CoordinateValidityFormat(latRoute, lonRoute)) {
        waypoints.close();
        return false;
      }
    }
    // Range checks from 2nd waypoint
    if (i > 3) {
      if (!CoordinateValidityRange(latRouteOld, lonRouteOld, latRoute, lonRoute)) {
        waypoints.close();
        return false;
      }
    }
    latRouteOld = latRoute;
    lonRouteOld = lonRoute;
  }

  return true;
}
// Check if route is valid (SD and EEPROM)
bool RouteCheck() {
  return RouteSDValid() && CompareEepromSdRoute();
}

bool PathingProcess() {
  GnssUpdate();

  // Calculate distance from current pos to target pos
  WaypointUpdate();

  // Calculate bearing to target
  BearingUpdate();
}

// Updates waypoint based on distance to target
void WaypointUpdate() {
  if (WaypointWithinRange) {
    IncrementWaypoint();
  }
}

// Updates bearing offset based on target and current bearing
void BearingUpdate() {
  long heading       = GnssGetHeading();  // deg * 10^-5
  long targetHeading = (long)(CourseToLong(latCurrent, lonCurrent, latTarget, lonTarget)) * 100000;

  headingError = targetHeading - heading;

  // Check headingError > 180 degree to take smallest rotation (190 degrees to avoid rotation overlap)
  if(headingError > 190){
    headingError -= 360;
  } else if (headingError < -190){
    headingError += 360;
  }

  if (abs(headingError) < MAX_VALID_BEARING) {
    navigationDir = 0;
  } else if (headingError > MAX_VALID_BEARING) {
    navigationDir = MAX_AUTONOMOUS_TURN;
  } else {
    navigationDir = -MAX_AUTONOMOUS_TURN;
  }

}

// Returns latest navigation direction
float BearingDirection() {
  return navigationDir;
}

// Checks if distance from current position to target is < min
bool WaypointWithinRange() {
  long distance = DistanceBetweenLong(latCurrent, lonCurrent, latTarget, lonTarget);
  return distance < MAX_DISTANCE_WAYPOINT_ACCEPT;
}

// Increments current waypoint returns to 0 if > total number of points
void IncrementWaypoint() {
  if (waypointIndex == lengthRoute) {
    waypointIndex = 0;
  } else {
    waypointIndex++;
  }

  UpdateWaypoint();
}

// Reads EEPROM waypoint lat, lon from current waypointIndex
void UpdateWaypoint() {
  EEPROMReadLatLon(waypointIndex);
  EEPROM_WRITE_INDEX(waypointIndex);
}

// Reads latitude and longitude at given index
bool RouteSD(long &lat, long &lon, int index) {
  if (!SDRoute) {
    return false;
  }

  File waypoints = SD.open("Waypoints.csv", FILE_READ);

  if (!waypoints) {
    DEBUG_PRINTLN(F("Error opening waypoints file"));
    return false;
  }

  // Read waypoint file until index
  int idx;
  while (waypoints.available() && idx < index + 3) {
    waypoints.readStringUntil('\n');
    idx++;
  }

  if (idx != index + 3) {
    DEBUG_PRINTLN(F("Waypoint file doesn't contain index"));
    return false;
  }

  if (!PopulateLatLon(waypoints.readStringUntil('\n'), lat, lon)) {
    waypoints.close();
    return false;
  }

  waypoints.close();
  return true;
}

// Reads EEPROM Latitude and Longitude values at given index, returns false if non found
bool RouteEe(long &lat, long &lon, int index) {
  EEPROM_READ_LAT(index, lat);
  EEPROM_READ_LON(index, lon);

  if (lat > MAX_LAT_VALUE || lat < -MAX_LAT_VALUE || lat == 0) {
    // DEBUG_PRINTLN(F("EEPROM lat value outside allowed values or 0"));
    return false;
  }
  if (lon > MAX_LON_VALUE || lon < -MAX_LON_VALUE || lon == 0) {
    // DEBUG_PRINTLN(F("EEPROM lon value outside allowed values or 0"));
    return false;
  }

  return true;
}

// Flash waypoints to EEPROM
bool FlashRouteEeprom() {
  if (!SDRoute) {
    return false;
  }

  DEBUG_PRINTLN(F("Flashing Waypoint file to EEPROM"));

  File waypoints = SD.open("Waypoints.csv", FILE_READ);

  if (!waypoints) {
    DEBUG_PRINTLN(F("Error opening waypoints file"));
    return false;
  }

  // Read waypoint file
  for (int i = 0; i < lengthRoute + 3; i++) {
    ParseWaypoint(waypoints.readStringUntil('\n'), i);
    // Skip first 3 (header, data, header)
    if (i > 2) {
      EEPROMWriteLatLon(i - 3);
    }
  }

  return true;
}

/* Parses waypoint string separating into latitude and longitude values
    waypointIdx,Latitude,Longitude
*/
bool PopulateLatLon(String waypoint, long &lat, long &lon) {
  char waypointBuffer[waypoint.length() + 1];
  waypoint.toCharArray(waypointBuffer, waypoint.length() + 1);

  char *strtokIndx;  // strtok() index

  // Route Index
  strtokIndx = strtok(waypointBuffer, ",");
  idxRoute   = atoi(strtokIndx);

  // Latitude
  strtokIndx = strtok(NULL, ",");
  latRoute   = atol(strtokIndx);
  lat        = latRoute;

  // Longitude
  strtokIndx = strtok(NULL, ",");
  lonRoute   = atol(strtokIndx);
  lon        = lonRoute;

  DEBUG_PRINT(F("Latitude: "));
  DEBUG_PRINT(lat);
  DEBUG_PRINT(F("\t Longitude: "));
  DEBUG_PRINTLN(lon);

  if (!CoordinateValidityFormat(lat, lon)) {
    return false;
  }

  return true;
}

bool PopulateLatLon(String waypoint) {
  PopulateLatLon(waypoint, latRoute, lonRoute);
}

void PopulateWaypointData(String waypoint) {
  char waypointBuffer[waypoint.length() + 1];
  waypoint.toCharArray(waypointBuffer, waypoint.length() + 1);

  char *strtokIndx;  // strtok() index

  // Route Index
  strtokIndx = strtok(waypointBuffer, ",");
  indexRoute = atoi(strtokIndx);

  // Waypoint Length
  strtokIndx  = strtok(NULL, ",");
  lengthRoute = atoi(strtokIndx);

  // Operator
  strtokIndx = strtok(NULL, ",");
  strcpy(operatorName, strtokIndx);

  DEBUG_PRINT(F("Route Index: "));
  DEBUG_PRINT(indexRoute);
  DEBUG_PRINT(F("\t Route Length: "));
  DEBUG_PRINT(lengthRoute);
  DEBUG_PRINT(F("\t Operator: "));
  DEBUG_PRINTLN(operatorName);
}

// Parse waypoint string
void ParseWaypoint(String waypoint, int index) {
  switch (index) {
    case 0:
      // Header (Waypoint Index,Waypoint Length,Operator)
      break;
    case 1:
      PopulateWaypointData(waypoint);
      break;
    case 2:
      // Header (Index,Lat,Lon)
      break;
    default:
      PopulateLatLon(waypoint);
      break;
  }
}

// Reset route status flag
void ResetRouteStatus() {
  routeStatus  = false;
  routeTestRun = false;
}

// Populate buffer latitude longitude into EEPROM
void EEPROMWriteLatLon(int index) {
  EEPROM_WRITE_LAT(index, latRoute);
  EEPROM_WRITE_LON(index, lonRoute);
}

// Populate buffer latitude longitude from EEPROM
void EEPROMReadLatLon(int index) {
  EEPROM_READ_LAT(index, latRoute);
  EEPROM_READ_LON(index, lonRoute);
}

// Checks validity of coordinate format (-90 <= lat <= 90 && -180 <= lon <= 180)
bool CoordinateValidityFormat(long lat, long lon) {
  DEBUG_PRINT("Checking format of coordinates... lat: " + String(lat, DEC) + ", lon: " + String(lon, DEC) + " ... ");
  bool validity = true;
  if (lat == 0 || lon == 0) {
    validity = false;
    DEBUG_PRINT(F("coordinates must be non zero ... "));
  }

  if (lat < -MAX_LAT_VALUE || lat > MAX_LAT_VALUE) {
    validity = false;
    DEBUG_PRINT(F("latitude range must be (-90 < lat < 90) ... "));
  }

  if (lon < -MAX_LON_VALUE || lon > MAX_LON_VALUE) {
    validity = false;
    DEBUG_PRINT(F("longitude range must be (-180 < lon < 180) ... "));
  }

  if (validity) {
    DEBUG_PRINTLN(F("Coordinates format valid!"));
  } else {
    DEBUG_PRINTLN(F("Coordinates format NOT valid!"));
  }

  return validity;
}

// Checks distance between two coordinate sets and returns true if within MAX_DISTANCE_VALID_WAYPOINT
bool CoordinateValidityRange(long lat1, long lon1, long lat2, long lon2) {
  DEBUG_PRINT(F("Checking range between"));
  DEBUG_PRINT(F(" lat: "));
  DEBUG_PRINT(lat1);
  DEBUG_PRINT(F(" lon: "));
  DEBUG_PRINT(lon1);
  DEBUG_PRINT(F("\t and \t"));
  DEBUG_PRINT(F(" lat: "));
  DEBUG_PRINT(lat2);
  DEBUG_PRINT(F(" lon: "));
  DEBUG_PRINTLN(lon2);

  long distance = DistanceBetweenLong(lat1, lon1, lat2, lon2);

  DEBUG_PRINT(F("Distance: "));
  DEBUG_PRINTLN(distance);

  return distance <= MAX_DISTANCE_VALID_WAYPOINT;
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins() {
  // External Inputs
  pinMode(PI_BUTTON_ESTOP, INPUT_PULLUP);
  pinMode(PI_BUTTON_MODE, INPUT_PULLUP);
  pinMode(PI_BUTTON_SELECT, INPUT_PULLUP);

  // Heartbeat
  pinMode(PI_BACKUP_HRTBEAT, INPUT_PULLUP);
  pinMode(PO_BACKUP_HRTBEAT, OUTPUT);

  // Reset
  digitalWrite(PO_BACKUP_RST, true);
  pinMode(PO_BACKUP_RST, OUTPUT);

  digitalWrite(PO_BACKUP_HRTBEAT, true);
  digitalWrite(PO_BACKUP_RST, true);

  // Status LED/Sound
  pinMode(PO_LED_STATUS_GRN, OUTPUT);
  pinMode(PO_LED_STATUS_YEL, OUTPUT);
  pinMode(PO_LED_STATUS_RED, OUTPUT);

  digitalWrite(PO_LED_STATUS_RED, LOW);
  digitalWrite(PO_LED_STATUS_YEL, LOW);
  digitalWrite(PO_LED_STATUS_GRN, LOW);

  // Power control (Relays)
  pinMode(PO_POWER_MOTOR, OUTPUT);
  pinMode(PO_POWER_24V, OUTPUT);
  pinMode(PO_POWER_12V, OUTPUT);
  pinMode(PO_POWER_5V, OUTPUT);
  pinMode(PO_POWER_RF, OUTPUT);
  pinMode(PO_POWER_IRIDIUM, OUTPUT);
  pinMode(PO_POWER_HEATING, OUTPUT);

  digitalWrite(PO_POWER_MOTOR, LOW);
  digitalWrite(PO_POWER_24V, LOW);
  digitalWrite(PO_POWER_12V, LOW);
  digitalWrite(PO_POWER_5V, LOW);
  digitalWrite(PO_POWER_RF, LOW);
  digitalWrite(PO_POWER_IRIDIUM, LOW);
  digitalWrite(PO_POWER_HEATING, LOW);

  // Analog Sensors
  pinMode(PA_SENSOR_TEMP1, INPUT);
  pinMode(PA_SENSOR_TEMP2, INPUT);
  pinMode(PA_SENSOR_TEMP3, INPUT);
  pinMode(PA_SENSOR_BATT, INPUT);

  // Motor Control
  pinMode(PO_MOTOR_EN, OUTPUT);
  digitalWrite(PO_MOTOR_EN, false);
}

// Initialization of the interrupts assigned to buttons
void InitButtons() {
  lastMillisMode  = millis();
  lastMillisEstop = millis();

  // Input interrupts
  attachInterrupt(PI_INT_BUTTON_MODE, ModeButtonInterruptHandler, FALLING);    // Mode Button
  attachInterrupt(PI_INT_BUTTON_ESTOP, EstopButtonInterruptHandler, FALLING);  // E-STOP
  attachInterrupt(PI_INT_HRTBEAT, HeartBeatInInterrupt, FALLING);              // Backup-CPU Heartbeat
}

// Activates Emergency Strategy. Triggered by Estop button interrupt
void EstopButtonInterruptHandler() {
  systemReset();
}

// Mode Library activated by mode button interrupt
void ModeButtonInterruptHandler() {
  if (millis() - lastMillisMode > BTN_DEBOUNCE_TIME_LONG) {
    if (GetStatus(MODULE_ESTOP)) {
      DEBUG_PRINTLN(F("Mode button press, changing mode to Mode Library"));
      lastMillisMode = millis();
      SetMode(MODE_MODELIBRARY);
    } else {
      DEBUG_PRINTLN(F("Emergency Mode, Input Blocked!"));
    }
  }
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
/*
  GeoRover Power control

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

// BatteryReading = 19.2 kOhm / (19.2 kOhm + 110 kOhm) * Battery Voltage
// 3.567 V -> ~5V ... 730 -> 1022 (int)
// 24.0 V -> 33.6 V
const int batteryLevelVoltage[] = {1022, 1004, 986, 956, 931, 920, 913, 898, 883, 839, 730};
const int batteryLevelPct[]     = {100, 98, 95, 91, 81, 62, 43, 25, 15, 6, 1};
//                            33.6, 33.0, 32.4, 31.44, 30.6, 30.24, 30.0, 29.52, 29.04, 27.6, 24.0

bool charge;  // Battery Charge status

// Checks voltage levels above critical values?
bool VoltageCheck() {
  return BatteryLevel() > BATTERY_STD_CHARGE;
}

bool BatteryStatus() {
  return VoltageCheck();
}

bool BatteryStatus(bool print) {
  if (!print) {
    return VoltageCheck();
  }

  DEBUG_PRINT(F("Battery Level: "));
  DEBUG_PRINTLN(BatteryLevel());

  DEBUG_PRINT(F("Battery Voltage: "));
  DEBUG_PRINTLN(BatteryVoltage());
  return VoltageCheck();
}

int BatteryLevel() {
  int voltage = analogRead(PA_SENSOR_BATT);

  int i;

  while (voltage < batteryLevelVoltage[i]) {
    i++;
  }

  return batteryLevelPct[i];
}

float BatteryVoltage() {
  int voltageInt = analogRead(PA_SENSOR_BATT);

  float voltageRead = voltageInt * 5.0f / 1024.0f;

  float voltageBattery = voltageRead * 129.2f / 19.2f;

  return voltageBattery;
}

// Sets charge flag based on current battery level, once charge started will charge until BATTERY_STD_RECHARGE before resuming
bool PowerCycle() {
  int batteryLevel = BatteryLevel();

  if (!charge) {
    if (batteryLevel > BATTERY_STD_RECHARGE) {
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Battery Recharged to > 80%"));
      DEBUG_PRINTLINE();
      charge = true;
    }
  }

  if (charge && batteryLevel < BATTERY_STD_CHARGE) {
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Battery Level < 20%, starting charge cycle"));
    DEBUG_PRINTLINE();
    charge = false;
  }

  return charge;
}


void ResetPowerCycle(){
  charge = true;
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
/*
  GeoRover local storage protocols

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/
#include <SD.h>

// Initialize SD card reader module.
bool InitializeSDReader() {
  bool status = false;
  if (!SDReaderStatus()) {
    status = SD.begin(PO_SPISS_SDCARD);

    SetStatus(MODULE_SD, status);
  }
  return status;
}

void TerminateSDReader() {
  SD.end();
  SetStatus(MODULE_SD, false);
}
// Checks status of SD reader
bool SDReaderStatus() {
  return GetStatus(MODULE_SD);
}

// Query all files on SD card and print to serial prompt
void SDQuery() {
  if (SDReaderStatus()) {
    File file = SD.open("/");
    file.rewindDirectory();
    DBG_ONLY(printFiles(file));
    file.rewindDirectory();
    file.close();
    delay(10);
  }
}

// Print all files to serial port
void printFiles(File dir) {
  DEBUG_PRINTLN(F(("Files in system:")));
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      File entry = dir.openNextFile();
      if (!entry)
        break;
    }

    if (!entry.isDirectory()) {
      DEBUG_PRINT(entry.name());
      DEBUG_PRINT(F(("\t\t")));
      DEBUG_PRINTLN2(entry.size(), DEC);
    }
    entry.close();
  }
  DEBUG_PRINTLN(F(("End of storage")));
  DEBUG_PRINTLINE();
}

// Print size of file on SD card
void SDSize(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    DEBUG_PRINT(F("Opening file: "));
    DEBUG_PRINTLN(fileName);
    File file = SD.open(fileName);
    if (file) {
      DEBUG_PRINTLN("File size: " + (String)file.size() + " bytes");
      file.close();
    } else {
      DEBUG_PRINTLN(F("File not found!"));
    }
  }
}

// Print all data from datafile to serial port
void SDDownload(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    DEBUG_PRINTLN(F("Downloading file: "));
    DEBUG_PRINTLN(fileName);
    File file = SD.open(fileName);
    if (file) {
      while (file.available()) {
        DEBUG_WRITE(file.read());
      }
      file.close();
      DEBUG_PRINTLN(F("End of File"));
    } else {
      DEBUG_PRINTLN(F("File not found!"));
    }
  }
}

// Delete file on SD card
void SDDelete(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    if (SD.exists(fileName)) {
      DEBUG_PRINTLN(F("Deleting file: "));
      DEBUG_PRINTLN(fileName);
      SD.remove(fileName);
      DEBUG_PRINTLN(F("File Removed"));
    } else {
      DEBUG_PRINTLN(F("File not found"));
    }
  }
}

// Create empty file on SD card
void SDCreate(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    if (SD.exists(fileName)) {
      DEBUG_PRINTLN(F("File already exist"));
    } else {
      DEBUG_PRINTLN(F("Creating file: "));
      DEBUG_PRINTLN(fileName);
      File file = SD.open(fileName, FILE_WRITE);
      file.close();
    }
  }
}

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SensorStack.ino"
/*
  GeoRover Sensor Stack

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

// void InitializeSensorStack() {
//   DEBUG_PRINT("Initializing Sensor stack... ");
//   DEBUG_PRINT("Initializing Communication... ");
//   Wire.begin();
//   DEBUG_PRINTLN("Communication Active.");
// }

// // Checks GNSS signal availability
// bool GPSValid() {
//   bool valid = true;

//   // Signal valid?

//   return valid;
// }

// // Checks temperature sensor availability
// bool TempSensorCheck() {
//   bool valid = true;

//   // Currently measuring?

//   return valid;
// }

// // Checks accelerometer sensor availability and values within limits
// bool AccelSensorCheck() {
//   bool valid = true;

//   // Currently measurements valid?

//   return valid;
// }

#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
byte currentLedStatus = B000;
unsigned long lastMillisLed;
unsigned long millisSinceLast;
unsigned long lastMillisModeBlinkState;

// Set LED
void InitStatusLed() {
  // LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_SHORT, LED_BLINK_VERY_SHORT);
  // LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_SHORT, 0);
}

bool LedStatus() {
  return (digitalRead(PO_LED_STATUS_GRN) || digitalRead(PO_LED_STATUS_YEL) || digitalRead(PO_LED_STATUS_GRN));
}

// Sets current LED Status from byte
// [100] [RED - -]
// [010] [- YEL -]
// [001] [- - GRN]
void LedSet(byte color) {
  // Do nothing if arguments are the same
  if (color == currentLedStatus)
    return;

  digitalWrite(PO_LED_STATUS_GRN, (BINARY_CODE_LED_GRN & color) > 0);
  digitalWrite(PO_LED_STATUS_YEL, (BINARY_CODE_LED_YEL & color) > 0);
  digitalWrite(PO_LED_STATUS_RED, (BINARY_CODE_LED_RED & color) > 0);

  currentLedStatus = color;
}

void LedSetSignal(LED_SIGNAL signal){
  switch (signal)
  {
  case LED_SIGNAL_OK:
    LedSet(BINARY_CODE_LED_GRN);
    break;
  case LED_SIGNAL_IDLE:
    LedSet(BINARY_CODE_LED_YEL);
    break;
  case LED_SIGNAL_LOADING:
    LedSet(BINARY_CODE_LED_YEL);
    break;
  case LED_SIGNAL_ERROR:
    LedSet(BINARY_CODE_LED_RED);
    break;
  default:
    LedSet(0);
    break;
  }
}

void LedSetSignal(){
  LedSet(0);
}

// Sets current LED Status from byte and alternate blink based on input durations
// [100] [RED - -]
// [010] [- YEL -]
// [001] [- - GRN]
bool LedBlink(byte color, unsigned int onDuration, unsigned int offDuration) {
  millisSinceLast = millis() - lastMillisLed;
  bool cycleDone  = false;
  if (currentLedStatus > 0 && millisSinceLast > onDuration) {
    LedSet(0);
    lastMillisLed = millis();
    cycleDone     = true;
  }
  else if (currentLedStatus == 0 && millisSinceLast > offDuration) {
    LedSet(color);
    lastMillisLed = millis();
  }
  return cycleDone;
}

int LedColor = 0;
void LedBlink(byte colorOne, byte colorTwo, unsigned int onDuration, unsigned int offDuration) {
  switch (LedColor) {
    case 0:
      if (LedBlink(colorOne, onDuration/2, offDuration)) LedColor++;
      break;
    case 1:
      if (LedBlink(colorTwo, onDuration/2, 0)) LedColor++;
      break;
    default:
      LedColor = 0;
      break;
  }
}

// Resets current flashing LED and resets timers
void ResetLed(){
  lastMillisLed = 0;
  LedColor = 0;
  LedSet(0);
}

// Flash LED in specified color while halting system for specified duration
void LedBlinkHalt(byte color, unsigned int duration) {
  LedSet(color);
  delay(duration);
  LedSet(0);
}

// Flash LED in specified color while halting system for specified duration, during and after LED duration.
void LedBlinkHalt(byte color, unsigned int duration, unsigned int afterHalt) {
  LedSet(color);
  delay(duration);
  LedSet(0);
  delay(afterHalt);
}

// Flahses LED once in specified color (Halts program)
void LedBlinkSingleShort(byte color) {
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT);
}

// Flashes LED twice in specified color (very short duration)
void LedBlinkDoubleShort(byte color) {
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT);
}

// Flashes LED twice in specified colors (very short duration)
void LedBlinkDoubleShort(byte color1, byte color2) {
  LedBlinkHalt(color1, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color2, LED_BLINK_VERY_SHORT);
}

// Flashes LED twice in specified color (very short duration)
void LedBlinkTripleShort(byte color) {
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT);
}

// Flashes LED twice in specified colors (very short duration)
void LedBlinkTripleShort(byte color1, byte color2, byte color3) {
  LedBlinkHalt(color1, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color2, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color3, LED_BLINK_VERY_SHORT);
}

void StrategyStartLed() {
  // switch (mode) {
  //   case MODE_EMERGENCY:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_RED);
  //     break;
  //   case MODE_IDLE:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_YEL);
  //     break;
  //   case MODE_SYSTEMTEST:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_YEL, BINARY_CODE_LED_GRN);
  //     break;
  //   case MODE_REMOTECONTROL:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_GRN, BINARY_CODE_LED_YEL);
  //     break;
  //   case MODE_AUTONOMOUS:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
  //     break;
  //   case MODE_MODELIBRARY:
  //     LedBlinkSingleShort(BINARY_CODE_LED_YEL);
  //     break;
  //   default:
  //     break;
  // }
}

// Blink light of given mode (Non-blocking)
void StrategyRunLed(byte mode) {
  switch (mode) {
    case MODE_EMERGENCY:
      LedBlink(BINARY_CODE_LED_RED, LED_BLINK_LONG, LED_BLINK_LONG);
      break;
    case MODE_IDLE:
      LedBlink(BINARY_CODE_LED_YEL, LED_BLINK_LONG, LED_BLINK_LONG);
      break;
    case MODE_SYSTEMTEST:
      LedBlink(BINARY_CODE_LED_RED, BINARY_CODE_LED_YEL, LED_BLINK_LONG, LED_BLINK_LONG);
      break;
    case MODE_REMOTECONTROL:
      LedBlink(BINARY_CODE_LED_GRN, BINARY_CODE_LED_YEL, LED_BLINK_LONG, LED_BLINK_LONG);
      break;
    case MODE_AUTONOMOUS:
      LedBlink(BINARY_CODE_LED_GRN, LED_BLINK_LONG, LED_BLINK_LONG);
      break;
    default:
      break;
  }
}

// Blink light of current mode (Non-blocking)
void StrategyRunLed() {
  StrategyRunLed(mode);
}

// Blink light with specific signal (Non-blocking)
void StatusRunLed(LED_SIGNAL signal) {
  switch (signal) {
    case LED_SIGNAL_OK:
      LedBlink(BINARY_CODE_LED_GRN, LED_BLINK_SHORT, LED_BLINK_VERY_LONG);
      break;
    case LED_SIGNAL_ERROR:
      LedBlink(BINARY_CODE_LED_RED, LED_BLINK_SHORT, LED_BLINK_VERY_LONG);
      break;
    case LED_SIGNAL_IDLE:
      LedBlink(BINARY_CODE_LED_YEL, LED_BLINK_SHORT, LED_BLINK_VERY_LONG);
      break;
    case LED_SIGNAL_LOADING:
      LedBlink(BINARY_CODE_LED_YEL, BINARY_CODE_LED_GRN, LED_BLINK_SHORT, LED_BLINK_VERY_LONG);
      break;
    default:
      break;
  }
}

// Blink light with specific signal (Blocking)
void StatusHaltLed(LED_SIGNAL signal) {
  switch (signal) {
    case LED_SIGNAL_OK:
      LedBlinkHalt(BINARY_CODE_LED_GRN, LED_BLINK_NORMAL);
      break;
    case LED_SIGNAL_OK_SHORT:
      LedBlinkHalt(BINARY_CODE_LED_GRN, LED_BLINK_VERY_SHORT);
      break;
    case LED_SIGNAL_OK_SHORT_HALT:
      LedBlinkHalt(BINARY_CODE_LED_GRN, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
      break;
    case LED_SIGNAL_ERROR:
      LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_NORMAL);
      break;
    case LED_SIGNAL_ERROR_SHORT:
      LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_VERY_SHORT);
      break;
    case LED_SIGNAL_ERROR_SHORT_HALT:
      LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
      break;
    case LED_SIGNAL_IDLE:
      LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_SHORT);
      break;
    case LED_SIGNAL_LOADING:
      LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_SHORT, LED_BLINK_SHORT);
      LedBlinkHalt(BINARY_CODE_LED_GRN, LED_BLINK_SHORT);
      break;
    default:
      break;
  }
}

void StrategyFinishLed(int strategy) {
}

void ModuleStartLed(int module) {
}

void ModuleStopLed(int module) {
}

void LedTest() {
  Serial.println("Red");
  LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_VERY_LONG, LED_BLINK_LONG);
  Serial.println("Yellow");
  LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_VERY_LONG, LED_BLINK_LONG);
  Serial.println("Green");
  LedBlinkHalt(BINARY_CODE_LED_GRN, LED_BLINK_VERY_LONG, LED_BLINK_LONG);

  // LedBlinkDoubleShort(BINARY_CODE_LED_RED,BINARY_CODE_LED_RED);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_RED,BINARY_CODE_LED_YEL);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_RED,BINARY_CODE_LED_GRN);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_YEL,BINARY_CODE_LED_RED);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_YEL,BINARY_CODE_LED_YEL);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_YEL,BINARY_CODE_LED_GRN);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_GRN,BINARY_CODE_LED_RED);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_GRN,BINARY_CODE_LED_YEL);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_GRN,BINARY_CODE_LED_GRN);
}
#line 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
// Set pointers for strategies methods
void InitStrategyMethods() {
  strategyMethods[0][MODE_EMERGENCY] = StartStrategyEmergency;
  strategyMethods[1][MODE_EMERGENCY] = RunStrategyEmergency;
  strategyMethods[2][MODE_EMERGENCY] = FinishStrategyEmergency;
  strategyMethods[3][MODE_EMERGENCY] = SelectFunctionEmergency;

  strategyMethods[0][MODE_MODELIBRARY] = StartStrategyModeLibrary;
  strategyMethods[1][MODE_MODELIBRARY] = RunStrategyModeLibrary;
  strategyMethods[2][MODE_MODELIBRARY] = FinishStrategyModeLibrary;
  strategyMethods[3][MODE_MODELIBRARY] = SelectFunctionModeLibrary;

  strategyMethods[0][MODE_IDLE] = StartStrategyIdle;
  strategyMethods[1][MODE_IDLE] = RunStrategyIdle;
  strategyMethods[2][MODE_IDLE] = FinishStrategyIdle;
  strategyMethods[3][MODE_IDLE] = SelectFunctionIdle;

  strategyMethods[0][MODE_SYSTEMTEST] = StartStrategySystemTest;
  strategyMethods[1][MODE_SYSTEMTEST] = RunStrategySystemTest;
  strategyMethods[2][MODE_SYSTEMTEST] = FinishStrategySystemTest;
  strategyMethods[3][MODE_SYSTEMTEST] = SelectFunctionSystemTest;

  strategyMethods[0][MODE_REMOTECONTROL] = StartStrategyRemote;
  strategyMethods[1][MODE_REMOTECONTROL] = RunStrategyRemote;
  strategyMethods[2][MODE_REMOTECONTROL] = FinishStrategyRemote;
  strategyMethods[3][MODE_REMOTECONTROL] = SelectFunctionRemote;

  strategyMethods[0][MODE_AUTONOMOUS] = StartStrategyAutonomous;
  strategyMethods[1][MODE_AUTONOMOUS] = RunStrategyAutonomous;
  strategyMethods[2][MODE_AUTONOMOUS] = FinishStrategyAutonomous;
  strategyMethods[3][MODE_AUTONOMOUS] = SelectFunctionAutonomous;
}

// Set last mode from EEPROM
void InitMode() {
  mode = EEPROM.read(MEMADDR_LASTMODE);
  if (!digitalRead(PI_BUTTON_ESTOP)) {
    mode = MODE_EMERGENCY;
  }
  if (mode == MODE_MODELIBRARY) mode = MODE_IDLE;

  SetMode(mode);

  strategyMethods[0][mode]();
}

// Checks if mode is updated and finish exit operations before changing
void ModeUpdater() {
  if (!isModeUpdated) {
    return;
  }

  isModeUpdated = false;
  // Skip finish operation when going to emergency
  if (mode != MODE_EMERGENCY) {
    strategyMethods[2][prevMode]();  // finish any operations for prevMode here
  }

  strategyMethods[0][mode]();  // init new strategy according to the new mode value
}

void AttachSelectButton() {
  lastMillisSelect = millis();
  attachInterrupt(PI_INT_BUTTON_SELECT, strategyMethods[3][mode], FALLING);
}

void AttachModeButton() {
  attachInterrupt(PI_INT_BUTTON_MODE, ModeButtonInterruptHandler, FALLING);
}

void DetachModeButton() {
  detachInterrupt(PI_INT_BUTTON_MODE);
}

void DetachSelectButton() {
  detachInterrupt(PI_INT_BUTTON_SELECT);
}

// Tries set the mode and isModeUpdated flag
boolean SetMode(byte newMode) {
  if (newMode < MODES_MAX) {
    prevMode      = mode;
    mode          = newMode;
    IncrementModeCycle();
    isModeUpdated = true;
    EEPROM.write(MEMADDR_LASTMODE, mode);
    DEBUG_PRINT(F("Mode Set: "));
    DEBUG_PRINTLN(ModeToString(mode));
    return true;
  }
  return false;
}

// Cycle Mode
void IncrementModeCycle() {
  if(mode == MODE_MODELIBRARY){
    return;
  }
  modeCycle = mode;
  if (modeCycle + 1 < MODES_MIN_BROWSABLE || modeCycle + 1 >= MODES_MAX) {
    modeCycle = MODES_MIN_BROWSABLE;
  } else {
    modeCycle = modeCycle + 1;
  }
}

