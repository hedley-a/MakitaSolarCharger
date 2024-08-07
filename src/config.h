

// Constants to be checked during commission. 

const int Batt100pcV = 1410; // 100% voltage for MapBatteryVolts - dependent on Epever
const int Batt100pcI = 30; // Idle curent@14.2Volts guesstimate 0.3 amps
//const int SunsOut = 1800;  // PV Volts x 100 
const int BurdenWhDay  = 168;  // 16.8Wh per day  50ma@14 Volts equivalent to 0.4% battery charge

const int LCDpageDly = 15000;              // interval at which display reverts to home
unsigned long menuStart;
const unsigned long menuTimeout = 15000;
//bool sleepState = false;
//bool idleState = true;
//bool deferState = false;
//bool chargeState = false;
//bool postChrge = false;
const int temperatureDefer = 35; // upper limit to start charge
const int temperatureStart = 30;  // stop during charge
const int temperatureOver = 48;  // stop during charge


byte oldSwitchState = HIGH;  // switch open because of pull-up resistor
const unsigned long debounceTime = 40;  // milliseconds

//statetimeouts
const unsigned long idleTimeout = 15000;    // 30 S 
const unsigned long deferTimeout = 86400000; // 24 Hours
const unsigned long chrgTimeout = 8400000;  // 140 mins.
const unsigned long overTimeout = 43200000;  // 12 hrs.
const unsigned long postTimeout = 15000;


//VARIABLES 

// INT Vols
int chargeTime;
int8_t solarBattIdleTime = 0;
int8_t battIdle = 0;
int8_t loadIdleDly = 0;
//unsigned long time2It;
unsigned long displayUpdate;
volatile unsigned long currentMillis;
unsigned long previousMillis;
unsigned long buttonInhibit;
unsigned long stateTimeout;  // delay timeout in all states 
unsigned long stateStartTime;
uint8_t  readDataCount = 9; // counts ten calls before sampling powers every 20 Seconds

volatile unsigned long pirIntTime;
volatile unsigned long clkIntTime;
volatile unsigned long readIntTime;
//volatile unsigned long stateStartTick;
volatile uint16_t long countReadInts;
volatile bool firstState = true; // starts display on first boot
int lastStartState = HIGH;
int lastStopState = HIGH;
int lastMenuState = HIGH;

volatile bool pirIntFlg = false;
volatile bool clkIntFlg = false;
//volatile bool clk20sINT;

uint32_t  UnixTime = 0;
uint8_t elapsedHrs;
uint8_t elapsedMin;
uint8_t elapsedSec;

uint8_t ccDow;
uint8_t ccHr;
uint8_t ccMin;

long  temperatureInternal = 0;

bool postCharge = false;
bool chargerIdle = true;
uint16_t count20Secs = 0;
uint8_t stateStartDC;

uint8_t  Count180 = 0; // one hours worth of 20's

bool  SolBattLow = 0; // set if battery < 25% at start of charge - delay until clear

//Makita Battery globals

int8_t DisplayPower [4] = {};
int16_t DisplayPwrTot = 0;
int16_t loadPower = 0;
uint32_t demandPower [4] = {};

//int16_t TotalWh = 0;
//int16_t WattHrs [4] = {0,0,0,0};  // display values
int32_t adcValue;      // array for reading adc vals
const int16_t adc_idle [4] = {20087,20062,20074,20120};      
const int8_t adc_scale =  68;

//Solar Battery globals
int32_t demandSecs;
int32_t chargeSecs;
int8_t count20min;
int32_t demandWattHours;
int32_t chargeWattHours;
int32_t startWattHours;
int32_t energyMeter;

int16_t wattOneHr = 0;  // 300Ah@ 14Volts = 4200


int32_t watt20secs = 0;

///int32_t Solar_Power = 0;      // display values
//int32_t Solar_WattSec = 0;  // display values
//int16_t SolarTotalWh = 0;
//int16_t Solar_Watt_Hrs = 0;  // display values

// watt hour counter 
//int16_t DiscWh_count = 0; // Makita LCD updates avoid DIV 0
//int16_t ChrgWh_count = 0; // no of samples counter.avoid DIV 0

//int8_t count20s = 0;
//int8_t count10m = 0;



// Pins

#define ledPin           13     // int 0 used for control input

// IO pin assignments------------------------------------------------------

#define pirIN           2   //PD2      // PIR input to wake the controller 
#define clockIN         3   //PD3      // Start Charge Pushbutton 
#define chrgLED         4   //PD4      // Indicate Charging
#define LEDchrgd        5   //PD5      // Indicate Charged
#define buttonStart     6
#define buttonStop      7  //PB2      // Pushbutton
#define MAX485_DE       8   //data or
#define MAX485_RE       9  // recv enable

// AltSerialRx
// AltSerialTx

//From bottom RHS
#define Enble05V         10  //PC1      // 5VSW_EN
#define mode01           11
#define mode02           12
#define buttonMenu       14  //PB3      // Pushbutton    
#define Enble24V         15  //PC3      // Turns on 24 Volts
#define balON            16  //PC2      // Battery equaliser 
#define EnblRS485        17
                         


// pin assignments
const byte startSwitch = buttonStart;
const byte stopSwitch = buttonStop;
const byte menuSwitch = buttonMenu;


// END IO pin assignments------------------------------------------------------

// ModBus Register Locations
#define LIVE_DATA       0x3100     // start of live-data 
#define LIVE_DATA_CNT   16         // 16 regs

#define PANEL_VOLTS     0x00       
#define PANEL_AMPS      0x01
#define PANEL_POWER_L   0x02
#define PANEL_POWER_H   0x03

#define BATT_VOLTS      0x04
#define BATT_AMPS       0x05
#define BATT_POWER_L    0x06
#define BATT_POWER_H    0x07
#define CHARGE_MODE     0x08

#define LOAD_VOLTS      0x0C
#define LOAD_AMPS       0x0D
#define LOAD_POWER_L    0x0E
#define LOAD_POWER_H    0x0F

#define CCMODEL         0x2b

#define RTC_CLOCK           0x9013  // D7-0 Sec, D15-8 Min  :   D7-0 Hour, D15-8 Day  :  D7-0 Month, D15-8 Year
#define RTC_CLOCK_CNT       3       // 3 regs

#define CASE_TEMPERATURE    0x3111  // Case temp - AH 1406
#define SINK_TEMPERATURE    0x3112  // Heatsink temp - AH 1406
#define BATTERY_SOC         0x311A  // State of Charge in percent, 1 reg

#define BATT_TEMPERATURE    0x3110  // Batery Temp

#define BATTERY_CURRENT_L   0x331B  // Battery current L
#define BATTERY_CURRENT_H   0x331C  // Battery current H

#define STATISTICS      0x3300 // start of statistical data
#define STATISTICS_CNT  22     // 22 regs


uint16_t ErrorCounter=0;
uint8_t  solarBatterySOC;
//uint8_t  dummyBat = 100;
//uint16_t ChargingStatus;
//uint16_t SolarVoltage;
//uint16_t SolarAmps;
//uint16_t SolarWattage;
//uint16_t BatteryVoltage;
//uint16_t BatteryAmps;
//uint16_t BatteryWattage;
//uint16_t BatteryStatus;
//uint16_t LoadVoltage;
//uint16_t LoadAmps;
//uint16_t LoadWattage;
//uint16_t LoadStatus;
//uint16_t LoadSwitchstate;
//uint16_t Status;
//uint16_t TPPassthrough;
//uint16_t SinkTemperature;

//uint16_t OverVoltDist;
//uint16_t OverVoltRecon;
//uint16_t EQChargeVolt;
//uint16_t BoostChargeVolt;
//uint16_t FloatChargeVolt;
//uint16_t BoostReconChargeVolt;
//uint16_t BatteryChargePercent;
//uint16_t ChargeLimitVolt;
//uint16_t DischargeLimitVolt;
//uint16_t LowVoltDisconnect;
//uint16_t LowVoltReconnect;
//uint16_t UnderVoltWarningVolt;
//uint16_t UnderVoltReconnectVolt;
//uint16_t BatteryDischargePercent;
//uint16_t BoostDuration;
//uint16_t EQDuration;
//uint16_t BatteryCapactity;
//uint16_t BatteryType;
//uint16_t ConsumedEnergyToday;
//uint16_t GeneratedEnergyMonth;
//uint16_t GeneratedEnergyYear;
//uint16_t TotalGeneratedEnergy;
//uint16_t NetBatteryCurrent;
uint16_t BattTemperature;
uint16_t CaseTemperature;
//uint16_t EQChargeVoltValue;
uint8_t i, result;

  // datastructures, also for buffer to values conversion
  //
  // clock
  union {
    struct {
    uint8_t  s;
    uint8_t  m;
    uint8_t  h;
    uint8_t  d;
    uint8_t  M;
    uint8_t  y;  
   } r;
    uint16_t buf[3];
  } rtc ;


  // live data
  union {
    struct {

      int16_t  pV;
      int16_t  pI;
      int32_t  pP;
   
      int16_t  bV;
      int16_t  bI;
      int32_t  bP;
      
      
      uint16_t  dummy[4];
      
      int16_t  lV;
      int16_t  lI;
      int32_t  lP; 

    } l;
    uint16_t  buf[16];
  } live;
  /* 
// statistics
  union {
    struct {
  
      // 4*1 = 4
      uint16_t  pVmax;
      uint16_t  pVmin;
      uint16_t  bVmax;
      uint16_t  bVmin;
  
      // 4*2 = 8
      uint32_t  consEnerDay;
      uint32_t  consEnerMon;
      uint32_t  consEnerYear;
      uint32_t  consEnerTotal;
  
      // 4*2 = 8
      uint32_t  genEnerDay;
      uint32_t  genEnerMon;
      uint32_t  genEnerYear;
      uint32_t  genEnerTotal;
  
      // 1*2 = 2
      uint32_t  c02Reduction;
     
    } s;
    uint16_t  buf[22];  
  } stats;
  */
  // these are too far away for the union conversion trick
  uint16_t batterySOC = 0;
  int32_t  batteryCurrent = 0;
      
  // battery status
  struct {
    uint8_t volt;        // d3-d0  Voltage:     00H Normal, 01H Overvolt, 02H UnderVolt, 03H Low Volt Disconnect, 04H Fault
    uint8_t temp;        // d7-d4  Temperature: 00H Normal, 01H Over warning settings, 02H Lower than the warning settings
    uint8_t resistance;  // d8     abnormal 1, normal 0
    uint8_t rated_volt;  // d15    1-Wrong identification for rated voltage
  } status_batt;

  const char* batt_volt_status[] = {
    "Normal",
    "Overvolt",
    "Low Volt Disconnect",
    "Fault"
  };
  
  const char* batt_temp_status[] = {
    "Normal",
    "Over WarnTemp",
    "Below WarnTemp"
  };

  uint8_t charger_mode  = 0;

  const char* charger_charging_status[] = {
    "Off",
    "Float",
    "Boost",
    "Equalization"
  };

/*
// just for reference, not used in code
#define PV_MAX     0x00 // Maximum input volt (PV) today  
#define PV_MIN     0x01 // Minimum input volt (PV) today
#define BATT_MAX   0x02 // Maximum battery volt today
#define BATT_MIN   0x03 // Minimum battery volt today

#define CONS_ENERGY_DAY_L   0x04 // Consumed energy today L
#define CONS_ENGERY_DAY_H   0x05 // Consumed energy today H
#define CONS_ENGERY_MON_L   0x06 // Consumeduint16_t BatteryVoltage; energy this month L 
#define CONS_ENGERY_MON_H   0x07 // Consumed energy this month H
#define CONS_ENGERY_YEAR_L  0x08 // Consumed energy this year L
#define CONS_ENGERY_YEAR_H  0x09 // Consumed energy this year H
#define CONS_ENGERY_TOT_L   0x0A // Total consumed energy L
#define CONS_ENGERY_TOT_H   0x0B // Total consumed energy  H

#define GEN_ENERGY_DAY_L   0x0C // Generated energy today L
#define GEN_ENERGY_DAY_H   0x0D // Generated energy today H
#define GEN_ENERGY_MON_L   0x0E // Generated energy this month L
#define GEN_ENERGY_MON_H   0x0F // Generated energy this month H
#define GEN_ENERGY_YEAR_L  0x10 // Generated energy this year L
#define GEN_ENERGY_YEAR_H  0x11 // Generated energy this year H
#define GEN_ENERGY_TOT_L   0x12 // Total generated energy L
#define GEN_ENERGY_TOT_H   0x13 // Total Generated energy  H

#define CO2_REDUCTION_L    0x14 // Carbon dioxide reduction L  
#define CO2_REDUCTION_H    0x15 // Carbon dioxide reduction H 

#define LOAD_STATE         0x02 // r/w load switch state

#define STATUS_FLAGS      0x3200
#define STATUS_BATTERY    0x00  // Battery status register
#define STATUS_CHARGER    0x01  // Charging equipment status register
#define BATTERY_TYPE      0x9000  // Battery Type 0001H- Sealed , 0002H- GEL, 0003H- Flooded, 0000H- User defined
#define BATTERY_CAPACITY  0x9001  // Rated capacity of the battery in Ah
#define HIGH_VOLTAGE_DISCONNECT 0x9003  //
#define CHARGING_LIMIT_VOLT     0x9004  //
#define EQ_CHARGE_VOLT          0x9006
*/

