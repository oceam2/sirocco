
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////      VERSION PARA OLED     ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

/////////////////   Uno
//////////////////  Frecuencímetro
//////////////////  VERSION PARA LCD AZUL

#include "FreeRTOS_AVR.h"
#include <AESLib.h>

/* Demo includes. */
#include "basic_io_avr.h"
#include <stdlib.h>
#include "mcp_can_j1939_v2.h"
#include "can_ext_j1939_v2.h"
#include "OLedI2C.h"
#include <EEPROM.h>
//#include <vld.h>

OLedI2C LCD;

uint8_t alive_led = 0;   // 1=Alive 0=Led recepción
#define cases 50 // último menú+1
#define num_menu cases+2
#define Serial_N "S/N: BEGAS163133"
#define Options  "Version 5.2.8   "
#define PGN_extended 0  // 0=126 SPNs  1=360 SPNs

#if PGN_extended
#define tope_menu 359    // Modelo II
#define mem_tramas_can 100  // normalmente 200
#define mem_tasks 200  // normalmente 200
#define seguridad_activada 0
#else
 //#define tope_menu 125  //  Modelo I
#define tope_menu 135  //  Modelo I
#define mem_tramas_can 200  // normalmente 200
#define mem_tasks 200  // normalmente 200
#define seguridad_activada 0
#endif

#define restaurar_menu 1
#define key_1 { 0,11,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 }
#define key_2 { 0, 2,4,6,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,33 }

#define mem_tramas_can 200  // normalmente 200
#define mem_tasks 200  // normalmente 200

#define msgEngineTemperature_PGN 65262 //0xfeee
#define Engine_Total_Average_Fuel_Rate_PGN 65101 //0xfe4d
#define Specific_Heat_Ratio_PGN 65109 //fe55
#define Engine_Cylinder_1_Combustion_Time_PGN 65147 //fe7b
#define Engine_Fuel_Flow_Rate_PGN 65153 //fe81
#define Engine_Cylinder_1_Ignition_Timing_PGN 65154 //fe82
#define Engine_Power_PGN 65168 //fe90
#define Engine_Actual_Exhaust_Oxygen_PGN 65193 //fea9
#define Trip_Average_Fuel_Rate_PGN 65203 //feb3
#define Trip_Cruise_Distance_PGN 65206 //feb6
#define Engine_Total_Hours_of_Operation_PGN 65253 //fee5
#define Engine_Trip_Fuel_PGN 65257 //fee9

// Base de datos J1939
#include <avr/pgmspace.h>

#if !PGN_extended

const char string_1[] PROGMEM = "- -     ---- ---- - - - --- - - - -";
const char string_2[] PROGMEM = "0 8     10ms TSC1 4 1 8  518 Engine_Requested_Torque/Torque_Limit 1_%/bit -125 %";
const char string_3[] PROGMEM = "0 8     10ms TSC1 2 1 16  898 Engine_Requested_Speed/Speed_Limit 0.125_rpm/bit 0 rpm";
const char string_4[]  PROGMEM = "256 8 50ms TC1 2 1 8 684 Requested_Percent_Clutch_Slip 0.4_%/bit 0 %";
const char string_5[] PROGMEM = "256 8   50ms TC1 6 5 2 1854 Transmission_Mode_3 4_states/2_bit 0 bit";
const char string_6[] PROGMEM = "256 8   50ms TC1 6 7 2 1855 Transmission_Mode_4 4_states/2_bit 0 bit";
const char string_7[] PROGMEM = "53248 8  100ms  CL 1 1 8 1487 Illumination_Brightness_Percent 0.4_%/bit 0 %";
const char string_8[] PROGMEM = "57344 8 1000ms CM1 1 1 8 986 Requested_Percent_Fan_Speed 0.4_%/bit 0 %";
const char string_9[] PROGMEM = "57344 8 1000ms CM1 5 7 2 1653 Vehicle_Limiting_Speed_Governor_Enable_Switch 4_states/2_bit 0 bit";
const char string_10[] PROGMEM = "57344 8 1000ms CM1 5 5 2 1654 Vehicle_Limiting_Speed_Governor_Increment_Switch 4_states/2_bit 0 bit";
const char string_11[] PROGMEM = "57344 8 1000ms CM1 5 3 2 1655 Vehicle_Limiting_Speed_Governor_Decrement_Switch 4_states/2_bit 0 bit";
const char string_12[] PROGMEM = "57344 8 1000ms CM1 6 7 2 1656 Engine_Automatic_Start_Enable_Switch 4_states/2_bit 0 bit";
const char string_13[] PROGMEM = "57344 8 1000ms CM1 6 5 2 1666 Automatic_Gear_Shifting_Enable_Switch 4_states/2_bit 0 bit";
const char string_14[] PROGMEM = "57344 8 1000ms CM1 4 3 2 1682 Battery_Main_Switch_Hold_Request 4_states/2_bit 0 bit";
const char string_15[] PROGMEM = "57344 8 1000ms CM1 7 1 4 1683 Auxiliary_Heater_Mode_Request 16_states/4_bit 0 bit";
const char string_16[] PROGMEM = "57344 8 1000ms CM1 4 1 2 1684 Auxiliary_Heater_Coolant_Pump_Request 4_states/2_bit 0 bit";
const char string_17[] PROGMEM = "57344 8 1000ms CM1 7 5 2 1685 Request_Engine_Zone_Heating 4_states/2_bit 0 bit";
const char string_18[] PROGMEM = "57344 8 1000ms CM1 7 7 2 1686 Request_Cab_Zone_Heating 4_states/2_bit 0 bit";
const char string_19[] PROGMEM = "57344 8 1000ms CM1 2 1 16 1691 Cab_Interior_Temperature_Command 0.03125_deg_C/bit -273 C";
const char string_20[] PROGMEM = "57344 8 1000ms CM1 4 5 2 1714 Operator_Seat_Direction_Switch 4_states/2_bit 0 bit";
const char string_21[] PROGMEM = "57344 8 1000ms CM1 4 7 2 1856 Seat_Belt_Switch 4_states/2_bit 0 bit";
const char string_22[] PROGMEM = "57344 8 1000ms CM1 8 1 8 2596 Selected_Maximum_Vehicle_Speed_Limit 1_count/bit 0 count";
const char string_23[] PROGMEM = "57344 8 1000ms CM1 6 1 2 3695 Diesel_Particulate_Filter_Regeneration_Inhibit_Switch 4_states/2_bit 0 bit";
const char string_24[] PROGMEM = "57344 8 1000ms CM1 6 3 2 3696 Diesel_Particulate_Filter_Regeneration_Force_Switch 4_states/2_bit 0 bit";
const char string_25[] PROGMEM = "61441 8 100ms EBC1 2 1 8 521 Brake_Pedal_Position 0.4_%/bit 0 %";
const char string_26[] PROGMEM = "61441 8 100ms EBC1 1 1 2 561 ASR_Engine_Control_Active 4_states/2_bit 0 bit";
const char string_27[] PROGMEM = "61441 8 100ms EBC1 1 3 2 562 ASR_Brake_Control_Active 4_states/2_bit 0 bit";
const char string_28[] PROGMEM = "61441 8 100ms EBC1 1 5 2 563 Anti-Lock_Braking_(ABS)_Active 4_states/2_bit 0 bit";
const char string_29[] PROGMEM = "61441 8 100ms EBC1 3 1 2 575 ABS_Off-road_Switch 4_states/2_bit 0 bit";
const char string_30[] PROGMEM = "61441 8 100ms EBC1 3 3 2 576 ASR_Off-road_Switch 4_states/2_bit 0 bit";
const char string_31[] PROGMEM = "61441 8 100ms EBC1 3 5 2 577 ASR_Hill_Holder_Switch 4_states/2_bit 0 bit";
const char string_32[] PROGMEM = "61441 8 100ms EBC1 4 7 2 969 Remote_Accelerator_Enable_Switch 4_states/2_bit 0 bit";
const char string_33[] PROGMEM = "61441 8 100ms EBC1 4 5 2 970 Engine_Auxiliary_Shutdown_Switch 4_states/2_bit 0 bit";
const char string_34[] PROGMEM = "61441 8 100ms EBC1 4 3 2 971 Engine_Derate_Switch 4_states/2_bit 0 bit";
const char string_35[] PROGMEM = "61441 8 100ms EBC1 4 1 2 972 Accelerator_Interlock_Switch 4_states/2_bit 0 bit";
const char string_36[] PROGMEM = "61441 8 100ms EBC1 5 1 8 973 Engine_Retarder_Selection 0.4_%/bit 0 %";
const char string_37[] PROGMEM = "61441 8 100ms EBC1 1 7 2 1121 EBS_Brake_Switch 4_states/2_bit 0 bit";
const char string_38[] PROGMEM = "61441 8 100ms EBC1 1 1 2 1238 Traction_Control_Override_Switch 4_states/2_bit 0 bit";
const char string_39[] PROGMEM = "61441 8 100ms EBC1 6 1 2 1243 ABS_Fully_Operational 4_states/2_bit 0 bit";
const char string_40[] PROGMEM = "61441 8 100ms EBC1 6 5 2 1438 ABS/EBS_Amber_Warning_Signal_(Powered_Vehicle) 4_states/2_bit 0 bit";
const char string_41[] PROGMEM = "61441 8 100ms EBC1 6 3 2 1439 EBS_Red_Warning_Signal 4_states/2_bit 0 bit";
const char string_42[] PROGMEM = "61441 8 100ms EBC1 7 1 8 1481 Source_Address_of_Controlling_Device_for_Brake_Control 1000msource_address/bit 0 SA";
const char string_43[] PROGMEM = "61441 8 100ms EBC1 8 7 2 1792 Tractor-Mounted_Trailer_ABS_Warning_Signal 4_states/2_bit 0 bit";
const char string_44[] PROGMEM = "61441 8 100ms EBC1 6 7 2 1793 ATC/ASR_Information_Signal 4_states/2_bit 0 bit";
const char string_45[] PROGMEM = "61441 8 100ms EBC1 8 5 2 1836 Trailer_ABS_Status 4_states/2_bit 0 bit";
const char string_46[] PROGMEM = "61441 8 100ms EBC1 8 3 2 2911 Halt_brake_switch 4_states/2_bit 0 bit";
const char string_47[] PROGMEM = "61442 8  10ms ETC1 2 1 16 191 Transmission_Output_Shaft_Speed 0.125_rpm/bit 0 rpm";
const char string_48[] PROGMEM = "61443 8  50ms EEC2 2 1 8 91 Accelerator_Pedal_Position_1 0.4_%/bit 0 %";
const char string_49[] PROGMEM = "61443 8  50ms EEC2 3 1 8 92 Engine_Percent_Load_At_Current_Speed 1_%/bit 0 %";
const char string_50[] PROGMEM = "61444 8  20ms EEC1 4 1 16 190 Engine_Speed 0.125_rpm/bit 0 rpm";
const char string_51[] PROGMEM = "61444 8  20ms EEC1 2 1 8 512 Driver's_Demand_Engine_-_Percent_Torque 1_%/bit -125 %";
const char string_52[] PROGMEM = "61444 8  20ms EEC1 3 1 8 513 Actual_Engine_-_Percent_Torque 1_%/bit -125 %";
const char string_53[] PROGMEM = "61444 8  20ms EEC1 1 1 4 899 Engine_Torque_Mode 16_states/4_bit 0 bit";
const char string_54[] PROGMEM = "61444 8  20ms EEC1 6 1 8 1483 Source_Address_of_Controlling_Device_for_Engine_Control 1000msource_address/bit 0 SA";
const char string_55[] PROGMEM = "61444 8  20ms EEC1 7 1 4 1675 Engine_Starter_Mode 16_states/4_bit 0 bit";
const char string_56[] PROGMEM = "61444 8  20ms EEC1 8 1 8 2432 Engine_Demand_?_Percent_Torque 1_%/bit -125 %";
const char string_57[] PROGMEM = "61444 8  20ms EEC1 1 5 4 4154 Actual_Engine_-_Percent_Torque_High_Resolution 0.125%/bit 0 %";
const char string_58[] PROGMEM = "61445 8 100ms ETC2 5 1 16 162 Transmission_Requested_Range ASCII 0 ASCII";
const char string_59[] PROGMEM = "61445 8 100ms ETC2 4 1 8 523 Transmission_Current_Gear 1_gear_value/bit -125 gear_value";
const char string_60[] PROGMEM = "61445 8 100ms ETC2 1 1 8 524 Transmission_Selected_Gear 1_gear_value/bit -125 gear_value";
const char string_61[] PROGMEM = "61445 8 100ms ETC2 2 1 16 526 Transmission_Actual_Gear_Ratio 0.001/bit 0 Ratio";
const char string_62[] PROGMEM = "61448 8  50ms HPG  1 1 16 1762 Hydraulic_Pressure 2_kPa/bit 0 kPa";
const char string_63[] PROGMEM = "61449 8  10ms VDC2 1 1 16 1807 Steering_Wheel_Angle 1/1024_rad_per_bit -31,374 deg";
const char string_64[] PROGMEM = "61449 8  10ms VDC2 4 1 16 1808 Yaw_Rate 1/8192_rad/s_per_bit -3,92 rad/s";
const char string_65[] PROGMEM = "61449 8  10ms VDC  6 1 16 1809 Lateral_Acceleration 1/2048_m/s?_per_bit -15,687 m/s2";
const char string_66[] PROGMEM = "61449 8  10ms VDC2 8 1 8 1810 Longitudinal_Acceleration 0.1_m/s?_per_bit -12,5 m/s2";
const char string_67[] PROGMEM = "61449 8  10ms VDC2 3 1 6 1811 Steering_Wheel_Turn_Counter 1_turn/bit -32 count";
const char string_68[] PROGMEM = "61449 8  10ms VDC2 3 7 2 1812 Steering_Wheel_Angle_Sensor_Type 4_states/2_bit 0 bit";
const char string_69[] PROGMEM = "61450 8  50ms EGF  1 1 16 132 Engine_Inlet_Air_Mass_Flow_Rate 0.05_kg/h_per_bit 0 kg/h";
const char string_70[] PROGMEM = "61450 8  50ms EGF1 1 1 16 2659 Engine_Exhaust_Gas_Recirculation_(EGR)_Mass_Flow_Rate 0.05_kg/h_per_bit 0 kg/h";
const char string_71[] PROGMEM = "61462 8  20ms CCS  1 1 2 3387 Engine_Cylinder_1_Combustion_Status 4_states/2_bit 0 bit";
const char string_72[] PROGMEM = "61463 8  20ms KL1  1 1 8 1352 Engine_Cylinder_1_Knock_Level 1_%/bit 0 %";
const char string_73[] PROGMEM = "61466 8  50ms TFAC 1 1 16 3464 Engine_Throttle_Actuator_1_Control_Command 0.0025_%/bit 0 %";
const char string_74[] PROGMEM = "61469 8  10ms SAS  1 1 16 3683 Steering_Wheel_Angle 1/1024_rad_per_bit -31,374 deg";
const char string_75[] PROGMEM = "64923 8 1000ms CRI1 2 1 8 3516 AftertreatmentsCR_Catalyst_Reagent_Concentration 0.25_%/bit 0 %";
const char string_76[] PROGMEM = "64923 8 1000ms CRI1 3 1 8 3518 AftertreatmentsCR_Catalyst_Reagent_Conductivity 5_microSiemens/mm 0 mS/mm";
const char string_77[] PROGMEM = "64923 8 1000ms CRI1 6 1 4 3521 AftertreatmentsCR_Catalyst_Reagent_Type 16_states/4_bit 0 bit";
const char string_78[] PROGMEM = "64925 8 1000ms SSV  1 1 16 3509 Sensor_supply_voltage_1 0.05_V/bit 0 Volts";
const char string_79[] PROGMEM = "64993 8 1000ms CACI 1 1 8 2609 Cab_A/C_Refrigerant_Compressor_Outlet_Pressure 16_kPa/bit 0 kPa";
const char string_80[] PROGMEM = "64998 8  100ms HBS 1 1 8 2580 Hydraulic_Brake_Pressure_Circuit_1 100_kPa/bit 0 kPa";
const char string_81[] PROGMEM = "64998 8  100ms HBS 3 5 2 2582 Hydraulic_Brake_Pressure_Supply_State_Circuit_1 4_states/2_bit 0 bit";
const char string_82[] PROGMEM = "64998 8  100ms HBS 3 1 2 2584 Hydraulic_Brake_Pressure_Warning_State_Circuit_1 4_states/2_bit 0 bit";
const char string_83[] PROGMEM = "65031 8  500ms ET  1 1 16 2433 Engine_Exhaust_Gas_Temperature_-_Right_Manifold 0.03125_deg_C/bit -273 C";
const char string_84[] PROGMEM = "65104 8 1000ms BT1 1 1 8 1800 Battery_1_Temperature 1_deg_C/bit -40 C";
const char string_85[] PROGMEM = "65110 8 1000ms TI1 1 1 8 1761 AftertreatmentsCR_Catalyst_Tank_Level 0.4_%/bit 0 %";
const char string_86[] PROGMEM = "65110 8 1000ms TI1 2 1 8 3031 AftertreatmentsCR_Catalyst_Tank_Temperature 1_deg_C/bit -40 C";
const char string_87[] PROGMEM = "65128 8 1000ms VF  1 1 8 1638 Hydraulic_Temperature 1_deg_C/bit -40 C";
const char string_88[] PROGMEM = "65128 8 1000ms VF 2 1 2 1713 Hydraulic_Oil_Filter_Restriction_Switch 4_states/2_bit 0 bit";
const char string_89[] PROGMEM = "65129 8 1000ms ET3 1 1 16 1636 Engine_Intake_Manifold_1_Air_Temperature_(High_Resolution) 0.03125_deg_C/bit -273 C";
const char string_90[] PROGMEM = "65129 8 1000ms ET3 3 1 16 1637 Engine_Coolant_Temperature_(High_Resolution) 0.03125_deg_C/bit -273 C";
const char string_91[] PROGMEM = "65170 8 100ms  EI  2 1 16 1209 Engine_Exhaust_Gas_Pressure 1/128_kPa/bit -250 kPa";
const char string_92[] PROGMEM = "65198 8 1000ms AIR1 1 1 8 46 Pneumatic_Supply_Pressure 8_kPa/bit 0 kPa";
const char string_93[] PROGMEM = "65245 8 1000ms TC   1 1 8 104 Engine_Turbocharger_Tube_Oil_Pressure_1 4_kPa/bit 0 kPa";
const char string_94[] PROGMEM = "65251 39 5000ms EC1 9 1 8 541 Engine_Percent_Torque_At_Point_3_(Engine_Configuration) 1_%/bit -125 %";
const char string_95[] PROGMEM = "65252 8 1000ms SHUTDN 1 7 2 590 Engine_Idle_Shutdown_Timer_State 4_states/2_bit 0 bit";
const char string_96[] PROGMEM = "65252 8 1000ms SHUTDN 2 7 2 591 Engine_Idle_Shutdown_Timer_Function 4_states/2_bit 0 bit";
const char string_97[] PROGMEM = "65252 8 1000ms SHUTDN 1 5 2 592 Engine_Idle_Shutdown_Timer_Override 4_states/2_bit 0 bit";
const char string_98[] PROGMEM = "65252 8 1000ms SHUTDN 1 1 2 593 Engine_Idle_Shutdown_has_Shutdown_Engine 4_states/2_bit 0 bit";
const char string_99[] PROGMEM = "65252 8 1000ms SHUTDN 1 3 2 594 Engine_Idle_Shutdown_Driver_Alert_Mode 4_states/2_bit 0 bit";
const char string_100[] PROGMEM = "65252 8 1000ms SHUTDN 3 5 2 605 Refrigerant_High_Pressure_Switch 4_states/2_bit 0 bit";
const char string_101[] PROGMEM = "65252 8 1000ms SHUTDN 3 3 2 875 Refrigerant_Low_Pressure_Switch 4_states/2_bit 0 bit";
const char string_102[] PROGMEM = "65252 8 1000ms SHUTDN 3 1 2 985 A/C_High_Pressure_Fan_Switch 4_states/2_bit 0 bit";
const char string_103[] PROGMEM = "65254 8 1000ms TD 1 1 8 959 Seconds 0.25_s/bit 0 s";
const char string_104[] PROGMEM = "65254 8 1000ms TD 2 1 8 960 Minutes 1_min/bit 0 s";
const char string_105[] PROGMEM = "65254 8 1000ms TD 3 1 8 961 Hours 1_hr/bit 0 s";
const char string_106[] PROGMEM = "65254 8 1000ms TD 5 1 8 962 Day 0.25_days/bit 0 Days";
const char string_107[] PROGMEM = "65254 8 1000ms TD 4 1 8 963 Month 1_month/bit 0 Months";
const char string_108[] PROGMEM = "65254 8 1000ms TD 6 1 8 964 Year 1_year/bit 1985 Years";
const char string_109[] PROGMEM = "65262 8 1000ms ET1 1 1 8 110 Engine_Coolant_Temperature 1_deg_C/bit -40 C";
const char string_110[] PROGMEM = "65262 8 1000ms ET1 2 1 8 174 Engine_Fuel_Temperature_1 1_deg_C/bit -40 C";
const char string_111[] PROGMEM = "65262 8 1000ms ET1 3 1 16 175 Engine_Oil_Temperature_1 0.03125_deg_C/bit -273 C";
const char string_112[] PROGMEM = "65262 8 1000ms ET1 7 1 8 52 Engine_Intercooler_Temperature 1_deg_C/bit -40 C";
const char string_113[] PROGMEM = "65263 8 500ms EFL/P1 1 1 8 94 Fuel_Delivery_Preasure 4_kPa/bit 0 kPa";
const char string_114[] PROGMEM = "65263 8 500ms EFL/P1 3 1 8 98 Engine_Oil_Level 0.4_%/bit 0 %";
const char string_115[] PROGMEM = "65263 8 500ms EFL/P1 4 1 8 100 Engine_Oil_Pressure 4_kPa/bit 0 kPa";
const char string_116[] PROGMEM = "65263 8 500ms EFL/P1 1 1 16 101 Crankcase_Preasure 2_kPa/bit 0 kPa";
const char string_117[] PROGMEM = "65263 8 500ms EFL/P1 7 1 8 109 Engine_Coolant_Pressure 2_kPa/bit 0 kPa";
const char string_118[] PROGMEM = "65263 8 500ms EFL/P1 8 1 8 111 Engine_Coolant_Level 0.4_%/bit 0 %";
const char string_119[] PROGMEM = "65265 8 100ms CCVS 2 1 16 84 Wheel-Based_Vehicle_Speed 1/256_km/h_per_bit 0 kph";
const char string_120[] PROGMEM = "65265 8 100ms CCVS 4 1 2 595 Cruise_Control_Active 4_states/2_bit 0 bit";
const char string_121[] PROGMEM = "65266 8 100ms LFE 7 1 8 183 Throttle_Position 0.4_degree/bit 0 degree";
const char string_122[] PROGMEM = "65266 8 100ms LFE 1 1 16 183 Engine_Fuel_Rate 0.05_L/h_per_bit 0 l/h";
const char string_123[] PROGMEM = "65266 8 100ms LFE 3 1 16 184 Engine_Instantaneous_Fuel_Economy 1/512_km/L_per_bit 0 km/L";
const char string_124[] PROGMEM = "65269 8 1000ms AMB 1 1 8 108 Barometric_Pressure 0.5_kPa/bit 0 kPa";
const char string_125[] PROGMEM = "65269 8 1000ms AMB 6 1 8 172 Engine_Air_Inlet_Temperature 1_deg_C/bit -40 C";
const char string_126[] PROGMEM = "65270 8 500ms IC1 2 1 8 102 Engine_Intake_Manifold_#1_Pressure 2_kPa/bit 0 kPa";
const char string_127[] PROGMEM = "65270 8 500ms IC1 3 1 8 105 Engine_Intake_Manifold_1_Temperature 1_deg_C/bit -40 C";
const char string_128[] PROGMEM = "65270 8 500ms IC1 4 1 8 106 Engine_Air_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_129[] PROGMEM = "65270 8 500ms IC1 6 1 16 173 Engine_Exhaust_Gas_Temperature 0.03125_deg_C/bit -273 C";
const char string_130[] PROGMEM = "65271 8 1000ms VEP1 7 1 16 158 Keyswitch_Battery_Potential 0.05_V/bit 0 Volts";
const char string_131[] PROGMEM = "65271 8 1000ms VEP1 5 1 16 168 Electrical_Potential 0.05_V/bit 0 Volts";
const char string_132[] PROGMEM = "65272 8 1000ms TRF1 3 1 8 126 Transmission_Filter_Differential_Pressure 2_kPa/bit 0 kPa";
const char string_133[] PROGMEM = "65272 8 1000ms TRF1 4 1 8 127 Transmission_Oil_Pressure 16_kPa/bit 0 kPa";
const char string_134[] PROGMEM = "65272 8 1000ms TRF1 5 1 16 177 Transmission_Oil_Temperature 1_deg_C/bit -273 C";
const char string_135[] PROGMEM = "65276 8 1000ms   DD 2 1 8 96 Fuel_Level_1 0.4_%/bit 0 %";
const char string_136[] PROGMEM = "65279 8 10000ms WFI 1 1 2 97 Water_In_Fuel_Indicator 4_states/2_bit 0 bit";

#else
const char string_1[]  PROGMEM = "- -   ---- ---- - - - --- - - - -";
const char string_2[]  PROGMEM = "256 8 50ms TC1 2 1 8 684 Requested_Percent_Clutch_Slip 0.4_%/bit 0 %";
const char string_3[]  PROGMEM = "256 8 50ms TC1 3 1 8 525 Transmission_Requested_Gear 1_gear_value/bit -125 gear_value";//
const char string_4[]  PROGMEM = "256 8 50ms TC1 7 3 4 4255 Transmission_Requested_Launch_Gear 16_states/4_bit 0 bit";
const char string_5[]  PROGMEM = "52992 8 5000ms CTL 1 1 8 1784 Engine_Speed_Limit_Request_-_Minimum_Continuous 32_rpm/bit 0 rpm";
const char string_6[]  PROGMEM = "52992 8 5000ms CTL 2 1 8 1785 Engine_Speed_Limit_Request_-_Maximum_Continuous 32_rpm/bit 0 rpm";
const char string_7[]  PROGMEM = "52992 8 5000ms CTL 3 1 8 1786 Engine_Torque_Limit_Request_-_Minimum_Continuous 1_%/bit -125 %";
const char string_8[]  PROGMEM = "52992 8 5000ms CTL 4 1 8 1787 Engine_Torque_Limit_Request_-_Maximum_Continuous 1_%/bit -125 %";
const char string_9[]  PROGMEM = "52992 8 5000ms CTL 5 1 8 1788 Minimum_Continuous_Retarder_Speed_Limit_Request 32_rpm/bit 0 rpm";
const char string_10[] PROGMEM = "52992 8 5000ms CTL 6 1 8 1789 Maximum_Continuous_Retarder_Speed_Limit_Request 32_rpm/bit 0 rpm";
const char string_11[] PROGMEM = "52992 8 5000ms CTL 7 1 8 1790 Minimum_Continuous_Retarder_Torque_Limit_Request 1_%/bit -125 %";
const char string_12[] PROGMEM = "52992 8 5000ms CTL 8 1 8 1791 Maximum_Continuous_Retarder_Torque_Limit_Request 1_%/bit -125 %";
const char string_13[] PROGMEM = "53248 8 5000ms CL 1 1 8 1487 Illumination_Brightness_Percent 0.4_%/bit 0 %";
const char string_14[] PROGMEM = "53760 8 100ms ASC2 2 1 4 1751 Nominal_Level_Request_Front_Axle 16_states/4_bit 0 bit";
const char string_15[] PROGMEM = "53760 8 100ms ASC2 2 5 4 1750 Nominal_Level_Request_Rear_Axle 16_states/4_bit 0 bit";
const char string_16[] PROGMEM = "53760 8 100ms ASC2 3 1 4 1753 Level_Control_Mode_Request 16_states/4_bit 0 bit";
const char string_17[] PROGMEM = "53760 8 100ms ASC2 4 1 8 1718 Damper_Stiffness_Request_Front_Axle 0.4_%/bit 0 %";
const char string_18[] PROGMEM = "53760 8 100ms ASC2 5 1 8 1719 Damper_Stiffness_Request_Rear_Axle 0.4_%/bit 0 %";
const char string_19[] PROGMEM = "53760 8 100ms ASC2 6 1 8 1720 Damper_Stiffness_Request_Lift_/_Tag_Axle 0.4_%/bit 0 %";
const char string_20[] PROGMEM = "57344 8 1000ms CM1 1 1 8 986 Requested_Percent_Fan_Speed 0.4_%/bit 0 %";
const char string_21[] PROGMEM = "57344 8 1000ms CM1 7 1 4 1683 Auxiliary_Heater_Mode_Request 16_states/4_bit 0 bit";
const char string_22[] PROGMEM = "57344 8 1000ms CM1 8 1 8 2596 Selected_Maximum_Vehicle_Speed_Limit 1_count/bit 0 count";
const char string_23[] PROGMEM = "61440 8 100ms ERC1 1 1 4 900 Retarder_Torque_Mode 16_states/4_bit 0 bit";
const char string_24[] PROGMEM = "61440 8 100ms ERC1 2 1 8 520 Actual_Retarder_-_Percent_Torque 1_%/bit -125 %";
const char string_25[] PROGMEM = "61440 8 100ms ERC1 3 1 8 1085 Intended_Retarder_Percent_Torque 1_%/bit -125 %";
const char string_26[] PROGMEM = "61440 8 100ms ERC1 5 1 8 1480 Source_Address_of_Controlling_Device_for_Retarder_Control 256_states/8_bit 0 Sa";
const char string_27[] PROGMEM = "61440 8 100ms ERC1 6 1 8 1715 Drivers_Demand_Retarder_-__Percent_Torque 1_%/bit -125 %";
const char string_28[] PROGMEM = "61440 8 100ms ERC1 7 1 8 1716 Retarder_Selection_non-engine 0.4_%/bit 0 %";
const char string_29[] PROGMEM = "61440 8 100ms ERC1 8 1 8 1717 Actual_Maximum_Available_Retarder_-_Percent_Torque 1_%/bit -125 %";
const char string_30[] PROGMEM = "61441 8 100ms EBC1 1 1 2 561 ASR_Engine_Control_Active 4_states/2_bit 0 bit";
const char string_31[] PROGMEM = "61441 8 100ms EBC1 2 1 8 521 Brake_Pedal_Position 0.4_%/bit 0 %";
const char string_32[] PROGMEM = "61441 8 100ms EBC1 5 1 8 973 Engine_Retarder_Selection 0.4_%/bit 0 %";
const char string_33[] PROGMEM = "61441 8 100ms EBC1 7 1 8 1481 Source_Address_of_Controlling_Device_for_Brake_Control 256_states/8_bit 0 Sa";
const char string_34[] PROGMEM = "61442 8 10ms ETC1 4 1 8 522 Percent_Clutch_Slip 0.4_%/bit 0 %";
const char string_35[] PROGMEM = "61442 8 10ms ETC1 8 1 8 1482 Source_Address_of_Controlling_Device_for_Transmission_Control 256_states/8_bit 0 Sa";
const char string_36[]  PROGMEM = "61443 8 50ms EEC2 2 1 8 91 Accelerator_Pedal_Position_1 0.4_%/bit 0 %";
const char string_37[]  PROGMEM = "61443 8 50ms EEC2 3 1 8 92 Engine_Percent_Load_At_Current_Speed 1_%/bit 0 %";
const char string_38[]  PROGMEM = "61443 8 50ms EEC2 4 1 8 974 Remote_Accelerator_Pedal_Position 0.4_%/bit 0 %";
const char string_39[]  PROGMEM = "61443 8 50ms EEC2 7 1 8 3357 Actual_Maximum_Available_Engine_-_Percent_Torque 0.4_%/bit 0 %";
const char string_40[]  PROGMEM = "61445 8 100ms ETC2 1 1 8 524 Transmission_Selected_Gear 1_gear_value/bit -125 gear_value";
const char string_41[]  PROGMEM = "61445 8 100ms ETC2 4 1 8 523 Transmission_Current_Gear 1_gear_value/bit -125 gear_value";
const char string_42[]  PROGMEM = "61446 8 500ms EAC1 1 1 8 927 Location 256_states/8_bit 0 bit";
const char string_43[]  PROGMEM = "61449 8 10ms VDC2 3 1 6 1811 Steering_Wheel_Turn_Counter 1_turn/bit -32 count";
const char string_44[]  PROGMEM = "61449 8 10ms VDC2 8 1 8 1810 Longitudinal_Acceleration 0 1_m/s2_per_bit -12 5 m/s2";
const char string_45[]  PROGMEM = "61451 8 20ms ESC1 3 1 8 2928 Axle_Location 256_states/8_bit 0 bit";
const char string_46[]  PROGMEM = "61451 8 20ms ESC1 4 1 4 2923 Status_of_Steering_Axle 16_states/4_bit 0 bit";
const char string_47[]  PROGMEM = "61451 8 20ms ESC1 5 1 4 2924 Steering_Type 16_states/4_bit 0 bit";
const char string_48[]  PROGMEM = "61451 8 20ms ESC1 5 5 4 2925 Type_of_Steering_Forces 16_states/4_bit 0 bit";
const char string_49[]  PROGMEM = "61451 8 20ms ESC1 6 1 4 2926 Type_of_Steering_Transmission 16_states/4_bit 0 bit";
const char string_50[]  PROGMEM = "61453 8 100ms LOI 1 1 4 3156 Blade_Control_Mode_Switch 16_states/4_bit 0 bit";
const char string_51[]  PROGMEM = "61453 8 100ms LOI 1 5 4 3157 Desired_Grade_Offset_Switch 16_states/4_bit 0 bit";
const char string_52[]  PROGMEM = "61453 8 100ms LOI 2 1 4 3158 Blade_Auto_Mode_Command 16_states/4_bit 0 bit";
const char string_53[]  PROGMEM = "61453 8 100ms LOI 2 5 4 3334 Left_Blade_Control_Mode_Operator_Control 16_states/4_bit 0 bit";
const char string_54[]  PROGMEM = "61453 8 100ms LOI 3 1 4 3335 Right_Blade_Control_Mode_Operator_Control 16_states/4_bit 0 bit";
const char string_55[]  PROGMEM = "61453 8 100ms LOI 3 5 4 3336 Left_Desired_Blade_Offset_Operator_Control 16_states/4_bit 0 bit";
const char string_56[]  PROGMEM = "61453 8 100ms LOI 4 1 4 3337 Right_Desired_Blade_Offset_Operator_Control 16_states/4_bit 0 bit";
const char string_57[]  PROGMEM = "61453 8 100ms LOI 4 5 4 3338 Side-shift_Blade_Control_Mode_Operator_Control 16_states/4_bit 0 bit";
const char string_58[]  PROGMEM = "61453 8 100ms LOI 5 1 4 3339 Side-shift_Desired_Blade_Offset_Operator_Control 16_states/4_bit 0 bit";
const char string_59[]  PROGMEM = "61454 8 50ms ATIG 6 1 5 3222 Aftertreatment_1_Intake_Gas_Sensor_Heater_Preliminary_FMI Binary 0 binary";
const char string_60[]  PROGMEM = "61454 8 50ms ATIG 7 1 5 3224 Aftertreatment_1_Intake_NOx_Sensor_Preliminary_FMI Binary 0 binary";
const char string_61[]  PROGMEM = "61454 8 50ms ATIG 8 1 5 3225 Aftertreatment_1_Intake_Oxygen_Sensor_Preliminary_FMI Binary 0 binary";
const char string_62[]  PROGMEM = "61455 8 50ms ATOG 7 1 5 3234 Aftertreatment_1_Outlet_NOx_Sensor_Preliminary_FMI Binary 0 binary";
const char string_63[]  PROGMEM = "61455 8 50ms ATOG 8 1 5 3235 Aftertreatment_1_Outlet_Oxygen_Sensor_Preliminary_FMI Binary 0 binary";
const char string_64[]  PROGMEM = "61458 8 50ms FWSS 1 1 16 3308 Fifth_Wheel_Vertical_Force 5_N/bit 0 N";
const char string_65[]  PROGMEM = "61458 8 50ms FWSS 3 1 16 3309 Fifth_Wheel_Drawbar_Force 10_N/bit -320000 N";
const char string_66[]  PROGMEM = "61458 8 50ms FWSS 5 1 16 3310 Fifth_Wheel_Roll_Moment 10_N/bit -320000 N";
const char string_67[]  PROGMEM = "61459 8 10ms SSI 1 1 16 3318 Pitch_Angle 0.002_deg/bit -64 deg";
const char string_68[]  PROGMEM = "61459 8 10ms SSI 3 1 16 3319 Roll_Angle 0.002_deg/bit -64 deg";
const char string_69[]  PROGMEM = "61459 8 10ms SSI 5 1 16 3322 Pitch_Rate 0.002_deg/sec_per_bit -64 deg/seC";
const char string_70[]  PROGMEM = "61459 8 10ms SSI 8 1 8 3327 Roll_and_Pitch_Measurement_Latency 0.5_ms/bit 0 milliseconds";
const char string_71[]  PROGMEM = "61460 8 50ms BI 5 1 8 3366 Relative_Blade_Height_and_Blade_Rotation_Angle_Measurement_Latency 0.5_ms/bit 0 milliseconds";
const char string_72[]  PROGMEM = "61463 8 5000ms KL1 1 1 8 1352 Engine_Cylinder_1_Knock_Level 1_%/bit 0 %";
const char string_73[]  PROGMEM = "61469 8 10ms SAS 3 1 6 3684 Steering_Wheel_Angle_Range_Counter 1_range/bit -32 range";
const char string_74[]  PROGMEM = "61469 8 10ms SAS 8 1 4 3689 Message_Counter 1_count/bit 0 count";
const char string_75[]  PROGMEM = "61469 8 10ms SAS 8 5 4 3690 Message_Checksum 1_count/bit 0 count";
const char string_76[]  PROGMEM = "61475 8 50ms CRDI 6 1 8 4334 Aftertreatment_CR_Dosing_Reagent_Absolute_Pressure 8_kPa/bit 0 kPa";
const char string_77[]  PROGMEM = "61476 8 50ms CRDR 3 1 4 4349 Aftertreatment_CR_System_Requested_State 16_states/4_bit 0 bit";
const char string_78[]  PROGMEM = "61477 8 50ms CRAI 3 1 5 4378 Aftertreatment_1_Outlet_NH3_Sensor_Preliminary_FMI Binary 0 binary";
const char string_79[]  PROGMEM = "61477 8 50ms CRAI 5 1 5 4382 Aftertreatment_1_Outlet_NH3_Gas_Sensor_Heater_Preliminary_FMI Binary 0 binary";
const char string_80[]  PROGMEM = "61477 8 50ms CRAI 6 1 2 4383 Aftertreatment_1_Outlet_NH3_Gas_Sensor_Heater_Control 4_states/2_bit 0 bit";
const char string_81[]  PROGMEM = "64828 8 1000ms CRRS 3 1 8 4375 Aftertreatment_CR_Catalyst_Reagent_Pump_Drive_Percentage_(Command_to_pump) 0.4_%/bit 0 %";
const char string_82[]  PROGMEM = "64828 8 1000ms CRRS 4 1 8 4376 Aftertreatment_CR_Catalyst_Reagent_Return_Valve_(Directional_command_for_return) 0.4_%/bit 0 %";
const char string_83[]  PROGMEM = "64829 8 1000ms CRRT 1 1 8 4367 Aftertreatment_CR_Catalyst_Reagent_Tank_2_Level 0.4_%/bit 0 %";
const char string_84[]  PROGMEM = "64829 8 1000ms CRRT 2 1 8 4368 Aftertreatment_CR_Catalyst_Reagent_Tank_2_Temperature 1_deg_C/bit -40 C";
const char string_85[]  PROGMEM = "64829 8 1000ms CRRT 5 1 5 4370 Aftertreatment_CR_Catalyst_Reagent_Tank_2_Level_Preliminary_FMI Binary 0 binary";
const char string_86[]  PROGMEM = "64829 8 1000ms CRRT 6 1 5 4371 Aftertreatment_CR_Reagent_Tank_2_Temperature_Preliminary_FMI Binary 0 binary";
const char string_87[]  PROGMEM = "64829 8 1000ms CRRT 7 1 8 4372 Aftertreatment_CR_Catalyst_Reagent_Tank_2_Heater_(light-off) 0.4_%/bit 0 %";
const char string_88[]  PROGMEM = "64829 8 1000ms CRRT 8 1 5 4373 Aftertreatment_CR_Catalyst_Reagent_Tank_2_Heater_Preliminary_FMI Binary 0 binary";
const char string_89[]  PROGMEM = "64830 8 500ms CRET 3 1 5 4361 Aftertreatment_CR_Catalyst_Intake_Gas_Temperature_Preliminary_FMI 32_states/5_bit 0 bit";
const char string_90[]  PROGMEM = "64830 8 500ms CRET 6 1 5 4362 Aftertreatment_CR_Catalyst_Outlet_Gas_Temperature_Preliminary_FMI 32_states/5_bit 0 bit";
const char string_91[]  PROGMEM = "64831 8 500ms CREP 3 1 5 4359 Aftertreatment_CR_Catalyst_Exhaust_Gas_Differential_Pressure_Preliminary_FMI 32_states/5_bit 0 bit";
const char string_92[]  PROGMEM = "64832 8 500ms CRD2 1 3 2 4352 Aftertreatment_CR_Doser_Fault_Suppression_Reques_(request) 4_states/2_bit 0 bit";
const char string_93[]  PROGMEM = "64833 8 500ms CRD2 1 1 8 4335 Aftertreatment_CR_Dosing_Air_Assist_Absolute_Pressure 8_kPa/bit 0 kPa";
const char string_94[]  PROGMEM = "64833 8 500ms CRD2 2 1 8 4336 Aftertreatment_CR_Dosing_Air_Assist_Valve_(measured_0_=_closed) 0.4_%/bit 0 %";
const char string_95[]  PROGMEM = "64833 8 500ms CRD2 3 1 8 4337 Aftertreatment_CR_Dosing_Reagent_Temperature_(closest_to_dosing_valve) 1_deg_C/bit -40 C";
const char string_96[]  PROGMEM = "64833 8 500ms CRD2 4 1 3 4338 Aftertreatment_CR_Dosing_Valve_Exhaust_Temp_Red000mstate 4_states/2_bit 0 bit";
const char string_97[]  PROGMEM = "64833 8 500ms CRD2 5 3 5 4341 Aftertreatment_CR_Catalyst_Reagent_Line_Heater_1_Preliminary_FMI Binary 0 binary";
const char string_98[]  PROGMEM = "64833 8 500ms CRD2 6 3 5 4343 Aftertreatment_CR_Catalyst_Reagent_Line_Heater_2_Preliminary_FMI Binary 0 binary";
const char string_99[]  PROGMEM = "64833 8 500ms CRD2 7 3 5 4345 Aftertreatment_CR_Catalyst_Reagent_Line_Heater_3_Preliminary_FMI Binary 0 binary";
const char string_100[] PROGMEM = "64833 8 500ms CRD2 8 3 5 4347 Aftertreatment_CR_Catalyst_Reagent_Line_Heater_4_Preliminary_FMI 32_states/5_bit 0 bit";
const char string_101[] PROGMEM = "64838 8 500ms A1TWC 7 1 5 4292 Aftertreatment_1_3_W_Catalytic_Converter_Intake_Gas_Temp_Prel_FMI 32_states/5_bit 0 bit";
const char string_102[] PROGMEM = "64838 8 500ms A1TWC 8 3 5 4294 Aftertreatment_1_3_Wa_Catalytic_Converter_Diff_Pressure_Prel_FMI 32_states/5_bit 0 bit";
const char string_103[] PROGMEM = "64849 8 1000ms ACCVC 2 1 8 4199 Desired_Aftercooler_Coolant_Inlet_Temperature 1_deg_C/bit -40 C";
const char string_104[] PROGMEM = "64849 8 1000ms ACCVC 3 1 8 4200 Desired_Aftercooler_Coolant_Thermostat_Opening 0.4_%/bit 0 %";
const char string_105[] PROGMEM = "64850 8 1000ms ECCVC 1 1 2 4195 Engine_Coolant_Thermostat_Mode 4_states/2_bit 0 bit";
const char string_106[] PROGMEM = "64850 8 1000ms ECCVC 2 1 8 4196 Desired_Engine_Coolant_Pump_Outlet_Temperature 1_deg_C/bit -40 C";
const char string_107[] PROGMEM = "64850 8 1000ms ECCVC 3 1 8 4197 Desired_Engine_Coolant_Thermostat_Opening 0.4_%/bit 0 %";
const char string_108[] PROGMEM = "64870 8 1000ms ET4 1 1 8 4076 Engine_Coolant_Temperature_2 1_deg_C/bit -40 C";
const char string_109[] PROGMEM = "64870 8 1000ms ET4 2 1 8 4193 Engine_Coolant_Pump_Outlet_Temperature 1_deg_C/bit -40 C";
const char string_110[] PROGMEM = "64870 8 1000ms ET4 3 1 8 4194 Engine_Coolant_Thermostat_Opening 0.4_%/bit 0 %";
const char string_111[] PROGMEM = "64897 8 500ms EGRB 1 1 8 3672 EGR_Cooler_Bypass_Actuator_Postion 0.4_%/bit 0 %";
const char string_112[] PROGMEM = "64899 8 1000ms TCI 1 1 3 3645 Transfer_case_status 8_states/3_bit 0 bit";
const char string_113[] PROGMEM = "64914 8 250ms EOI 1 1 4 3543 Engine_Operating_State 16_states/4_bit 0 bit";
const char string_114[] PROGMEM = "64914 8 250ms EOI 1 5 2 4082 Fuel_Pump_Primer_Control 4_states/2_bit 0 bit";
const char string_115[] PROGMEM = "64914 8 250ms EOI 8 1 8 3644 Engine_Derate_Request 0.4_%/bit 0 %";
const char string_116[] PROGMEM = "64916 8 100ms EEC7 1 1 16 27 Engine_Exhaust_Gas_Recirculation_Valve_Position 0.0025_%/bit 0 %";
const char string_117[] PROGMEM = "64916 8 100ms EEC7 3 1 16 3822 Engine_Exhaust_Gas_Recirculation_Valve_2_Position 0.0025_%/bit 0 %";
const char string_118[] PROGMEM = "64917 8 1000ms TRF2 4 1 8 4177 Transmission_Oil_Life_Remaining 0.4_%/bit 0 %";
const char string_119[] PROGMEM = "64923 8 1000ms CRI1 1 1 8 3515 Aftertreatment_CR_Catalyst_Reagent_Temperature_2 1_deg_C/bit -40 C";
const char string_120[] PROGMEM = "64923 8 1000ms CRI1 2 1 8 3516 Aftertreatment_CR_Catalyst_Reagent_Concentration 0 25_%/bit 0 %";
const char string_121[] PROGMEM = "64923 8 1000ms CRI1 3 1 8 3518 Aftertreatment_CR_Catalyst_Reagent_Conductivity 5_microSiemens/mm 0 microSiemens/mm";
const char string_122[] PROGMEM = "64923 8 1000ms CRI1 4 1 5 3519 Aftertreatment_CR_Catalyst_Reagent_Temperature_2_Preliminary_FMI Binary 0 binary";
const char string_123[] PROGMEM = "64923 8 1000ms CRI1 5 1 5 3520 Aftertreatment_CR_Catalyst_Reagent_Properties_Preliminary_FMI Binary 0 binary";
const char string_124[] PROGMEM = "64923 8 1000ms CRI1 6 1 4 3521 Aftertreatment_CR_Catalyst_Reagent_Type 16_states/4_bit 0 bit";
const char string_125[] PROGMEM = "64927 8 500ms A1AC 7 1 8 3488 Aftertreatment_1_Air_Pressure_Actuator_Position 0.4_%/bit 0 %";
const char string_126[] PROGMEM = "64929 8 500ms ATFC 8 1 8 4301 Aftertreatment_1_Fuel_Injector_1_Heater_Control 0.4_%/bit 0 %";
const char string_127[] PROGMEM = "64931 8 100ms EEC6 3 1 8 641 Engine_Variable_Geometry_Turbocharger_Actuator_#1 0.4_%/bit 0 %";
const char string_128[] PROGMEM = "64931 8 100ms EEC6 4 1 8 3675 Engine_Turbocharger_Compressor_Bypass_Actuator_Position 0.4_%/bit 0 %";
const char string_129[] PROGMEM = "64938 8 500ms EFLP4 1 1 8 3340 Engine_Charge_Air_Cooler_1_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_130[] PROGMEM = "64938 8 500ms EFLP4 2 1 8 3341 Engine_Charge_Air_Cooler_2_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_131[] PROGMEM = "64938 8 500ms EFLP4 3 1 8 3342 Engine_Coolant_Pump_Differential_Pressure 64_kPa/bit -7 kPa";
const char string_132[] PROGMEM = "64938 8 500ms EFLP4 6 1 8 3668 Engine_Intercooler_Coolant_Level 0.4_%/bit 0 %";
const char string_133[] PROGMEM = "64938 8 500ms EFLP4 7 1 8 3676 Engine_Aftercooler_Coolant_Level 0.4_%/bit 0 %";
const char string_134[] PROGMEM = "64938 8 500ms EFLP4 8 1 8 2631 Engine_Charge_Air_Cooler_Outlet_Pressure 2_kPa/bit 0 kPa";
const char string_135[] PROGMEM = "64946 8 500ms AT1IM 1 1 16 3249 Aftertreatment_1_Exhaust_Gas_Temperature_2 0.03125_deg_C/bit -273 C";
const char string_136[] PROGMEM = "64946 8 500ms AT1IM 3 1 16 3250 Aftertreatment_1_Diesel_Particulate_Filter_Intermediate_Gas_Temperature 0.03125_deg_C/bit -273 C";
const char string_137[] PROGMEM = "64946 8 500ms AT1IM 5 1 16 3251 Aftertreatment_1_Diesel_Particulate_Filter_Differential_Pressure 0 1_kPa/bit 0 kPa";
const char string_138[] PROGMEM = "64946 8 500ms AT1IM 7 1 5 3252 Aftertreatment_1_Exhaust_Gas_Temperature_2_Preliminary_FMI Binary 0 binary";
const char string_139[] PROGMEM = "64946 8 500ms AT1IM 7 6 5 3253 Aftertreatment_1_Diesel_Particulate_Filter_Delta_Pressure_Preliminary_FMI Binary 0 binary";
const char string_140[] PROGMEM = "64946 8 500ms AT1IM 8 3 5 3254 Aftertreatment_1_Diesel_Particulate_Filter_Intermediate_Gas_Temperature_Preliminary_FMI Binary 0 binary";
const char string_141[] PROGMEM = "64947 8 500ms AT1O2 1 1 16 3245 Aftertreatment_1_Exhaust_Gas_Temperature_3 0.03125_deg_C/bit -273 C";
const char string_142[] PROGMEM = "64947 8 500ms AT1O2 3 1 16 3246 Aftertreatment_1_Diesel_Particulate_Filter_Outlet_Gas_Temperature 0.03125_deg_C/bit -273 C";
const char string_143[] PROGMEM = "64947 8 500ms AT1O2 5 1 5 3247 Aftertreatment_1_Exhaust_Gas_Temperature_3_Preliminary_FMI Binary 0 binary";
const char string_144[] PROGMEM = "64947 8 500ms AT1O2 6 1 5 3248 Aftertreatment_1_Diesel_Particulate_Filter_Outlet_Exhaust_Gas_Temperature__Preliminary_FMI Binary 0 binary";
const char string_145[] PROGMEM = "64948 8 500ms AT1I2 1 1 16 3241 Aftertreatment_1_Exhaust_Gas_Temperature_1 0.03125_deg_C/bit -273 C";
const char string_146[] PROGMEM = "64948 8 500ms AT1I2 3 1 16 3242 Aftertreatment_1_Diesel_Particulate_Filter_Intake_Gas_Temperature 0.03125_deg_C/bit -273 C";
const char string_147[] PROGMEM = "64948 8 500ms AT1I2 5 1 5 3243 Aftertreatment_1_Exhaust_Gas_Temperature_1_Preliminary_FMI Binary 0 binary";
const char string_148[] PROGMEM = "64948 8 500ms AT1I2 6 1 5 3244 Aftertreatment_1_Diesel_Particulate_Filter_Intake_Gas_Temperature_Preliminary_FMI Binary 0 binary";
const char string_149[] PROGMEM = "64953 8 1000ms TPRI 1 1 8 3190 Tire_Location 256_states/8_bit 0 bit";
const char string_150[] PROGMEM = "64953 8 1000ms TPRI 2 1 8 3191 Reference_Tire_Pressure 8_kPa/bit 0 kPa";
const char string_151[] PROGMEM = "64954 8 1000ms TR6 2 1 7 3181 Farebox_Alarm_Identifier 128_states/7_bit 0 bit";
const char string_152[] PROGMEM = "64955 8 1000ms TR5 1 1 4 3170 Transaction_Type 16_states/4_bit 0 bit";
const char string_153[] PROGMEM = "64955 8 1000ms TR5 1 5 4 3171 Passenger_Type 16_states/4_bit 0 bit";
const char string_154[] PROGMEM = "64955 8 1000ms TR5 2 1 4 3176 Type_of_Fare 16_states/4_bit 0 bit";
const char string_155[] PROGMEM = "64955 8 1000ms TR5 2 5 4 3177 Payment_Details 16_states/4_bit 0 bit";
const char string_156[] PROGMEM = "64955 8 1000ms TR5 3 1 4 3165 Fare_Validity 1_count/bit 0 count";
const char string_157[] PROGMEM = "64955 8 1000ms TR5 3 5 4 3166 Pass_Category 1_count/bit 0 count";
const char string_158[] PROGMEM = "64955 8 1000ms TR5 4 1 5 3167 Initial_Fare_Agency 1_count/bit 0 count";
const char string_159[] PROGMEM = "64955 8 1000ms TR5 5 1 3 3172 Type_of_Service 8_states/3_bit 0 bit";
const char string_160[] PROGMEM = "64955 8 1000ms TR5 5 4 5 3173 Transfer_Type 32_states/5_bit 0 bit";
const char string_161[] PROGMEM = "64955 8 1000ms TR5 8 1 8 3168 Transfer_Sold 1_count/bit 0 count";
const char string_162[] PROGMEM = "64956 15 1000ms TR4 3 1 8 3175 Fare_Presets 256_states/8_bit 0 bit";
const char string_163[] PROGMEM = "64957 8 1000ms TR3 3 4 16 3085 Vehicle_ID 1_count/bit 0 count";
const char string_164[] PROGMEM = "64958 8 1000ms TR1 1 1 8 3078 Agency 1_count/bit 0 count";
const char string_165[] PROGMEM = "64958 8 1000ms TR1 2 1 8 3071 Number_of_bytes_in_the_Transit_Assigned_Route_Identity 1_byte/bit 0 bytes";
const char string_166[] PROGMEM = "64958 8 1000ms TR1 3 1 8 3072 Number_of_bytes_in_the_Transit_Assigned_Run_Identity 1_byte/bit 0 bytes";
const char string_167[] PROGMEM = "64958 8 1000ms TR1 4 1 8 3073 Number_of_bytes_in_the_Transit_Assigned_Block_Identity 1_byte/bit 0 bytes";
const char string_168[] PROGMEM = "64959 8 1000ms TR2 1 1 8 3070 Number_of_bytes_in_the_Milepost_Identification 1_byte/bit 0 bytes";
const char string_169[] PROGMEM = "64960 8 1000ms TR7 1 1 8 3043 Type_of_Passenger_Count 256_states/8_bit 0 bit";
const char string_170[] PROGMEM = "64960 8 1000ms TR7 2 1 8 3047 Patron_Count 1_count/bit 0 count";
const char string_171[] PROGMEM = "64961 8 500ms EFLP 3 3 8 3358 Engine_Exhaust_Gas_Recirculation_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_172[] PROGMEM = "64964 8 100ms EBC5 2 5 4 2918 XBR_Active_Control_Mode 16_states/4_bit 0 bit";
const char string_173[] PROGMEM = "64964 8 100ms EBC5 3 1 8 2921 XBR_Acceleration_Limit 0.1_m/s2_per_bit -12 5 m/s2";
const char string_174[] PROGMEM = "64967 8 5000ms OHCSS 2 1 8 2888 Engine_Alternate_Rating_Select_State 256_states/8_bit 0 bit";
const char string_175[] PROGMEM = "64967 8 5000ms OHCSS 3 1 4 2889 Engine_Alternate_Droop_Accelerator_select_State 16_states/4_bit 0 bit";
const char string_176[] PROGMEM = "64967 8 5000ms OHCSS 4 1 4 2894 Engine_Alternate_Droop_Remote_Accelerator_Select_State 16_states/4_bit 0 bit";
const char string_177[] PROGMEM = "64967 8 5000ms OHCSS 4 5 4 2895 Engine_Alternate_Droop_Auxiliary_Input_Select_State 16_states/4_bit 0 bit";
const char string_178[] PROGMEM = "64968 8 1000ms ISCS 1 1 4 2892 Engine_Operator_Primary_Intermediate_Speed_Select_State 16_states/4_bit 0 bit";
const char string_179[] PROGMEM = "64971 8 500ms OHECS 2 8 2 882 Engine_Alternate_Rating_Select_selection 4_states/2_bit 0 bit";
const char string_180[] PROGMEM = "64971 8 500ms OHECS 3 1 4 2881 Engine_Alternate_Droop_Accelerator_select 16_states/4_bit 0 bit";
const char string_181[] PROGMEM = "64971 8 500ms OHECS 4 1 4 2886 Engine_Alternate_Droop_Remote_Accelerator_Select 16_states/4_bit 0 bit";
const char string_182[] PROGMEM = "64971 8 500ms OHECS 4 5 4 2885 Engine_Alternate_Droop_Auxiliary_Input_Select 16_states/4_bit 0 bit";
const char string_183[] PROGMEM = "64972 8 1000ms OEL 1 1 4 2873 Work_Light_Switch 16_states 16_states/4_bit  0 bit";
const char string_184[] PROGMEM = "64972 8 1000ms OEL 1 5 4 2872 Main_Light_Switch 16_states  16_states/4_bit 0 bit";
const char string_185[] PROGMEM = "64972 8 1000ms OEL 2 1 4 2876 Turn_Signal_Switch 16_states  16_states/4_bit 0 bit";
const char string_186[] PROGMEM = "64972 8 1000ms OEL 3 1 8 2878 Operators_Desired_Back-light 0.4_%/bit 0 %";
const char string_187[] PROGMEM = "64973 8 200ms OWW 1 1 4 2864 Front_Non-operator_Wiper_Switch 16_states/4_bit 0 bit";
const char string_188[] PROGMEM = "64973 8 200ms OWW 1 5 4 2863 Front_Operator_Wiper_Switch 16_states/4_bit 0 bit";
const char string_189[] PROGMEM = "64973 8 200ms OWW 2 5 4 2865 Rear_Wiper_Switch 16_states/4_bit 0 bit";
const char string_190[] PROGMEM = "64973 8 200ms OWW 3 1 8 2869 Front_Operator_Wiper_Delay_Control 0.4_%/bit 0 %";
const char string_191[] PROGMEM = "64973 8 200ms OWW 4 1 8 2870 Front_Non-operator_Wiper_Delay_Control 0.4_%/bit 0 %";
const char string_192[] PROGMEM = "64973 8 200ms OWW 5 1 8 2871 Rear_Wiper_Delay_Control 0.4_%/bit 0 %";
const char string_193[] PROGMEM = "64976 8 5000ms IC2 5 1 8 3563 Engine_Intake_Manifold_#1_Absolute_Pressure 2_kPa/bit 0 kPa";
const char string_194[] PROGMEM = "64988 8 1000ms MCI 1 1 4 2615 Engine_Throttle_Synchronization_Mode_Status 16_states/4_bit 0 bit";
const char string_195[] PROGMEM = "64992 8 1000ms AMB2 1 1 8 2610 Solar_Intensity_Percent 0.4_%/bit 0 %";
const char string_196[] PROGMEM = "64992 8 1000ms AMB2 2 1 8 2611 Solar_Sensor_Maximum 0.4_mW/cm^2_per_bit 0 mW/cm^2";
const char string_197[] PROGMEM = "64993 8 1000ms CACI 1 1 8 2609 Cab_A/C_Refrigerant_Compressor_Outlet_Pressure 16_kPa/bit 0 kPa";
const char string_198[] PROGMEM = "64994 8 1000ms SPR 1 1 8 2603 Pneumatic_Supply_Pressure_Request 8_kPa/bit 0 kPa";
const char string_199[] PROGMEM = "64994 8 1000ms SPR 2 1 8 2604 Parking_and/or_Trailer_Air_Pressure_Request 8_kPa/bit 0 kPa";
const char string_200[] PROGMEM = "64994 8 1000ms SPR 3 1 8 2605 Service_Brake_Air_Pressure_Request_Circuit_#1 8_kPa/bit 0 kPa";
const char string_201[] PROGMEM = "64994 8 1000ms SPR 4 1 8 2606 Service_Brake_Air_Pressure_Request_Circuit_#2 8_kPa/bit 0 kPa";
const char string_202[] PROGMEM = "64994 8 1000ms SPR 5 1 8 2607 Auxiliary_Equipment_Supply_Pressure_Request 8_kPa/bit 0 kPa";
const char string_203[] PROGMEM = "64994 8 1000ms SPR 6 1 8 2608 Air_Suspension_Supply_Pressure_Request 8_kPa/bit 0 kPa";
const char string_204[] PROGMEM = "64995 8 250ms EOAC 1 1 8 2601 Travel_Velocity_Control_Position 1_%/bit -125 %";
const char string_205[] PROGMEM = "64996 8 500ms EPD 1 1 8 2600 Payload_Percentage 1_%/bit 0 %";
const char string_206[] PROGMEM = "64997 8 1000ms MVS 1 1 8 2588 Maximum_Vehicle_Speed_Limit_1 1_km/h_per_bit 0 kph";
const char string_207[] PROGMEM = "64997 8 1000ms MVS 8 1 8 2595 Applied_Vehicle_Speed_Limit 1_km/h_per_bit 0 kph";
const char string_208[] PROGMEM = "64998 8 100ms HBS 1 1 8 2580 Hydraulic_Brake_Pressure_Circuit_1 100_kPa/bit 0 kPa";
const char string_209[] PROGMEM = "64998 8 100ms HBS 2 1 8 2581 Hydraulic_Brake_Pressure_Circuit_2 100_kPa/bit 0 kPa";
const char string_210[] PROGMEM = "65100 8 500ms ML 8 1 8 1844 Operators_Black_Out_Intensity_Selection 0.4_%/bit 0 %";
const char string_211[] PROGMEM = "65102 8 100ms DC1 1 1 4 1821 Position_of_doors 16_states/4_bit 0 bit";
const char string_212[] PROGMEM = "65104 8 1000ms BT1 1 1 8 1800 Battery_1_Temperature 1_deg_C/bit -40 C";
const char string_213[] PROGMEM = "65104 8 1000ms BT1 2 1 8 1801 Battery_2_Temperature 1_deg_C/bit -40 C";
const char string_214[] PROGMEM = "65107 8 5000ms RTC1 1 1 8 1776 Low_Limit_Threshold_for_Maximum_RPM_from_Retarder 32_rpm/bit 0 rpm";
const char string_215[] PROGMEM = "65107 8 5000ms RTC1 2 1 8 1777 High_Limit_Threshold_for_Minimum_Continuous_RPM_from_Retarder 32_rpm/bit 0 rpm";
const char string_216[] PROGMEM = "65107 8 5000ms RTC1 3 1 8 1778 Low_Limit_Threshold_for_Maximum_Torque_from_Retarder 1_%/bit -125 %";
const char string_217[] PROGMEM = "65107 8 5000ms RTC1 4 1 8 1779 High_Limit_Threshold_for_Minimum_Continuous_Torque_from_Retarder 1_%/bit -125 %";
const char string_218[] PROGMEM = "65107 8 5000ms RTC1 5 1 8 1780 Maximum_Continuous_Retarder_Speed 32_rpm/bit 0 rpm";
const char string_219[] PROGMEM = "65107 8 5000ms RTC1 6 1 8 1781 Minimum_Continuous_Retarder_Speed 32_rpm/bit 0 rpm";
const char string_220[] PROGMEM = "65107 8 5000ms RTC1 7 1 8 1782 Maximum_Continuous_Retarder_Torque 1_%/bit -125 %";
const char string_221[] PROGMEM = "65107 8 5000ms RTC1 8 1 8 1783 Minimum_Continuous_Retarder_Torque 1_%/bit -125 %";
const char string_222[] PROGMEM = "65108 8 5000ms ECT1 1 1 8 1768 Engine_Low_Limit_Threshold_for_Maximum_RPM_from_Engine 32_rpm/bit 0 rpm";
const char string_223[] PROGMEM = "65108 8 5000ms ECT1 2 1 8 1769 Engine_High_Limit_Threshold_for_Minimum_Continuous_Engine_RPM 32_rpm/bit 0 rpm";
const char string_224[] PROGMEM = "65108 8 5000ms ECT1 3 1 8 1770 Engine_Low_Limit_Threshold_for_Maximum_Torque_from_Engine 1_%/bit -125 %";
const char string_225[] PROGMEM = "65108 8 5000ms ECT1 4 1 8 1771 Engine_High_Limit_Threshold_for_Minimum_Continuous_Torque_from_Engine 1_%/bit -125 %";
const char string_226[] PROGMEM = "65108 8 5000ms ECT1 5 1 8 1772 Engine_Maximum_Continuous_RPM 32_rpm/bit 0 rpm";
const char string_227[] PROGMEM = "65108 8 5000ms ECT1 6 1 8 1773 Engine_Minimum_Continuous_RPM 32_rpm/bit 0 rpm";
const char string_228[] PROGMEM = "65108 8 5000ms ECT1 7 1 8 1774 Engine_Maximum_Continuous_Torque 1_%/bit -125 %";
const char string_229[] PROGMEM = "65108 8 5000ms ECT1 8 1 8 1775 Engine_Minimum_Continuous_Torque 1_%/bit -125 %";
const char string_230[] PROGMEM = "65110 8 1000ms TI1 1 1 8 1761 Aftertreatment_CR_Catalyst_Tank_Level 0.4_%/bit 0 %";
const char string_231[] PROGMEM = "65110 8 1000ms TI1 2 1 8 3031 Aftertreatment_CR_Catalyst_Tank_Temperature 1_deg_C/bit -40 C";
const char string_232[] PROGMEM = "65110 8 1000ms TI1 5 1 5 3532 Aftertreatment_CR_Catalyst_Tank_Level_Preliminary_FMI Binary 0 binary";
const char string_233[] PROGMEM = "65110 8 1000ms TI1 6 1 5 4365 Aftertreatment_CR_Catalyst_Reagent_Tank_1_Temperature_Preliminary_FMI Binary 0 binary";
const char string_234[] PROGMEM = "65110 8 1000ms TI1 8 1 5 4366 Aftertreatment_CR_Catalyst_Reagent_Tank_1_Heater_Preliminary_FMI Binary 0 binary";
const char string_235[] PROGMEM = "65111 8 100ms ASC5 1 1 8 1729 Damper_Stiffness_Front_Axle 0.4_%/bit 0 %";
const char string_236[] PROGMEM = "65111 8 100ms ASC5 2 1 8 1730 Damper_Stiffness_Rear_Axle 0.4_%/bit 0 %";
const char string_237[] PROGMEM = "65111 8 100ms ASC5 3 1 8 1731 Damper_Stiffness_Lift_/_Tag_Axle 0.4_%/bit 0 %";
const char string_238[] PROGMEM = "65111 8 100ms ASC5 4 1 2 1833 Electronic_Shock_Absorber_Control_Mode_-__Front_Axle 4_states/2_bit 0 bit";
const char string_239[] PROGMEM = "65128 8 1000ms  VF 1 1 8 1638 Hydraulic_Temperature 1_deg_C/bit -40 C";
const char string_240[] PROGMEM = "65128 8 1000ms  VF 2 1 2 1713 Hydraulic_Oil_Filter_Restriction_Switch 4_states/2_bit 0 bit";
const char string_241[] PROGMEM = "65128 8 1000ms  VF 2 3 2 1857 Winch_Oil_Pressure_Switch 4_states/2_bit 0 bit";
const char string_242[] PROGMEM = "65128 8 1000ms  VF 3 1 8 2602 Hydraulic_Oil_Level 0.4_%/bit 0 %";
const char string_243[] PROGMEM = "65130 8 5000ms EFS 1 1 8 1380 Engine_Oil_Level_Remote_Reservoir 0.4_%/bit 0 %";
const char string_244[] PROGMEM = "65130 8 5000ms EFS 2 1 8 1381 Engine_Fuel_Supply_Pump_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_245[] PROGMEM = "65130 8 5000ms EFS 3 1 8 1382 Engine_Fuel_Filter_(suction_side)_Differential_Pressure 2_kPa/bit 0 kPa";
const char string_246[] PROGMEM = "65130 8 5000ms EFS 4 1 8 3548 Engine_Waste_Oil_Reservoir_Level 0.4_%/bit 0 %";
const char string_247[] PROGMEM = "65130 8 5000ms EFS 5 1 8 3549 Engine_Oil-Filter_Outlet_Pressure 4_kPa/bit 0 kPa";
const char string_248[] PROGMEM = "65133 8 1000ms HTR 1 1 8 1687 Auxiliary_Heater_Output_Coolant_Temperature 1_deg_C/bit -40 C";
const char string_249[] PROGMEM = "65133 8 1000ms HTR 2 1 8 1688 Auxiliary_Heater_Input_Air_Temperature 1_deg_C/bit -40 C";
const char string_250[] PROGMEM = "65133 8 1000ms HTR 3 1 8 1689 Auxiliary_Heater_Output_Power_Percent 0.4_%/bit 0 %";
const char string_251[] PROGMEM = "65133 8 1000ms HTR 4 1 4 1677 Auxiliary_Heater_Mode 16_states/4_bit 0 bit";
const char string_252[] PROGMEM = "65135 8 100ms ACC1 1 1 8 1586 Speed_of_forward_vehicle 1_km/h_per_bit 0 kph";
const char string_253[] PROGMEM = "65135 8 100ms ACC1 2 1 8 1587 Distance_to_forward_vehicle 1_m/bit 0 m";
const char string_254[] PROGMEM = "65135 8 100ms ACC1 3 1 8 1588 Adaptive_Cruise_Control_Set_Speed 1_km/h_per_bit 0 kph";
const char string_255[] PROGMEM = "65135 8 100ms ACC1 4 1 3 1590 Adaptive_Cruise_Control_Mode 8_states/3_bit 0 bit";
const char string_256[] PROGMEM = "65137 8 50ms  LTP5 1 1 8 1581 Laser_Tracer_Horizontal_Deviation 1_%/bit 0 %";
const char string_257[] PROGMEM = "65137 8 50ms  LTP6 1 1 8 1582 LED_Display_Data_#2 256_states/8_bit 0 bit";
const char string_258[] PROGMEM = "65137 8 50ms  LTP7 1 1 8 1583 Laser_Tracer_Information 256_states/8_bit 0 bit";
const char string_259[] PROGMEM = "65138 8 50ms  LBC3 1 1 8 1578 Blade_Control_Mode 256_states/8_bit 0 bit";
const char string_260[] PROGMEM = "65141 8 50ms  LVD3 1 1 8 2576 Laser_Receiver_Type 1_count/bit 0 count";
const char string_261[] PROGMEM = "65142 8 100ms LVDD 1 1 8 1573 LED_Display_Data_#1 256_states/8_bit 0 bit";
const char string_262[] PROGMEM = "65142 8 100ms LVDD 2 1 4 1805 LED_Display_Mode_Control 16_states/4_bit 0 bit";
const char string_263[] PROGMEM = "65142 8 100ms LVDD 2 5 4 1806 LED_Display_Deadband_Control 16_states/4_bit 0 bit";
const char string_264[] PROGMEM = "65142 8 100ms LVDD 3 1 4 2578 LED_Pattern_Control 16_states/4_bit 0 bit";
const char string_265[] PROGMEM = "65142 8 100ms LVDD 3 5 4 2577 Display_Deadbands 16_states/4_bit 0 bit";
const char string_266[] PROGMEM = "65144 8 1000ms TP1 1 1 8 39 Tire_Pressure_Check_Interval 1_min/bit 0 Seconds";
const char string_267[] PROGMEM = "65144 8 1000ms TP1 2 1 4 1466 Steer_Channel_Mode 16_states/4_bit 0 bit";
const char string_268[] PROGMEM = "65144 8 1000ms TP1 3 1 4 1467 Trailer/tag_Channel_Mode 16_states/4_bit 0 bit";
const char string_269[] PROGMEM = "65144 8 1000ms TP1 3 5 4 1468 Drive_Channel_Mode 16_states/4_bit 0 bit";
const char string_270[] PROGMEM = "65170 8 100ms   EI 1 1 8 1208 Engine_Pre-filter_Oil_Pressure 4_kPa/bit 0 kPa";
const char string_271[] PROGMEM = "65170 8 100ms   EI 4 1 8 1210 Engine_Fuel_Rack_Position 0.4_%/bit 0 %";
const char string_272[] PROGMEM = "65172 8 5000ms EAC 1 1 8 1203 Engine_Auxiliary_Coolant_Pressure 4_kPa/bit 0 kPa";
const char string_273[] PROGMEM = "65172 8 5000ms EAC 2 1 8 1212 Engine_Auxiliary_Coolant_Temperature 1_deg_C/bit -40 C";
const char string_274[] PROGMEM = "65172 8 5000ms EAC 3 1 8 2435 Sea_Water_Pump_Outlet_Pressure 2_kPa/bit 0 kPa";
const char string_275[] PROGMEM = "65174 8 100ms  TCW 1 1 8 1188 Engine_Turbocharger_1_Wastegate_Drive 0.4_%/bit 0 %";
const char string_276[] PROGMEM = "65174 8 100ms  TCW 5 1 8 1192 Engine_Turbocharger_Wastegate_Actuator_Control_Air_Pressure 4_kPa/bit 0 kPa";
const char string_277[] PROGMEM = "65179 8 1000ms TCI 1 1 8 1168 Engine_Turbocharger_Lube_Oil_Pressure_2 4_kPa/bit 0 kPa";
const char string_278[] PROGMEM = "65191 8 1000ms  AT 1 1 8 1122 Engine_Alternator_Bearing_1_Temperature 1_deg_C/bit -40 C";
const char string_279[] PROGMEM = "65191 8 1000ms  AT 3 1 8 1124 Engine_Alternator_Winding_1_Temperature 1_deg_C/bit -40 C";
const char string_280[] PROGMEM = "65197 8 100ms EBC3 1 1 8 1091 Brake_Application_Pressure_High_Range_Front_Axle_Left_Wheel 5_kPa/bit 0 kPa";
const char string_281[] PROGMEM = "65197 8 100ms EBC3 2 1 8 1092 Brake_Application_Pressure_High_Range_Front_Axle_Right_Wheel 5_kPa/bit 0 kPa";
const char string_282[] PROGMEM = "65197 8 100ms EBC3 3 1 8 1093 Brake_Application_Pressure_High_Range_Rear_Axle_#1_Left_Wheel 5_kPa/bit 0 kPa";
const char string_283[] PROGMEM = "65197 8 100ms EBC3 4 1 8 1094 Brake_Application_Pressure_High_Range_Rear_Axle_#1_Right_Wheel 5_kPa/bit 0 kPa";
const char string_284[] PROGMEM = "65198 8 1000ms AIR1 1 1 8 46 Pneumatic_Supply_Pressure 8_kPa/bit 0 kPa";
const char string_285[] PROGMEM = "65198 8 1000ms AIR1 2 1 8 1086 Parking_and/or_Trailer_Air_Pressure 8_kPa/bit 0 kPa";
const char string_286[] PROGMEM = "65198 8 1000ms AIR1 3 1 8 1087 Service_Brake_Circuit_1_Air_Pressure 8_kPa/bit 0 kPa";
const char string_287[] PROGMEM = "65198 8 1000ms AIR1 4 1 8 1088 Service_Brake_Circuit_2_Air_Pressure 8_kPa/bit 0 kPa";
const char string_288[] PROGMEM = "65198 8 1000ms AIR1 5 1 8 1089 Auxiliary_Equipment_Supply_Pressure 8_kPa/bit 0 kPa";
const char string_289[] PROGMEM = "65198 8 1000ms AIR1 6 1 8 1090 Air_Suspension_Supply_Pressure 8_kPa/bit 0 kPa";
const char string_290[] PROGMEM = "65213 8 1000ms FD 1 1 8 975 Estimated_Percent_Fan_Speed 0.4_%/bit 0 %";
const char string_291[] PROGMEM = "65213 8 1000ms FD 2 1 4 977 Fan_Drive_State 16_states/4_bit 0 bit";
const char string_292[] PROGMEM = "65213 8 1000ms FD 7 1 8 4212 Fan_Drive_Bypass_Command_Status 0.4_%/bit 0 %";
const char string_293[] PROGMEM = "65215 8 100ms EBC2 3 1 8 905 Relative_Speed_Front_Axle_Left_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_294[] PROGMEM = "65215 8 100ms EBC2 4 1 8 906 Relative_Speed_Front_Axle_Right_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_295[] PROGMEM = "65215 8 100ms EBC2 5 1 8 907 Relative_Speed_Rear_Axle_#1_Left_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_296[] PROGMEM = "65215 8 100ms EBC2 6 1 8 908 Relative_Speed_Rear_Axle_#1_Right_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_297[] PROGMEM = "65215 8 100ms EBC2 7 1 8 909 Relative_Speed_Rear_Axle_#2_Left_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_298[] PROGMEM = "65215 8 100ms EBC2 8 1 8 910 Relative_Speed_Rear_Axle_#2_Right_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_299[] PROGMEM = "65216 8 100ms SERV 1 1 8 911 Service_Component_Identification 1_ID/bit 0 ID";
const char string_300[] PROGMEM = "65216 8 100ms SERV 4 1 8 912 Service_Component_Identification 1_ID/bit 0 ID";
const char string_301[] PROGMEM = "65216 8 100ms SERV 5 1 8 915 Service_Delay/Calendar_Time_Based 1_week/bit -125 Weeks";
const char string_302[] PROGMEM = "65216 8 100ms SERV 6 1 8 913 Service_Component_Identification 1_ID/bit 0 ID";
const char string_303[] PROGMEM = "65245 8 1000ms  TC 1 1 8 104 Engine_Turbocharger_Lube_Oil_Pressure_1 4_kPa/bit 0 kPa";
const char string_304[] PROGMEM = "65247 8 250ms EEC3 1 1 8 514 Nominal_Friction_-_Percent_Torque 1_%/bit -125 %";
const char string_305[] PROGMEM = "65247 8 250ms EEC3 4 1 18 519 Engine's_Desired_Operating_Speed_Asymmetry_Adjustment 1/bit 0 Ratio";
const char string_306[] PROGMEM = "65247 8 250ms EEC3 5 1 8 2978 Estimated_Engine_Parasitic_Losses_-_Percent_Torque 1_%/bit -125 %";
const char string_307[] PROGMEM = "65254 8 1000ms TD 1 1 8 959 Seconds 25000ms/bit 0 Seconds";
const char string_308[] PROGMEM = "65254 8 1000ms TD 2 1 8 960 Minutes 1_min/bit 0 Seconds";
const char string_309[] PROGMEM = "65254 8 1000ms TD 3 1 8 961 Hours 1_hr/bit 0 Seconds";
const char string_310[] PROGMEM = "65254 8 1000ms TD 4 1 8 963 Month 1_month/bit 0 Months";
const char string_311[] PROGMEM = "65254 8 1000ms TD 5 1 8 962 Day 25_days/bit 0 Days";
const char string_312[] PROGMEM = "65254 8 1000ms TD 6 1 8 964 Year 1_year/bit 1985 Years";
const char string_313[] PROGMEM = "65254 8 1000ms TD 7 1 8 1601 Local_minute_offset 1_min/bit -7500 Seconds";
const char string_314[] PROGMEM = "65254 8 1000ms TD 8 1 8 1602 Local_hour_offset 1_hr/bit -450000 Seconds";
const char string_315[] PROGMEM = "65262 8 1000ms ET1 1 1 8 110 Engine_Coolant_Temperature 1_deg_C/bit -40 C";
const char string_316[] PROGMEM = "65262 8 1000ms ET1 2 1 8 174 Engine_Fuel_Temperature_1 1_deg_C/bit -40 C";
const char string_317[] PROGMEM = "65262 8 1000ms ET1 7 1 8 52 Engine_Intercooler_Temperature 1_deg_C/bit -40 C";
const char string_318[] PROGMEM = "65262 8 1000ms ET1 8 1 8 1134 Engine_Intercooler_Thermostat_Opening 0.4_%/bit 0 %";
const char string_319[] PROGMEM = "65263 8 5000ms EFLP1 1 1 8 94 Engine_Fuel_Delivery_Pressure 4_kPa/bit 0 kPa";
const char string_320[] PROGMEM = "65263 8 5000ms EFLP1 2 1 8 22 Engine_Extended_Crankcase_Blow-by_Pressure 0 05_kPa/bit 0 kPa";
const char string_321[] PROGMEM = "65263 8 5000ms EFLP1 3 1 8 98 Engine_Oil_Level 0.4_%/bit 0 %";
const char string_322[] PROGMEM = "65263 8 5000ms EFLP1 4 1 8 100 Engine_Oil_Pressure 4_kPa/bit 0 kPa";
const char string_323[] PROGMEM = "65263 8 5000ms EFLP1 7 1 8 109 Engine_Coolant_Pressure 2_kPa/bit 0 kPa";
const char string_324[] PROGMEM = "65263 8 5000ms EFLP1 8 1 8 111 Engine_Coolant_Level 0.4_%/bit 0 %";
const char string_325[] PROGMEM = "65264 8 100ms      PTO 1 1 8 90 Power_Takeoff_Oil_Temperature 1_deg_C/bit -40 C";
const char string_326[] PROGMEM = "65265 8 100ms     CCVS 6 1 8 86 Cruise_Control_Set_Speed 1_km/h_per_bit 0 kph";
const char string_327[] PROGMEM = "65265 8 100ms     CCVS 7 1 5 976 PTO_Governor_State 32_states/5_bit 0 bit";
const char string_328[] PROGMEM = "65266 8 100ms      LFE 7 1 8 51 Engine_Throttle_Position 0.4_%/bit 0 %";
const char string_329[] PROGMEM = "65266 8 100ms      LFE 8 1 8 3673 Engine_Throttle_2_Position 0.4_%/bit 0 %";
const char string_330[] PROGMEM = "65268 8 10000ms TIRE 1 1 8 929 Tire_Location 256_states/8_bit 0 bit";
const char string_331[] PROGMEM = "65268 8 10000ms TIRE 2 1 8 241 Tire_Pressure 4_kPa/bit 0 kPa";
const char string_332[] PROGMEM = "65269 8 1000ms AMB 1 1 8 108 Barometric_Pressure 0.5_kPa/bit 0 kPa";
const char string_333[] PROGMEM = "65269 8 1000ms AMB 6 1 8 172 Engine_Air_Inlet_Temperature 1_deg_C/bit -40 C";
const char string_334[] PROGMEM = "65270 8 5000ms IC1 1 1 8 81 Engine_Diesel_Particulate_Filter_Inlet_Pressure 0.5_kPa/bit 0 kPa";
const char string_335[] PROGMEM = "65270 8 5000ms IC1 2 1 8 102 Engine_Intake_Manifold_#1_Pressure 2_kPa/bit 0 kPa";
const char string_336[] PROGMEM = "65270 8 5000ms IC1 3 1 8 105 Engine_Intake_Manifold_1_Temperature 1_deg_C/bit -40 C";
const char string_337[] PROGMEM = "65270 8 5000ms IC1 4 1 18 106 Engine_Air_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_338[] PROGMEM = "65270 8 5000ms IC1 5 1 8 107 Engine_Air_Filter_1_Differential_Pressure 0 05_kPa/bit 0 kPa";
const char string_339[] PROGMEM = "65270 8 5000ms IC1 8 1 8 112 Engine_Coolant_Filter_Differential_Pressure 0.5_kPa/bit 0 kPa";
const char string_340[] PROGMEM = "65272 8 1000ms TRF1 3 1 8 126 Transmission_Filter_Differential_Pressure 2_kPa/bit 0 kPa";
const char string_341[] PROGMEM = "65272 8 1000ms TRF1 4 1 8 127 Transmission_Oil_Pressure 16_kPa/bit 0 kPa";
const char string_342[] PROGMEM = "65272 8 1000ms TRF1 7 1 8 3027 Transmission_Oil_Level_High_/_Low 0.5_L/bit -62 5 liters";
const char string_343[] PROGMEM = "65273 8 1000ms AI 1 1 8 75 Steering_Axle_Temperature 1_deg_C/bit -40 C";
const char string_344[] PROGMEM = "65273 8 1000ms AI 2 1 8 930 Drive_Axle_Location 256_states/8_bit 0 bit";
const char string_345[] PROGMEM = "65273 8 1000ms AI 3 1 8 579 Drive_Axle_Lift_Air_Pressure 4_kPa/bit 0 kPa";
const char string_346[] PROGMEM = "65273 8 1000ms AI 4 1 8 578 Drive_Axle_Temperature 1_deg_C/bit -40 C";
const char string_347[] PROGMEM = "65273 8 1000ms AI 5 1 8 2613 Drive_Axle_Lube_Pressure 4_kPa/bit 0 kPa";
const char string_348[] PROGMEM = "65273 8 1000ms AI 8 1 8 2614 Steering_Axle_Lube_Pressure 4_kPa/bit 0 kPa";
const char string_349[] PROGMEM = "65274 8 1000ms B 1 1 8 116 Brake_Application_Pressure 4_kPa/bit 0 kPa";
const char string_350[] PROGMEM = "65274 8 1000ms B 2 1 8 117 Brake_Primary_Pressure 4_kPa/bit 0 kPa";
const char string_351[] PROGMEM = "65274 8 1000ms B 3 1 8 118 Brake_Secondary_Pressure 4_kPa/bit 0 kPa";
const char string_352[] PROGMEM = "65275 8 1000ms RF 1 1 8 119 Hydraulic_Retarder_Pressure 16_kPa/bit 0 kPa";
const char string_353[] PROGMEM = "65275 8 1000ms RF 2 1 8 120 Hydraulic_Retarder_Oil_Temperature 1_deg_C/bit -40 C";
const char string_354[] PROGMEM = "65276 8 1000ms DD 1 1 8 80 Washer_Fluid_Level 0.4_%/bit 0 %";
const char string_355[] PROGMEM = "65276 8 1000ms DD 2 1 8 96 Fuel_Level_1 0.4_%/bit 0 %";
const char string_356[] PROGMEM = "65276 8 1000ms DD 3 1 8 95 Engine_Fuel_Filter_Differential_Pressure 2_kPa/bit 0 kPa";
const char string_357[] PROGMEM = "65276 8 1000ms DD 4 1 8 99 Engine_Oil_Filter_Differential_Pressure 0.5_kPa/bit 0 kPa";
const char string_358[] PROGMEM = "65276 8 1000ms DD 7 1 8 38 Fuel_Level_2 0.4_%/bit 0 %";
const char string_359[] PROGMEM = "65277 8 500ms  A1 1 1 8 72 Engine_Blower_Bypass_Valve_Position 0.4_%/bit 0 %";
const char string_360[] PROGMEM = "65278 8 1000ms AWPP 1 1 8 73 Auxiliary_Pump_Pressure 16_kPa/bit 0 kPa";

#endif

#if !PGN_extended
const char* const string_table[] PROGMEM = {
	string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9, string_10,
	string_11, string_12, string_13, string_14, string_15, string_16, string_17, string_18, string_19, string_20,
	string_21, string_22, string_23, string_24, string_25, string_26, string_27, string_28, string_29, string_30,
	string_31, string_32, string_33, string_34, string_35, string_36, string_37, string_38, string_39, string_40,
	string_41, string_42, string_43, string_44, string_45, string_46, string_47, string_48, string_49, string_50,
	string_51, string_52, string_53, string_54, string_55, string_56, string_57, string_58, string_59, string_60,
	string_61, string_62, string_63, string_64, string_65, string_66, string_67, string_68, string_69, string_70,
	string_71, string_72, string_73, string_74, string_75, string_76, string_77, string_78, string_79, string_80,
	string_81, string_82, string_83, string_84, string_85, string_86, string_87, string_88, string_89, string_90,
	string_91, string_92, string_93, string_94, string_95, string_96, string_97, string_98, string_99, string_100,
	string_101, string_102, string_103, string_104, string_105, string_106, string_107, string_108, string_109, string_110,
	string_111, string_112, string_113, string_114, string_115, string_116, string_117, string_118, string_119, string_120,
	string_121, string_122, string_123, string_124, string_125, string_126,string_127, string_128, string_129, string_130, string_131, string_132,
	string_133, string_134, string_135, string_136 };
#else

const char* const string_table[] PROGMEM = {
	string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9, string_10,
	string_11, string_12, string_13, string_14, string_15, string_16, string_17, string_18, string_19, string_20,
	string_21, string_22, string_23, string_24, string_25, string_26, string_27, string_28, string_29, string_30,
	string_31, string_32, string_33, string_34, string_35, string_36, string_37, string_38, string_39, string_40,
	string_41, string_42, string_43, string_44, string_45, string_46, string_47, string_48, string_49, string_50,
	string_51, string_52, string_53, string_54, string_55, string_56, string_57, string_58, string_59, string_60,
	string_61, string_62, string_63, string_64, string_65, string_66, string_67, string_68, string_69, string_70,
	string_71, string_72, string_73, string_74, string_75, string_76, string_77, string_78, string_79, string_80,
	string_81, string_82, string_83, string_84, string_85, string_86, string_87, string_88, string_89, string_90,
	string_91, string_92, string_93, string_94, string_95, string_96, string_97, string_98, string_99, string_100,
	string_101, string_102, string_103, string_104, string_105, string_106, string_107, string_108, string_109, string_110,
	string_111, string_112, string_113, string_114, string_115, string_116, string_117, string_118, string_119, string_120,
	string_121, string_122, string_123, string_124, string_125, string_126, string_127, string_128, string_129, string_130,
	string_131, string_132, string_133, string_134, string_135, string_136, string_137, string_138, string_139, string_140,
	string_141, string_142, string_143, string_144, string_145, string_146, string_147, string_148, string_149, string_150,
	string_151, string_152, string_153, string_154, string_155, string_156, string_157, string_158, string_159, string_160,
	string_161, string_162, string_163, string_164, string_165, string_166, string_167, string_168, string_169, string_170,
	string_171, string_172, string_173, string_174, string_175, string_176, string_177, string_178, string_179, string_180,
	string_181, string_182, string_183, string_184, string_185, string_186, string_187, string_188, string_189, string_190,
	string_191, string_192, string_193, string_194, string_195, string_196, string_197, string_198, string_199, string_200,
	string_201, string_202, string_203, string_204, string_205, string_206, string_207, string_208, string_209, string_210,
	string_211, string_212, string_213, string_214, string_215, string_216, string_217, string_218, string_219, string_220,
	string_221, string_222, string_223, string_224, string_225, string_226, string_227, string_228, string_229, string_230,
	string_231, string_232, string_233, string_234, string_235, string_236, string_237, string_238, string_239, string_240,
	string_241, string_242, string_243, string_244, string_245, string_246, string_247, string_248, string_249, string_250,
	string_251, string_252, string_253, string_254, string_255, string_256, string_257, string_258, string_259, string_260,
	string_261, string_262, string_263, string_264, string_265, string_266, string_267, string_268, string_269, string_270,
	string_271, string_272, string_273, string_274, string_275, string_276, string_277, string_278, string_279, string_280,
	string_281, string_282, string_283, string_284, string_285, string_286, string_287, string_288, string_289, string_290,
	string_291, string_292, string_293, string_294, string_295, string_296, string_297, string_298, string_299, string_300,
	string_301, string_302, string_303, string_304, string_305, string_306, string_307, string_308, string_309, string_310,
	string_311, string_312, string_313, string_314, string_315, string_316, string_317, string_318, string_319, string_320,
	string_321, string_322, string_323, string_324, string_325, string_326, string_327, string_328, string_329, string_330,
	string_331, string_332, string_333, string_334, string_335, string_336, string_337, string_338, string_339, string_340,
	string_341, string_342, string_343, string_344, string_345, string_346, string_347, string_348, string_349, string_350,
	string_351, string_352, string_353, string_354, string_355, string_356, string_357, string_358, string_359, string_360 };
#endif

// Variables  p-tp-p
const uint8_t msgEngineTemperature[] = { 0xa0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; // PGN 65262 0Xfeee
const uint8_t Engine_Total_Average_Fuel_Rate[] = { 0xa2, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };// PGN 65101 0xfe4d
const uint8_t Specific_Heat_Ratio[] = { 0xbc, 0x7d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };// PGN 65109 fe55
const uint8_t Engine_Cylinder_1_Combustion_Time[] = { 0xaa, 0x5e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };// PGN 65147 fe7b
const uint8_t Engine_Fuel_Flow_Rate[] = { 0x05, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; // PGN 65153 fe81
const uint8_t Engine_Cylinder_1_Ignition_Timing[] = { 0xbb, 0x74, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; //PGN 65154 fe82
const uint8_t Engine_Power[] = { 0xff, 0x34, 0x40, 0xff, 0xff, 0xff, 0xff, 0xff }; // PGN 65168 fe90
const uint8_t Engine_Actual_Exhaust_Oxygen[] = { 0xff, 0xff, 0xff, 0xff, 0xb1, 0x33, 0xff, 0xff }; // PGN 65193 fea9
const uint8_t Trip_Average_Fuel_Rate[] = { 0xff, 0xff, 0xff, 0xff, 0x53, 0x01, 0xff, 0xff }; //PGN 65203 feb3
const uint8_t Trip_Cruise_Distance[] = { 0xff, 0xff, 0x12, 0x22, 0x10, 0x00, 0xff, 0xff }; // PGN 65206 feb6
const uint8_t Engine_Total_Hours_of_Operation[] = { 0x11, 0x22, 0x20, 0x00, 0xff, 0xff, 0xff, 0xff }; // PGN 65253 fee5
const uint8_t Engine_Trip_Fuel[] = { 0x90, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff }; //PGN 65257 fee9

char buffer[200];    // make sure this is large enough for the largest string_it must hold
uint16_t velocidades[] = { 10,20,50,100,250,500,1000,5000,10000 };

/*-----------------------------------------------------------*/
// Variables encoders
/*-----------------------------------------------------------*/
uint8_t encoderPin1 = 2;
uint8_t encoderPin2 = 3;
uint8_t encoderPin3 = 18;
uint8_t encoderPin4 = 19;
uint8_t encoderSwitchPin1 = 23; //push button switch 1
uint8_t encoderSwitchPin2 = 22; //push button switch 2

// Marca temporal interrupción CAN
static long time_old;

// Control encoders
volatile int  lastEncoded1 = 1;
volatile long encoderValue1 = 1;
volatile long Value1, Value2, Value1_ant = 1;
volatile int  lastEncoded2 = 1;
volatile long encoderValue2 = 1;
long lastencoderValue = 0;

struct valores_PGN {
	char address_PGN_hex[10];
	uint8_t orden_PGN;
} frames_PGN[10];

struct menu {
	volatile uint16_t valor_new = 0;
	volatile uint16_t valor_old = 99;
	uint8_t valor_excase;
	uint8_t posicion_int = 0;
	uint8_t posicion2_int = 0;
	uint8_t SPN_length_int = 0;
	uint8_t frame_activo = 0;
} encoder_menu[num_menu];

struct time {
	uint8_t sec;
	uint8_t min;
	uint8_t hor;
} tiempo;

/*-----------------------------------------------------------*/
// Variables CAN
/*-----------------------------------------------------------*/
struct mystruct
{
	uint16_t PGN;
	uint16_t periodo;
	uint8_t address;
	uint8_t priority;
	uint8_t msg[8];
};

struct mystruct mensaje_can[10];
long lMessageID;
uint8_t flag_1 = 0;
uint8_t lock = 0;
uint16_t PGN_anterior = 0;
uint16_t PGN_mas, PGN_menos = 0;

/* The tasks function */
void vPeriodicTask3(struct mystruct* can_tx);
void vPeriodicTask4(void *pvParameters);
void vPeriodicTask2(void *pvParameters);
void recibir_can();
int parseString(char* line, char*** argv);

/*-----------------------------------------------------------*/
// Variables operativas
/*-----------------------------------------------------------*/
uint8_t pin_led_verde = 12; //led verde
uint8_t PINtoRESET = 7;     //Reset
uint8_t pin_led_rojo = 11;  //led rojo
							//uint8_t enable_BL = 6;      //Bluetooth
							//uint8_t status_BL = 8;      //Bluetooth
uint8_t pin_beep = 24;      //Beep

							// Declarations
struct AMessage {
	byte nPriority;
	byte nSrcAddr;
	byte nDestAddr;
	byte nData[8];
	int nDataLen;
	long lPGN;
	long time;
	uint32_t cuenta = 0;
} xMessage;

char sString[80];
TaskHandle_t xHandle[10];  // colas CAN
uint32_t kilometros = 0;

char PGN[5];  // PGN
char length_PGN[5];  // Length
char rate[8];  // Transmission Rate
char acronym[5];  // Acronym
char position[5];  // pos
char position2[5];  // pos2
char SPN_length[10];  // SPN length
char SPN[10];  // SPN
char name[70]; // Name
char resolution[10];  // Resolution
char offset[10];  // Offset
char units[10]; // Units

int carga_buffer(int i) {
	strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i]))); // Necessary casts and dereferencing, just copy
	int n = sscanf(buffer, "%s %s %s %s %s %s %s %s %s %s %s %s", PGN, length_PGN, rate, acronym, position, position2, SPN_length, SPN, name, resolution, offset, units);
	return n;
}

#define DTC_code(n,n1,n2) itoa((encoder_menu[n].valor_new >> 2), array_prov, 2); LCD.sendString(array_prov, ((encoder_menu[n].valor_new)<8) + n1, n2)

void valores_msg(uint8_t e_encoder) {
	mensaje_can[9].msg[e_encoder - 40] = encoder_menu[e_encoder].valor_new >> 2;
	LCD.sendString("**", 2 * (e_encoder - 40), 0);

	for (uint8_t i = 0; i < 8; i++) {
		char array_prov[3];
		itoa(mensaje_can[9].msg[i], array_prov, 16);
		if (mensaje_can[9].msg[i] > 16) LCD.sendString(array_prov, 2 * i, 1);
		else { LCD.sendString(array_prov, 2 * i + 1, 1); LCD.sendString("0", 2 * i, 1); }
	}
	rec_eeprom(e_encoder);
	encoder_menu[e_encoder].valor_old = encoder_menu[e_encoder].valor_new >> 2;
	encoder_menu[0].valor_excase = e_encoder;
}

void ver_PGN(uint8_t igual0, uint8_t igual1, uint8_t num) {

	if ((encoder_menu[37].valor_new >> 2) > 7) encoder_menu[37].valor_new = 7 << 2;
	// valor maximo de T
	if (encoder_menu[38].valor_new >> 2 > 8) encoder_menu[38].valor_new = 0;
	static char temp[16];
	// printear
	sprintf(temp, "PGN:%05u", (uint16_t)65269 + (encoder_menu[35].valor_new >> 2) - 8192);
	LCD.sendString(temp, 0, 0);

	sprintf(temp, "S:%02X", (encoder_menu[36].valor_new >> 2));
	LCD.sendString(temp, 10, 0);

	sprintf(temp, "P:%01d T:%03d ", encoder_menu[37].valor_new >> 2, velocidades[encoder_menu[38].valor_new >> 2]);
	LCD.sendString(temp, 0, 1);
	LCD.sendString(">", igual0, igual1);
	if (num != 39) { LCD.sendString(">", igual0, igual1); LCD.sendString(" ", 13, 1); }
	else LCD.sendString(">", igual0, igual1);

	// Encolar
	mensaje_can[9].PGN = 65269 + (encoder_menu[35].valor_new >> 2) - 8192;
	mensaje_can[9].priority = encoder_menu[37].valor_new >> 2;
	mensaje_can[9].periodo = velocidades[encoder_menu[38].valor_new >> 2];
	mensaje_can[9].address = encoder_menu[36].valor_new >> 2;

	rec_eeprom(num);
	encoder_menu[num].valor_old = encoder_menu[num].valor_new >> 2;
	encoder_menu[0].valor_excase = num;
}

void DTC(int p) {
	char array_prov[3];
	DTC_code(30, 6, 0);
	DTC_code(31, 6, 1);
	DTC_code(32, 14, 0);
	DTC_code(33, 14, 1);
	rec_eeprom(p);
	encoder_menu[p].valor_old = encoder_menu[p].valor_new >> 2;
	encoder_menu[0].valor_excase = p;
}

void mostrar_valores_LCD(uint8_t canal) {
	encoder_menu[canal].valor_old = (encoder_menu[canal].valor_new >> 2);
	LCD.sendString("                ", 0, 1);
	LCD.sendString(PGN, 0, 1);
	if (atoi(rate) == 100) LCD.sendString(".1s", 6, 1);
	else if (atoi(rate) == 500) LCD.sendString(".5s", 6, 1);
	else if (atoi(rate) == 1000) LCD.sendString("1s", 6, 1);
	else if (atoi(rate) == 5000) LCD.sendString("5s", 6, 1);
	else if (atoi(rate) == 10000) LCD.sendString("10s", 6, 1);
	else if (atoi(rate) == 20000) LCD.sendString("20s", 6, 1);
	else if (atoi(rate) == 30000) LCD.sendString("30s", 6, 1);
	else LCD.sendString(rate, 6, 1);
	LCD.sendString(position, 10, 1);
	LCD.sendString(".", 11, 1);
	LCD.sendString(position2, 12, 1);
	LCD.sendString(SPN_length, 14, 1);
	LCD.sendString(acronym, 6, 0);
	LCD.sendString(SPN, 11, 0);
	LCD.sendFloat(encoder_menu[canal].valor_new >> 2, 3, 0, 1, 0);
	encoder_menu[0].valor_excase = canal;
	flag_1 = 0;
}

//variables inicio
int eeAddress = 0;
int inicio = 26;
int match = 0;
bool restored = false;

SemaphoreHandle_t xSemaphore = NULL;
uint32_t count = 0; // costate 4

					// actualiza valores en eeprom
void rec_eeprom(uint8_t i)
{
	eeAddress = sizeof(int);
	for (uint8_t u = 1; u < i; u++) eeAddress += sizeof(menu);
	EEPROM.put(eeAddress, encoder_menu[i].valor_new); // guarda nuevo valor al inicio
#if restaurar_menu
	encoder_menu[cases].valor_new = encoderValue2;
	eeAddress = sizeof(int);
	for (uint8_t u = 1; u < cases; u++) eeAddress += sizeof(menu);
	EEPROM.put(eeAddress, encoder_menu[cases].valor_new); // guarda nuevo valor al inicio
#endif
}

static bool IsOdd(int value)
{
	return value % 2 != 0;
}

QueueHandle_t xQueue1;
struct AMessage *pxMessage;
uint8_t ok = 0;

void llenar_source_address()
{
	LCD.sendString("A: ", 0, 0);
	LCD.sendString("B: ", 0, 1);
	LCD.sendString("C: ", 5, 0);
	LCD.sendString("D: ", 5, 1);
	LCD.sendString("E: ", 10, 0);
	LCD.sendString("F: ", 10, 1);
	if (encoder_menu[1].frame_activo) LCD.sendFloat((encoder_menu[9].valor_new >> 2), 3, 0, 2, 0); else LCD.sendString("---", 2, 0);
	if (encoder_menu[2].frame_activo) LCD.sendFloat((encoder_menu[10].valor_new >> 2), 3, 0, 2, 1); else LCD.sendString("---", 2, 1);
	if (encoder_menu[3].frame_activo) LCD.sendFloat((encoder_menu[11].valor_new >> 2), 3, 0, 7, 0); else LCD.sendString("---", 7, 0);
	if (encoder_menu[4].frame_activo) LCD.sendFloat((encoder_menu[12].valor_new >> 2), 3, 0, 7, 1); else LCD.sendString("---", 7, 1);
	if (encoder_menu[5].frame_activo) LCD.sendFloat((encoder_menu[13].valor_new >> 2), 3, 0, 12, 0); else LCD.sendString("---", 12, 0);
	if (encoder_menu[6].frame_activo) LCD.sendFloat((encoder_menu[14].valor_new >> 2), 3, 0, 12, 1); else LCD.sendString("---", 12, 1);
}

// AES
void recibir() {
	int MAX_CMD_LENGTH = 30;
	static int cmdIndex;
	char cmd[30];
	static const uint8_t key1[] = key_1;
	static const uint8_t key2[] = key_2;
	static char data[16]; //16 chars == 16 bytes
	static char data_old[16];
	for (int i = 0; i < 16; i++) data_old[i] = (char)random(0, 9);
	strcpy(data, data_old);
	aes256_enc_single(key1, data);
	Serial3.print(data);
	Serial3.print("\n");
	delay(100);
	while (Serial3.available()) {
		char byteIn = Serial3.read();
		cmd[cmdIndex] = byteIn;
		if (byteIn == '\n') {
			cmd[cmdIndex] = '\0';
			cmdIndex = 0;
			aes256_dec_single(key2, cmd);
			if (strncmp(cmd, data_old, 16) == 0) ok = 1; else ok = 0;
		}
		else if (cmdIndex++ >= MAX_CMD_LENGTH)cmdIndex = 0;
	}
}// fin recibir

 //-----------------------------------------------------------------
 //                            SETUP
 //-----------------------------------------------------------------

void setup(void)
{
	//analogReference(EXTERNAL); // a 5Vcc de la alimentación
	Serial3.begin(9600); // comunicaciones UNO
	Serial2.begin(9600); // comunicaciones BL

	// tiempo por defecto
	tiempo.hor = 0;
	tiempo.min = 0;
	tiempo.sec = 0;

	//Apago led rojo
	digitalWrite(pin_led_rojo, HIGH);     // turn the LED on (HIGH is the voltage level)

	// Tramas configurables
	mensaje_can[0] = { 65001,10,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
	mensaje_can[1] = { 65002,20,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
	mensaje_can[2] = { 65003,50,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
	mensaje_can[3] = { 65004,100,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
	mensaje_can[4] = { 65005,250,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
	mensaje_can[5] = { 65006,500,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };

	// Tramas autogeneradas por Arduino
	mensaje_can[6] = { 65248,1000,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } }; // Trip distance
	mensaje_can[7] = { 65254,1000,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } }; // calendario reloj
	mensaje_can[8] = { 65226,1000,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } }; // DM01
	mensaje_can[9] = { 65269,10000,0,6,{ 0xCB, 0x40, 0x25, 0xA0, 0x25, 0x60, 0x27, 0x25 } };// Condiciones ambientales configurables

	EEPROM.get(eeAddress, match);

	if (!digitalRead(encoderSwitchPin1)) alive_led = 1; else digitalWrite(pin_led_verde, HIGH);

	if (match != inicio || !digitalRead(encoderSwitchPin2)) {
		EEPROM.put(eeAddress, inicio);

		// valores por defecto tramas CAN
		eeAddress += sizeof(int);
		EEPROM.put(eeAddress, encoder_menu[1].valor_new = 0 << 2);
#if !PGN_extended
		encoder_menu[1].valor_new = 0 << 2; // Menú A
		encoder_menu[2].valor_new = 0 << 2; // Menú B
		encoder_menu[3].valor_new = 0 << 2; // Menú C
		encoder_menu[4].valor_new = 0 << 2; // Menú D
		encoder_menu[5].valor_new = 0 << 2; // Menú E
		encoder_menu[6].valor_new = 0 << 2; // Menú F
#else
		encoder_menu[1].valor_new = 0 << 2; // Menú A
		encoder_menu[2].valor_new = 0 << 2; // Menú B
		encoder_menu[3].valor_new = 0 << 2; // Menú C
		encoder_menu[4].valor_new = 0 << 2; // Menú D
		encoder_menu[5].valor_new = 0 << 2; // Menú E
		encoder_menu[6].valor_new = 0 << 2; // Menú F
#endif
		encoder_menu[7].valor_new = 7 << 2;  // 250kps
		encoder_menu[8].valor_new = 1 << 2;  // Inicio Serial y BL -> TXGen -> TXGen
		encoder_menu[9].valor_new = 0 << 2;  // A
		encoder_menu[10].valor_new = 0 << 2; // B
		encoder_menu[11].valor_new = 0 << 2; // C
		encoder_menu[12].valor_new = 0 << 2; // D
		encoder_menu[13].valor_new = 0 << 2; // E
		encoder_menu[14].valor_new = 0 << 2; // F
		encoder_menu[15].valor_new = 6 << 2; // Prioridad 6 por defecto
		encoder_menu[16].valor_new = 0 << 2; //  Destination
		encoder_menu[17].valor_new = 9 << 2; // 115.2K
		encoder_menu[18].valor_new = 1 << 2; // dia
		encoder_menu[19].valor_new = 1 << 2; // mes
		encoder_menu[20].valor_new = 2016 << 2; // año
		encoder_menu[26].valor_new = 1208 << 2; // SPN DTC
		encoder_menu[27].valor_new = 3 << 2; // FMI DTC
		encoder_menu[28].valor_new = 1 << 2; // OC DTC
		encoder_menu[29].valor_new = 0 << 2; // CM DTC
		encoder_menu[30].valor_new = 0; // MILS
		encoder_menu[31].valor_new = 0; // RSLS
		encoder_menu[32].valor_new = 0; // AWLS
		encoder_menu[33].valor_new = 0; // PLS
		encoder_menu[34].valor_new = 0; //
		encoder_menu[35].valor_new = 8192 << 2; // PGN offset onfigurable
		encoder_menu[36].valor_new = 0; // Dest address
		encoder_menu[37].valor_new = 6 << 2; // preference
		encoder_menu[38].valor_new = 1 << 2; // periodo T
		encoder_menu[39].valor_new = 1; // Y/N
		encoder_menu[40].valor_new = mensaje_can[9].msg[0] << 2;
		encoder_menu[41].valor_new = mensaje_can[9].msg[1] << 2;
		encoder_menu[42].valor_new = mensaje_can[9].msg[2] << 2;
		encoder_menu[43].valor_new = mensaje_can[9].msg[3] << 2;
		encoder_menu[44].valor_new = mensaje_can[9].msg[4] << 2;
		encoder_menu[45].valor_new = mensaje_can[9].msg[5] << 2;
		encoder_menu[46].valor_new = mensaje_can[9].msg[6] << 2;
		encoder_menu[47].valor_new = mensaje_can[9].msg[7] << 2;
		encoder_menu[48].valor_new = 0 << 2; // Banners
		encoder_menu[49].valor_new = 0xFF << 2;
		encoder_menu[50].valor_new = 1 << 2; // uLTIMO MENU

		restored = true;
	}
	else
	{  // recuperar valores from eeprom
		eeAddress += sizeof(int);
		EEPROM.get(eeAddress, encoder_menu[1].valor_new);
		for (int i = 2; i < (num_menu - 1); i++) {
			eeAddress += sizeof(menu);
			EEPROM.get(eeAddress, encoder_menu[i].valor_new);
		}// for
		for (int i = 0; i < 6; i++) mensaje_can[i].address = (encoder_menu[9 + i].valor_new >> 2); // asignación direcciones A...F
		encoderValue2 = encoder_menu[cases].valor_new;
		restored = false;
	}//if

	const long velocidades[] = { 300,600,1200,2400,4800,9600,14400,19200,57600,115200,128000,153600,230400,250000,256000 };

	Serial.begin(velocidades[encoder_menu[17].valor_new >> 2]);
	while (!Serial) { ; } // wait for serial port to connect. Needed for native USB port only

	pinMode(pin_led_verde, OUTPUT);   // led 12
	pinMode(pin_led_rojo, OUTPUT);    // led 11
	pinMode(pin_beep, OUTPUT);        // beep
	pinMode(A8, OUTPUT);  // digitalWrite(A8, HIGH); // azul
	pinMode(A9, OUTPUT);  // digitalWrite(A9, HIGH); // rojo
	pinMode(A10, OUTPUT); // digitalWrite(A10, HIGH); // amarillo
	pinMode(A11, OUTPUT); // digitalWrite(A11, HIGH); // verde
	digitalWrite(pin_led_rojo, LOW);

	// hello world
	delay(500); digitalWrite(pin_led_rojo, HIGH);
	digitalWrite(A8, LOW); delay(300);  digitalWrite(A8, HIGH);
	digitalWrite(A9, LOW); delay(300); digitalWrite(A9, HIGH);
	digitalWrite(A10, LOW); delay(300); digitalWrite(A10, HIGH);
	digitalWrite(A11, LOW); delay(300); digitalWrite(A11, HIGH);

	// Reset HW
	pinMode(PINtoRESET, INPUT);    // Just to be clear, as default is INPUT. Not really needed
	digitalWrite(PINtoRESET, LOW); // Prime it, but does not actually set output. Does disable 10K pull Up, but who cares

	Wire.begin();
	LCD.init();

	// Pines encoders
	pinMode(encoderPin1, INPUT);
	pinMode(encoderPin2, INPUT);
	pinMode(encoderPin3, INPUT);
	pinMode(5, INPUT);
	pinMode(6, INPUT);
	pinMode(encoderPin4, INPUT);
	pinMode(encoderSwitchPin1, INPUT);
	pinMode(encoderSwitchPin2, INPUT);

	// pullup pulsadores
	digitalWrite(encoderSwitchPin1, HIGH); //turn pullup resistor on
	digitalWrite(encoderSwitchPin2, HIGH); //turn pullup resistor on

										   //call updateEncoder() when any high/low changed seen
	attachInterrupt(digitalPinToInterrupt(encoderPin2), updateEncoder1, CHANGE); //Der.
	attachInterrupt(digitalPinToInterrupt(encoderPin3), updateEncoder2, CHANGE); //Izq.
	attachInterrupt(digitalPinToInterrupt(encoderPin4), updateEncoder2, CHANGE); //Izq.
	attachInterrupt(digitalPinToInterrupt(encoderPin1), recibir_can, FALLING);   //Can

	// Creamos la cola
	xQueue1 = xQueueCreate(10, sizeof(struct AMessage *)); // 10
	if (xQueue1 == NULL)
	{        /* Queue was not created and must not be used. */
		Serial.println("Queue Fail !");
	}
	else Serial.println("Queue Created OK.");

#if seguridad_activada
	while (!ok) recibir();
	ok = 0;
#endif

	// Initialize the CAN controller
	if (canInitialize((encoder_menu[7].valor_new >> 2) + 3) == CAN_OK)
		Serial.print("CAN Init OK.\n\r\n\r");
	else
		Serial.print("CAN Init Failed.\n\r");

	// Start the scheduler so our tasks start executing
	xSemaphore = xSemaphoreCreateMutex();

	// Task msgs CAN
	BaseType_t xReturned;

	TaskHandle_t serial_task = NULL;

	Serial.print("Tarea:");
	for (byte i = 0; i < 10; i++) {
		xReturned = xTaskCreate(vPeriodicTask3, "Tramas_CAN", mem_tramas_can, &mensaje_can[i], tskIDLE_PRIORITY + 3, &xHandle[i]); // mantener +3
		if (xReturned == pdPASS)
		{
			/* The task was created.  Use the task's handle to delete the task */
			Serial.print(" ");
			Serial.print(i);

			// no desactivo los que estaban activos, que los recargo
			if ((i < 6) | (0)) // No creo DM01 tampoco
			{
				if (encoder_menu[i + 1].valor_new >> 2 == 0) vTaskSuspend(xHandle[i]);
				carga_buffer((int)encoder_menu[i + 1].valor_new >> 2);
				mensaje_can[i].PGN = atoi(PGN);
				mensaje_can[i].periodo = atoi(rate);
				encoder_menu[i].posicion_int = atoi(position);
				encoder_menu[i].posicion2_int = atoi(position2);
				encoder_menu[i].SPN_length_int = atoi(SPN_length);
			}
		}
		delay(100);
	} Serial.println("");

	//recarga valores trama PGN variable
	for (byte i = 0; i < 8; i++) mensaje_can[9].msg[i] = encoder_menu[40 + i].valor_new >> 2;

	// Presentación
	LCD.sendString("Begas Motor (", 0, 0);
	LCD.sendString("J1939 Emulator 2", 0, 1);
	if (restored == true) LCD.sendString("r) ", 13, 0); else LCD.sendString("c) ", 13, 0);
	delay(2000);

	// Task S.O.
	xTaskCreate(vPeriodicTask1, "Task 1", mem_tasks, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(vPeriodicTask2, "Task 2", mem_tasks, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(vPeriodicTask4, "Task 4", mem_tasks, NULL, tskIDLE_PRIORITY + 1, &serial_task); // serial

#if seguridad_activada
	while (!ok) recibir();
	ok = 0;
#endif

	for (byte i = 0; i < 10; i++) { // recupero valors frame
		encoder_menu[i].frame_activo = encoder_menu[i].valor_new >> 2;
	}

	vTaskStartScheduler();

	// nunca se llega aquí
	for (;;);

} // fin setup

  // States´ machine
static void vPeriodicTask1(void *pvParameters)
{
	TickType_t xLastWakeTime2 = xTaskGetTickCount();

	/* As per most tasks, this task is implemented in an infinite loop. */
	for (;;)
	{
		Value2 = encoderValue2 >> 2;

		switch (Value2) {
		case 0: {
			if (!digitalRead(encoderSwitchPin2)) {
				LCD.sendString(Serial_N, 0, 0);
				LCD.sendString(Options, 0, 1);
				delay(3000);
			}
			else {
				static bool ree = true;
				LCD.sendString("   SIROCCO (c)   ", 0, 0);
				if (ree) LCD.scrollString("J1939 ECU Emulator 2 All In One", 1, 50); // Banner
				ree = false;
				LCD.sendString("J1939 Emulator 2", 0, 1);
				encoder_menu[0].valor_excase = 0;
				flag_1 = 0;
				encoder_menu[1].valor_new = encoder_menu[1].frame_activo << 2;
				rec_eeprom(0);
			}
		} break;

		case 1: {
			if (encoder_menu[1].frame_activo == (encoder_menu[1].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) LCD.sendString("A", 0, 0); else LCD.sendString(" ", 0, 0); }
			if ((encoder_menu[1].valor_old != encoder_menu[1].valor_new >> 2) || (encoder_menu[0].valor_excase != 1)) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				carga_buffer((int)encoder_menu[1].valor_new >> 2);
				LCD.sendString("A               ", 0, 0);
				mostrar_valores_LCD(1);
				rec_eeprom(1);
			}
		} break; // case 1

		case 2: {
			if (encoder_menu[2].frame_activo == (encoder_menu[2].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) LCD.sendString("B", 0, 0); else LCD.sendString(" ", 0, 0); }
			if (encoder_menu[2].valor_old != encoder_menu[2].valor_new >> 2 || encoder_menu[0].valor_excase != 2) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				if (1) encoder_menu[1].valor_new = encoder_menu[1].frame_activo << 2;
				carga_buffer((int)encoder_menu[2].valor_new >> 2);
				LCD.sendString("B               ", 0, 0);
				mostrar_valores_LCD(2);
				rec_eeprom(2);
			}
		} break; // case 2

		case 3: {
			if (encoder_menu[3].frame_activo == (encoder_menu[3].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) LCD.sendString("C", 0, 0); else LCD.sendString(" ", 0, 0); }
			if (encoder_menu[3].valor_old != encoder_menu[3].valor_new >> 2 || encoder_menu[0].valor_excase != 3) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				if (1) encoder_menu[2].valor_new = encoder_menu[2].frame_activo << 2;
				carga_buffer((int)encoder_menu[3].valor_new >> 2);
				LCD.sendString("C               ", 0, 0);
				mostrar_valores_LCD(3);
				rec_eeprom(3);
			}
		} break; // case 3

		case 4: {
			if (encoder_menu[4].frame_activo == (encoder_menu[4].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) LCD.sendString("D", 0, 0); else LCD.sendString(" ", 0, 0); }
			if (encoder_menu[4].valor_old != encoder_menu[4].valor_new >> 2 || encoder_menu[0].valor_excase != 4) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				if (1) encoder_menu[3].valor_new = encoder_menu[3].frame_activo << 2;
				carga_buffer((int)encoder_menu[4].valor_new >> 2);
				LCD.sendString("D               ", 0, 0);
				mostrar_valores_LCD(4);
				rec_eeprom(4);
			}
		} break; // case 4

		case 5: {
			if (encoder_menu[5].frame_activo == (encoder_menu[5].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) LCD.sendString("E", 0, 0); else LCD.sendString(" ", 0, 0); }
			if (encoder_menu[5].valor_old != encoder_menu[5].valor_new >> 2 || encoder_menu[0].valor_excase != 5) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				if (1) encoder_menu[4].valor_new = encoder_menu[4].frame_activo << 2;
				carga_buffer((int)encoder_menu[5].valor_new >> 2);
				LCD.sendString("E               ", 0, 0);
				mostrar_valores_LCD(5);
				rec_eeprom(5);
			}
		} break; // case 5

		case 6: {
			if (encoder_menu[6].frame_activo == (encoder_menu[6].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) LCD.sendString("F", 0, 0); else LCD.sendString(" ", 0, 0); }
			if (encoder_menu[6].valor_old != encoder_menu[6].valor_new >> 2 || encoder_menu[0].valor_excase != 6) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				if (1) encoder_menu[5].valor_new = encoder_menu[5].frame_activo << 2;
				carga_buffer((int)encoder_menu[6].valor_new >> 2);
				LCD.sendString("F               ", 0, 0);
				mostrar_valores_LCD(6);
				rec_eeprom(6);
			}
		} break; // case 6

		case 7: {
			if (encoder_menu[7].valor_old != encoder_menu[7].valor_new >> 2 || encoder_menu[0].valor_excase != 7) {
				if (1) encoder_menu[6].valor_new = encoder_menu[6].frame_activo << 2;
				LCD.sendString("CAN bps:  ", 0, 0);
				LCD.sendString("(Set & Push)     ", 0, 1);
				if (encoder_menu[7].valor_new >> 2 > 9) (encoder_menu[7].valor_new = 9 << 2);
				if (encoder_menu[7].valor_new >> 2 == 0) LCD.sendString(" 20K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 1) LCD.sendString(" 40K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 2) LCD.sendString(" 50K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 3) LCD.sendString(" 80K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 4) LCD.sendString("100K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 5) LCD.sendString("125K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 6) LCD.sendString("200K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 7) LCD.sendString(" 250K ", 10, 0);
				else if (encoder_menu[7].valor_new >> 2 == 8) LCD.sendString(" 500K ", 10, 0);
				else if (encoder_menu[7].valor_new >> 2 == 9) LCD.sendString("1000K", 10, 0);
				rec_eeprom(7);
				encoder_menu[7].valor_old = encoder_menu[7].valor_new >> 2;
				encoder_menu[0].valor_excase = 7;
				flag_1 = 0;
			}
		} break; // case 7

		case 8: {
			if (encoder_menu[8].valor_old != encoder_menu[8].valor_new >> 2 || encoder_menu[0].valor_excase != 8) {
				if ((encoder_menu[8].valor_new >> 2) > 8) encoder_menu[8].valor_new = 0;
				LCD.sendString("Bluetooh: ", 0, 0);
				LCD.sendString("  Serial: ", 0, 1);
				if (encoder_menu[8].valor_new >> 2 == 0) { LCD.sendString("OFF    ", 9, 0);   LCD.sendString("OFF    ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 1) { LCD.sendString("TXGen   ", 9, 0);  LCD.sendString("TXGen   ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 2) { LCD.sendString("TXGen   ", 9, 0);  LCD.sendString("RXCAN   ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 3) { LCD.sendString("RXCAN   ", 9, 0);  LCD.sendString("TXGen   ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 4) { LCD.sendString("RXCAN   ", 9, 0);  LCD.sendString("RXCAN   ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 5) { LCD.sendString("TXGen   ", 9, 0);  LCD.sendString("OFF    ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 6) { LCD.sendString("OFF    ", 9, 0);  LCD.sendString("TXGen   ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 7) { LCD.sendString("RXCAN   ", 9, 0);  LCD.sendString("OFF    ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 8) { LCD.sendString("OFF    ", 9, 0);  LCD.sendString("RXCAN   ", 9, 1); }
				rec_eeprom(8);
				encoder_menu[8].valor_old = encoder_menu[8].valor_new >> 2;
				encoder_menu[0].valor_excase = 8;
			}
		} break; // case 8

		case 9: { // Address A: Address B:
			if (encoder_menu[9].valor_old != encoder_menu[9].valor_new >> 2 || encoder_menu[0].valor_excase != 9) {
				if ((encoder_menu[9].valor_new >> 2) > 255) encoder_menu[9].valor_new = 0;
				llenar_source_address();
				LCD.sendString("A>", 0, 0);
				mensaje_can[0].address = (encoder_menu[9].valor_new >> 2); // asignación dirección
				rec_eeprom(9);
				encoder_menu[9].valor_old = encoder_menu[9].valor_new >> 2;
				encoder_menu[0].valor_excase = 9;
			}
		} break; // case 9

		case 10: {
			if (encoder_menu[10].valor_old != encoder_menu[10].valor_new >> 2 || encoder_menu[0].valor_excase != 10) {
				if ((encoder_menu[10].valor_new >> 2) > 255) encoder_menu[10].valor_new = 0;
				llenar_source_address();
				LCD.sendString("B>", 0, 1);
				mensaje_can[1].address = (encoder_menu[10].valor_new >> 2); // asignación dirección
				rec_eeprom(10);
				encoder_menu[10].valor_old = encoder_menu[10].valor_new >> 2;
				encoder_menu[0].valor_excase = 10;
			}
		} break; // case 10

		case 11: {
			if (encoder_menu[11].valor_old != encoder_menu[11].valor_new >> 2 || encoder_menu[0].valor_excase != 11) {
				if ((encoder_menu[11].valor_new >> 2) > 255) encoder_menu[11].valor_new = 0;
				llenar_source_address();
				LCD.sendString("C>", 5, 0);
				mensaje_can[2].address = (encoder_menu[11].valor_new >> 2); // asignación dirección
				rec_eeprom(11);
				encoder_menu[11].valor_old = encoder_menu[11].valor_new >> 2;
				encoder_menu[0].valor_excase = 11;
			}
		} break; // case 11

		case 12: {
			if (encoder_menu[12].valor_old != encoder_menu[12].valor_new >> 2 || encoder_menu[0].valor_excase != 12) {
				if ((encoder_menu[12].valor_new >> 2) > 255) encoder_menu[12].valor_new = 0;
				llenar_source_address();
				LCD.sendString("D>", 5, 1);
				mensaje_can[3].address = (encoder_menu[12].valor_new >> 2); // asignación dirección
				rec_eeprom(12);
				encoder_menu[12].valor_old = encoder_menu[12].valor_new >> 2;
				encoder_menu[0].valor_excase = 12;
			}
		} break; // case 12

		case 13: {
			if (encoder_menu[13].valor_old != encoder_menu[13].valor_new >> 2 || encoder_menu[0].valor_excase != 13) {
				if ((encoder_menu[13].valor_new >> 2) > 255) encoder_menu[13].valor_new = 0;
				llenar_source_address();
				LCD.sendString("E>", 10, 0);
				mensaje_can[4].address = (encoder_menu[13].valor_new >> 2); // asignación dirección
				rec_eeprom(13);
				encoder_menu[13].valor_old = encoder_menu[13].valor_new >> 2;
				encoder_menu[0].valor_excase = 13;
			}
		} break; // case 13

		case 14: {
			if (encoder_menu[14].valor_old != encoder_menu[14].valor_new >> 2 || encoder_menu[0].valor_excase != 14) {
				if (encoder_menu[0].valor_excase != 14) LCD.clearLcd();
				if ((encoder_menu[14].valor_new >> 2) > 255) encoder_menu[14].valor_new = 0;
				llenar_source_address();
				LCD.sendString("F>", 10, 1);
				mensaje_can[5].address = (encoder_menu[14].valor_new >> 2); // asignación dirección
				rec_eeprom(14);
				encoder_menu[14].valor_old = encoder_menu[14].valor_new >> 2;
				encoder_menu[0].valor_excase = 14;
			}
		} break; // case 14

		case 15: { // Asignación Prioridades CAN
			if (encoder_menu[15].valor_old != encoder_menu[15].valor_new >> 2 || encoder_menu[0].valor_excase != 15) {
				// if (encoder_menu[0].valor_excase != 15) LCD.clearLcd();
				LCD.sendString(">Priority(0-7):", 0, 0);              // can A
				LCD.sendString(" DestAddress:", 0, 1);
				LCD.sendFloat((encoder_menu[16].valor_new >> 2), 3, 0, 13, 1);
				if ((encoder_menu[15].valor_new >> 2) > 7) encoder_menu[15].valor_new = 0;
				LCD.sendFloat((encoder_menu[15].valor_new >> 2), 1, 0, 15, 0);

				// Todas las tramas comparte prioridad
				for (uint8_t i = 0; i < 10; i++) mensaje_can[i].priority = (encoder_menu[15].valor_new >> 2);

				rec_eeprom(15);
				encoder_menu[15].valor_old = encoder_menu[15].valor_new >> 2;
				encoder_menu[0].valor_excase = 15;
			}
		} break; // case 15

		case 16: {     // Prioridad y Destination

			if (encoder_menu[16].valor_old != encoder_menu[16].valor_new >> 2 || encoder_menu[0].valor_excase != 16) {
				//if ((encoder_menu[0].valor_excase != 16) & (encoder_menu[0].valor_excase != 10)) LCD.clearLcd();
				LCD.sendString(" Priority(0-7):", 0, 0);
				LCD.sendFloat((encoder_menu[15].valor_new >> 2), 1, 0, 15, 0);
				LCD.sendString(">DestAddress:", 0, 1);
				if ((encoder_menu[16].valor_new >> 2) > 255) encoder_menu[16].valor_new = 0;
				LCD.sendFloat((encoder_menu[16].valor_new >> 2), 3, 0, 13, 1);
				rec_eeprom(16);
				encoder_menu[16].valor_old = encoder_menu[16].valor_new >> 2;
				encoder_menu[0].valor_excase = 16;

			}
		} break; // case 16

		case 17: {
			if (encoder_menu[17].valor_old != encoder_menu[17].valor_new >> 2 || encoder_menu[0].valor_excase != 17) {
				LCD.sendString("Serial :  ", 0, 0);
				LCD.sendString("(Set & Push)     ", 0, 1);
				if (encoder_menu[17].valor_new >> 2 == 0) LCD.sendString("  300", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 1) LCD.sendString("  600", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 2) LCD.sendString(" 1200", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 3) LCD.sendString(" 2400 ", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 4) LCD.sendString(" 4800", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 5) LCD.sendString(" 9600 ", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 6) LCD.sendString(" 14.4K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 7) LCD.sendString(" 19.2K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 8)  LCD.sendString(" 57.6K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 9)  LCD.sendString("115.2K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 10) LCD.sendString("128.0K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 11) LCD.sendString("153.6K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 12) LCD.sendString("230.4K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 13) LCD.sendString("250.0K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 14) LCD.sendString("256.0K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 > 14) (encoder_menu[17].valor_new = 14 << 2);
				rec_eeprom(17);
				encoder_menu[17].valor_old = encoder_menu[17].valor_new >> 2;
				encoder_menu[0].valor_excase = 17;
				flag_1 = 0;
			}
		} break; // case 17

		case 18: { // cambio día
				   //if (encoder_menu[0].valor_excase != 18) LCD.clearLcd();
			static char temp[16];
			sprintf(temp, "H/M/S %02d:%02d:%02d  ", tiempo.hor, tiempo.min, tiempo.sec);
			LCD.sendString(temp, 0, 0);
			if (encoder_menu[18].valor_old != encoder_menu[18].valor_new >> 2 || encoder_menu[0].valor_excase != 18) {
				if (encoder_menu[18].valor_new >> 2 == 32)encoder_menu[18].valor_new = 0;
				sprintf(temp, "D/M/Y>%02d-%02d-%d", encoder_menu[18].valor_new >> 2, encoder_menu[19].valor_new >> 2, encoder_menu[20].valor_new >> 2);
				LCD.sendString(temp, 0, 1);
				rec_eeprom(18);
				encoder_menu[18].valor_old = encoder_menu[18].valor_new >> 2;
			}
			encoder_menu[0].valor_excase = 18;
		} break; // case 18

		case 19: { // cambio mes
				   //if (encoder_menu[0].valor_excase != 19) LCD.clearLcd();
			static char temp[16];
			sprintf(temp, "H/M/S %02d:%02d:%02d", tiempo.hor, tiempo.min, tiempo.sec);
			LCD.sendString(temp, 0, 0);
			if (encoder_menu[19].valor_old != encoder_menu[19].valor_new >> 2 || encoder_menu[0].valor_excase != 19) {
				if (encoder_menu[19].valor_new >> 2 == 13)encoder_menu[19].valor_new = 0;
				sprintf(temp, "D/M/Y %02d>%02d-%d", encoder_menu[18].valor_new >> 2, encoder_menu[19].valor_new >> 2, encoder_menu[20].valor_new >> 2);
				LCD.sendString(temp, 0, 1);
				rec_eeprom(19);
				encoder_menu[19].valor_old = encoder_menu[19].valor_new >> 2;
			}
			encoder_menu[0].valor_excase = 19;
		} break; // case 19

		case 20: {
			//if (encoder_menu[0].valor_excase != 20) LCD.clearLcd();
			static char temp[16];
			sprintf(temp, "H/M/S %02d:%02d:%02d", tiempo.hor, tiempo.min, tiempo.sec);
			LCD.sendString(temp, 0, 0);
			if (encoder_menu[20].valor_old != encoder_menu[20].valor_new >> 2 || encoder_menu[0].valor_excase != 20) {
				if (encoder_menu[20].valor_new >> 2 <= 1985) encoder_menu[20].valor_new = 1985 << 2;
				sprintf(temp, "D/M/Y %02d-%02d>%d", encoder_menu[18].valor_new >> 2, encoder_menu[19].valor_new >> 2, encoder_menu[20].valor_new >> 2);
				LCD.sendString(temp, 0, 1);
				rec_eeprom(20);
				encoder_menu[20].valor_old = encoder_menu[20].valor_new >> 2;
			}
			encoder_menu[0].valor_excase = 20;
		} break; // case 20

		case 21: {  //mensaje_can[6]
			if (encoder_menu[0].valor_excase != 21) LCD.clearLcd();

			LCD.sendString(frames_PGN[Value2 - 15].address_PGN_hex, 0, 0);
			LCD.sendString("Trip", 9, 0);
			for (int i = 0; i < 8; i++) {
				char array_prov[3];
				itoa(mensaje_can[Value2 - 15].msg[i], array_prov, 16);
				if (mensaje_can[Value2 - 15].msg[i] > 16) LCD.sendString(array_prov, 2 * i, 1);
				else { LCD.sendString(array_prov, 2 * i + 1, 1); LCD.sendString("0", 2 * i, 1); }
			}
			rec_eeprom(21);
			encoder_menu[0].valor_excase = 21;
		} break; // case 21

		case 22: { // mensaje_can[7]
			if (encoder_menu[0].valor_excase != 22) LCD.clearLcd();
			LCD.sendString(frames_PGN[Value2 - 15].address_PGN_hex, 0, 0);
			LCD.sendString("Time/C", 9, 0);
			for (int i = 0; i < 8; i++) {
				char array_prov[3];
				itoa(mensaje_can[Value2 - 15].msg[i], array_prov, 16);
				if (mensaje_can[Value2 - 15].msg[i] > 16) LCD.sendString(array_prov, 2 * i, 1);
				else { LCD.sendString(array_prov, 2 * i + 1, 1); LCD.sendString("0", 2 * i, 1); }
			}
			rec_eeprom(22);
			encoder_menu[0].valor_excase = 22;
		} break; // case 22

		case 23: {  // can 8
			if (encoder_menu[0].valor_excase != 23) LCD.clearLcd();
			LCD.sendString(frames_PGN[Value2 - 15].address_PGN_hex, 0, 0);
			LCD.sendString("DM01", 9, 0);
			for (int i = 0; i < 8; i++) {
				char array_prov[3];
				itoa(mensaje_can[Value2 - 15].msg[i], array_prov, 16);
				if (mensaje_can[Value2 - 15].msg[i] > 16) LCD.sendString(array_prov, 2 * i, 1);
				else { LCD.sendString(array_prov, 2 * i + 1, 1); LCD.sendString("0", 2 * i, 1); }
			}
			rec_eeprom(23);
			encoder_menu[0].valor_excase = 23;
		} break; // case 23

		case 24: {
			if (encoder_menu[0].valor_excase != 24) LCD.clearLcd();
			LCD.sendString(frames_PGN[Value2 - 15].address_PGN_hex, 0, 0);
			LCD.sendString("AMB", 9, 0);
			for (int i = 0; i < 8; i++) {
				char array_prov[3];
				itoa(mensaje_can[Value2 - 15].msg[i], array_prov, 16);
				if (mensaje_can[Value2 - 15].msg[i] > 16) LCD.sendString(array_prov, 2 * i, 1);
				else { LCD.sendString(array_prov, 2 * i + 1, 1); LCD.sendString("0", 2 * i, 1); }
			}
			rec_eeprom(24);
			encoder_menu[0].valor_excase = 24;
		} break; // case 24

		case 25: { //Recepcción
			if (encoder_menu[0].valor_excase != 25) {
				LCD.clearLcd();
				LCD.sendString("R", 0, 0);
			}
			rec_eeprom(25);
			encoder_menu[0].valor_excase = 25;
		} break; // case 25

		case 26: {
			if (encoder_menu[26].valor_old != encoder_menu[26].valor_new >> 2 || encoder_menu[0].valor_excase != 26) {
				if ((encoder_menu[0].valor_excase != 26) & (1)) LCD.clearLcd();
				LCD.sendString(">DM1 SPN:", 0, 0);
				LCD.sendFloat((encoder_menu[26].valor_new >> 2), 3, 0, 10, 0);
				LCD.sendFloat((encoder_menu[27].valor_new >> 2), 3, 0, 11, 1);
				LCD.sendString(" DM1 FMI:", 0, 1);

				rec_eeprom(26);
				encoder_menu[26].valor_old = encoder_menu[26].valor_new >> 2;
				encoder_menu[0].valor_excase = 26;
			}
		} break; // case 26

		case 27: {
			if (encoder_menu[27].valor_old != encoder_menu[27].valor_new >> 2 || encoder_menu[0].valor_excase != 27) {
				if ((encoder_menu[0].valor_excase != 27) & (1)) LCD.clearLcd();
				if ((encoder_menu[27].valor_new >> 2) >31) encoder_menu[27].valor_new = 0;
				LCD.sendString(" DM1 SPN:", 0, 0);
				LCD.sendFloat((encoder_menu[27].valor_new >> 2), 3, 0, 11, 1);
				LCD.sendFloat((encoder_menu[26].valor_new >> 2), 3, 0, 10, 0);
				LCD.sendString(">DM1 FMI:", 0, 1);
				rec_eeprom(27);
				encoder_menu[27].valor_old = encoder_menu[27].valor_new >> 2;
				encoder_menu[0].valor_excase = 27;
			}
		} break; // case 27

		case 28: {
			if (encoder_menu[28].valor_old != encoder_menu[28].valor_new >> 2 || encoder_menu[0].valor_excase != 28) {
				if ((encoder_menu[0].valor_excase != 28) & (1)) LCD.clearLcd();
				if ((encoder_menu[28].valor_new >> 2) > 127) encoder_menu[28].valor_new = 0;
				LCD.sendString(">DM1 OC10:", 0, 0);
				LCD.sendFloat((encoder_menu[28].valor_new >> 2), 3, 0, 11, 0);
				LCD.sendFloat((encoder_menu[29].valor_new >> 2), 3, 0, 11, 1);
				LCD.sendString(" DM1 CM:", 0, 1);
				rec_eeprom(28);
				encoder_menu[28].valor_old = encoder_menu[28].valor_new >> 2;
				encoder_menu[0].valor_excase = 28;
			}
		} break; // case 28

		case 29: {
			if (encoder_menu[29].valor_old != encoder_menu[29].valor_new >> 2 || encoder_menu[0].valor_excase != 29) {
				if ((encoder_menu[0].valor_excase != 29) & (1)) LCD.clearLcd();
				if ((encoder_menu[29].valor_new >> 2) > 1) encoder_menu[29].valor_new = 0;
				LCD.sendString(" DM1 OC10:", 0, 0);
				LCD.sendFloat((encoder_menu[29].valor_new >> 2), 3, 0, 11, 1);
				LCD.sendFloat((encoder_menu[28].valor_new >> 2), 3, 0, 11, 0);
				LCD.sendString(">DM1 CM:", 0, 1);
				rec_eeprom(29);
				encoder_menu[29].valor_old = encoder_menu[29].valor_new >> 2;
				encoder_menu[0].valor_excase = 29;
			}
		} break; // case 29

		case 30: {
			if (encoder_menu[30].valor_old != encoder_menu[30].valor_new >> 2 || encoder_menu[0].valor_excase != 30) {
				// if ((encoder_menu[0].valor_excase != 30) & (1)) LCD.clearLcd();
				if ((encoder_menu[30].valor_new >> 2) > 3) encoder_menu[30].valor_new = 0;
				LCD.sendString(">MILS:0 ", 0, 0);
				LCD.sendString(" RSLS:0 ", 0, 1);
				LCD.sendString(" AWLS:0 ", 8, 0);
				LCD.sendString("  PLS:0 ", 8, 1);
				DTC(30);
			}
		} break; // case 30

		case 31: {
			if (encoder_menu[31].valor_old != encoder_menu[31].valor_new >> 2 || encoder_menu[0].valor_excase != 31) {
				//if ((encoder_menu[0].valor_excase != 31) & (1)) LCD.clearLcd();
				if ((encoder_menu[31].valor_new >> 2) > 3) encoder_menu[31].valor_new = 0;
				LCD.sendString(" MILS:0 ", 0, 0);
				LCD.sendString(">RSLS:0 ", 0, 1);
				LCD.sendString(" AWLS:0 ", 8, 0);
				LCD.sendString("  PLS:0 ", 8, 1);
				DTC(31);
			}
		} break; // case 31

		case 32: {
			if (encoder_menu[32].valor_old != encoder_menu[32].valor_new >> 2 || encoder_menu[0].valor_excase != 32) {
				//if ((encoder_menu[0].valor_excase != 32) & (1)) LCD.clearLcd();
				if ((encoder_menu[32].valor_new >> 2) > 3) encoder_menu[32].valor_new = 0;
				LCD.sendString(" MILS:0 ", 0, 0);
				LCD.sendString(" RSLS:0 ", 0, 1);
				LCD.sendString(">AWLS:0 ", 8, 0);
				LCD.sendString("  PLS:0 ", 8, 1);
				DTC(32);
			}
		} break; // case 32

		case 33: {
			if (encoder_menu[33].valor_old != encoder_menu[33].valor_new >> 2 || encoder_menu[0].valor_excase != 33) {
				// if ((encoder_menu[0].valor_excase != 33) & (1)) LCD.clearLcd();
				if ((encoder_menu[33].valor_new >> 2) > 3) encoder_menu[33].valor_new = 0;
				LCD.sendString(" MILS:0 ", 0, 0);
				LCD.sendString(" RSLS:0 ", 0, 1);
				LCD.sendString(" AWLS:0 ", 8, 0);
				LCD.sendString("> PLS:0 ", 8, 1);
				DTC(33);
			}
		} break; // case 33

		case 34: {  // PGN variable
			if (encoder_menu[34].valor_old != encoder_menu[34].valor_new >> 2 || encoder_menu[0].valor_excase != 34) {
				if ((encoder_menu[0].valor_excase != 34) & (encoder_menu[0].valor_excase != 34)) LCD.clearLcd();
				char temp1[sizeof(long)];
				LCD.sendString("VAR-ID:", 0, 0);
				long lMessageID_local = ((long)mensaje_can[9].priority << 26) + ((long)mensaje_can[9].PGN << 8) + ((long)mensaje_can[9].address);
				ltoa(lMessageID_local, temp1, 16);
				LCD.sendString(temp1, 8, 0);

				for (uint8_t i = 0; i < 8; i++) {
					char array_prov[3];
					itoa(mensaje_can[9].msg[i], array_prov, 16);
					if (mensaje_can[9].msg[i] > 16) LCD.sendString(array_prov, 2 * i, 1);
					else { LCD.sendString(array_prov, 2 * i + 1, 1); LCD.sendString("0", 2 * i, 1); }
				}

				rec_eeprom(34);
				encoder_menu[34].valor_old = encoder_menu[34].valor_new >> 2;
				encoder_menu[0].valor_excase = 34;
			}
		} break; // case 34

		case 35: {     // PGN
			if (encoder_menu[35].valor_old != encoder_menu[35].valor_new >> 2 || encoder_menu[0].valor_excase != 35) {
				if ((encoder_menu[0].valor_excase != 35) & (encoder_menu[0].valor_excase != 35)) LCD.clearLcd();
				// tope en PGN=65535
				if ((encoder_menu[35].valor_new >> 2) >= (267 + 8192)) encoder_menu[35].valor_new = (8192 + 266) << 2;
				ver_PGN(3, 0, 35);
			}
		} break; // case 35

		case 36: {     // D
			if (encoder_menu[36].valor_old != encoder_menu[36].valor_new >> 2 || encoder_menu[0].valor_excase != 36) {
				ver_PGN(11, 0, 36);
			}
		} break; // case 36

		case 37: {     // P
			if (encoder_menu[37].valor_old != encoder_menu[37].valor_new >> 2 || encoder_menu[0].valor_excase != 37) {
				ver_PGN(1, 1, 37);
			}
		} break; // case 37

		case 38: {     // T
			if (encoder_menu[38].valor_old != encoder_menu[38].valor_new >> 2 || encoder_menu[0].valor_excase != 38) {
				ver_PGN(5, 1, 38);
			}
		} break; // case 38

		case 39: { // Y/N
			if (encoder_menu[39].valor_old != encoder_menu[39].valor_new >> 2 || encoder_menu[0].valor_excase != 39) {
				if ((encoder_menu[0].valor_excase != 39) & (encoder_menu[0].valor_excase != 39)) LCD.clearLcd();
				//   if (encoder_menu[39].valor_new >>2) vTaskResume(xHandle[9]); else vTaskSuspend(xHandle[9]);
				ver_PGN(13, 1, 39);
			}
		} break; // case 39


		case 40: {
			if (encoder_menu[40].valor_old != encoder_menu[40].valor_new >> 2 || encoder_menu[0].valor_excase != 40) {
				if ((encoder_menu[0].valor_excase != 40) & (encoder_menu[0].valor_excase != 40)) LCD.sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 40

		case 41: {
			if (encoder_menu[41].valor_old != encoder_menu[41].valor_new >> 2 || encoder_menu[0].valor_excase != 41) {
				if ((encoder_menu[0].valor_excase != 41) & (encoder_menu[0].valor_excase != 41)) LCD.sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 41

		case 42: {
			if (encoder_menu[42].valor_old != encoder_menu[42].valor_new >> 2 || encoder_menu[0].valor_excase != 42) {
				if ((encoder_menu[0].valor_excase != 42) & (encoder_menu[0].valor_excase != 42)) LCD.sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 42

		case 43: {
			if (encoder_menu[43].valor_old != encoder_menu[43].valor_new >> 2 || encoder_menu[0].valor_excase != 43) {
				if ((encoder_menu[0].valor_excase != 43) & (encoder_menu[0].valor_excase != 43)) LCD.sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 43

		case 44: {
			if (encoder_menu[44].valor_old != encoder_menu[44].valor_new >> 2 || encoder_menu[0].valor_excase != 44) {
				if ((encoder_menu[0].valor_excase != 44) & (encoder_menu[0].valor_excase != 44)) LCD.sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 44

		case 45: {
			if (encoder_menu[45].valor_old != encoder_menu[45].valor_new >> 2 || encoder_menu[0].valor_excase != 45) {
				if ((encoder_menu[0].valor_excase != 45) & (encoder_menu[0].valor_excase != 45)) LCD.sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 45

		case 46: {
			if (encoder_menu[46].valor_old != encoder_menu[46].valor_new >> 2 || encoder_menu[0].valor_excase != 46) {
				if ((encoder_menu[0].valor_excase != 46) & (encoder_menu[0].valor_excase != 46)) LCD.sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 46

		case 47: {
			if (encoder_menu[47].valor_old != encoder_menu[47].valor_new >> 2 || encoder_menu[0].valor_excase != 47) {
				if ((encoder_menu[0].valor_excase != 47) & (encoder_menu[0].valor_excase != 47)) LCD.sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 47

		case 48: {
			if (encoder_menu[48].valor_old != encoder_menu[48].valor_new >> 2 || encoder_menu[0].valor_excase != 48) {
				if ((encoder_menu[0].valor_excase != 48) & (encoder_menu[0].valor_excase != 48)) LCD.clearLcd();
				if ((encoder_menu[48].valor_new >> 2) > 1) encoder_menu[48].valor_new = 1 << 2;

				LCD.sendString(" Contrast: ", 0, 1);
				LCD.sendFloat((encoder_menu[49].valor_new >> 2), 3, 0, 11, 1);  // Contraste

				if ((encoder_menu[48].valor_new >> 2) == 1) LCD.sendString(">Banners: YES ", 0, 0);     // can A
				else LCD.sendString(">Banners: NO ", 0, 0);

				rec_eeprom(48);
				encoder_menu[48].valor_old = encoder_menu[48].valor_new >> 2;
				encoder_menu[0].valor_excase = 48;
			}
		} break; // case 48
		case 49: {
			if (encoder_menu[49].valor_old != encoder_menu[49].valor_new >> 2 || encoder_menu[0].valor_excase != 49) {
				if ((encoder_menu[0].valor_excase != 49) & (encoder_menu[0].valor_excase != 49)) LCD.clearLcd();
				if ((encoder_menu[49].valor_new >> 2) > 0xFF) encoder_menu[49].valor_new = 1 << 2;

				if ((encoder_menu[48].valor_new >> 2) == 1) LCD.sendString(" Banners: YES ", 0, 0);     // can A
				else LCD.sendString(" Banners: NO ", 0, 0);

				//encoder_menu[49].valor_new += 4;

				LCD.sendString(">Contrast: ", 0, 1);
				LCD.sendFloat((encoder_menu[49].valor_new >> 2), 3, 0, 10, 1);  // Contraste
				LCD.setContrast((encoder_menu[49].valor_new >> 2));

				rec_eeprom(49);
				encoder_menu[49].valor_old = encoder_menu[49].valor_new >> 2;
				encoder_menu[0].valor_excase = 49;
			}
		} break; // case 49

		} // FIN SWITCH

#define valor_max_byte 0xFA
#define valor_max_word 0xFAFF
#define valor_max_dword 0xFAFFFFFF

		 // CORE: actualización tramas CAN
		for (uint8_t k = 0; k < 6; k++) {
			uint8_t SPN_length = encoder_menu[k].SPN_length_int;
			if (SPN_length != 0) {  // sólo si se inicializa pulsando Der.
				uint32_t p = pow(2, SPN_length);
				uint8_t atoip = encoder_menu[k].posicion_int;
				uint8_t atoip2 = encoder_menu[k].posicion2_int;
				if (SPN_length == 32)
				{
					uint32_t muestra = map(analogRead(k + 54), 0, 1023, 0, valor_max_dword);
					mensaje_can[k].msg[atoip - 1] = (uint8_t)(muestra & 0xFF);
					mensaje_can[k].msg[atoip] = (uint8_t)((muestra & 0xFF00) >> 8);
					mensaje_can[k].msg[atoip + 1] = (uint8_t)((muestra & 0xFF0000) >> 16);
					mensaje_can[k].msg[atoip + 2] = (uint8_t)((muestra & 0xFF000000) >> 24);
				}
				else if (SPN_length == 16)
				{
					uint16_t muestra = map(analogRead(k + 54), 0, 1023, 0, valor_max_word);
					mensaje_can[k].msg[atoip - 1] = (uint8_t)(muestra & 0x00FF);
					mensaje_can[k].msg[atoip] = (uint8_t)(muestra >> 8);
				}
				else {
					uint32_t muestra = map(analogRead(k + 54), 0, 1023, 0, (SPN_length == 8) * valor_max_byte + (SPN_length == 2) * 3 + (SPN_length == 4) * 15 + (SPN_length == 1) * 1);
					mensaje_can[k].msg[atoip - 1] = (mensaje_can[Value2 - 1].msg[atoip - 1] & (0xFF - (p << (atoip2 - 1)))) | muestra << (atoip2 - 1);
				}
			}//if
		}// for

		 // boton DER.
		if (!digitalRead(encoderSwitchPin1)) {

			if (Value2 == 16) encoder_menu[16].valor_new = 255 << 2; // carga 0xFF en Destino
			if (Value2 == 49) encoder_menu[49].valor_new = 0xFF << 2; // carga 0xFF en Destino

																	  // PGN configurable
			if (Value2 == 35) encoder_menu[35].valor_new = 8192 << 2;      // PGN
			else if (Value2 == 36) encoder_menu[36].valor_new = 0 << 2;    // Source address
			else if (Value2 == 37) encoder_menu[37].valor_new = 6 << 2;    // prioridad
			else if (Value2 == 38) encoder_menu[38].valor_new = 4 << 2;    // 250ms

			if ((Value2 == 7) | (Value2 == 17)) pinMode(PINtoRESET, OUTPUT); // reset cambio serial or CAN bps

			if (Value2 == 0) encoderValue2 = 25 << 2; // recepcción si botón derecho en arranque

			uint16_t mPGN = atoi(PGN); // no asigna si ya existe excepto en el propio
			uint8_t mPGN_bool;
			if (((mensaje_can[0].PGN != mPGN)
				&  (mensaje_can[1].PGN != mPGN)
				&  (mensaje_can[2].PGN != mPGN)
				&  (mensaje_can[3].PGN != mPGN)
				&  (mensaje_can[4].PGN != mPGN)
				&  (mensaje_can[5].PGN != mPGN)) | (mensaje_can[Value2 - 1].PGN == mPGN)) mPGN_bool = 1; else mPGN_bool = 0;// existe? o sea mPGN_bool = 0

			char ch[sizeof("A")]; // dónde está siendo usado?
			if (mensaje_can[0].PGN == mPGN) strcpy(ch, "A");
			else if (mensaje_can[1].PGN == mPGN) strcpy(ch, "B");
			else if (mensaje_can[2].PGN == mPGN) strcpy(ch, "C");
			else if (mensaje_can[3].PGN == mPGN) strcpy(ch, "D");
			else if (mensaje_can[4].PGN == mPGN) strcpy(ch, "E");
			else if (mensaje_can[5].PGN == mPGN) strcpy(ch, "F");

			if (mPGN_bool) encoder_menu[Value2].frame_activo = encoder_menu[Value2].valor_new >> 2;

			if ((Value2 <= 6) & (encoder_menu[Value2].valor_new >> 2 > 0)) { // Montar nuevas tramas

				if (mPGN_bool)
				{
					mensaje_can[Value2 - 1].PGN = atoi(PGN);

					// Borrar msg´s a 0xFF si PGN es nuevo
					if (mensaje_can[Value2 - 1].PGN != PGN_anterior)
					{
						for (uint8_t i = 0; i < 8; i++) mensaje_can[Value2 - 1].msg[i] = 0xFF;
						PGN_anterior = mensaje_can[Value2 - 1].PGN;
					}

					mensaje_can[Value2 - 1].periodo = atoi(rate); // periodo ms
					encoder_menu[Value2 - 1].posicion_int = atoi(position);
					encoder_menu[Value2 - 1].posicion2_int = atoi(position2);
					encoder_menu[Value2 - 1].SPN_length_int = atoi(SPN_length);

					vTaskResume(xHandle[Value2 - 1]); //activa trama
					delay(1000);

					if ((encoder_menu[48].valor_new >> 2) == 1) LCD.scrollString(buffer, 0, 100); // Banner
				}
				else { LCD.clearLcd(); LCD.sendString("PGN USED IN", 0, 0); LCD.sendString(ch, 12, 0); delay(1000); }
			}//if

			if ((Value2 <= 6) & (Value2 > 0) & (encoder_menu[Value2].valor_new >> 2 == 0)) vTaskSuspend(xHandle[Value2 - 1]); // borrar trama

			encoder_menu[Value2].valor_old = 99;
		}

		//  botón IZQ.

		if ((!digitalRead(encoderSwitchPin2) & (Value2 < 7) & (Value2 > 0)) || (flag_1 == 1)) {

			if (flag_1 == 0) LCD.clearLcd();
			encoder_menu[8].valor_new = 2 << 2;
			flag_1 = 1;
			LCD.sendString("CAN-ID:", 0, 0);
			LCD.sendString(frames_PGN[Value2 - 1].address_PGN_hex, 8, 0);
			for (int i = 0; i < 8; i++) {
				char array_prov[3];
				itoa(mensaje_can[Value2 - 1].msg[i], array_prov, 16);
				if (mensaje_can[Value2 - 1].msg[i] > 16) LCD.sendString(array_prov, 2 * i, 1);
				else { LCD.sendString(array_prov, 2 * i + 1, 1); LCD.sendString("0", 2 * i, 1); }
			}
		}

		// Encendido led rojo -> activación DTCs
		if ((!digitalRead(encoderSwitchPin1) & ((Value2 >= 26) & (Value2 <= 33))) | lock) {
			lock = 1;
			//vTaskResume(xHandle[8]); //activa trama DM01
			digitalWrite(pin_led_rojo, LOW);
			((encoder_menu[30].valor_new >> 2) == 1) ? digitalWrite(A8, LOW) : digitalWrite(A8, HIGH);
			((encoder_menu[31].valor_new >> 2) == 1) ? digitalWrite(A9, LOW) : digitalWrite(A9, HIGH);
			((encoder_menu[32].valor_new >> 2) == 1) ? digitalWrite(A10, LOW) : digitalWrite(A10, HIGH);
			((encoder_menu[33].valor_new >> 2) == 1) ? digitalWrite(A11, LOW) : digitalWrite(A11, HIGH);
			mensaje_can[8].msg[0] = (encoder_menu[33].valor_new >> 2) + (encoder_menu[32].valor_new) + (encoder_menu[31].valor_new << 2) + (encoder_menu[30].valor_new << 4); //MIL LED on
			mensaje_can[8].msg[1] = 0xFF;                                             //Flash
			mensaje_can[8].msg[2] = (uint8_t)(encoder_menu[26].valor_new >> 2);       //SPN
			mensaje_can[8].msg[3] = (uint8_t)(encoder_menu[26].valor_new >> 10);      //SPN
			mensaje_can[8].msg[4] = (uint8_t)(encoder_menu[27].valor_new >> 2);       //FMI
			mensaje_can[8].msg[5] = (uint8_t)(encoder_menu[28].valor_new >> 2);       //OC
			mensaje_can[8].msg[5] |= (encoder_menu[29].valor_new << 5);               //CM
			mensaje_can[8].msg[6] = 0xFF;
			mensaje_can[8].msg[7] = 0xFF;
		}

		if (!digitalRead(encoderSwitchPin2) & (Value2 == 35)) encoder_menu[35].valor_new = 8203 << 2;      // PGN

		 // Apagado led rojo -> desactivación DTCs
		if (!digitalRead(encoderSwitchPin2) & ((Value2 >= 26) & (Value2 <= 33))) {
			digitalWrite(pin_led_rojo, HIGH);
			digitalWrite(A8, HIGH); digitalWrite(A9, HIGH); digitalWrite(A10, HIGH); digitalWrite(A11, HIGH);
			//vTaskSuspend(xHandle[8]); // Apago trama DM01
			mensaje_can[8] = { 65226,1000,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } }; // DM01
			lock = 0;
		}

		/* We want this task to execute exactly every period milliseconds. */
		vTaskDelayUntil(&xLastWakeTime2, (50 / portTICK_PERIOD_MS));
	}
}

static void vPeriodicTask2(void *pvParameters)
{
	TickType_t xLastWakeTime2 = xTaskGetTickCount();  // en ms

	uint8_t *xmsg;

	/* As per most tasks, this task is implemented in an infinite loop. */
	for (;; )
	{

		if (alive_led) {
			// Alive
			digitalWrite(pin_led_verde, LOW);
			vTaskDelay(25 / portTICK_PERIOD_MS);
			digitalWrite(pin_led_verde, HIGH);
		}

		// Lectura cola CAN RX
		struct AMessage *pxRxedMessage;
		if (xQueue1 != 0)
		{
			// Receive a message on the created queue.  Block for 10 ticks if a message is not immediately available
			if (xQueueReceive(xQueue1, &(pxRxedMessage), (TickType_t)10))
			{
				// pcRxedMessage now points to the struct AMessage variable posted by vATask
				// Analyze the received PGN

				if (!alive_led) {
					digitalWrite(pin_led_verde, LOW);
				}

				if (pxRxedMessage->lPGN == 0xEA00) // peer-to-peer
				{
					switch (pxRxedMessage->nData[0] + (uint16_t)(pxRxedMessage->nData[1] << 8)) // en función del PGN recibido
					{
					case (msgEngineTemperature_PGN): {xmsg = msgEngineTemperature; } break;
					case (Engine_Total_Average_Fuel_Rate_PGN): {xmsg = Engine_Total_Average_Fuel_Rate; } break;
					case (Specific_Heat_Ratio_PGN): {xmsg = Specific_Heat_Ratio; } break;
					case (Engine_Cylinder_1_Combustion_Time_PGN): {xmsg = Engine_Cylinder_1_Combustion_Time; } break;
					case (Engine_Fuel_Flow_Rate_PGN): {xmsg = Engine_Fuel_Flow_Rate; } break;
					case (Engine_Cylinder_1_Ignition_Timing_PGN): {xmsg = Engine_Cylinder_1_Ignition_Timing; } break;
					case (Engine_Power_PGN): {xmsg = Engine_Power; } break;
					case (Engine_Actual_Exhaust_Oxygen_PGN): {xmsg = Engine_Actual_Exhaust_Oxygen; } break;
					case (Trip_Average_Fuel_Rate_PGN): {xmsg = Trip_Average_Fuel_Rate; } break;
					case (Trip_Cruise_Distance_PGN): {xmsg = Trip_Cruise_Distance; } break;
					case (Engine_Total_Hours_of_Operation_PGN): {xmsg = Engine_Total_Hours_of_Operation; } break;
					case (Engine_Trip_Fuel_PGN): {xmsg = Engine_Trip_Fuel; } break;
					} // fin switch

					if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE)
					{
						noInterrupts();
						j1939Transmit(pxRxedMessage->nData[0] + (uint16_t)(pxRxedMessage->nData[1] << 8) + (uint16_t)(pxRxedMessage->nData[2] << 16), pxRxedMessage->nPriority, encoder_menu[16].valor_new >> 2, pxRxedMessage->nSrcAddr, (char*)xmsg, 8);  // Transmit the message
						interrupts();
						xSemaphoreGive(xSemaphore);
					}

				} // fin peer-to-peer

				if ((encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 8)) {
					Serial.print("F:");
					Serial.print(pxRxedMessage->cuenta);
					Serial.print(" ");
				}

				if ((encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 7)) {
					Serial.print("F:");
					Serial2.print(pxRxedMessage->cuenta);
					Serial2.print(" ");
				}

				sprintf(sString, "PGN:%05u 0x%X P:%X Src:0x%X Dest:0x%X ", (int)pxRxedMessage->lPGN, (int)pxRxedMessage->lPGN, pxRxedMessage->nPriority, pxRxedMessage->nSrcAddr, pxRxedMessage->nDestAddr);
				if ((encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 7)) Serial2.print(sString);
				if ((encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 8)) Serial.print(sString);

				// Recepcíón
				// mostrar en LCD si menu=25
				if (Value2 == 25) {
					if (IsOdd(tiempo.sec) == true) LCD.sendString("R ", 0, 0); else LCD.sendString("  ", 0, 0); // parpadeo

					long lMessageID2 = ((long)pxRxedMessage->nPriority << 26) + ((long)pxRxedMessage->lPGN << 8) + (long)pxRxedMessage->nSrcAddr;
					ltoa(lMessageID2, sString, 16);
					LCD.sendString(sString, 2, 0);

					LCD.sendFloat(pxRxedMessage->time, 4, 0, 11, 0); // periodo

					if ((pxRxedMessage->time) < 1000) LCD.sendString(" ", 15, 0);

					// Evitar restos de datos anteriores
					for (int p = 0; p < 2 * (8 - pxRxedMessage->nDataLen); p++)
						LCD.sendString(" ", 16 - p, 1);
				}

				if (xMessage.nDataLen == 0) Serial.print("No Data.\n\r");
				else
				{
					if ((encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 8)) Serial.print("Data: ");
					if ((encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 7)) Serial2.print("Data: ");

					for (int nIndex = 0; nIndex < pxRxedMessage->nDataLen; nIndex++)
					{
						sprintf(sString, "0x%X ", pxRxedMessage->nData[nIndex]);
						if ((encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 8)) Serial.print(sString);
						if ((encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 7))  Serial2.print(sString);

						// mostrar en LCD si menu=25
						if (Value2 == 25) {
							sprintf(sString, "%X ", pxRxedMessage->nData[nIndex]);
							if (pxRxedMessage->nData[nIndex] > 16) LCD.sendString(sString, 2 * nIndex, 1);
							else { LCD.sendString(sString, 2 * nIndex + 1, 1); LCD.sendString("0", 2 * nIndex, 1); }
						}

					}// end for

					if ((encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 8)) {
						Serial.print("T:");
						Serial.print(time_old);
						Serial.print("\n\r");
					}

					if ((encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 7)) {
						Serial2.print("T:");
						Serial2.print(time_old);
						Serial2.print("\n\r");
					}

				}// end else
			}

			if (!alive_led) {
				digitalWrite(pin_led_verde, HIGH);
			}

		}
		/* We want this task to execute exactly every 100 milliseconds. */
		vTaskDelayUntil(&xLastWakeTime2, (100 / portTICK_PERIOD_MS));
	}
}

void vPeriodicTask3(struct mystruct* can_tx) // Creación tramas CAN
{
	TickType_t xLastWakeTime = xTaskGetTickCount();

	/* As per most tasks, this task is implemented in an infinite loop. */
	for (;; )
	{
		// Ponderación del periodo
		if (can_tx->periodo == 50) can_tx->periodo = 49;
		else if (can_tx->periodo == 100) can_tx->periodo = 98;
		else if (can_tx->periodo == 200) can_tx->periodo = 196;
		else if (can_tx->periodo == 250) can_tx->periodo = 244;
		else if (can_tx->periodo == 500) can_tx->periodo = 488;
		else if (can_tx->periodo == 1000) can_tx->periodo = 977;
		else if (can_tx->periodo == 5000) can_tx->periodo = 4883;
		else if (can_tx->periodo == 10000) can_tx->periodo = 9766;

		if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE)
		{
			noInterrupts();
			j1939Transmit(can_tx->PGN, can_tx->priority, can_tx->address, (encoder_menu[16].valor_new >> 2), can_tx->msg, 8);  // Transmit the message
			interrupts();
			xSemaphoreGive(xSemaphore);
		}

		/* We want this task to execute exactly every period milliseconds. */
		vTaskDelayUntil(&xLastWakeTime, (can_tx->periodo / portTICK_PERIOD_MS));
	}
}

// Serial TX
void vPeriodicTask4(void *pvParameters)
{
	TickType_t xLastWakeTime = xTaskGetTickCount();

	/* As per most tasks, this task is implemented in an infinite loop. */
	for (;;)
	{
		if ((encoder_menu[8].valor_new >> 2 == 1) | (encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 6)) {
			for (int u = 0; u < 10; u++) {
				Serial.print("PGN:");
				if (mensaje_can[u].PGN<1000) Serial.print("  ");
				else if (mensaje_can[u].PGN<10000) Serial.print(" ");
				Serial.print(mensaje_can[u].PGN); Serial.print(" ID:");
				lMessageID = ((long)mensaje_can[u].priority << 26) + ((long)mensaje_can[u].PGN << 8) + ((long)mensaje_can[u].address);
				ltoa(lMessageID, frames_PGN[u].address_PGN_hex, 16);
				Serial.print(lMessageID, HEX); Serial.print(" Data: ");
				for (int i = 0; i < 8; i++) {
					Serial.print("0x"); if (mensaje_can[u].msg[i] < 17) Serial.print("0");
					Serial.print(mensaje_can[u].msg[i], HEX); if (i != 7) Serial.print(",");
				}Serial.println();
			}//for
			Serial.print(encoder_menu[7].valor_new >> 2 + 3); Serial.print(count);
			if (!(encoder_menu[8].valor_new >> 2 == 1) | (encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 5)) count++;
			Serial.print(" Time(ms):");
			Serial.println(xTaskGetTickCount()); Serial.println("");
		} // fin if

		if ((encoder_menu[8].valor_new >> 2 == 1) | (encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 5)) {
			for (int u = 0; u < 10; u++) {
				Serial2.print("PGN:");
				if (mensaje_can[u].PGN<1000) Serial2.print("  ");
				else if (mensaje_can[u].PGN<10000) Serial2.print(" ");
				Serial2.print(mensaje_can[u].PGN); Serial2.print(" ID:");
				lMessageID = ((long)mensaje_can[u].priority << 26) + ((long)mensaje_can[u].PGN << 8) + ((long)mensaje_can[u].address);
				ltoa(lMessageID, frames_PGN[u].address_PGN_hex, 16);
				Serial2.print(lMessageID, HEX); Serial2.print(" Data: ");
				for (int i = 0; i < 8; i++) {
					Serial2.print("0x"); if (mensaje_can[u].msg[i] < 17) Serial2.print("0");
					Serial2.print(mensaje_can[u].msg[i], HEX); if (i != 7) Serial2.print(",");
				}Serial2.println();
			}Serial2.print(encoder_menu[7].valor_new >> 2 + 3); Serial2.print(count); count++; Serial2.print(" Time(ms):");
			Serial2.println(xTaskGetTickCount()); Serial2.println("");
		} // fin if

		tiempo.sec++; if (tiempo.sec == 60) { tiempo.sec = 0; tiempo.min++; }
		if (tiempo.min == 60) { tiempo.min = 0; tiempo.hor++; }

		// Paso día
		if (tiempo.hor == 24) {
			tiempo.hor = 0;  encoder_menu[18].valor_new += 4;
		}

		// Paso mes
		switch (encoder_menu[19].valor_new >> 2)
		{
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			if ((encoder_menu[18].valor_new >> 2) == 32) {
				encoder_menu[18].valor_new = 0;
				encoder_menu[19].valor_new += 4;
			}
			break;
		case 4: case 6: case 9: case 11:
			if ((encoder_menu[18].valor_new >> 2) == 31) {
				encoder_menu[18].valor_new = 0;
				encoder_menu[19].valor_new += 4;
			}
			break;
		case 2:
			if ((encoder_menu[20].valor_new >> 2) % 4 == 0)
			{
				if ((encoder_menu[18].valor_new >> 2) == 29) {
					encoder_menu[18].valor_new = 0;
					encoder_menu[19].valor_new += 4;
				}
				else
					if ((encoder_menu[18].valor_new >> 2) == 30) {
						encoder_menu[18].valor_new = 0;
						encoder_menu[19].valor_new += 4;
					}
			}
			break;
		}

		// paso año
		if ((encoder_menu[19].valor_new >> 2) == 13) {
			encoder_menu[19].valor_new = 1 << 2;
			encoder_menu[20].valor_new += 4;
		}

		// calendario reloj en tramas can
		mensaje_can[7].msg[0] = 4 * tiempo.sec;
		mensaje_can[7].msg[1] = tiempo.min;
		mensaje_can[7].msg[2] = tiempo.hor;
		mensaje_can[7].msg[3] = encoder_menu[19].valor_new >> 2;
		mensaje_can[7].msg[4] = 4 * (encoder_menu[18].valor_new >> 2);
		mensaje_can[7].msg[5] = (encoder_menu[20].valor_new >> 2) - 193;

		// Trip distance
		for (byte i = 0; i < 4; i++) mensaje_can[6].msg[i] = (uint8_t)(kilometros & 0x00FF << i * 8) >> 8 * i;
		kilometros++;

		/* We want this task to execute exactly every period milliseconds. */
		vTaskDelayUntil(&xLastWakeTime, (1000 / portTICK_PERIOD_MS));
	}
}
//------------------------------------------------------------------------------

// Interrupción encoder 1
void loop() {}

void recibir_can() {

	if (xQueue1 != NULL)
	{
		xMessage.time = millis() - time_old;
		time_old = millis();
		xMessage.cuenta++;

		j1939Receive(&xMessage.lPGN, &xMessage.nPriority, &xMessage.nSrcAddr, &xMessage.nDestAddr, xMessage.nData, &xMessage.nDataLen);

		// Send a pointer to a struct AMessage object.  Don't block if the queue is already full
		pxMessage = &xMessage;
		xQueueSend(xQueue1, (void *)&pxMessage, (TickType_t)0);
	}
}

void updateEncoder1() { //derecho

	static uint8_t MSB, LSB, encoded, sum;

	MSB = digitalRead(5); //MSB = most significant bit
	LSB = digitalRead(6); //LSB = least significant bit
	encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
	sum = (lastEncoded1 << 2) | encoded; //adding it to the previous encoded value

	if ((sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) && encoder_menu[Value2].valor_new >= 1) encoder_menu[Value2].valor_new--;
	else if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoder_menu[Value2].valor_new++;

	lastEncoded1 = encoded; //store this value for next time
}

// Interrupción encoder 2
void updateEncoder2() {

	static uint8_t MSB, LSB, encoded, sum;

	MSB = digitalRead(encoderPin3); //MSB = most significant bit
	LSB = digitalRead(encoderPin4); //LSB = least significant bit
	encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
	sum = (lastEncoded2 << 2) | encoded; //adding it to the previous encoded value

	if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue2++;
	else if ((sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) && 1) encoderValue2--;

	if (encoderValue2 < 0)  encoderValue2 = (cases << 2); // revolver en menús
	else if (encoderValue2 >(cases << 2))  encoderValue2 = 0;

	lastEncoded2 = encoded; //store this value for next time
}

/*
//Uno

#include <AESLib.h>
#include <SoftwareSerial.h>
SoftwareSerial portOne(10, 11);

int MAX_CMD_LENGTH = 30;
char cmd[30];
int cmdIndex;
uint8_t PINtoRESET = 9;
long watch_dog=0;

uint8_t key[] = {0,11,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
uint8_t key2[] = {0,2,4,6,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,33};
uint8_t spin=0;

void setup() {

// Setup portOne Port with baud rate of 9600bps
portOne.begin(9600);
Serial.begin(9600);
pinMode(13, OUTPUT); // led

pinMode(2, INPUT);
pinMode(3, INPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);

cmdIndex = 0;

attachInterrupt(digitalPinToInterrupt(2), updateEncoder1, CHANGE); //Der.
attachInterrupt(digitalPinToInterrupt(3), updateEncoder1, CHANGE); //Der.
}

void updateEncoder1() {

uint8_t MSB = digitalRead(2); //MSB = most significant bit
uint8_t LSB = digitalRead(3); //LSB = least significant bit
if (MSB) digitalWrite (5,HIGH); else digitalWrite (5,LOW);
if (LSB) digitalWrite (6,HIGH); else digitalWrite (6,LOW);
if (spin) {digitalWrite (4,HIGH); spin=0;} else {digitalWrite (4,LOW); spin=1;}

}

void loop() {

//if (digitalRead (8) == 1) digitalWrite (9,HIGH); else digitalWrite (9,LOW);

if (portOne.available()) {

char byteIn = portOne.read();
cmd[cmdIndex] = byteIn;

if(byteIn=='\n'){
//command finished
cmd[cmdIndex] = '\0';
//portOne.println(cmd);
cmdIndex = 0;
digitalWrite(13,HIGH);
aes256_dec_single(key, cmd);
aes256_enc_single(key2, cmd);
portOne.println(cmd);
digitalWrite(13,LOW);
}else if(cmdIndex++ >= MAX_CMD_LENGTH) cmdIndex = 0;
}

} // loop
*/


// Frecuencímetro

/*#include <AESLib.h>
#include <SoftwareSerial.h>
SoftwareSerial portOne(10, 11);

int MAX_CMD_LENGTH = 30;
char cmd[30];
int cmdIndex;
uint8_t PINtoRESET = 9;
uint8_t key[] = {0,11,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
uint8_t key2[] = {0,2,4,6,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,33};

long rango=2; // este es el rongo por el que se disparará la salida 2 y pasa a estado lógico 1
long ultimamedicion; // contiene el valor de la última medición que disparó a lógico 1, la salida 2
int ciclo=0; // 1=alto 0=bajo
int cambiodeciclo=0;
int picodetension;
int valledetension=1023;
long contadorvisualizacion;
long contadorciclo;
uint8_t cuenta=0;

void setup() {

pinMode(7, INPUT);    // Just to be clear, as default is INPUT. Not really needed.
digitalWrite(7, LOW); // Prime it, but does not actually set output. Does disable 10K pull Up, but who cares.
pinMode(6, INPUT);    // Just to be clear, as default is INPUT. Not really needed.
digitalWrite(7, LOW); // Prime it, but does not actually set output. Does disable 10K pull Up, but who cares.

pinMode(8, INPUT);  // entrada Reset 2560
pinMode(9, OUTPUT); // Salida hacia /Reset2560
Serial.begin(9600);
portOne.begin(9600);
pinMode(A0, INPUT); // Se establece el pin analógico 0 como pin de lectura de datos.
pinMode(13, OUTPUT);          // led

}

void loop() {

long sensorValue = analogRead(A0); // Se vuelca a la varibla sensorValue el valor de la tensión medida por el pin 0 analógico

while(portOne.available()) {

char byteIn = portOne.read();
cmd[cmdIndex] = byteIn;

if(byteIn=='\n'){
//command finished
cmd[cmdIndex] = '\0';
//portOne.println(cmd);
cmdIndex = 0;
digitalWrite(13,HIGH);
aes256_dec_single(key, cmd);
aes256_enc_single(key2, cmd);
portOne.println(cmd);
digitalWrite(13,LOW);
}else if(cmdIndex++ >= MAX_CMD_LENGTH) cmdIndex = 0;
}


if (micros()>contadorvisualizacion+1000000) // Muestra por el puerto serie los resultados, transcurrido un segundo

{

// AQUÍ SE IMPRIME EN EL PUERTO SERIE LOS RESULTADOS

Serial.print(" Hz=");
Serial.println(contadorciclo);

if (cuenta>5){
if ((contadorciclo < 6)| (contadorciclo > 15)) {pinMode(7, OUTPUT);}
} else cuenta++;

rango=(2+((picodetension-valledetension)/5)); // SE CALCULA EL GANGO MÁS ADECUADO PARA LA SEÑAL, CON EL FIN, DE REDUCIR ERRORES EN SEÑALES CON RUIDO DE FONDO
contadorvisualizacion=micros();//SE ASIGNA A LA VARIABLE CONTADORVISUALIZACIÓN EL TIEMPO EN MICROSENGUNDOS QUE LLEVA EL PROGRAMA EN EJECUCIÓN
picodetension=sensorValue;//SE ASIGNA A LA VARIABLE PICODETENSIÓN EL VALOR DE LA TENSIÒN LEIDA POR EL PUERTO ANALÓGICO CERO
valledetension=sensorValue;//SE ASIGNA A LA VALLEDETENSION EL VALOR DE LA TENSIÒN LEIDA POR EL PUERTO ANALÓGICO CERO
contadorciclo=0; // SE PONE A CERO LOS CICLOS CONTADOS O HERCIOS.
}

if (sensorValue >= ( ultimamedicion+rango) ) // La salida 2 pasa a 1 logico si la tensión medida en la entrada analógica 0 es mayor que la anterior lectura + latensión de RANGO

{

ultimamedicion = sensorValue;  // SE ASIGANA A LA VARIABLE ULTIMAMEDICION EL VALOR LEIDO POR LA ENTRADA ANALÓGICA CERO
ciclo=1;

if (sensorValue>picodetension) // SI LA TENSIÓN MEDIDA POR LA ENTRADA CERO, ES LA MAYOR DETECTADA, SE ASIGNA A LA VARIABLE PICODETENSIÓN EL VALOR LEYDO POR AL ENTRADA CERO ANALÓGICA

{
picodetension=sensorValue; // SE ASIGNA EL VALOR LEYDO POR LA ENTRADA CERO ANALÓGICA A LA VARIABLE PICODETENSIÓN.
}

}


if (sensorValue <= ( ultimamedicion-rango))  // La salida 2 pasa a 1 lógico si la tensión medida en la entrada analógica 0 es menor que la anterior lectura - la tensión de RANGO

{

ultimamedicion = sensorValue; // SE ASIGNA A LA VARIABLE ULTIMAMEDICIÓN LA LECTURA MEDIDA POR EL PUERTO ANALÓGICO CERO
ciclo=0; // EL CICLO SE PONE A CERO, ES DECIR, QUE EL VOLTAJE EMPIEZA A BAJAR DESDE EL PICO DE TENSIÓN MÁS ALTA

if (sensorValue<valledetension) // SE CUMPLE LA CONDICIÓN SI LA TENSIÓN DETECTADA POR EL PUERTO ANALÓGICO CERO ES MENOR QUE LA CONTENIDA EN LA VARIABLE VALLEDETENSIÓN
{
valledetension=sensorValue; //Se asigna a la variable valledetensión el valor medido por la entrada analógica cero
}

}


if (ciclo==1 && cambiodeciclo==0)

{
cambiodeciclo=1;
contadorciclo++;
}

if (ciclo==0 && cambiodeciclo==1)

{
cambiodeciclo=0;
}

}
*/


/*
void setup()
{
Serial.begin(9600);
Serial2.begin(34800); // comunicaciones Bluetooth

Serial.println("Enter AT commands:");
}

void loop()
{
if (Serial2.available())
{   char c = Serial2.read() ;
Serial.write(c);
}

if (Serial.available())
{ char c = Serial.read() ;
Serial.print(c);
Serial2.print(c);
}

}*/


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////                VERSION PARA LCD AZUL   ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

/*
#include "FreeRTOS_AVR.h"
#include <AESLib.h>

// Demo includes
#include "basic_io_avr.h"
#include <stdlib.h>
#include "mcp_can_j1939_v2.h"
#include "can_ext_j1939_v2.h"
#include <EEPROM.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
//#include <vld.h>

#define I2C_ADDR 0x3f
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7);

uint8_t alive_led = 0;   // 1=Alive 0=Led recepción
#define cases 50 // último menú+1
#define num_menu cases+2
#define Serial_N "S/N: BEGAS163133"
#define Options  "Version 6.0.0   "

#define PGN_extended 0  // 0=126 SPNs  1=360 SPNs

#if PGN_extended
#define tope_menu 359    // Modelo II
#define mem_tramas_can 100  // normalmente 200
#define mem_tasks 200  // normalmente 200
#define seguridad_activada 0
#else
						 //#define tope_menu 125  //  Modelo I
#define tope_menu 135  //  Modelo I
#define mem_tramas_can 200  // normalmente 200
#define mem_tasks 200  // normalmente 200
#define seguridad_activada 1
#endif

#define restaurar_menu 1
#define key_1 { 0,11,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 }
#define key_2 { 0, 2,4,6,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,33 }

#define mem_tramas_can 200  // normalmente 200
#define mem_tasks 200  // normalmente 200

#define msgEngineTemperature_PGN 65262 //0xfeee
#define Engine_Total_Average_Fuel_Rate_PGN 65101 //0xfe4d
#define Specific_Heat_Ratio_PGN 65109 //fe55
#define Engine_Cylinder_1_Combustion_Time_PGN 65147 //fe7b
#define Engine_Fuel_Flow_Rate_PGN 65153 //fe81
#define Engine_Cylinder_1_Ignition_Timing_PGN 65154 //fe82
#define Engine_Power_PGN 65168 //fe90
#define Engine_Actual_Exhaust_Oxygen_PGN 65193 //fea9
#define Trip_Average_Fuel_Rate_PGN 65203 //feb3
#define Trip_Cruise_Distance_PGN 65206 //feb6
#define Engine_Total_Hours_of_Operation_PGN 65253 //fee5
#define Engine_Trip_Fuel_PGN 65257 //fee9

// Base de datos J1939
#include <avr/pgmspace.h>

#if !PGN_extended

const char string_1[] PROGMEM = "- -     ---- ---- - - - --- - - - -";
const char string_2[] PROGMEM = "0 8     10ms TSC1 4 1 8  518 Engine_Requested_Torque/Torque_Limit 1_%/bit -125 %";
const char string_3[] PROGMEM = "0 8     10ms TSC1 2 1 16  898 Engine_Requested_Speed/Speed_Limit 0.125_rpm/bit 0 rpm";
const char string_4[]  PROGMEM = "256 8 50ms TC1 2 1 8 684 Requested_Percent_Clutch_Slip 0.4_%/bit 0 %";
const char string_5[] PROGMEM = "256 8   50ms TC1 6 5 2 1854 Transmission_Mode_3 4_states/2_bit 0 bit";
const char string_6[] PROGMEM = "256 8   50ms TC1 6 7 2 1855 Transmission_Mode_4 4_states/2_bit 0 bit";
const char string_7[] PROGMEM = "53248 8  100ms  CL 1 1 8 1487 Illumination_Brightness_Percent 0.4_%/bit 0 %";
const char string_8[] PROGMEM = "57344 8 1000ms CM1 1 1 8 986 Requested_Percent_Fan_Speed 0.4_%/bit 0 %";
const char string_9[] PROGMEM = "57344 8 1000ms CM1 5 7 2 1653 Vehicle_Limiting_Speed_Governor_Enable_Switch 4_states/2_bit 0 bit";
const char string_10[] PROGMEM = "57344 8 1000ms CM1 5 5 2 1654 Vehicle_Limiting_Speed_Governor_Increment_Switch 4_states/2_bit 0 bit";
const char string_11[] PROGMEM = "57344 8 1000ms CM1 5 3 2 1655 Vehicle_Limiting_Speed_Governor_Decrement_Switch 4_states/2_bit 0 bit";
const char string_12[] PROGMEM = "57344 8 1000ms CM1 6 7 2 1656 Engine_Automatic_Start_Enable_Switch 4_states/2_bit 0 bit";
const char string_13[] PROGMEM = "57344 8 1000ms CM1 6 5 2 1666 Automatic_Gear_Shifting_Enable_Switch 4_states/2_bit 0 bit";
const char string_14[] PROGMEM = "57344 8 1000ms CM1 4 3 2 1682 Battery_Main_Switch_Hold_Request 4_states/2_bit 0 bit";
const char string_15[] PROGMEM = "57344 8 1000ms CM1 7 1 4 1683 Auxiliary_Heater_Mode_Request 16_states/4_bit 0 bit";
const char string_16[] PROGMEM = "57344 8 1000ms CM1 4 1 2 1684 Auxiliary_Heater_Coolant_Pump_Request 4_states/2_bit 0 bit";
const char string_17[] PROGMEM = "57344 8 1000ms CM1 7 5 2 1685 Request_Engine_Zone_Heating 4_states/2_bit 0 bit";
const char string_18[] PROGMEM = "57344 8 1000ms CM1 7 7 2 1686 Request_Cab_Zone_Heating 4_states/2_bit 0 bit";
const char string_19[] PROGMEM = "57344 8 1000ms CM1 2 1 16 1691 Cab_Interior_Temperature_Command 0.03125_deg_C/bit -273 C";
const char string_20[] PROGMEM = "57344 8 1000ms CM1 4 5 2 1714 Operator_Seat_Direction_Switch 4_states/2_bit 0 bit";
const char string_21[] PROGMEM = "57344 8 1000ms CM1 4 7 2 1856 Seat_Belt_Switch 4_states/2_bit 0 bit";
const char string_22[] PROGMEM = "57344 8 1000ms CM1 8 1 8 2596 Selected_Maximum_Vehicle_Speed_Limit 1_count/bit 0 count";
const char string_23[] PROGMEM = "57344 8 1000ms CM1 6 1 2 3695 Diesel_Particulate_Filter_Regeneration_Inhibit_Switch 4_states/2_bit 0 bit";
const char string_24[] PROGMEM = "57344 8 1000ms CM1 6 3 2 3696 Diesel_Particulate_Filter_Regeneration_Force_Switch 4_states/2_bit 0 bit";
const char string_25[] PROGMEM = "61441 8 100ms EBC1 2 1 8 521 Brake_Pedal_Position 0.4_%/bit 0 %";
const char string_26[] PROGMEM = "61441 8 100ms EBC1 1 1 2 561 ASR_Engine_Control_Active 4_states/2_bit 0 bit";
const char string_27[] PROGMEM = "61441 8 100ms EBC1 1 3 2 562 ASR_Brake_Control_Active 4_states/2_bit 0 bit";
const char string_28[] PROGMEM = "61441 8 100ms EBC1 1 5 2 563 Anti-Lock_Braking_(ABS)_Active 4_states/2_bit 0 bit";
const char string_29[] PROGMEM = "61441 8 100ms EBC1 3 1 2 575 ABS_Off-road_Switch 4_states/2_bit 0 bit";
const char string_30[] PROGMEM = "61441 8 100ms EBC1 3 3 2 576 ASR_Off-road_Switch 4_states/2_bit 0 bit";
const char string_31[] PROGMEM = "61441 8 100ms EBC1 3 5 2 577 ASR_Hill_Holder_Switch 4_states/2_bit 0 bit";
const char string_32[] PROGMEM = "61441 8 100ms EBC1 4 7 2 969 Remote_Accelerator_Enable_Switch 4_states/2_bit 0 bit";
const char string_33[] PROGMEM = "61441 8 100ms EBC1 4 5 2 970 Engine_Auxiliary_Shutdown_Switch 4_states/2_bit 0 bit";
const char string_34[] PROGMEM = "61441 8 100ms EBC1 4 3 2 971 Engine_Derate_Switch 4_states/2_bit 0 bit";
const char string_35[] PROGMEM = "61441 8 100ms EBC1 4 1 2 972 Accelerator_Interlock_Switch 4_states/2_bit 0 bit";
const char string_36[] PROGMEM = "61441 8 100ms EBC1 5 1 8 973 Engine_Retarder_Selection 0.4_%/bit 0 %";
const char string_37[] PROGMEM = "61441 8 100ms EBC1 1 7 2 1121 EBS_Brake_Switch 4_states/2_bit 0 bit";
const char string_38[] PROGMEM = "61441 8 100ms EBC1 1 1 2 1238 Traction_Control_Override_Switch 4_states/2_bit 0 bit";
const char string_39[] PROGMEM = "61441 8 100ms EBC1 6 1 2 1243 ABS_Fully_Operational 4_states/2_bit 0 bit";
const char string_40[] PROGMEM = "61441 8 100ms EBC1 6 5 2 1438 ABS/EBS_Amber_Warning_Signal_(Powered_Vehicle) 4_states/2_bit 0 bit";
const char string_41[] PROGMEM = "61441 8 100ms EBC1 6 3 2 1439 EBS_Red_Warning_Signal 4_states/2_bit 0 bit";
const char string_42[] PROGMEM = "61441 8 100ms EBC1 7 1 8 1481 Source_Address_of_Controlling_Device_for_Brake_Control 1000msource_address/bit 0 SA";
const char string_43[] PROGMEM = "61441 8 100ms EBC1 8 7 2 1792 Tractor-Mounted_Trailer_ABS_Warning_Signal 4_states/2_bit 0 bit";
const char string_44[] PROGMEM = "61441 8 100ms EBC1 6 7 2 1793 ATC/ASR_Information_Signal 4_states/2_bit 0 bit";
const char string_45[] PROGMEM = "61441 8 100ms EBC1 8 5 2 1836 Trailer_ABS_Status 4_states/2_bit 0 bit";
const char string_46[] PROGMEM = "61441 8 100ms EBC1 8 3 2 2911 Halt_brake_switch 4_states/2_bit 0 bit";
const char string_47[] PROGMEM = "61442 8  10ms ETC1 2 1 16 191 Transmission_Output_Shaft_Speed 0.125_rpm/bit 0 rpm";
const char string_48[] PROGMEM = "61443 8  50ms EEC2 2 1 8 91 Accelerator_Pedal_Position_1 0.4_%/bit 0 %";
const char string_49[] PROGMEM = "61443 8  50ms EEC2 3 1 8 92 Engine_Percent_Load_At_Current_Speed 1_%/bit 0 %";
const char string_50[] PROGMEM = "61444 8  20ms EEC1 4 1 16 190 Engine_Speed 0.125_rpm/bit 0 rpm";
const char string_51[] PROGMEM = "61444 8  20ms EEC1 2 1 8 512 Driver's_Demand_Engine_-_Percent_Torque 1_%/bit -125 %";
const char string_52[] PROGMEM = "61444 8  20ms EEC1 3 1 8 513 Actual_Engine_-_Percent_Torque 1_%/bit -125 %";
const char string_53[] PROGMEM = "61444 8  20ms EEC1 1 1 4 899 Engine_Torque_Mode 16_states/4_bit 0 bit";
const char string_54[] PROGMEM = "61444 8  20ms EEC1 6 1 8 1483 Source_Address_of_Controlling_Device_for_Engine_Control 1000msource_address/bit 0 SA";
const char string_55[] PROGMEM = "61444 8  20ms EEC1 7 1 4 1675 Engine_Starter_Mode 16_states/4_bit 0 bit";
const char string_56[] PROGMEM = "61444 8  20ms EEC1 8 1 8 2432 Engine_Demand_?_Percent_Torque 1_%/bit -125 %";
const char string_57[] PROGMEM = "61444 8  20ms EEC1 1 5 4 4154 Actual_Engine_-_Percent_Torque_High_Resolution 0.125%/bit 0 %";
const char string_58[] PROGMEM = "61445 8 100ms ETC2 5 1 16 162 Transmission_Requested_Range ASCII 0 ASCII";
const char string_59[] PROGMEM = "61445 8 100ms ETC2 4 1 8 523 Transmission_Current_Gear 1_gear_value/bit -125 gear_value";
const char string_60[] PROGMEM = "61445 8 100ms ETC2 1 1 8 524 Transmission_Selected_Gear 1_gear_value/bit -125 gear_value";
const char string_61[] PROGMEM = "61445 8 100ms ETC2 2 1 16 526 Transmission_Actual_Gear_Ratio 0.001/bit 0 Ratio";
const char string_62[] PROGMEM = "61448 8  50ms HPG  1 1 16 1762 Hydraulic_Pressure 2_kPa/bit 0 kPa";
const char string_63[] PROGMEM = "61449 8  10ms VDC2 1 1 16 1807 Steering_Wheel_Angle 1/1024_rad_per_bit -31,374 deg";
const char string_64[] PROGMEM = "61449 8  10ms VDC2 4 1 16 1808 Yaw_Rate 1/8192_rad/s_per_bit -3,92 rad/s";
const char string_65[] PROGMEM = "61449 8  10ms VDC  6 1 16 1809 Lateral_Acceleration 1/2048_m/s?_per_bit -15,687 m/s2";
const char string_66[] PROGMEM = "61449 8  10ms VDC2 8 1 8 1810 Longitudinal_Acceleration 0.1_m/s?_per_bit -12,5 m/s2";
const char string_67[] PROGMEM = "61449 8  10ms VDC2 3 1 6 1811 Steering_Wheel_Turn_Counter 1_turn/bit -32 count";
const char string_68[] PROGMEM = "61449 8  10ms VDC2 3 7 2 1812 Steering_Wheel_Angle_Sensor_Type 4_states/2_bit 0 bit";
const char string_69[] PROGMEM = "61450 8  50ms EGF  1 1 16 132 Engine_Inlet_Air_Mass_Flow_Rate 0.05_kg/h_per_bit 0 kg/h";
const char string_70[] PROGMEM = "61450 8  50ms EGF1 1 1 16 2659 Engine_Exhaust_Gas_Recirculation_(EGR)_Mass_Flow_Rate 0.05_kg/h_per_bit 0 kg/h";
const char string_71[] PROGMEM = "61462 8  20ms CCS  1 1 2 3387 Engine_Cylinder_1_Combustion_Status 4_states/2_bit 0 bit";
const char string_72[] PROGMEM = "61463 8  20ms KL1  1 1 8 1352 Engine_Cylinder_1_Knock_Level 1_%/bit 0 %";
const char string_73[] PROGMEM = "61466 8  50ms TFAC 1 1 16 3464 Engine_Throttle_Actuator_1_Control_Command 0.0025_%/bit 0 %";
const char string_74[] PROGMEM = "61469 8  10ms SAS  1 1 16 3683 Steering_Wheel_Angle 1/1024_rad_per_bit -31,374 deg";
const char string_75[] PROGMEM = "64923 8 1000ms CRI1 2 1 8 3516 AftertreatmentsCR_Catalyst_Reagent_Concentration 0.25_%/bit 0 %";
const char string_76[] PROGMEM = "64923 8 1000ms CRI1 3 1 8 3518 AftertreatmentsCR_Catalyst_Reagent_Conductivity 5_microSiemens/mm 0 mS/mm";
const char string_77[] PROGMEM = "64923 8 1000ms CRI1 6 1 4 3521 AftertreatmentsCR_Catalyst_Reagent_Type 16_states/4_bit 0 bit";
const char string_78[] PROGMEM = "64925 8 1000ms SSV  1 1 16 3509 Sensor_supply_voltage_1 0.05_V/bit 0 Volts";
const char string_79[] PROGMEM = "64993 8 1000ms CACI 1 1 8 2609 Cab_A/C_Refrigerant_Compressor_Outlet_Pressure 16_kPa/bit 0 kPa";
const char string_80[] PROGMEM = "64998 8  100ms HBS 1 1 8 2580 Hydraulic_Brake_Pressure_Circuit_1 100_kPa/bit 0 kPa";
const char string_81[] PROGMEM = "64998 8  100ms HBS 3 5 2 2582 Hydraulic_Brake_Pressure_Supply_State_Circuit_1 4_states/2_bit 0 bit";
const char string_82[] PROGMEM = "64998 8  100ms HBS 3 1 2 2584 Hydraulic_Brake_Pressure_Warning_State_Circuit_1 4_states/2_bit 0 bit";
const char string_83[] PROGMEM = "65031 8  500ms ET  1 1 16 2433 Engine_Exhaust_Gas_Temperature_-_Right_Manifold 0.03125_deg_C/bit -273 C";
const char string_84[] PROGMEM = "65104 8 1000ms BT1 1 1 8 1800 Battery_1_Temperature 1_deg_C/bit -40 C";
const char string_85[] PROGMEM = "65110 8 1000ms TI1 1 1 8 1761 AftertreatmentsCR_Catalyst_Tank_Level 0.4_%/bit 0 %";
const char string_86[] PROGMEM = "65110 8 1000ms TI1 2 1 8 3031 AftertreatmentsCR_Catalyst_Tank_Temperature 1_deg_C/bit -40 C";
const char string_87[] PROGMEM = "65128 8 1000ms VF  1 1 8 1638 Hydraulic_Temperature 1_deg_C/bit -40 C";
const char string_88[] PROGMEM = "65128 8 1000ms VF 2 1 2 1713 Hydraulic_Oil_Filter_Restriction_Switch 4_states/2_bit 0 bit";
const char string_89[] PROGMEM = "65129 8 1000ms ET3 1 1 16 1636 Engine_Intake_Manifold_1_Air_Temperature_(High_Resolution) 0.03125_deg_C/bit -273 C";
const char string_90[] PROGMEM = "65129 8 1000ms ET3 3 1 16 1637 Engine_Coolant_Temperature_(High_Resolution) 0.03125_deg_C/bit -273 C";
const char string_91[] PROGMEM = "65170 8 100ms  EI  2 1 16 1209 Engine_Exhaust_Gas_Pressure 1/128_kPa/bit -250 kPa";
const char string_92[] PROGMEM = "65198 8 1000ms AIR1 1 1 8 46 Pneumatic_Supply_Pressure 8_kPa/bit 0 kPa";
const char string_93[] PROGMEM = "65245 8 1000ms TC   1 1 8 104 Engine_Turbocharger_Tube_Oil_Pressure_1 4_kPa/bit 0 kPa";
const char string_94[] PROGMEM = "65251 39 5000ms EC1 9 1 8 541 Engine_Percent_Torque_At_Point_3_(Engine_Configuration) 1_%/bit -125 %";
const char string_95[] PROGMEM = "65252 8 1000ms SHUTDN 1 7 2 590 Engine_Idle_Shutdown_Timer_State 4_states/2_bit 0 bit";
const char string_96[] PROGMEM = "65252 8 1000ms SHUTDN 2 7 2 591 Engine_Idle_Shutdown_Timer_Function 4_states/2_bit 0 bit";
const char string_97[] PROGMEM = "65252 8 1000ms SHUTDN 1 5 2 592 Engine_Idle_Shutdown_Timer_Override 4_states/2_bit 0 bit";
const char string_98[] PROGMEM = "65252 8 1000ms SHUTDN 1 1 2 593 Engine_Idle_Shutdown_has_Shutdown_Engine 4_states/2_bit 0 bit";
const char string_99[] PROGMEM = "65252 8 1000ms SHUTDN 1 3 2 594 Engine_Idle_Shutdown_Driver_Alert_Mode 4_states/2_bit 0 bit";
const char string_100[] PROGMEM = "65252 8 1000ms SHUTDN 3 5 2 605 Refrigerant_High_Pressure_Switch 4_states/2_bit 0 bit";
const char string_101[] PROGMEM = "65252 8 1000ms SHUTDN 3 3 2 875 Refrigerant_Low_Pressure_Switch 4_states/2_bit 0 bit";
const char string_102[] PROGMEM = "65252 8 1000ms SHUTDN 3 1 2 985 A/C_High_Pressure_Fan_Switch 4_states/2_bit 0 bit";
const char string_103[] PROGMEM = "65254 8 1000ms TD 1 1 8 959 Seconds 0.25_s/bit 0 s";
const char string_104[] PROGMEM = "65254 8 1000ms TD 2 1 8 960 Minutes 1_min/bit 0 s";
const char string_105[] PROGMEM = "65254 8 1000ms TD 3 1 8 961 Hours 1_hr/bit 0 s";
const char string_106[] PROGMEM = "65254 8 1000ms TD 5 1 8 962 Day 0.25_days/bit 0 Days";
const char string_107[] PROGMEM = "65254 8 1000ms TD 4 1 8 963 Month 1_month/bit 0 Months";
const char string_108[] PROGMEM = "65254 8 1000ms TD 6 1 8 964 Year 1_year/bit 1985 Years";
const char string_109[] PROGMEM = "65262 8 1000ms ET1 1 1 8 110 Engine_Coolant_Temperature 1_deg_C/bit -40 C";
const char string_110[] PROGMEM = "65262 8 1000ms ET1 2 1 8 174 Engine_Fuel_Temperature_1 1_deg_C/bit -40 C";
const char string_111[] PROGMEM = "65262 8 1000ms ET1 3 1 16 175 Engine_Oil_Temperature_1 0.03125_deg_C/bit -273 C";
const char string_112[] PROGMEM = "65262 8 1000ms ET1 7 1 8 52 Engine_Intercooler_Temperature 1_deg_C/bit -40 C";
const char string_113[] PROGMEM = "65263 8 500ms EFL/P1 1 1 8 94 Fuel_Delivery_Preasure 4_kPa/bit 0 kPa";
const char string_114[] PROGMEM = "65263 8 500ms EFL/P1 3 1 8 98 Engine_Oil_Level 0.4_%/bit 0 %";
const char string_115[] PROGMEM = "65263 8 500ms EFL/P1 4 1 8 100 Engine_Oil_Pressure 4_kPa/bit 0 kPa";
const char string_116[] PROGMEM = "65263 8 500ms EFL/P1 1 1 16 101 Crankcase_Preasure 2_kPa/bit 0 kPa";
const char string_117[] PROGMEM = "65263 8 500ms EFL/P1 7 1 8 109 Engine_Coolant_Pressure 2_kPa/bit 0 kPa";
const char string_118[] PROGMEM = "65263 8 500ms EFL/P1 8 1 8 111 Engine_Coolant_Level 0.4_%/bit 0 %";
const char string_119[] PROGMEM = "65265 8 100ms CCVS 2 1 16 84 Wheel-Based_Vehicle_Speed 1/256_km/h_per_bit 0 kph";
const char string_120[] PROGMEM = "65265 8 100ms CCVS 4 1 2 595 Cruise_Control_Active 4_states/2_bit 0 bit";
const char string_121[] PROGMEM = "65266 8 100ms LFE 7 1 8 183 Throttle_Position 0.4_degree/bit 0 degree";
const char string_122[] PROGMEM = "65266 8 100ms LFE 1 1 16 183 Engine_Fuel_Rate 0.05_L/h_per_bit 0 l/h";
const char string_123[] PROGMEM = "65266 8 100ms LFE 3 1 16 184 Engine_Instantaneous_Fuel_Economy 1/512_km/L_per_bit 0 km/L";
const char string_124[] PROGMEM = "65269 8 1000ms AMB 1 1 8 108 Barometric_Pressure 0.5_kPa/bit 0 kPa";
const char string_125[] PROGMEM = "65269 8 1000ms AMB 6 1 8 172 Engine_Air_Inlet_Temperature 1_deg_C/bit -40 C";
const char string_126[] PROGMEM = "65270 8 500ms IC1 2 1 8 102 Engine_Intake_Manifold_#1_Pressure 2_kPa/bit 0 kPa";
const char string_127[] PROGMEM = "65270 8 500ms IC1 3 1 8 105 Engine_Intake_Manifold_1_Temperature 1_deg_C/bit -40 C";
const char string_128[] PROGMEM = "65270 8 500ms IC1 4 1 8 106 Engine_Air_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_129[] PROGMEM = "65270 8 500ms IC1 6 1 16 173 Engine_Exhaust_Gas_Temperature 0.03125_deg_C/bit -273 C";
const char string_130[] PROGMEM = "65271 8 1000ms VEP1 7 1 16 158 Keyswitch_Battery_Potential 0.05_V/bit 0 Volts";
const char string_131[] PROGMEM = "65271 8 1000ms VEP1 5 1 16 168 Electrical_Potential 0.05_V/bit 0 Volts";
const char string_132[] PROGMEM = "65272 8 1000ms TRF1 3 1 8 126 Transmission_Filter_Differential_Pressure 2_kPa/bit 0 kPa";
const char string_133[] PROGMEM = "65272 8 1000ms TRF1 4 1 8 127 Transmission_Oil_Pressure 16_kPa/bit 0 kPa";
const char string_134[] PROGMEM = "65272 8 1000ms TRF1 5 1 16 177 Transmission_Oil_Temperature 1_deg_C/bit -273 C";
const char string_135[] PROGMEM = "65276 8 1000ms   DD 2 1 8 96 Fuel_Level_1 0.4_%/bit 0 %";
const char string_136[] PROGMEM = "65279 8 10000ms WFI 1 1 2 97 Water_In_Fuel_Indicator 4_states/2_bit 0 bit";

#else
const char string_1[]  PROGMEM = "- -   ---- ---- - - - --- - - - -";
const char string_2[]  PROGMEM = "256 8 50ms TC1 2 1 8 684 Requested_Percent_Clutch_Slip 0.4_%/bit 0 %";
const char string_3[]  PROGMEM = "256 8 50ms TC1 3 1 8 525 Transmission_Requested_Gear 1_gear_value/bit -125 gear_value";//
const char string_4[]  PROGMEM = "256 8 50ms TC1 7 3 4 4255 Transmission_Requested_Launch_Gear 16_states/4_bit 0 bit";
const char string_5[]  PROGMEM = "52992 8 5000ms CTL 1 1 8 1784 Engine_Speed_Limit_Request_-_Minimum_Continuous 32_rpm/bit 0 rpm";
const char string_6[]  PROGMEM = "52992 8 5000ms CTL 2 1 8 1785 Engine_Speed_Limit_Request_-_Maximum_Continuous 32_rpm/bit 0 rpm";
const char string_7[]  PROGMEM = "52992 8 5000ms CTL 3 1 8 1786 Engine_Torque_Limit_Request_-_Minimum_Continuous 1_%/bit -125 %";
const char string_8[]  PROGMEM = "52992 8 5000ms CTL 4 1 8 1787 Engine_Torque_Limit_Request_-_Maximum_Continuous 1_%/bit -125 %";
const char string_9[]  PROGMEM = "52992 8 5000ms CTL 5 1 8 1788 Minimum_Continuous_Retarder_Speed_Limit_Request 32_rpm/bit 0 rpm";
const char string_10[] PROGMEM = "52992 8 5000ms CTL 6 1 8 1789 Maximum_Continuous_Retarder_Speed_Limit_Request 32_rpm/bit 0 rpm";
const char string_11[] PROGMEM = "52992 8 5000ms CTL 7 1 8 1790 Minimum_Continuous_Retarder_Torque_Limit_Request 1_%/bit -125 %";
const char string_12[] PROGMEM = "52992 8 5000ms CTL 8 1 8 1791 Maximum_Continuous_Retarder_Torque_Limit_Request 1_%/bit -125 %";
const char string_13[] PROGMEM = "53248 8 5000ms CL 1 1 8 1487 Illumination_Brightness_Percent 0.4_%/bit 0 %";
const char string_14[] PROGMEM = "53760 8 100ms ASC2 2 1 4 1751 Nominal_Level_Request_Front_Axle 16_states/4_bit 0 bit";
const char string_15[] PROGMEM = "53760 8 100ms ASC2 2 5 4 1750 Nominal_Level_Request_Rear_Axle 16_states/4_bit 0 bit";
const char string_16[] PROGMEM = "53760 8 100ms ASC2 3 1 4 1753 Level_Control_Mode_Request 16_states/4_bit 0 bit";
const char string_17[] PROGMEM = "53760 8 100ms ASC2 4 1 8 1718 Damper_Stiffness_Request_Front_Axle 0.4_%/bit 0 %";
const char string_18[] PROGMEM = "53760 8 100ms ASC2 5 1 8 1719 Damper_Stiffness_Request_Rear_Axle 0.4_%/bit 0 %";
const char string_19[] PROGMEM = "53760 8 100ms ASC2 6 1 8 1720 Damper_Stiffness_Request_Lift_/_Tag_Axle 0.4_%/bit 0 %";
const char string_20[] PROGMEM = "57344 8 1000ms CM1 1 1 8 986 Requested_Percent_Fan_Speed 0.4_%/bit 0 %";
const char string_21[] PROGMEM = "57344 8 1000ms CM1 7 1 4 1683 Auxiliary_Heater_Mode_Request 16_states/4_bit 0 bit";
const char string_22[] PROGMEM = "57344 8 1000ms CM1 8 1 8 2596 Selected_Maximum_Vehicle_Speed_Limit 1_count/bit 0 count";
const char string_23[] PROGMEM = "61440 8 100ms ERC1 1 1 4 900 Retarder_Torque_Mode 16_states/4_bit 0 bit";
const char string_24[] PROGMEM = "61440 8 100ms ERC1 2 1 8 520 Actual_Retarder_-_Percent_Torque 1_%/bit -125 %";
const char string_25[] PROGMEM = "61440 8 100ms ERC1 3 1 8 1085 Intended_Retarder_Percent_Torque 1_%/bit -125 %";
const char string_26[] PROGMEM = "61440 8 100ms ERC1 5 1 8 1480 Source_Address_of_Controlling_Device_for_Retarder_Control 256_states/8_bit 0 Sa";
const char string_27[] PROGMEM = "61440 8 100ms ERC1 6 1 8 1715 Drivers_Demand_Retarder_-__Percent_Torque 1_%/bit -125 %";
const char string_28[] PROGMEM = "61440 8 100ms ERC1 7 1 8 1716 Retarder_Selection_non-engine 0.4_%/bit 0 %";
const char string_29[] PROGMEM = "61440 8 100ms ERC1 8 1 8 1717 Actual_Maximum_Available_Retarder_-_Percent_Torque 1_%/bit -125 %";
const char string_30[] PROGMEM = "61441 8 100ms EBC1 1 1 2 561 ASR_Engine_Control_Active 4_states/2_bit 0 bit";
const char string_31[] PROGMEM = "61441 8 100ms EBC1 2 1 8 521 Brake_Pedal_Position 0.4_%/bit 0 %";
const char string_32[] PROGMEM = "61441 8 100ms EBC1 5 1 8 973 Engine_Retarder_Selection 0.4_%/bit 0 %";
const char string_33[] PROGMEM = "61441 8 100ms EBC1 7 1 8 1481 Source_Address_of_Controlling_Device_for_Brake_Control 256_states/8_bit 0 Sa";
const char string_34[] PROGMEM = "61442 8 10ms ETC1 4 1 8 522 Percent_Clutch_Slip 0.4_%/bit 0 %";
const char string_35[] PROGMEM = "61442 8 10ms ETC1 8 1 8 1482 Source_Address_of_Controlling_Device_for_Transmission_Control 256_states/8_bit 0 Sa";
const char string_36[]  PROGMEM = "61443 8 50ms EEC2 2 1 8 91 Accelerator_Pedal_Position_1 0.4_%/bit 0 %";
const char string_37[]  PROGMEM = "61443 8 50ms EEC2 3 1 8 92 Engine_Percent_Load_At_Current_Speed 1_%/bit 0 %";
const char string_38[]  PROGMEM = "61443 8 50ms EEC2 4 1 8 974 Remote_Accelerator_Pedal_Position 0.4_%/bit 0 %";
const char string_39[]  PROGMEM = "61443 8 50ms EEC2 7 1 8 3357 Actual_Maximum_Available_Engine_-_Percent_Torque 0.4_%/bit 0 %";
const char string_40[]  PROGMEM = "61445 8 100ms ETC2 1 1 8 524 Transmission_Selected_Gear 1_gear_value/bit -125 gear_value";
const char string_41[]  PROGMEM = "61445 8 100ms ETC2 4 1 8 523 Transmission_Current_Gear 1_gear_value/bit -125 gear_value";
const char string_42[]  PROGMEM = "61446 8 500ms EAC1 1 1 8 927 Location 256_states/8_bit 0 bit";
const char string_43[]  PROGMEM = "61449 8 10ms VDC2 3 1 6 1811 Steering_Wheel_Turn_Counter 1_turn/bit -32 count";
const char string_44[]  PROGMEM = "61449 8 10ms VDC2 8 1 8 1810 Longitudinal_Acceleration 0 1_m/s2_per_bit -12 5 m/s2";
const char string_45[]  PROGMEM = "61451 8 20ms ESC1 3 1 8 2928 Axle_Location 256_states/8_bit 0 bit";
const char string_46[]  PROGMEM = "61451 8 20ms ESC1 4 1 4 2923 Status_of_Steering_Axle 16_states/4_bit 0 bit";
const char string_47[]  PROGMEM = "61451 8 20ms ESC1 5 1 4 2924 Steering_Type 16_states/4_bit 0 bit";
const char string_48[]  PROGMEM = "61451 8 20ms ESC1 5 5 4 2925 Type_of_Steering_Forces 16_states/4_bit 0 bit";
const char string_49[]  PROGMEM = "61451 8 20ms ESC1 6 1 4 2926 Type_of_Steering_Transmission 16_states/4_bit 0 bit";
const char string_50[]  PROGMEM = "61453 8 100ms LOI 1 1 4 3156 Blade_Control_Mode_Switch 16_states/4_bit 0 bit";
const char string_51[]  PROGMEM = "61453 8 100ms LOI 1 5 4 3157 Desired_Grade_Offset_Switch 16_states/4_bit 0 bit";
const char string_52[]  PROGMEM = "61453 8 100ms LOI 2 1 4 3158 Blade_Auto_Mode_Command 16_states/4_bit 0 bit";
const char string_53[]  PROGMEM = "61453 8 100ms LOI 2 5 4 3334 Left_Blade_Control_Mode_Operator_Control 16_states/4_bit 0 bit";
const char string_54[]  PROGMEM = "61453 8 100ms LOI 3 1 4 3335 Right_Blade_Control_Mode_Operator_Control 16_states/4_bit 0 bit";
const char string_55[]  PROGMEM = "61453 8 100ms LOI 3 5 4 3336 Left_Desired_Blade_Offset_Operator_Control 16_states/4_bit 0 bit";
const char string_56[]  PROGMEM = "61453 8 100ms LOI 4 1 4 3337 Right_Desired_Blade_Offset_Operator_Control 16_states/4_bit 0 bit";
const char string_57[]  PROGMEM = "61453 8 100ms LOI 4 5 4 3338 Side-shift_Blade_Control_Mode_Operator_Control 16_states/4_bit 0 bit";
const char string_58[]  PROGMEM = "61453 8 100ms LOI 5 1 4 3339 Side-shift_Desired_Blade_Offset_Operator_Control 16_states/4_bit 0 bit";
const char string_59[]  PROGMEM = "61454 8 50ms ATIG 6 1 5 3222 Aftertreatment_1_Intake_Gas_Sensor_Heater_Preliminary_FMI Binary 0 binary";
const char string_60[]  PROGMEM = "61454 8 50ms ATIG 7 1 5 3224 Aftertreatment_1_Intake_NOx_Sensor_Preliminary_FMI Binary 0 binary";
const char string_61[]  PROGMEM = "61454 8 50ms ATIG 8 1 5 3225 Aftertreatment_1_Intake_Oxygen_Sensor_Preliminary_FMI Binary 0 binary";
const char string_62[]  PROGMEM = "61455 8 50ms ATOG 7 1 5 3234 Aftertreatment_1_Outlet_NOx_Sensor_Preliminary_FMI Binary 0 binary";
const char string_63[]  PROGMEM = "61455 8 50ms ATOG 8 1 5 3235 Aftertreatment_1_Outlet_Oxygen_Sensor_Preliminary_FMI Binary 0 binary";
const char string_64[]  PROGMEM = "61458 8 50ms FWSS 1 1 16 3308 Fifth_Wheel_Vertical_Force 5_N/bit 0 N";
const char string_65[]  PROGMEM = "61458 8 50ms FWSS 3 1 16 3309 Fifth_Wheel_Drawbar_Force 10_N/bit -320000 N";
const char string_66[]  PROGMEM = "61458 8 50ms FWSS 5 1 16 3310 Fifth_Wheel_Roll_Moment 10_N/bit -320000 N";
const char string_67[]  PROGMEM = "61459 8 10ms SSI 1 1 16 3318 Pitch_Angle 0.002_deg/bit -64 deg";
const char string_68[]  PROGMEM = "61459 8 10ms SSI 3 1 16 3319 Roll_Angle 0.002_deg/bit -64 deg";
const char string_69[]  PROGMEM = "61459 8 10ms SSI 5 1 16 3322 Pitch_Rate 0.002_deg/sec_per_bit -64 deg/seC";
const char string_70[]  PROGMEM = "61459 8 10ms SSI 8 1 8 3327 Roll_and_Pitch_Measurement_Latency 0.5_ms/bit 0 milliseconds";
const char string_71[]  PROGMEM = "61460 8 50ms BI 5 1 8 3366 Relative_Blade_Height_and_Blade_Rotation_Angle_Measurement_Latency 0.5_ms/bit 0 milliseconds";
const char string_72[]  PROGMEM = "61463 8 5000ms KL1 1 1 8 1352 Engine_Cylinder_1_Knock_Level 1_%/bit 0 %";
const char string_73[]  PROGMEM = "61469 8 10ms SAS 3 1 6 3684 Steering_Wheel_Angle_Range_Counter 1_range/bit -32 range";
const char string_74[]  PROGMEM = "61469 8 10ms SAS 8 1 4 3689 Message_Counter 1_count/bit 0 count";
const char string_75[]  PROGMEM = "61469 8 10ms SAS 8 5 4 3690 Message_Checksum 1_count/bit 0 count";
const char string_76[]  PROGMEM = "61475 8 50ms CRDI 6 1 8 4334 Aftertreatment_CR_Dosing_Reagent_Absolute_Pressure 8_kPa/bit 0 kPa";
const char string_77[]  PROGMEM = "61476 8 50ms CRDR 3 1 4 4349 Aftertreatment_CR_System_Requested_State 16_states/4_bit 0 bit";
const char string_78[]  PROGMEM = "61477 8 50ms CRAI 3 1 5 4378 Aftertreatment_1_Outlet_NH3_Sensor_Preliminary_FMI Binary 0 binary";
const char string_79[]  PROGMEM = "61477 8 50ms CRAI 5 1 5 4382 Aftertreatment_1_Outlet_NH3_Gas_Sensor_Heater_Preliminary_FMI Binary 0 binary";
const char string_80[]  PROGMEM = "61477 8 50ms CRAI 6 1 2 4383 Aftertreatment_1_Outlet_NH3_Gas_Sensor_Heater_Control 4_states/2_bit 0 bit";
const char string_81[]  PROGMEM = "64828 8 1000ms CRRS 3 1 8 4375 Aftertreatment_CR_Catalyst_Reagent_Pump_Drive_Percentage_(Command_to_pump) 0.4_%/bit 0 %";
const char string_82[]  PROGMEM = "64828 8 1000ms CRRS 4 1 8 4376 Aftertreatment_CR_Catalyst_Reagent_Return_Valve_(Directional_command_for_return) 0.4_%/bit 0 %";
const char string_83[]  PROGMEM = "64829 8 1000ms CRRT 1 1 8 4367 Aftertreatment_CR_Catalyst_Reagent_Tank_2_Level 0.4_%/bit 0 %";
const char string_84[]  PROGMEM = "64829 8 1000ms CRRT 2 1 8 4368 Aftertreatment_CR_Catalyst_Reagent_Tank_2_Temperature 1_deg_C/bit -40 C";
const char string_85[]  PROGMEM = "64829 8 1000ms CRRT 5 1 5 4370 Aftertreatment_CR_Catalyst_Reagent_Tank_2_Level_Preliminary_FMI Binary 0 binary";
const char string_86[]  PROGMEM = "64829 8 1000ms CRRT 6 1 5 4371 Aftertreatment_CR_Reagent_Tank_2_Temperature_Preliminary_FMI Binary 0 binary";
const char string_87[]  PROGMEM = "64829 8 1000ms CRRT 7 1 8 4372 Aftertreatment_CR_Catalyst_Reagent_Tank_2_Heater_(light-off) 0.4_%/bit 0 %";
const char string_88[]  PROGMEM = "64829 8 1000ms CRRT 8 1 5 4373 Aftertreatment_CR_Catalyst_Reagent_Tank_2_Heater_Preliminary_FMI Binary 0 binary";
const char string_89[]  PROGMEM = "64830 8 500ms CRET 3 1 5 4361 Aftertreatment_CR_Catalyst_Intake_Gas_Temperature_Preliminary_FMI 32_states/5_bit 0 bit";
const char string_90[]  PROGMEM = "64830 8 500ms CRET 6 1 5 4362 Aftertreatment_CR_Catalyst_Outlet_Gas_Temperature_Preliminary_FMI 32_states/5_bit 0 bit";
const char string_91[]  PROGMEM = "64831 8 500ms CREP 3 1 5 4359 Aftertreatment_CR_Catalyst_Exhaust_Gas_Differential_Pressure_Preliminary_FMI 32_states/5_bit 0 bit";
const char string_92[]  PROGMEM = "64832 8 500ms CRD2 1 3 2 4352 Aftertreatment_CR_Doser_Fault_Suppression_Reques_(request) 4_states/2_bit 0 bit";
const char string_93[]  PROGMEM = "64833 8 500ms CRD2 1 1 8 4335 Aftertreatment_CR_Dosing_Air_Assist_Absolute_Pressure 8_kPa/bit 0 kPa";
const char string_94[]  PROGMEM = "64833 8 500ms CRD2 2 1 8 4336 Aftertreatment_CR_Dosing_Air_Assist_Valve_(measured_0_=_closed) 0.4_%/bit 0 %";
const char string_95[]  PROGMEM = "64833 8 500ms CRD2 3 1 8 4337 Aftertreatment_CR_Dosing_Reagent_Temperature_(closest_to_dosing_valve) 1_deg_C/bit -40 C";
const char string_96[]  PROGMEM = "64833 8 500ms CRD2 4 1 3 4338 Aftertreatment_CR_Dosing_Valve_Exhaust_Temp_Red000mstate 4_states/2_bit 0 bit";
const char string_97[]  PROGMEM = "64833 8 500ms CRD2 5 3 5 4341 Aftertreatment_CR_Catalyst_Reagent_Line_Heater_1_Preliminary_FMI Binary 0 binary";
const char string_98[]  PROGMEM = "64833 8 500ms CRD2 6 3 5 4343 Aftertreatment_CR_Catalyst_Reagent_Line_Heater_2_Preliminary_FMI Binary 0 binary";
const char string_99[]  PROGMEM = "64833 8 500ms CRD2 7 3 5 4345 Aftertreatment_CR_Catalyst_Reagent_Line_Heater_3_Preliminary_FMI Binary 0 binary";
const char string_100[] PROGMEM = "64833 8 500ms CRD2 8 3 5 4347 Aftertreatment_CR_Catalyst_Reagent_Line_Heater_4_Preliminary_FMI 32_states/5_bit 0 bit";
const char string_101[] PROGMEM = "64838 8 500ms A1TWC 7 1 5 4292 Aftertreatment_1_3_W_Catalytic_Converter_Intake_Gas_Temp_Prel_FMI 32_states/5_bit 0 bit";
const char string_102[] PROGMEM = "64838 8 500ms A1TWC 8 3 5 4294 Aftertreatment_1_3_Wa_Catalytic_Converter_Diff_Pressure_Prel_FMI 32_states/5_bit 0 bit";
const char string_103[] PROGMEM = "64849 8 1000ms ACCVC 2 1 8 4199 Desired_Aftercooler_Coolant_Inlet_Temperature 1_deg_C/bit -40 C";
const char string_104[] PROGMEM = "64849 8 1000ms ACCVC 3 1 8 4200 Desired_Aftercooler_Coolant_Thermostat_Opening 0.4_%/bit 0 %";
const char string_105[] PROGMEM = "64850 8 1000ms ECCVC 1 1 2 4195 Engine_Coolant_Thermostat_Mode 4_states/2_bit 0 bit";
const char string_106[] PROGMEM = "64850 8 1000ms ECCVC 2 1 8 4196 Desired_Engine_Coolant_Pump_Outlet_Temperature 1_deg_C/bit -40 C";
const char string_107[] PROGMEM = "64850 8 1000ms ECCVC 3 1 8 4197 Desired_Engine_Coolant_Thermostat_Opening 0.4_%/bit 0 %";
const char string_108[] PROGMEM = "64870 8 1000ms ET4 1 1 8 4076 Engine_Coolant_Temperature_2 1_deg_C/bit -40 C";
const char string_109[] PROGMEM = "64870 8 1000ms ET4 2 1 8 4193 Engine_Coolant_Pump_Outlet_Temperature 1_deg_C/bit -40 C";
const char string_110[] PROGMEM = "64870 8 1000ms ET4 3 1 8 4194 Engine_Coolant_Thermostat_Opening 0.4_%/bit 0 %";
const char string_111[] PROGMEM = "64897 8 500ms EGRB 1 1 8 3672 EGR_Cooler_Bypass_Actuator_Postion 0.4_%/bit 0 %";
const char string_112[] PROGMEM = "64899 8 1000ms TCI 1 1 3 3645 Transfer_case_status 8_states/3_bit 0 bit";
const char string_113[] PROGMEM = "64914 8 250ms EOI 1 1 4 3543 Engine_Operating_State 16_states/4_bit 0 bit";
const char string_114[] PROGMEM = "64914 8 250ms EOI 1 5 2 4082 Fuel_Pump_Primer_Control 4_states/2_bit 0 bit";
const char string_115[] PROGMEM = "64914 8 250ms EOI 8 1 8 3644 Engine_Derate_Request 0.4_%/bit 0 %";
const char string_116[] PROGMEM = "64916 8 100ms EEC7 1 1 16 27 Engine_Exhaust_Gas_Recirculation_Valve_Position 0.0025_%/bit 0 %";
const char string_117[] PROGMEM = "64916 8 100ms EEC7 3 1 16 3822 Engine_Exhaust_Gas_Recirculation_Valve_2_Position 0.0025_%/bit 0 %";
const char string_118[] PROGMEM = "64917 8 1000ms TRF2 4 1 8 4177 Transmission_Oil_Life_Remaining 0.4_%/bit 0 %";
const char string_119[] PROGMEM = "64923 8 1000ms CRI1 1 1 8 3515 Aftertreatment_CR_Catalyst_Reagent_Temperature_2 1_deg_C/bit -40 C";
const char string_120[] PROGMEM = "64923 8 1000ms CRI1 2 1 8 3516 Aftertreatment_CR_Catalyst_Reagent_Concentration 0 25_%/bit 0 %";
const char string_121[] PROGMEM = "64923 8 1000ms CRI1 3 1 8 3518 Aftertreatment_CR_Catalyst_Reagent_Conductivity 5_microSiemens/mm 0 microSiemens/mm";
const char string_122[] PROGMEM = "64923 8 1000ms CRI1 4 1 5 3519 Aftertreatment_CR_Catalyst_Reagent_Temperature_2_Preliminary_FMI Binary 0 binary";
const char string_123[] PROGMEM = "64923 8 1000ms CRI1 5 1 5 3520 Aftertreatment_CR_Catalyst_Reagent_Properties_Preliminary_FMI Binary 0 binary";
const char string_124[] PROGMEM = "64923 8 1000ms CRI1 6 1 4 3521 Aftertreatment_CR_Catalyst_Reagent_Type 16_states/4_bit 0 bit";
const char string_125[] PROGMEM = "64927 8 500ms A1AC 7 1 8 3488 Aftertreatment_1_Air_Pressure_Actuator_Position 0.4_%/bit 0 %";
const char string_126[] PROGMEM = "64929 8 500ms ATFC 8 1 8 4301 Aftertreatment_1_Fuel_Injector_1_Heater_Control 0.4_%/bit 0 %";
const char string_127[] PROGMEM = "64931 8 100ms EEC6 3 1 8 641 Engine_Variable_Geometry_Turbocharger_Actuator_#1 0.4_%/bit 0 %";
const char string_128[] PROGMEM = "64931 8 100ms EEC6 4 1 8 3675 Engine_Turbocharger_Compressor_Bypass_Actuator_Position 0.4_%/bit 0 %";
const char string_129[] PROGMEM = "64938 8 500ms EFLP4 1 1 8 3340 Engine_Charge_Air_Cooler_1_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_130[] PROGMEM = "64938 8 500ms EFLP4 2 1 8 3341 Engine_Charge_Air_Cooler_2_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_131[] PROGMEM = "64938 8 500ms EFLP4 3 1 8 3342 Engine_Coolant_Pump_Differential_Pressure 64_kPa/bit -7 kPa";
const char string_132[] PROGMEM = "64938 8 500ms EFLP4 6 1 8 3668 Engine_Intercooler_Coolant_Level 0.4_%/bit 0 %";
const char string_133[] PROGMEM = "64938 8 500ms EFLP4 7 1 8 3676 Engine_Aftercooler_Coolant_Level 0.4_%/bit 0 %";
const char string_134[] PROGMEM = "64938 8 500ms EFLP4 8 1 8 2631 Engine_Charge_Air_Cooler_Outlet_Pressure 2_kPa/bit 0 kPa";
const char string_135[] PROGMEM = "64946 8 500ms AT1IM 1 1 16 3249 Aftertreatment_1_Exhaust_Gas_Temperature_2 0.03125_deg_C/bit -273 C";
const char string_136[] PROGMEM = "64946 8 500ms AT1IM 3 1 16 3250 Aftertreatment_1_Diesel_Particulate_Filter_Intermediate_Gas_Temperature 0.03125_deg_C/bit -273 C";
const char string_137[] PROGMEM = "64946 8 500ms AT1IM 5 1 16 3251 Aftertreatment_1_Diesel_Particulate_Filter_Differential_Pressure 0 1_kPa/bit 0 kPa";
const char string_138[] PROGMEM = "64946 8 500ms AT1IM 7 1 5 3252 Aftertreatment_1_Exhaust_Gas_Temperature_2_Preliminary_FMI Binary 0 binary";
const char string_139[] PROGMEM = "64946 8 500ms AT1IM 7 6 5 3253 Aftertreatment_1_Diesel_Particulate_Filter_Delta_Pressure_Preliminary_FMI Binary 0 binary";
const char string_140[] PROGMEM = "64946 8 500ms AT1IM 8 3 5 3254 Aftertreatment_1_Diesel_Particulate_Filter_Intermediate_Gas_Temperature_Preliminary_FMI Binary 0 binary";
const char string_141[] PROGMEM = "64947 8 500ms AT1O2 1 1 16 3245 Aftertreatment_1_Exhaust_Gas_Temperature_3 0.03125_deg_C/bit -273 C";
const char string_142[] PROGMEM = "64947 8 500ms AT1O2 3 1 16 3246 Aftertreatment_1_Diesel_Particulate_Filter_Outlet_Gas_Temperature 0.03125_deg_C/bit -273 C";
const char string_143[] PROGMEM = "64947 8 500ms AT1O2 5 1 5 3247 Aftertreatment_1_Exhaust_Gas_Temperature_3_Preliminary_FMI Binary 0 binary";
const char string_144[] PROGMEM = "64947 8 500ms AT1O2 6 1 5 3248 Aftertreatment_1_Diesel_Particulate_Filter_Outlet_Exhaust_Gas_Temperature__Preliminary_FMI Binary 0 binary";
const char string_145[] PROGMEM = "64948 8 500ms AT1I2 1 1 16 3241 Aftertreatment_1_Exhaust_Gas_Temperature_1 0.03125_deg_C/bit -273 C";
const char string_146[] PROGMEM = "64948 8 500ms AT1I2 3 1 16 3242 Aftertreatment_1_Diesel_Particulate_Filter_Intake_Gas_Temperature 0.03125_deg_C/bit -273 C";
const char string_147[] PROGMEM = "64948 8 500ms AT1I2 5 1 5 3243 Aftertreatment_1_Exhaust_Gas_Temperature_1_Preliminary_FMI Binary 0 binary";
const char string_148[] PROGMEM = "64948 8 500ms AT1I2 6 1 5 3244 Aftertreatment_1_Diesel_Particulate_Filter_Intake_Gas_Temperature_Preliminary_FMI Binary 0 binary";
const char string_149[] PROGMEM = "64953 8 1000ms TPRI 1 1 8 3190 Tire_Location 256_states/8_bit 0 bit";
const char string_150[] PROGMEM = "64953 8 1000ms TPRI 2 1 8 3191 Reference_Tire_Pressure 8_kPa/bit 0 kPa";
const char string_151[] PROGMEM = "64954 8 1000ms TR6 2 1 7 3181 Farebox_Alarm_Identifier 128_states/7_bit 0 bit";
const char string_152[] PROGMEM = "64955 8 1000ms TR5 1 1 4 3170 Transaction_Type 16_states/4_bit 0 bit";
const char string_153[] PROGMEM = "64955 8 1000ms TR5 1 5 4 3171 Passenger_Type 16_states/4_bit 0 bit";
const char string_154[] PROGMEM = "64955 8 1000ms TR5 2 1 4 3176 Type_of_Fare 16_states/4_bit 0 bit";
const char string_155[] PROGMEM = "64955 8 1000ms TR5 2 5 4 3177 Payment_Details 16_states/4_bit 0 bit";
const char string_156[] PROGMEM = "64955 8 1000ms TR5 3 1 4 3165 Fare_Validity 1_count/bit 0 count";
const char string_157[] PROGMEM = "64955 8 1000ms TR5 3 5 4 3166 Pass_Category 1_count/bit 0 count";
const char string_158[] PROGMEM = "64955 8 1000ms TR5 4 1 5 3167 Initial_Fare_Agency 1_count/bit 0 count";
const char string_159[] PROGMEM = "64955 8 1000ms TR5 5 1 3 3172 Type_of_Service 8_states/3_bit 0 bit";
const char string_160[] PROGMEM = "64955 8 1000ms TR5 5 4 5 3173 Transfer_Type 32_states/5_bit 0 bit";
const char string_161[] PROGMEM = "64955 8 1000ms TR5 8 1 8 3168 Transfer_Sold 1_count/bit 0 count";
const char string_162[] PROGMEM = "64956 15 1000ms TR4 3 1 8 3175 Fare_Presets 256_states/8_bit 0 bit";
const char string_163[] PROGMEM = "64957 8 1000ms TR3 3 4 16 3085 Vehicle_ID 1_count/bit 0 count";
const char string_164[] PROGMEM = "64958 8 1000ms TR1 1 1 8 3078 Agency 1_count/bit 0 count";
const char string_165[] PROGMEM = "64958 8 1000ms TR1 2 1 8 3071 Number_of_bytes_in_the_Transit_Assigned_Route_Identity 1_byte/bit 0 bytes";
const char string_166[] PROGMEM = "64958 8 1000ms TR1 3 1 8 3072 Number_of_bytes_in_the_Transit_Assigned_Run_Identity 1_byte/bit 0 bytes";
const char string_167[] PROGMEM = "64958 8 1000ms TR1 4 1 8 3073 Number_of_bytes_in_the_Transit_Assigned_Block_Identity 1_byte/bit 0 bytes";
const char string_168[] PROGMEM = "64959 8 1000ms TR2 1 1 8 3070 Number_of_bytes_in_the_Milepost_Identification 1_byte/bit 0 bytes";
const char string_169[] PROGMEM = "64960 8 1000ms TR7 1 1 8 3043 Type_of_Passenger_Count 256_states/8_bit 0 bit";
const char string_170[] PROGMEM = "64960 8 1000ms TR7 2 1 8 3047 Patron_Count 1_count/bit 0 count";
const char string_171[] PROGMEM = "64961 8 500ms EFLP 3 3 8 3358 Engine_Exhaust_Gas_Recirculation_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_172[] PROGMEM = "64964 8 100ms EBC5 2 5 4 2918 XBR_Active_Control_Mode 16_states/4_bit 0 bit";
const char string_173[] PROGMEM = "64964 8 100ms EBC5 3 1 8 2921 XBR_Acceleration_Limit 0.1_m/s2_per_bit -12 5 m/s2";
const char string_174[] PROGMEM = "64967 8 5000ms OHCSS 2 1 8 2888 Engine_Alternate_Rating_Select_State 256_states/8_bit 0 bit";
const char string_175[] PROGMEM = "64967 8 5000ms OHCSS 3 1 4 2889 Engine_Alternate_Droop_Accelerator_select_State 16_states/4_bit 0 bit";
const char string_176[] PROGMEM = "64967 8 5000ms OHCSS 4 1 4 2894 Engine_Alternate_Droop_Remote_Accelerator_Select_State 16_states/4_bit 0 bit";
const char string_177[] PROGMEM = "64967 8 5000ms OHCSS 4 5 4 2895 Engine_Alternate_Droop_Auxiliary_Input_Select_State 16_states/4_bit 0 bit";
const char string_178[] PROGMEM = "64968 8 1000ms ISCS 1 1 4 2892 Engine_Operator_Primary_Intermediate_Speed_Select_State 16_states/4_bit 0 bit";
const char string_179[] PROGMEM = "64971 8 500ms OHECS 2 8 2 882 Engine_Alternate_Rating_Select_selection 4_states/2_bit 0 bit";
const char string_180[] PROGMEM = "64971 8 500ms OHECS 3 1 4 2881 Engine_Alternate_Droop_Accelerator_select 16_states/4_bit 0 bit";
const char string_181[] PROGMEM = "64971 8 500ms OHECS 4 1 4 2886 Engine_Alternate_Droop_Remote_Accelerator_Select 16_states/4_bit 0 bit";
const char string_182[] PROGMEM = "64971 8 500ms OHECS 4 5 4 2885 Engine_Alternate_Droop_Auxiliary_Input_Select 16_states/4_bit 0 bit";
const char string_183[] PROGMEM = "64972 8 1000ms OEL 1 1 4 2873 Work_Light_Switch 16_states 16_states/4_bit  0 bit";
const char string_184[] PROGMEM = "64972 8 1000ms OEL 1 5 4 2872 Main_Light_Switch 16_states  16_states/4_bit 0 bit";
const char string_185[] PROGMEM = "64972 8 1000ms OEL 2 1 4 2876 Turn_Signal_Switch 16_states  16_states/4_bit 0 bit";
const char string_186[] PROGMEM = "64972 8 1000ms OEL 3 1 8 2878 Operators_Desired_Back-light 0.4_%/bit 0 %";
const char string_187[] PROGMEM = "64973 8 200ms OWW 1 1 4 2864 Front_Non-operator_Wiper_Switch 16_states/4_bit 0 bit";
const char string_188[] PROGMEM = "64973 8 200ms OWW 1 5 4 2863 Front_Operator_Wiper_Switch 16_states/4_bit 0 bit";
const char string_189[] PROGMEM = "64973 8 200ms OWW 2 5 4 2865 Rear_Wiper_Switch 16_states/4_bit 0 bit";
const char string_190[] PROGMEM = "64973 8 200ms OWW 3 1 8 2869 Front_Operator_Wiper_Delay_Control 0.4_%/bit 0 %";
const char string_191[] PROGMEM = "64973 8 200ms OWW 4 1 8 2870 Front_Non-operator_Wiper_Delay_Control 0.4_%/bit 0 %";
const char string_192[] PROGMEM = "64973 8 200ms OWW 5 1 8 2871 Rear_Wiper_Delay_Control 0.4_%/bit 0 %";
const char string_193[] PROGMEM = "64976 8 5000ms IC2 5 1 8 3563 Engine_Intake_Manifold_#1_Absolute_Pressure 2_kPa/bit 0 kPa";
const char string_194[] PROGMEM = "64988 8 1000ms MCI 1 1 4 2615 Engine_Throttle_Synchronization_Mode_Status 16_states/4_bit 0 bit";
const char string_195[] PROGMEM = "64992 8 1000ms AMB2 1 1 8 2610 Solar_Intensity_Percent 0.4_%/bit 0 %";
const char string_196[] PROGMEM = "64992 8 1000ms AMB2 2 1 8 2611 Solar_Sensor_Maximum 0.4_mW/cm^2_per_bit 0 mW/cm^2";
const char string_197[] PROGMEM = "64993 8 1000ms CACI 1 1 8 2609 Cab_A/C_Refrigerant_Compressor_Outlet_Pressure 16_kPa/bit 0 kPa";
const char string_198[] PROGMEM = "64994 8 1000ms SPR 1 1 8 2603 Pneumatic_Supply_Pressure_Request 8_kPa/bit 0 kPa";
const char string_199[] PROGMEM = "64994 8 1000ms SPR 2 1 8 2604 Parking_and/or_Trailer_Air_Pressure_Request 8_kPa/bit 0 kPa";
const char string_200[] PROGMEM = "64994 8 1000ms SPR 3 1 8 2605 Service_Brake_Air_Pressure_Request_Circuit_#1 8_kPa/bit 0 kPa";
const char string_201[] PROGMEM = "64994 8 1000ms SPR 4 1 8 2606 Service_Brake_Air_Pressure_Request_Circuit_#2 8_kPa/bit 0 kPa";
const char string_202[] PROGMEM = "64994 8 1000ms SPR 5 1 8 2607 Auxiliary_Equipment_Supply_Pressure_Request 8_kPa/bit 0 kPa";
const char string_203[] PROGMEM = "64994 8 1000ms SPR 6 1 8 2608 Air_Suspension_Supply_Pressure_Request 8_kPa/bit 0 kPa";
const char string_204[] PROGMEM = "64995 8 250ms EOAC 1 1 8 2601 Travel_Velocity_Control_Position 1_%/bit -125 %";
const char string_205[] PROGMEM = "64996 8 500ms EPD 1 1 8 2600 Payload_Percentage 1_%/bit 0 %";
const char string_206[] PROGMEM = "64997 8 1000ms MVS 1 1 8 2588 Maximum_Vehicle_Speed_Limit_1 1_km/h_per_bit 0 kph";
const char string_207[] PROGMEM = "64997 8 1000ms MVS 8 1 8 2595 Applied_Vehicle_Speed_Limit 1_km/h_per_bit 0 kph";
const char string_208[] PROGMEM = "64998 8 100ms HBS 1 1 8 2580 Hydraulic_Brake_Pressure_Circuit_1 100_kPa/bit 0 kPa";
const char string_209[] PROGMEM = "64998 8 100ms HBS 2 1 8 2581 Hydraulic_Brake_Pressure_Circuit_2 100_kPa/bit 0 kPa";
const char string_210[] PROGMEM = "65100 8 500ms ML 8 1 8 1844 Operators_Black_Out_Intensity_Selection 0.4_%/bit 0 %";
const char string_211[] PROGMEM = "65102 8 100ms DC1 1 1 4 1821 Position_of_doors 16_states/4_bit 0 bit";
const char string_212[] PROGMEM = "65104 8 1000ms BT1 1 1 8 1800 Battery_1_Temperature 1_deg_C/bit -40 C";
const char string_213[] PROGMEM = "65104 8 1000ms BT1 2 1 8 1801 Battery_2_Temperature 1_deg_C/bit -40 C";
const char string_214[] PROGMEM = "65107 8 5000ms RTC1 1 1 8 1776 Low_Limit_Threshold_for_Maximum_RPM_from_Retarder 32_rpm/bit 0 rpm";
const char string_215[] PROGMEM = "65107 8 5000ms RTC1 2 1 8 1777 High_Limit_Threshold_for_Minimum_Continuous_RPM_from_Retarder 32_rpm/bit 0 rpm";
const char string_216[] PROGMEM = "65107 8 5000ms RTC1 3 1 8 1778 Low_Limit_Threshold_for_Maximum_Torque_from_Retarder 1_%/bit -125 %";
const char string_217[] PROGMEM = "65107 8 5000ms RTC1 4 1 8 1779 High_Limit_Threshold_for_Minimum_Continuous_Torque_from_Retarder 1_%/bit -125 %";
const char string_218[] PROGMEM = "65107 8 5000ms RTC1 5 1 8 1780 Maximum_Continuous_Retarder_Speed 32_rpm/bit 0 rpm";
const char string_219[] PROGMEM = "65107 8 5000ms RTC1 6 1 8 1781 Minimum_Continuous_Retarder_Speed 32_rpm/bit 0 rpm";
const char string_220[] PROGMEM = "65107 8 5000ms RTC1 7 1 8 1782 Maximum_Continuous_Retarder_Torque 1_%/bit -125 %";
const char string_221[] PROGMEM = "65107 8 5000ms RTC1 8 1 8 1783 Minimum_Continuous_Retarder_Torque 1_%/bit -125 %";
const char string_222[] PROGMEM = "65108 8 5000ms ECT1 1 1 8 1768 Engine_Low_Limit_Threshold_for_Maximum_RPM_from_Engine 32_rpm/bit 0 rpm";
const char string_223[] PROGMEM = "65108 8 5000ms ECT1 2 1 8 1769 Engine_High_Limit_Threshold_for_Minimum_Continuous_Engine_RPM 32_rpm/bit 0 rpm";
const char string_224[] PROGMEM = "65108 8 5000ms ECT1 3 1 8 1770 Engine_Low_Limit_Threshold_for_Maximum_Torque_from_Engine 1_%/bit -125 %";
const char string_225[] PROGMEM = "65108 8 5000ms ECT1 4 1 8 1771 Engine_High_Limit_Threshold_for_Minimum_Continuous_Torque_from_Engine 1_%/bit -125 %";
const char string_226[] PROGMEM = "65108 8 5000ms ECT1 5 1 8 1772 Engine_Maximum_Continuous_RPM 32_rpm/bit 0 rpm";
const char string_227[] PROGMEM = "65108 8 5000ms ECT1 6 1 8 1773 Engine_Minimum_Continuous_RPM 32_rpm/bit 0 rpm";
const char string_228[] PROGMEM = "65108 8 5000ms ECT1 7 1 8 1774 Engine_Maximum_Continuous_Torque 1_%/bit -125 %";
const char string_229[] PROGMEM = "65108 8 5000ms ECT1 8 1 8 1775 Engine_Minimum_Continuous_Torque 1_%/bit -125 %";
const char string_230[] PROGMEM = "65110 8 1000ms TI1 1 1 8 1761 Aftertreatment_CR_Catalyst_Tank_Level 0.4_%/bit 0 %";
const char string_231[] PROGMEM = "65110 8 1000ms TI1 2 1 8 3031 Aftertreatment_CR_Catalyst_Tank_Temperature 1_deg_C/bit -40 C";
const char string_232[] PROGMEM = "65110 8 1000ms TI1 5 1 5 3532 Aftertreatment_CR_Catalyst_Tank_Level_Preliminary_FMI Binary 0 binary";
const char string_233[] PROGMEM = "65110 8 1000ms TI1 6 1 5 4365 Aftertreatment_CR_Catalyst_Reagent_Tank_1_Temperature_Preliminary_FMI Binary 0 binary";
const char string_234[] PROGMEM = "65110 8 1000ms TI1 8 1 5 4366 Aftertreatment_CR_Catalyst_Reagent_Tank_1_Heater_Preliminary_FMI Binary 0 binary";
const char string_235[] PROGMEM = "65111 8 100ms ASC5 1 1 8 1729 Damper_Stiffness_Front_Axle 0.4_%/bit 0 %";
const char string_236[] PROGMEM = "65111 8 100ms ASC5 2 1 8 1730 Damper_Stiffness_Rear_Axle 0.4_%/bit 0 %";
const char string_237[] PROGMEM = "65111 8 100ms ASC5 3 1 8 1731 Damper_Stiffness_Lift_/_Tag_Axle 0.4_%/bit 0 %";
const char string_238[] PROGMEM = "65111 8 100ms ASC5 4 1 2 1833 Electronic_Shock_Absorber_Control_Mode_-__Front_Axle 4_states/2_bit 0 bit";
const char string_239[] PROGMEM = "65128 8 1000ms  VF 1 1 8 1638 Hydraulic_Temperature 1_deg_C/bit -40 C";
const char string_240[] PROGMEM = "65128 8 1000ms  VF 2 1 2 1713 Hydraulic_Oil_Filter_Restriction_Switch 4_states/2_bit 0 bit";
const char string_241[] PROGMEM = "65128 8 1000ms  VF 2 3 2 1857 Winch_Oil_Pressure_Switch 4_states/2_bit 0 bit";
const char string_242[] PROGMEM = "65128 8 1000ms  VF 3 1 8 2602 Hydraulic_Oil_Level 0.4_%/bit 0 %";
const char string_243[] PROGMEM = "65130 8 5000ms EFS 1 1 8 1380 Engine_Oil_Level_Remote_Reservoir 0.4_%/bit 0 %";
const char string_244[] PROGMEM = "65130 8 5000ms EFS 2 1 8 1381 Engine_Fuel_Supply_Pump_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_245[] PROGMEM = "65130 8 5000ms EFS 3 1 8 1382 Engine_Fuel_Filter_(suction_side)_Differential_Pressure 2_kPa/bit 0 kPa";
const char string_246[] PROGMEM = "65130 8 5000ms EFS 4 1 8 3548 Engine_Waste_Oil_Reservoir_Level 0.4_%/bit 0 %";
const char string_247[] PROGMEM = "65130 8 5000ms EFS 5 1 8 3549 Engine_Oil-Filter_Outlet_Pressure 4_kPa/bit 0 kPa";
const char string_248[] PROGMEM = "65133 8 1000ms HTR 1 1 8 1687 Auxiliary_Heater_Output_Coolant_Temperature 1_deg_C/bit -40 C";
const char string_249[] PROGMEM = "65133 8 1000ms HTR 2 1 8 1688 Auxiliary_Heater_Input_Air_Temperature 1_deg_C/bit -40 C";
const char string_250[] PROGMEM = "65133 8 1000ms HTR 3 1 8 1689 Auxiliary_Heater_Output_Power_Percent 0.4_%/bit 0 %";
const char string_251[] PROGMEM = "65133 8 1000ms HTR 4 1 4 1677 Auxiliary_Heater_Mode 16_states/4_bit 0 bit";
const char string_252[] PROGMEM = "65135 8 100ms ACC1 1 1 8 1586 Speed_of_forward_vehicle 1_km/h_per_bit 0 kph";
const char string_253[] PROGMEM = "65135 8 100ms ACC1 2 1 8 1587 Distance_to_forward_vehicle 1_m/bit 0 m";
const char string_254[] PROGMEM = "65135 8 100ms ACC1 3 1 8 1588 Adaptive_Cruise_Control_Set_Speed 1_km/h_per_bit 0 kph";
const char string_255[] PROGMEM = "65135 8 100ms ACC1 4 1 3 1590 Adaptive_Cruise_Control_Mode 8_states/3_bit 0 bit";
const char string_256[] PROGMEM = "65137 8 50ms  LTP5 1 1 8 1581 Laser_Tracer_Horizontal_Deviation 1_%/bit 0 %";
const char string_257[] PROGMEM = "65137 8 50ms  LTP6 1 1 8 1582 LED_Display_Data_#2 256_states/8_bit 0 bit";
const char string_258[] PROGMEM = "65137 8 50ms  LTP7 1 1 8 1583 Laser_Tracer_Information 256_states/8_bit 0 bit";
const char string_259[] PROGMEM = "65138 8 50ms  LBC3 1 1 8 1578 Blade_Control_Mode 256_states/8_bit 0 bit";
const char string_260[] PROGMEM = "65141 8 50ms  LVD3 1 1 8 2576 Laser_Receiver_Type 1_count/bit 0 count";
const char string_261[] PROGMEM = "65142 8 100ms LVDD 1 1 8 1573 LED_Display_Data_#1 256_states/8_bit 0 bit";
const char string_262[] PROGMEM = "65142 8 100ms LVDD 2 1 4 1805 LED_Display_Mode_Control 16_states/4_bit 0 bit";
const char string_263[] PROGMEM = "65142 8 100ms LVDD 2 5 4 1806 LED_Display_Deadband_Control 16_states/4_bit 0 bit";
const char string_264[] PROGMEM = "65142 8 100ms LVDD 3 1 4 2578 LED_Pattern_Control 16_states/4_bit 0 bit";
const char string_265[] PROGMEM = "65142 8 100ms LVDD 3 5 4 2577 Display_Deadbands 16_states/4_bit 0 bit";
const char string_266[] PROGMEM = "65144 8 1000ms TP1 1 1 8 39 Tire_Pressure_Check_Interval 1_min/bit 0 Seconds";
const char string_267[] PROGMEM = "65144 8 1000ms TP1 2 1 4 1466 Steer_Channel_Mode 16_states/4_bit 0 bit";
const char string_268[] PROGMEM = "65144 8 1000ms TP1 3 1 4 1467 Trailer/tag_Channel_Mode 16_states/4_bit 0 bit";
const char string_269[] PROGMEM = "65144 8 1000ms TP1 3 5 4 1468 Drive_Channel_Mode 16_states/4_bit 0 bit";
const char string_270[] PROGMEM = "65170 8 100ms   EI 1 1 8 1208 Engine_Pre-filter_Oil_Pressure 4_kPa/bit 0 kPa";
const char string_271[] PROGMEM = "65170 8 100ms   EI 4 1 8 1210 Engine_Fuel_Rack_Position 0.4_%/bit 0 %";
const char string_272[] PROGMEM = "65172 8 5000ms EAC 1 1 8 1203 Engine_Auxiliary_Coolant_Pressure 4_kPa/bit 0 kPa";
const char string_273[] PROGMEM = "65172 8 5000ms EAC 2 1 8 1212 Engine_Auxiliary_Coolant_Temperature 1_deg_C/bit -40 C";
const char string_274[] PROGMEM = "65172 8 5000ms EAC 3 1 8 2435 Sea_Water_Pump_Outlet_Pressure 2_kPa/bit 0 kPa";
const char string_275[] PROGMEM = "65174 8 100ms  TCW 1 1 8 1188 Engine_Turbocharger_1_Wastegate_Drive 0.4_%/bit 0 %";
const char string_276[] PROGMEM = "65174 8 100ms  TCW 5 1 8 1192 Engine_Turbocharger_Wastegate_Actuator_Control_Air_Pressure 4_kPa/bit 0 kPa";
const char string_277[] PROGMEM = "65179 8 1000ms TCI 1 1 8 1168 Engine_Turbocharger_Lube_Oil_Pressure_2 4_kPa/bit 0 kPa";
const char string_278[] PROGMEM = "65191 8 1000ms  AT 1 1 8 1122 Engine_Alternator_Bearing_1_Temperature 1_deg_C/bit -40 C";
const char string_279[] PROGMEM = "65191 8 1000ms  AT 3 1 8 1124 Engine_Alternator_Winding_1_Temperature 1_deg_C/bit -40 C";
const char string_280[] PROGMEM = "65197 8 100ms EBC3 1 1 8 1091 Brake_Application_Pressure_High_Range_Front_Axle_Left_Wheel 5_kPa/bit 0 kPa";
const char string_281[] PROGMEM = "65197 8 100ms EBC3 2 1 8 1092 Brake_Application_Pressure_High_Range_Front_Axle_Right_Wheel 5_kPa/bit 0 kPa";
const char string_282[] PROGMEM = "65197 8 100ms EBC3 3 1 8 1093 Brake_Application_Pressure_High_Range_Rear_Axle_#1_Left_Wheel 5_kPa/bit 0 kPa";
const char string_283[] PROGMEM = "65197 8 100ms EBC3 4 1 8 1094 Brake_Application_Pressure_High_Range_Rear_Axle_#1_Right_Wheel 5_kPa/bit 0 kPa";
const char string_284[] PROGMEM = "65198 8 1000ms AIR1 1 1 8 46 Pneumatic_Supply_Pressure 8_kPa/bit 0 kPa";
const char string_285[] PROGMEM = "65198 8 1000ms AIR1 2 1 8 1086 Parking_and/or_Trailer_Air_Pressure 8_kPa/bit 0 kPa";
const char string_286[] PROGMEM = "65198 8 1000ms AIR1 3 1 8 1087 Service_Brake_Circuit_1_Air_Pressure 8_kPa/bit 0 kPa";
const char string_287[] PROGMEM = "65198 8 1000ms AIR1 4 1 8 1088 Service_Brake_Circuit_2_Air_Pressure 8_kPa/bit 0 kPa";
const char string_288[] PROGMEM = "65198 8 1000ms AIR1 5 1 8 1089 Auxiliary_Equipment_Supply_Pressure 8_kPa/bit 0 kPa";
const char string_289[] PROGMEM = "65198 8 1000ms AIR1 6 1 8 1090 Air_Suspension_Supply_Pressure 8_kPa/bit 0 kPa";
const char string_290[] PROGMEM = "65213 8 1000ms FD 1 1 8 975 Estimated_Percent_Fan_Speed 0.4_%/bit 0 %";
const char string_291[] PROGMEM = "65213 8 1000ms FD 2 1 4 977 Fan_Drive_State 16_states/4_bit 0 bit";
const char string_292[] PROGMEM = "65213 8 1000ms FD 7 1 8 4212 Fan_Drive_Bypass_Command_Status 0.4_%/bit 0 %";
const char string_293[] PROGMEM = "65215 8 100ms EBC2 3 1 8 905 Relative_Speed_Front_Axle_Left_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_294[] PROGMEM = "65215 8 100ms EBC2 4 1 8 906 Relative_Speed_Front_Axle_Right_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_295[] PROGMEM = "65215 8 100ms EBC2 5 1 8 907 Relative_Speed_Rear_Axle_#1_Left_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_296[] PROGMEM = "65215 8 100ms EBC2 6 1 8 908 Relative_Speed_Rear_Axle_#1_Right_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_297[] PROGMEM = "65215 8 100ms EBC2 7 1 8 909 Relative_Speed_Rear_Axle_#2_Left_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_298[] PROGMEM = "65215 8 100ms EBC2 8 1 8 910 Relative_Speed_Rear_Axle_#2_Right_Wheel 1/16_km/h_per_bit 18125 kph";
const char string_299[] PROGMEM = "65216 8 100ms SERV 1 1 8 911 Service_Component_Identification 1_ID/bit 0 ID";
const char string_300[] PROGMEM = "65216 8 100ms SERV 4 1 8 912 Service_Component_Identification 1_ID/bit 0 ID";
const char string_301[] PROGMEM = "65216 8 100ms SERV 5 1 8 915 Service_Delay/Calendar_Time_Based 1_week/bit -125 Weeks";
const char string_302[] PROGMEM = "65216 8 100ms SERV 6 1 8 913 Service_Component_Identification 1_ID/bit 0 ID";
const char string_303[] PROGMEM = "65245 8 1000ms  TC 1 1 8 104 Engine_Turbocharger_Lube_Oil_Pressure_1 4_kPa/bit 0 kPa";
const char string_304[] PROGMEM = "65247 8 250ms EEC3 1 1 8 514 Nominal_Friction_-_Percent_Torque 1_%/bit -125 %";
const char string_305[] PROGMEM = "65247 8 250ms EEC3 4 1 18 519 Engine's_Desired_Operating_Speed_Asymmetry_Adjustment 1/bit 0 Ratio";
const char string_306[] PROGMEM = "65247 8 250ms EEC3 5 1 8 2978 Estimated_Engine_Parasitic_Losses_-_Percent_Torque 1_%/bit -125 %";
const char string_307[] PROGMEM = "65254 8 1000ms TD 1 1 8 959 Seconds 25000ms/bit 0 Seconds";
const char string_308[] PROGMEM = "65254 8 1000ms TD 2 1 8 960 Minutes 1_min/bit 0 Seconds";
const char string_309[] PROGMEM = "65254 8 1000ms TD 3 1 8 961 Hours 1_hr/bit 0 Seconds";
const char string_310[] PROGMEM = "65254 8 1000ms TD 4 1 8 963 Month 1_month/bit 0 Months";
const char string_311[] PROGMEM = "65254 8 1000ms TD 5 1 8 962 Day 25_days/bit 0 Days";
const char string_312[] PROGMEM = "65254 8 1000ms TD 6 1 8 964 Year 1_year/bit 1985 Years";
const char string_313[] PROGMEM = "65254 8 1000ms TD 7 1 8 1601 Local_minute_offset 1_min/bit -7500 Seconds";
const char string_314[] PROGMEM = "65254 8 1000ms TD 8 1 8 1602 Local_hour_offset 1_hr/bit -450000 Seconds";
const char string_315[] PROGMEM = "65262 8 1000ms ET1 1 1 8 110 Engine_Coolant_Temperature 1_deg_C/bit -40 C";
const char string_316[] PROGMEM = "65262 8 1000ms ET1 2 1 8 174 Engine_Fuel_Temperature_1 1_deg_C/bit -40 C";
const char string_317[] PROGMEM = "65262 8 1000ms ET1 7 1 8 52 Engine_Intercooler_Temperature 1_deg_C/bit -40 C";
const char string_318[] PROGMEM = "65262 8 1000ms ET1 8 1 8 1134 Engine_Intercooler_Thermostat_Opening 0.4_%/bit 0 %";
const char string_319[] PROGMEM = "65263 8 5000ms EFLP1 1 1 8 94 Engine_Fuel_Delivery_Pressure 4_kPa/bit 0 kPa";
const char string_320[] PROGMEM = "65263 8 5000ms EFLP1 2 1 8 22 Engine_Extended_Crankcase_Blow-by_Pressure 0 05_kPa/bit 0 kPa";
const char string_321[] PROGMEM = "65263 8 5000ms EFLP1 3 1 8 98 Engine_Oil_Level 0.4_%/bit 0 %";
const char string_322[] PROGMEM = "65263 8 5000ms EFLP1 4 1 8 100 Engine_Oil_Pressure 4_kPa/bit 0 kPa";
const char string_323[] PROGMEM = "65263 8 5000ms EFLP1 7 1 8 109 Engine_Coolant_Pressure 2_kPa/bit 0 kPa";
const char string_324[] PROGMEM = "65263 8 5000ms EFLP1 8 1 8 111 Engine_Coolant_Level 0.4_%/bit 0 %";
const char string_325[] PROGMEM = "65264 8 100ms      PTO 1 1 8 90 Power_Takeoff_Oil_Temperature 1_deg_C/bit -40 C";
const char string_326[] PROGMEM = "65265 8 100ms     CCVS 6 1 8 86 Cruise_Control_Set_Speed 1_km/h_per_bit 0 kph";
const char string_327[] PROGMEM = "65265 8 100ms     CCVS 7 1 5 976 PTO_Governor_State 32_states/5_bit 0 bit";
const char string_328[] PROGMEM = "65266 8 100ms      LFE 7 1 8 51 Engine_Throttle_Position 0.4_%/bit 0 %";
const char string_329[] PROGMEM = "65266 8 100ms      LFE 8 1 8 3673 Engine_Throttle_2_Position 0.4_%/bit 0 %";
const char string_330[] PROGMEM = "65268 8 10000ms TIRE 1 1 8 929 Tire_Location 256_states/8_bit 0 bit";
const char string_331[] PROGMEM = "65268 8 10000ms TIRE 2 1 8 241 Tire_Pressure 4_kPa/bit 0 kPa";
const char string_332[] PROGMEM = "65269 8 1000ms AMB 1 1 8 108 Barometric_Pressure 0.5_kPa/bit 0 kPa";
const char string_333[] PROGMEM = "65269 8 1000ms AMB 6 1 8 172 Engine_Air_Inlet_Temperature 1_deg_C/bit -40 C";
const char string_334[] PROGMEM = "65270 8 5000ms IC1 1 1 8 81 Engine_Diesel_Particulate_Filter_Inlet_Pressure 0.5_kPa/bit 0 kPa";
const char string_335[] PROGMEM = "65270 8 5000ms IC1 2 1 8 102 Engine_Intake_Manifold_#1_Pressure 2_kPa/bit 0 kPa";
const char string_336[] PROGMEM = "65270 8 5000ms IC1 3 1 8 105 Engine_Intake_Manifold_1_Temperature 1_deg_C/bit -40 C";
const char string_337[] PROGMEM = "65270 8 5000ms IC1 4 1 18 106 Engine_Air_Inlet_Pressure 2_kPa/bit 0 kPa";
const char string_338[] PROGMEM = "65270 8 5000ms IC1 5 1 8 107 Engine_Air_Filter_1_Differential_Pressure 0 05_kPa/bit 0 kPa";
const char string_339[] PROGMEM = "65270 8 5000ms IC1 8 1 8 112 Engine_Coolant_Filter_Differential_Pressure 0.5_kPa/bit 0 kPa";
const char string_340[] PROGMEM = "65272 8 1000ms TRF1 3 1 8 126 Transmission_Filter_Differential_Pressure 2_kPa/bit 0 kPa";
const char string_341[] PROGMEM = "65272 8 1000ms TRF1 4 1 8 127 Transmission_Oil_Pressure 16_kPa/bit 0 kPa";
const char string_342[] PROGMEM = "65272 8 1000ms TRF1 7 1 8 3027 Transmission_Oil_Level_High_/_Low 0.5_L/bit -62 5 liters";
const char string_343[] PROGMEM = "65273 8 1000ms AI 1 1 8 75 Steering_Axle_Temperature 1_deg_C/bit -40 C";
const char string_344[] PROGMEM = "65273 8 1000ms AI 2 1 8 930 Drive_Axle_Location 256_states/8_bit 0 bit";
const char string_345[] PROGMEM = "65273 8 1000ms AI 3 1 8 579 Drive_Axle_Lift_Air_Pressure 4_kPa/bit 0 kPa";
const char string_346[] PROGMEM = "65273 8 1000ms AI 4 1 8 578 Drive_Axle_Temperature 1_deg_C/bit -40 C";
const char string_347[] PROGMEM = "65273 8 1000ms AI 5 1 8 2613 Drive_Axle_Lube_Pressure 4_kPa/bit 0 kPa";
const char string_348[] PROGMEM = "65273 8 1000ms AI 8 1 8 2614 Steering_Axle_Lube_Pressure 4_kPa/bit 0 kPa";
const char string_349[] PROGMEM = "65274 8 1000ms B 1 1 8 116 Brake_Application_Pressure 4_kPa/bit 0 kPa";
const char string_350[] PROGMEM = "65274 8 1000ms B 2 1 8 117 Brake_Primary_Pressure 4_kPa/bit 0 kPa";
const char string_351[] PROGMEM = "65274 8 1000ms B 3 1 8 118 Brake_Secondary_Pressure 4_kPa/bit 0 kPa";
const char string_352[] PROGMEM = "65275 8 1000ms RF 1 1 8 119 Hydraulic_Retarder_Pressure 16_kPa/bit 0 kPa";
const char string_353[] PROGMEM = "65275 8 1000ms RF 2 1 8 120 Hydraulic_Retarder_Oil_Temperature 1_deg_C/bit -40 C";
const char string_354[] PROGMEM = "65276 8 1000ms DD 1 1 8 80 Washer_Fluid_Level 0.4_%/bit 0 %";
const char string_355[] PROGMEM = "65276 8 1000ms DD 2 1 8 96 Fuel_Level_1 0.4_%/bit 0 %";
const char string_356[] PROGMEM = "65276 8 1000ms DD 3 1 8 95 Engine_Fuel_Filter_Differential_Pressure 2_kPa/bit 0 kPa";
const char string_357[] PROGMEM = "65276 8 1000ms DD 4 1 8 99 Engine_Oil_Filter_Differential_Pressure 0.5_kPa/bit 0 kPa";
const char string_358[] PROGMEM = "65276 8 1000ms DD 7 1 8 38 Fuel_Level_2 0.4_%/bit 0 %";
const char string_359[] PROGMEM = "65277 8 500ms  A1 1 1 8 72 Engine_Blower_Bypass_Valve_Position 0.4_%/bit 0 %";
const char string_360[] PROGMEM = "65278 8 1000ms AWPP 1 1 8 73 Auxiliary_Pump_Pressure 16_kPa/bit 0 kPa";

#endif

#if !PGN_extended
const char* const string_table[] PROGMEM = {
	string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9, string_10,
	string_11, string_12, string_13, string_14, string_15, string_16, string_17, string_18, string_19, string_20,
	string_21, string_22, string_23, string_24, string_25, string_26, string_27, string_28, string_29, string_30,
	string_31, string_32, string_33, string_34, string_35, string_36, string_37, string_38, string_39, string_40,
	string_41, string_42, string_43, string_44, string_45, string_46, string_47, string_48, string_49, string_50,
	string_51, string_52, string_53, string_54, string_55, string_56, string_57, string_58, string_59, string_60,
	string_61, string_62, string_63, string_64, string_65, string_66, string_67, string_68, string_69, string_70,
	string_71, string_72, string_73, string_74, string_75, string_76, string_77, string_78, string_79, string_80,
	string_81, string_82, string_83, string_84, string_85, string_86, string_87, string_88, string_89, string_90,
	string_91, string_92, string_93, string_94, string_95, string_96, string_97, string_98, string_99, string_100,
	string_101, string_102, string_103, string_104, string_105, string_106, string_107, string_108, string_109, string_110,
	string_111, string_112, string_113, string_114, string_115, string_116, string_117, string_118, string_119, string_120,
	string_121, string_122, string_123, string_124, string_125, string_126,string_127, string_128, string_129, string_130, string_131, string_132,
	string_133, string_134, string_135, string_136 };
#else

const char* const string_table[] PROGMEM = {
	string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9, string_10,
	string_11, string_12, string_13, string_14, string_15, string_16, string_17, string_18, string_19, string_20,
	string_21, string_22, string_23, string_24, string_25, string_26, string_27, string_28, string_29, string_30,
	string_31, string_32, string_33, string_34, string_35, string_36, string_37, string_38, string_39, string_40,
	string_41, string_42, string_43, string_44, string_45, string_46, string_47, string_48, string_49, string_50,
	string_51, string_52, string_53, string_54, string_55, string_56, string_57, string_58, string_59, string_60,
	string_61, string_62, string_63, string_64, string_65, string_66, string_67, string_68, string_69, string_70,
	string_71, string_72, string_73, string_74, string_75, string_76, string_77, string_78, string_79, string_80,
	string_81, string_82, string_83, string_84, string_85, string_86, string_87, string_88, string_89, string_90,
	string_91, string_92, string_93, string_94, string_95, string_96, string_97, string_98, string_99, string_100,
	string_101, string_102, string_103, string_104, string_105, string_106, string_107, string_108, string_109, string_110,
	string_111, string_112, string_113, string_114, string_115, string_116, string_117, string_118, string_119, string_120,
	string_121, string_122, string_123, string_124, string_125, string_126, string_127, string_128, string_129, string_130,
	string_131, string_132, string_133, string_134, string_135, string_136, string_137, string_138, string_139, string_140,
	string_141, string_142, string_143, string_144, string_145, string_146, string_147, string_148, string_149, string_150,
	string_151, string_152, string_153, string_154, string_155, string_156, string_157, string_158, string_159, string_160,
	string_161, string_162, string_163, string_164, string_165, string_166, string_167, string_168, string_169, string_170,
	string_171, string_172, string_173, string_174, string_175, string_176, string_177, string_178, string_179, string_180,
	string_181, string_182, string_183, string_184, string_185, string_186, string_187, string_188, string_189, string_190,
	string_191, string_192, string_193, string_194, string_195, string_196, string_197, string_198, string_199, string_200,
	string_201, string_202, string_203, string_204, string_205, string_206, string_207, string_208, string_209, string_210,
	string_211, string_212, string_213, string_214, string_215, string_216, string_217, string_218, string_219, string_220,
	string_221, string_222, string_223, string_224, string_225, string_226, string_227, string_228, string_229, string_230,
	string_231, string_232, string_233, string_234, string_235, string_236, string_237, string_238, string_239, string_240,
	string_241, string_242, string_243, string_244, string_245, string_246, string_247, string_248, string_249, string_250,
	string_251, string_252, string_253, string_254, string_255, string_256, string_257, string_258, string_259, string_260,
	string_261, string_262, string_263, string_264, string_265, string_266, string_267, string_268, string_269, string_270,
	string_271, string_272, string_273, string_274, string_275, string_276, string_277, string_278, string_279, string_280,
	string_281, string_282, string_283, string_284, string_285, string_286, string_287, string_288, string_289, string_290,
	string_291, string_292, string_293, string_294, string_295, string_296, string_297, string_298, string_299, string_300,
	string_301, string_302, string_303, string_304, string_305, string_306, string_307, string_308, string_309, string_310,
	string_311, string_312, string_313, string_314, string_315, string_316, string_317, string_318, string_319, string_320,
	string_321, string_322, string_323, string_324, string_325, string_326, string_327, string_328, string_329, string_330,
	string_331, string_332, string_333, string_334, string_335, string_336, string_337, string_338, string_339, string_340,
	string_341, string_342, string_343, string_344, string_345, string_346, string_347, string_348, string_349, string_350,
	string_351, string_352, string_353, string_354, string_355, string_356, string_357, string_358, string_359, string_360 };

#endif

// Variables  p-tp-p
const uint8_t msgEngineTemperature[] = { 0xa0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; // PGN 65262 0Xfeee
const uint8_t Engine_Total_Average_Fuel_Rate[] = { 0xa2, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };// PGN 65101 0xfe4d
const uint8_t Specific_Heat_Ratio[] = { 0xbc, 0x7d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };// PGN 65109 fe55
const uint8_t Engine_Cylinder_1_Combustion_Time[] = { 0xaa, 0x5e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };// PGN 65147 fe7b
const uint8_t Engine_Fuel_Flow_Rate[] = { 0x05, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; // PGN 65153 fe81
const uint8_t Engine_Cylinder_1_Ignition_Timing[] = { 0xbb, 0x74, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; //PGN 65154 fe82
const uint8_t Engine_Power[] = { 0xff, 0x34, 0x40, 0xff, 0xff, 0xff, 0xff, 0xff }; // PGN 65168 fe90
const uint8_t Engine_Actual_Exhaust_Oxygen[] = { 0xff, 0xff, 0xff, 0xff, 0xb1, 0x33, 0xff, 0xff }; // PGN 65193 fea9
const uint8_t Trip_Average_Fuel_Rate[] = { 0xff, 0xff, 0xff, 0xff, 0x53, 0x01, 0xff, 0xff }; //PGN 65203 feb3
const uint8_t Trip_Cruise_Distance[] = { 0xff, 0xff, 0x12, 0x22, 0x10, 0x00, 0xff, 0xff }; // PGN 65206 feb6
const uint8_t Engine_Total_Hours_of_Operation[] = { 0x11, 0x22, 0x20, 0x00, 0xff, 0xff, 0xff, 0xff }; // PGN 65253 fee5
const uint8_t Engine_Trip_Fuel[] = { 0x90, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff }; //PGN 65257 fee9

char buffer[200];    // make sure this is large enough for the largest string_it must hold
uint16_t velocidades[] = { 10,20,50,100,250,500,1000,5000,10000 };

//-----------------------------------------------------------
// Variables encoders
//-----------------------------------------------------------
uint8_t encoderPin1 = 2;
uint8_t encoderPin2 = 3;
uint8_t encoderPin3 = 18;
uint8_t encoderPin4 = 19;
uint8_t encoderSwitchPin1 = 23; //push button switch 1
uint8_t encoderSwitchPin2 = 22; //push button switch 2

// Marca temporal interrupción CAN
static long time_old;

// Control encoders
volatile int  lastEncoded1 = 1;
volatile long encoderValue1 = 1;
volatile long Value1, Value2, Value1_ant = 1;
volatile int  lastEncoded2 = 1;
volatile long encoderValue2 = 1;
long lastencoderValue = 0;

struct valores_PGN {
	char address_PGN_hex[10];
	uint8_t orden_PGN;
} frames_PGN[10];

struct menu {
	volatile uint16_t valor_new = 0;
	volatile uint16_t valor_old = 99;
	uint8_t valor_excase;
	uint8_t posicion_int = 0;
	uint8_t posicion2_int = 0;
	uint8_t SPN_length_int = 0;
	uint8_t frame_activo = 0;
} encoder_menu[num_menu];

struct time {
	uint8_t sec;
	uint8_t min;
	uint8_t hor;
} tiempo;

//-----------------------------------------------------------
// Variables CAN
//-----------------------------------------------------------
struct mystruct
{
	uint16_t PGN;
	uint16_t periodo;
	uint8_t address;
	uint8_t priority;
	uint8_t msg[8];
};

struct mystruct mensaje_can[10];
long lMessageID;
uint8_t flag_1 = 0;
uint8_t lock = 0;
uint16_t PGN_anterior = 0;
uint16_t PGN_mas, PGN_menos = 0;

// The tasks function
void vPeriodicTask3(struct mystruct* can_tx);
void vPeriodicTask4(void *pvParameters);
void vPeriodicTask2(void *pvParameters);
void recibir_can();
int parseString(char* line, char*** argv);

//-----------------------------------------------------------
// Variables operativas
//-----------------------------------------------------------
uint8_t pin_led_verde = 12; //led verde
uint8_t PINtoRESET = 7;     //Reset
uint8_t pin_led_rojo = 11;  //led rojo
							//uint8_t enable_BL = 6;      //Bluetooth
							//uint8_t status_BL = 8;      //Bluetooth
uint8_t pin_beep = 24;      //Beep

							// Declarations
struct AMessage {
	byte nPriority;
	byte nSrcAddr;
	byte nDestAddr;
	byte nData[8];
	int nDataLen;
	long lPGN;
	long time;
	uint32_t cuenta = 0;
} xMessage;

char sString[80];
TaskHandle_t xHandle[10];  // colas CAN
uint32_t kilometros = 0;

char PGN[5];  // PGN
char length_PGN[5];  // Length
char rate[8];  // Transmission Rate
char acronym[5];  // Acronym
char position[5];  // pos
char position2[5];  // pos2
char SPN_length[10];  // SPN length
char SPN[10];  // SPN
char name[70]; // Name
char resolution[10];  // Resolution
char offset[10];  // Offset
char units[10]; // Units

void sendString(const char *String, uint8_t col, uint8_t row)
{
	lcd.setCursor(col, row);        // go to the 2nd line
	lcd.print(String);
}

void sendFloat(float digit, uint8_t dec, uint8_t nad, uint8_t col, uint8_t row)
{
	char line[10];//Ten characters, I hope that's enough
	dtostrf(digit, dec, nad, line);//Convert the float value to a string
	sendString(line, col, row);
}
void lcdclear()
{
	lcd.home(); // go home
	sendString("                ", 0, 0);
	sendString("                ", 0, 1);
	lcd.home(); // go home
}
void scrollString(char* message, byte row, unsigned int time)//written by Nathan Chantrell http://nathan.chantrell.net/
{
	char buffer[16];
	for (byte i = 0; i<strlen(message) + 16; i++) {
		byte pos = i + 1;
		for (byte j = 0; j<16; j++) {
			if ((pos<16) || (pos>strlen(message) + 15)) { // pad and trail with blank spaces
				buffer[j] = ' ';
			}
			else buffer[j] = message[pos - 16];
			pos++;
		}
		//cursPos(0,row); removed by PG
		sendString(buffer, 0, row); //Edited by PG tho include the cursor pos within the sendString command
		delay(time);
	}
}

int carga_buffer(int i) {
	strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i]))); // Necessary casts and dereferencing, just copy
	int n = sscanf(buffer, "%s %s %s %s %s %s %s %s %s %s %s %s", PGN, length_PGN, rate, acronym, position, position2, SPN_length, SPN, name, resolution, offset, units);
	return n;
}

#define DTC_code(n,n1,n2) itoa((encoder_menu[n].valor_new >> 2), array_prov, 2); sendString(array_prov, ((encoder_menu[n].valor_new)<8) + n1, n2)

void valores_msg(uint8_t e_encoder) {
	mensaje_can[9].msg[e_encoder - 40] = encoder_menu[e_encoder].valor_new >> 2;
	sendString("**", 2 * (e_encoder - 40), 0);

	for (uint8_t i = 0; i < 8; i++) {
		char array_prov[3];
		itoa(mensaje_can[9].msg[i], array_prov, 16);
		if (mensaje_can[9].msg[i] > 16) sendString(array_prov, 2 * i, 1);
		else { sendString(array_prov, 2 * i + 1, 1); sendString("0", 2 * i, 1); }
	}
	rec_eeprom(e_encoder);
	encoder_menu[e_encoder].valor_old = encoder_menu[e_encoder].valor_new >> 2;
	encoder_menu[0].valor_excase = e_encoder;
}

void ver_PGN(uint8_t igual0, uint8_t igual1, uint8_t num) {

	if ((encoder_menu[37].valor_new >> 2) > 7) encoder_menu[37].valor_new = 7 << 2;
	// valor maximo de T
	if (encoder_menu[38].valor_new >> 2 > 8) encoder_menu[38].valor_new = 0;
	static char temp[16];
	// printear
	sprintf(temp, "PGN:%05u", (uint16_t)65269 + (encoder_menu[35].valor_new >> 2) - 8192);
	sendString(temp, 0, 0);

	sprintf(temp, "S:%02X", (encoder_menu[36].valor_new >> 2));
	sendString(temp, 10, 0);

	sprintf(temp, "P:%01d T:%03d ", encoder_menu[37].valor_new >> 2, velocidades[encoder_menu[38].valor_new >> 2]);
	sendString(temp, 0, 1);
	sendString(">", igual0, igual1);
	if (num != 39) { sendString(">", igual0, igual1); sendString(" ", 13, 1); }
	else sendString(">", igual0, igual1);

	// Encolar
	mensaje_can[9].PGN = 65269 + (encoder_menu[35].valor_new >> 2) - 8192;
	mensaje_can[9].priority = encoder_menu[37].valor_new >> 2;
	mensaje_can[9].periodo = velocidades[encoder_menu[38].valor_new >> 2];
	mensaje_can[9].address = encoder_menu[36].valor_new >> 2;

	rec_eeprom(num);
	encoder_menu[num].valor_old = encoder_menu[num].valor_new >> 2;
	encoder_menu[0].valor_excase = num;
}

void DTC(int p) {
	char array_prov[3];
	DTC_code(30, 6, 0);
	DTC_code(31, 6, 1);
	DTC_code(32, 14, 0);
	DTC_code(33, 14, 1);
	rec_eeprom(p);
	encoder_menu[p].valor_old = encoder_menu[p].valor_new >> 2;
	encoder_menu[0].valor_excase = p;
}

void mostrar_valores_LCD(uint8_t canal) {
	encoder_menu[canal].valor_old = (encoder_menu[canal].valor_new >> 2);
	sendString("                ", 0, 1);
	sendString(PGN, 0, 1);
	if (atoi(rate) == 100) sendString(".1s", 6, 1);
	else if (atoi(rate) == 500) sendString(".5s", 6, 1);
	else if (atoi(rate) == 1000) sendString("1s", 6, 1);
	else if (atoi(rate) == 5000) sendString("5s", 6, 1);
	else if (atoi(rate) == 10000) sendString("10s", 6, 1);
	else if (atoi(rate) == 20000) sendString("20s", 6, 1);
	else if (atoi(rate) == 30000) sendString("30s", 6, 1);
	else sendString(rate, 6, 1);
	sendString(position, 10, 1);
	sendString(".", 11, 1);
	sendString(position2, 12, 1);
	sendString(SPN_length, 14, 1);
	sendString(acronym, 6, 0);
	sendString(SPN, 11, 0);
	sendFloat(encoder_menu[canal].valor_new >> 2, 3, 0, 1, 0);
	encoder_menu[0].valor_excase = canal;
	flag_1 = 0;
}

//variables inicio
int eeAddress = 0;
int inicio = 26;
int match = 0;
bool restored = false;

SemaphoreHandle_t xSemaphore = NULL;
uint32_t count = 0; // costate 4

					// actualiza valores en eeprom
void rec_eeprom(uint8_t i)
{
	eeAddress = sizeof(int);
	for (uint8_t u = 1; u < i; u++) eeAddress += sizeof(menu);
	EEPROM.put(eeAddress, encoder_menu[i].valor_new); // guarda nuevo valor al inicio
#if restaurar_menu
	encoder_menu[cases].valor_new = encoderValue2;
	eeAddress = sizeof(int);
	for (uint8_t u = 1; u < cases; u++) eeAddress += sizeof(menu);
	EEPROM.put(eeAddress, encoder_menu[cases].valor_new); // guarda nuevo valor al inicio
#endif
}

static bool IsOdd(int value)
{
	return value % 2 != 0;
}

QueueHandle_t xQueue1;
struct AMessage *pxMessage;
uint8_t ok = 0;

void llenar_source_address()
{
	sendString("A: ", 0, 0);
	sendString("B: ", 0, 1);
	sendString("C: ", 5, 0);
	sendString("D: ", 5, 1);
	sendString("E: ", 10, 0);
	sendString("F: ", 10, 1);
	if (encoder_menu[1].frame_activo) sendFloat((encoder_menu[9].valor_new >> 2), 3, 0, 2, 0); else sendString("---", 2, 0);
	if (encoder_menu[2].frame_activo) sendFloat((encoder_menu[10].valor_new >> 2), 3, 0, 2, 1); else sendString("---", 2, 1);
	if (encoder_menu[3].frame_activo) sendFloat((encoder_menu[11].valor_new >> 2), 3, 0, 7, 0); else sendString("---", 7, 0);
	if (encoder_menu[4].frame_activo) sendFloat((encoder_menu[12].valor_new >> 2), 3, 0, 7, 1); else sendString("---", 7, 1);
	if (encoder_menu[5].frame_activo) sendFloat((encoder_menu[13].valor_new >> 2), 3, 0, 12, 0); else sendString("---", 12, 0);
	if (encoder_menu[6].frame_activo) sendFloat((encoder_menu[14].valor_new >> 2), 3, 0, 12, 1); else sendString("---", 12, 1);
}

// AES
void recibir() {
	int MAX_CMD_LENGTH = 30;
	static int cmdIndex;
	char cmd[30];
	static const uint8_t key1[] = key_1;
	static const uint8_t key2[] = key_2;
	static char data[16]; //16 chars == 16 bytes
	static char data_old[16];
	for (int i = 0; i < 16; i++) data_old[i] = (char)random(0, 9);
	strcpy(data, data_old);
	aes256_enc_single(key1, data);
	Serial3.print(data);
	Serial3.print("\n");
	delay(100);
	while (Serial3.available()) {
		char byteIn = Serial3.read();
		cmd[cmdIndex] = byteIn;
		if (byteIn == '\n') {
			cmd[cmdIndex] = '\0';
			cmdIndex = 0;
			aes256_dec_single(key2, cmd);
			if (strncmp(cmd, data_old, 16) == 0) ok = 1; else ok = 0;
		}
		else if (cmdIndex++ >= MAX_CMD_LENGTH)cmdIndex = 0;
	}
}// fin recibir

 //-----------------------------------------------------------------
 //                            SETUP
 //-----------------------------------------------------------------

void setup(void)
{
	lcd.begin(16, 2);    // Inicializar el display con 16 caraceres 2 lineas
	lcd.setBacklightPin(3, POSITIVE);
	lcd.setBacklight(HIGH);

	lcd.home(); // go home
	sendString("Warming Up!", 0, 0);

	//analogReference(EXTERNAL); // a 5Vcc de la alimentación
	Serial3.begin(9600); // comunicaciones UNO
	Serial2.begin(9600); // comunicaciones BL

						 // tiempo por defecto
	tiempo.hor = 0;
	tiempo.min = 0;
	tiempo.sec = 0;

	//Apago led rojo
	digitalWrite(pin_led_rojo, HIGH);     // turn the LED on (HIGH is the voltage level)

										  // Tramas configurables
	mensaje_can[0] = { 65001,10,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
	mensaje_can[1] = { 65002,20,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
	mensaje_can[2] = { 65003,50,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
	mensaje_can[3] = { 65004,100,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
	mensaje_can[4] = { 65005,250,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
	mensaje_can[5] = { 65006,500,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };

	// Tramas autogeneradas por Arduino
	mensaje_can[6] = { 65248,1000,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } }; // Trip distance
	mensaje_can[7] = { 65254,1000,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } }; // calendario reloj
	mensaje_can[8] = { 65226,1000,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } }; // DM01
	mensaje_can[9] = { 65269,10000,0,6,{ 0xCB, 0x40, 0x25, 0xA0, 0x25, 0x60, 0x27, 0x25 } };// Condiciones ambientales configurables

	EEPROM.get(eeAddress, match);

	if (!digitalRead(encoderSwitchPin1)) alive_led = 1; else digitalWrite(pin_led_verde, HIGH);

	if (match != inicio || !digitalRead(encoderSwitchPin2)) {
		EEPROM.put(eeAddress, inicio);

		// valores por defecto tramas CAN
		eeAddress += sizeof(int);
		EEPROM.put(eeAddress, encoder_menu[1].valor_new = 0 << 2);
#if !PGN_extended
		encoder_menu[1].valor_new = 0 << 2; // Menú A
		encoder_menu[2].valor_new = 0 << 2; // Menú B
		encoder_menu[3].valor_new = 0 << 2; // Menú C
		encoder_menu[4].valor_new = 0 << 2; // Menú D
		encoder_menu[5].valor_new = 0 << 2; // Menú E
		encoder_menu[6].valor_new = 0 << 2; // Menú F
#else
		encoder_menu[1].valor_new = 0 << 2; // Menú A
		encoder_menu[2].valor_new = 0 << 2; // Menú B
		encoder_menu[3].valor_new = 0 << 2; // Menú C
		encoder_menu[4].valor_new = 0 << 2; // Menú D
		encoder_menu[5].valor_new = 0 << 2; // Menú E
		encoder_menu[6].valor_new = 0 << 2; // Menú F
#endif
		encoder_menu[7].valor_new = 7 << 2;  // 250kps
		encoder_menu[8].valor_new = 1 << 2;  // Inicio Serial y BL -> TXGen -> TXGen
		encoder_menu[9].valor_new = 0 << 2;  // A
		encoder_menu[10].valor_new = 0 << 2; // B
		encoder_menu[11].valor_new = 0 << 2; // C
		encoder_menu[12].valor_new = 0 << 2; // D
		encoder_menu[13].valor_new = 0 << 2; // E
		encoder_menu[14].valor_new = 0 << 2; // F
		encoder_menu[15].valor_new = 6 << 2; // Prioridad 6 por defecto
		encoder_menu[16].valor_new = 0 << 2; //  Destination
		encoder_menu[17].valor_new = 9 << 2; // 115.2K
		encoder_menu[18].valor_new = 1 << 2; // dia
		encoder_menu[19].valor_new = 1 << 2; // mes
		encoder_menu[20].valor_new = 2016 << 2; // año
		encoder_menu[26].valor_new = 1208 << 2; // SPN DTC
		encoder_menu[27].valor_new = 3 << 2; // FMI DTC
		encoder_menu[28].valor_new = 1 << 2; // OC DTC
		encoder_menu[29].valor_new = 0 << 2; // CM DTC
		encoder_menu[30].valor_new = 0; // MILS
		encoder_menu[31].valor_new = 0; // RSLS
		encoder_menu[32].valor_new = 0; // AWLS
		encoder_menu[33].valor_new = 0; // PLS
		encoder_menu[34].valor_new = 0; //
		encoder_menu[35].valor_new = 8192 << 2; // PGN offset onfigurable
		encoder_menu[36].valor_new = 0; // Dest address
		encoder_menu[37].valor_new = 6 << 2; // preference
		encoder_menu[38].valor_new = 1 << 2; // periodo T
		encoder_menu[39].valor_new = 1; // Y/N
		encoder_menu[40].valor_new = mensaje_can[9].msg[0] << 2;
		encoder_menu[41].valor_new = mensaje_can[9].msg[1] << 2;
		encoder_menu[42].valor_new = mensaje_can[9].msg[2] << 2;
		encoder_menu[43].valor_new = mensaje_can[9].msg[3] << 2;
		encoder_menu[44].valor_new = mensaje_can[9].msg[4] << 2;
		encoder_menu[45].valor_new = mensaje_can[9].msg[5] << 2;
		encoder_menu[46].valor_new = mensaje_can[9].msg[6] << 2;
		encoder_menu[47].valor_new = mensaje_can[9].msg[7] << 2;
		encoder_menu[48].valor_new = 0 << 2; // Banners
		encoder_menu[49].valor_new = 0xFF << 2;
		encoder_menu[50].valor_new = 1 << 2; // uLTIMO MENU

		restored = true;
	}
	else
	{  // recuperar valores from eeprom
		eeAddress += sizeof(int);
		EEPROM.get(eeAddress, encoder_menu[1].valor_new);
		for (int i = 2; i < (num_menu - 1); i++) {
			eeAddress += sizeof(menu);
			EEPROM.get(eeAddress, encoder_menu[i].valor_new);
		}// for
		for (int i = 0; i < 6; i++) mensaje_can[i].address = (encoder_menu[9 + i].valor_new >> 2); // asignación direcciones A...F
		encoderValue2 = encoder_menu[cases].valor_new;
		restored = false;
	}//if

	const long velocidades[] = { 300,600,1200,2400,4800,9600,14400,19200,57600,115200,128000,153600,230400,250000,256000 };

	Serial.begin(velocidades[encoder_menu[17].valor_new >> 2]);
	while (!Serial) { ; } // wait for serial port to connect. Needed for native USB port only

	pinMode(pin_led_verde, OUTPUT);   // led 12
	pinMode(pin_led_rojo, OUTPUT);    // led 11
	pinMode(pin_beep, OUTPUT);        // beep
	pinMode(A8, OUTPUT);  // digitalWrite(A8, HIGH); // azul
	pinMode(A9, OUTPUT);  // digitalWrite(A9, HIGH); // rojo
	pinMode(A10, OUTPUT); // digitalWrite(A10, HIGH); // amarillo
	pinMode(A11, OUTPUT); // digitalWrite(A11, HIGH); // verde
	digitalWrite(pin_led_rojo, LOW);

	// hello world
	delay(500); digitalWrite(pin_led_rojo, HIGH);
	digitalWrite(A8, LOW); delay(300);  digitalWrite(A8, HIGH);
	digitalWrite(A9, LOW); delay(300); digitalWrite(A9, HIGH);
	digitalWrite(A10, LOW); delay(300); digitalWrite(A10, HIGH);
	digitalWrite(A11, LOW); delay(300); digitalWrite(A11, HIGH);

	// Reset HW
	pinMode(PINtoRESET, INPUT);    // Just to be clear, as default is INPUT. Not really needed
	digitalWrite(PINtoRESET, LOW); // Prime it, but does not actually set output. Does disable 10K pull Up, but who cares

	Wire.begin();
	//LCD.init();

	// Pines encoders
	pinMode(encoderPin1, INPUT);
	pinMode(encoderPin2, INPUT);
	pinMode(encoderPin3, INPUT);
	pinMode(5, INPUT);
	pinMode(6, INPUT);
	pinMode(encoderPin4, INPUT);
	pinMode(encoderSwitchPin1, INPUT);
	pinMode(encoderSwitchPin2, INPUT);

	// pullup pulsadores
	digitalWrite(encoderSwitchPin1, HIGH); //turn pullup resistor on
	digitalWrite(encoderSwitchPin2, HIGH); //turn pullup resistor on

	//call updateEncoder() when any high/low changed seen
	attachInterrupt(digitalPinToInterrupt(encoderPin2), updateEncoder1, CHANGE); //Der.
	attachInterrupt(digitalPinToInterrupt(encoderPin3), updateEncoder2, CHANGE); //Izq.
	attachInterrupt(digitalPinToInterrupt(encoderPin4), updateEncoder2, CHANGE); //Izq.
	attachInterrupt(digitalPinToInterrupt(encoderPin1), recibir_can, FALLING);   //Can

	// Creamos la cola
	xQueue1 = xQueueCreate(10, sizeof(struct AMessage *)); // 10
	if (xQueue1 == NULL)
	{ // Queue was not created and must not be used
		Serial.println("Queue Fail !");
	}
	else Serial.println("Queue Created OK.");

#if seguridad_activada
	while (!ok) recibir();
	ok = 0;
#endif

	// Initialize the CAN controller
	if (canInitialize((encoder_menu[7].valor_new >> 2) + 3) == CAN_OK)
		Serial.print("CAN Init OK.\n\r\n\r");
	else
		Serial.print("CAN Init Failed.\n\r");

	// Start the scheduler so our tasks start executing
	xSemaphore = xSemaphoreCreateMutex();

	// Task msgs CAN
	BaseType_t xReturned;

	TaskHandle_t serial_task = NULL;

	Serial.print("Tarea:");
	for (byte i = 0; i < 10; i++) {
		xReturned = xTaskCreate(vPeriodicTask3, "Tramas_CAN", mem_tramas_can, &mensaje_can[i], tskIDLE_PRIORITY + 3, &xHandle[i]); // mantener +3
		if (xReturned == pdPASS)
		{
			// The task was created.  Use the task's handle to delete the task
			Serial.print(" ");
			Serial.print(i);

			// no desactivo los que estaban activos, que los recargo
			if ((i < 6) | (0)) // No creo DM01 tampoco
			{
				if (encoder_menu[i + 1].valor_new >> 2 == 0) vTaskSuspend(xHandle[i]);
				carga_buffer((int)encoder_menu[i + 1].valor_new >> 2);
				mensaje_can[i].PGN = atoi(PGN);
				mensaje_can[i].periodo = atoi(rate);
				encoder_menu[i].posicion_int = atoi(position);
				encoder_menu[i].posicion2_int = atoi(position2);
				encoder_menu[i].SPN_length_int = atoi(SPN_length);
			}
		}
		delay(100);
	} Serial.println("");

	//recarga valores trama PGN variable
	for (byte i = 0; i < 8; i++) mensaje_can[9].msg[i] = encoder_menu[40 + i].valor_new >> 2;

	// Presentación
	sendString("Begas Motor (", 0, 0);
	sendString("J1939 Emulator 2", 0, 1);
	if (restored == true) sendString("r) ", 13, 0); else sendString("c) ", 13, 0);
	delay(2000);

	// Task S.O.
	xTaskCreate(vPeriodicTask1, "Task 1", mem_tasks, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(vPeriodicTask2, "Task 2", mem_tasks, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(vPeriodicTask4, "Task 4", mem_tasks, NULL, tskIDLE_PRIORITY + 1, &serial_task); // serial

#if seguridad_activada
	while (!ok) recibir();
	ok = 0;
#endif

	for (byte i = 0; i < 10; i++) { // recupero valors frame
		encoder_menu[i].frame_activo = encoder_menu[i].valor_new >> 2;
	}

	vTaskStartScheduler();

	// nunca se llega aquí
	for (;;);

} // fin setup

  // States´ machine
static void vPeriodicTask1(void *pvParameters)
{
	TickType_t xLastWakeTime2 = xTaskGetTickCount();

	// As per most tasks, this task is implemented in an infinite loop
	for (;;)
	{
		Value2 = encoderValue2 >> 2;

		switch (Value2) {
		case 0: {
			if (!digitalRead(encoderSwitchPin2)) {
				sendString(Serial_N, 0, 0);
				sendString(Options, 0, 1);
				delay(3000);
			}
			else {
				static bool ree = true;
				sendString("   SIROCCO (c)   ", 0, 0);
				if (ree) scrollString("J1939 ECU Emulator 2 All In One", 1, 150); // Banner
				ree = false;
				sendString("J1939 Emulator 2", 0, 1);
				encoder_menu[0].valor_excase = 0;
				flag_1 = 0;
				encoder_menu[1].valor_new = encoder_menu[1].frame_activo << 2;
				rec_eeprom(0);
			}
		} break;

		case 1: {
			if (encoder_menu[1].frame_activo == (encoder_menu[1].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) sendString("A", 0, 0); else sendString(" ", 0, 0); }
			if ((encoder_menu[1].valor_old != encoder_menu[1].valor_new >> 2) || (encoder_menu[0].valor_excase != 1)) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				carga_buffer((int)encoder_menu[1].valor_new >> 2);
				sendString("A               ", 0, 0);
				mostrar_valores_LCD(1);
				rec_eeprom(1);
			}
		} break; // case 1

		case 2: {
			if (encoder_menu[2].frame_activo == (encoder_menu[2].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) sendString("B", 0, 0); else sendString(" ", 0, 0); }
			if (encoder_menu[2].valor_old != encoder_menu[2].valor_new >> 2 || encoder_menu[0].valor_excase != 2) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				if (1) encoder_menu[1].valor_new = encoder_menu[1].frame_activo << 2;
				carga_buffer((int)encoder_menu[2].valor_new >> 2);
				sendString("B               ", 0, 0);
				mostrar_valores_LCD(2);
				rec_eeprom(2);
			}
		} break; // case 2

		case 3: {
			if (encoder_menu[3].frame_activo == (encoder_menu[3].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) sendString("C", 0, 0); else sendString(" ", 0, 0); }
			if (encoder_menu[3].valor_old != encoder_menu[3].valor_new >> 2 || encoder_menu[0].valor_excase != 3) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				if (1) encoder_menu[2].valor_new = encoder_menu[2].frame_activo << 2;
				carga_buffer((int)encoder_menu[3].valor_new >> 2);
				sendString("C               ", 0, 0);
				mostrar_valores_LCD(3);
				rec_eeprom(3);
			}
		} break; // case 3

		case 4: {
			if (encoder_menu[4].frame_activo == (encoder_menu[4].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) sendString("D", 0, 0); else sendString(" ", 0, 0); }
			if (encoder_menu[4].valor_old != encoder_menu[4].valor_new >> 2 || encoder_menu[0].valor_excase != 4) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				if (1) encoder_menu[3].valor_new = encoder_menu[3].frame_activo << 2;
				carga_buffer((int)encoder_menu[4].valor_new >> 2);
				sendString("D               ", 0, 0);
				mostrar_valores_LCD(4);
				rec_eeprom(4);
			}
		} break; // case 4

		case 5: {
			if (encoder_menu[5].frame_activo == (encoder_menu[5].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) sendString("E", 0, 0); else sendString(" ", 0, 0); }
			if (encoder_menu[5].valor_old != encoder_menu[5].valor_new >> 2 || encoder_menu[0].valor_excase != 5) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				if (1) encoder_menu[4].valor_new = encoder_menu[4].frame_activo << 2;
				carga_buffer((int)encoder_menu[5].valor_new >> 2);
				sendString("E               ", 0, 0);
				mostrar_valores_LCD(5);
				rec_eeprom(5);
			}
		} break; // case 5

		case 6: {
			if (encoder_menu[6].frame_activo == (encoder_menu[6].valor_new >> 2)) { if (IsOdd(tiempo.sec) == true) sendString("F", 0, 0); else sendString(" ", 0, 0); }
			if (encoder_menu[6].valor_old != encoder_menu[6].valor_new >> 2 || encoder_menu[0].valor_excase != 6) {
				if (encoder_menu[Value2].valor_new >> 2 > tope_menu) encoder_menu[Value2].valor_new = tope_menu << 2; // Máx número de PGNs
				if (1) encoder_menu[5].valor_new = encoder_menu[5].frame_activo << 2;
				carga_buffer((int)encoder_menu[6].valor_new >> 2);
				sendString("F               ", 0, 0);
				mostrar_valores_LCD(6);
				rec_eeprom(6);
			}
		} break; // case 6

		case 7: {
			if (encoder_menu[7].valor_old != encoder_menu[7].valor_new >> 2 || encoder_menu[0].valor_excase != 7) {
				if (1) encoder_menu[6].valor_new = encoder_menu[6].frame_activo << 2;
				sendString("CAN bps:  ", 0, 0);
				sendString("(Set & Push)     ", 0, 1);
				if (encoder_menu[7].valor_new >> 2 > 9) (encoder_menu[7].valor_new = 9 << 2);
				if (encoder_menu[7].valor_new >> 2 == 0) sendString(" 20K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 1) sendString(" 40K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 2) sendString(" 50K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 3) sendString(" 80K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 4) sendString("100K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 5) sendString("125K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 6) sendString("200K ", 11, 0);
				else if (encoder_menu[7].valor_new >> 2 == 7) sendString(" 250K ", 10, 0);
				else if (encoder_menu[7].valor_new >> 2 == 8) sendString(" 500K ", 10, 0);
				else if (encoder_menu[7].valor_new >> 2 == 9) sendString("1000K", 10, 0);
				rec_eeprom(7);
				encoder_menu[7].valor_old = encoder_menu[7].valor_new >> 2;
				encoder_menu[0].valor_excase = 7;
				flag_1 = 0;
			}
		} break; // case 7

		case 8: {
			if (encoder_menu[8].valor_old != encoder_menu[8].valor_new >> 2 || encoder_menu[0].valor_excase != 8) {
				if ((encoder_menu[8].valor_new >> 2) > 8) encoder_menu[8].valor_new = 0;
				sendString("Bluetooh: ", 0, 0);
				sendString("  Serial: ", 0, 1);
				if (encoder_menu[8].valor_new >> 2 == 0) { sendString("OFF    ", 9, 0);   sendString("OFF    ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 1) { sendString("TXGen   ", 9, 0);  sendString("TXGen   ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 2) { sendString("TXGen   ", 9, 0);  sendString("RXCAN   ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 3) { sendString("RXCAN   ", 9, 0);  sendString("TXGen   ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 4) { sendString("RXCAN   ", 9, 0);  sendString("RXCAN   ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 5) { sendString("TXGen   ", 9, 0);  sendString("OFF    ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 6) { sendString("OFF    ", 9, 0);  sendString("TXGen   ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 7) { sendString("RXCAN   ", 9, 0);  sendString("OFF    ", 9, 1); }
				else if (encoder_menu[8].valor_new >> 2 == 8) { sendString("OFF    ", 9, 0);  sendString("RXCAN   ", 9, 1); }
				rec_eeprom(8);
				encoder_menu[8].valor_old = encoder_menu[8].valor_new >> 2;
				encoder_menu[0].valor_excase = 8;
			}
		} break; // case 8

		case 9: { // Address A: Address B:
			if (encoder_menu[9].valor_old != encoder_menu[9].valor_new >> 2 || encoder_menu[0].valor_excase != 9) {
				if ((encoder_menu[9].valor_new >> 2) > 255) encoder_menu[9].valor_new = 0;
				llenar_source_address();
				sendString("A>", 0, 0);
				mensaje_can[0].address = (encoder_menu[9].valor_new >> 2); // asignación dirección
				rec_eeprom(9);
				encoder_menu[9].valor_old = encoder_menu[9].valor_new >> 2;
				encoder_menu[0].valor_excase = 9;
			}
		} break; // case 9

		case 10: {
			if (encoder_menu[10].valor_old != encoder_menu[10].valor_new >> 2 || encoder_menu[0].valor_excase != 10) {
				if ((encoder_menu[10].valor_new >> 2) > 255) encoder_menu[10].valor_new = 0;
				llenar_source_address();
				sendString("B>", 0, 1);
				mensaje_can[1].address = (encoder_menu[10].valor_new >> 2); // asignación dirección
				rec_eeprom(10);
				encoder_menu[10].valor_old = encoder_menu[10].valor_new >> 2;
				encoder_menu[0].valor_excase = 10;
			}
		} break; // case 10

		case 11: {
			if (encoder_menu[11].valor_old != encoder_menu[11].valor_new >> 2 || encoder_menu[0].valor_excase != 11) {
				if ((encoder_menu[11].valor_new >> 2) > 255) encoder_menu[11].valor_new = 0;
				llenar_source_address();
				sendString("C>", 5, 0);
				mensaje_can[2].address = (encoder_menu[11].valor_new >> 2); // asignación dirección
				rec_eeprom(11);
				encoder_menu[11].valor_old = encoder_menu[11].valor_new >> 2;
				encoder_menu[0].valor_excase = 11;
			}
		} break; // case 11

		case 12: {
			if (encoder_menu[12].valor_old != encoder_menu[12].valor_new >> 2 || encoder_menu[0].valor_excase != 12) {
				if ((encoder_menu[12].valor_new >> 2) > 255) encoder_menu[12].valor_new = 0;
				llenar_source_address();
				sendString("D>", 5, 1);
				mensaje_can[3].address = (encoder_menu[12].valor_new >> 2); // asignación dirección
				rec_eeprom(12);
				encoder_menu[12].valor_old = encoder_menu[12].valor_new >> 2;
				encoder_menu[0].valor_excase = 12;
			}
		} break; // case 12

		case 13: {
			if (encoder_menu[13].valor_old != encoder_menu[13].valor_new >> 2 || encoder_menu[0].valor_excase != 13) {
				if ((encoder_menu[13].valor_new >> 2) > 255) encoder_menu[13].valor_new = 0;
				llenar_source_address();
				sendString("E>", 10, 0);
				mensaje_can[4].address = (encoder_menu[13].valor_new >> 2); // asignación dirección
				rec_eeprom(13);
				encoder_menu[13].valor_old = encoder_menu[13].valor_new >> 2;
				encoder_menu[0].valor_excase = 13;
			}
		} break; // case 13

		case 14: {
			if (encoder_menu[14].valor_old != encoder_menu[14].valor_new >> 2 || encoder_menu[0].valor_excase != 14) {
				if (encoder_menu[0].valor_excase != 14) lcdclear();
				if ((encoder_menu[14].valor_new >> 2) > 255) encoder_menu[14].valor_new = 0;
				llenar_source_address();
				sendString("F>", 10, 1);
				mensaje_can[5].address = (encoder_menu[14].valor_new >> 2); // asignación dirección
				rec_eeprom(14);
				encoder_menu[14].valor_old = encoder_menu[14].valor_new >> 2;
				encoder_menu[0].valor_excase = 14;
			}
		} break; // case 14

		case 15: { // Asignación Prioridades CAN
			if (encoder_menu[15].valor_old != encoder_menu[15].valor_new >> 2 || encoder_menu[0].valor_excase != 15) {
				// if (encoder_menu[0].valor_excase != 15) lcdclear();
				sendString(">Priority(0-7):", 0, 0);              // can A
				sendString(" DestAddress:", 0, 1);
				sendFloat((encoder_menu[16].valor_new >> 2), 3, 0, 13, 1);
				if ((encoder_menu[15].valor_new >> 2) > 7) encoder_menu[15].valor_new = 0;
				sendFloat((encoder_menu[15].valor_new >> 2), 1, 0, 15, 0);

				// Todas las tramas comparte prioridad
				for (uint8_t i = 0; i < 10; i++) mensaje_can[i].priority = (encoder_menu[15].valor_new >> 2);

				rec_eeprom(15);
				encoder_menu[15].valor_old = encoder_menu[15].valor_new >> 2;
				encoder_menu[0].valor_excase = 15;
			}
		} break; // case 15

		case 16: {     // Prioridad y Destination

			if (encoder_menu[16].valor_old != encoder_menu[16].valor_new >> 2 || encoder_menu[0].valor_excase != 16) {
				//if ((encoder_menu[0].valor_excase != 16) & (encoder_menu[0].valor_excase != 10)) lcdclear();
				sendString(" Priority(0-7):", 0, 0);
				sendFloat((encoder_menu[15].valor_new >> 2), 1, 0, 15, 0);
				sendString(">DestAddress:", 0, 1);
				if ((encoder_menu[16].valor_new >> 2) > 255) encoder_menu[16].valor_new = 0;
				sendFloat((encoder_menu[16].valor_new >> 2), 3, 0, 13, 1);
				rec_eeprom(16);
				encoder_menu[16].valor_old = encoder_menu[16].valor_new >> 2;
				encoder_menu[0].valor_excase = 16;

			}
		} break; // case 16

		case 17: {
			if (encoder_menu[17].valor_old != encoder_menu[17].valor_new >> 2 || encoder_menu[0].valor_excase != 17) {
				sendString("Serial :  ", 0, 0);
				sendString("(Set & Push)     ", 0, 1);
				if (encoder_menu[17].valor_new >> 2 == 0) sendString("  300", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 1) sendString("  600", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 2) sendString(" 1200", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 3) sendString(" 2400 ", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 4) sendString(" 4800", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 5) sendString(" 9600 ", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 6) sendString(" 14.4K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 7) sendString(" 19.2K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 8)  sendString(" 57.6K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 9)  sendString("115.2K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 10) sendString("128.0K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 11) sendString("153.6K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 12) sendString("230.4K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 13) sendString("250.0K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 == 14) sendString("256.0K", 10, 0);
				else if (encoder_menu[17].valor_new >> 2 > 14) (encoder_menu[17].valor_new = 14 << 2);
				rec_eeprom(17);
				encoder_menu[17].valor_old = encoder_menu[17].valor_new >> 2;
				encoder_menu[0].valor_excase = 17;
				flag_1 = 0;
			}
		} break; // case 17

		case 18: { // cambio día
				   //if (encoder_menu[0].valor_excase != 18) lcdclear();
			static char temp[16];
			sprintf(temp, "H/M/S %02d:%02d:%02d  ", tiempo.hor, tiempo.min, tiempo.sec);
			sendString(temp, 0, 0);
			if (encoder_menu[18].valor_old != encoder_menu[18].valor_new >> 2 || encoder_menu[0].valor_excase != 18) {
				if (encoder_menu[18].valor_new >> 2 == 32)encoder_menu[18].valor_new = 0;
				sprintf(temp, "D/M/Y>%02d-%02d-%d", encoder_menu[18].valor_new >> 2, encoder_menu[19].valor_new >> 2, encoder_menu[20].valor_new >> 2);
				sendString(temp, 0, 1);
				rec_eeprom(18);
				encoder_menu[18].valor_old = encoder_menu[18].valor_new >> 2;
			}
			encoder_menu[0].valor_excase = 18;
		} break; // case 18

		case 19: { // cambio mes
				   //if (encoder_menu[0].valor_excase != 19) lcdclear();
			static char temp[16];
			sprintf(temp, "H/M/S %02d:%02d:%02d", tiempo.hor, tiempo.min, tiempo.sec);
			sendString(temp, 0, 0);
			if (encoder_menu[19].valor_old != encoder_menu[19].valor_new >> 2 || encoder_menu[0].valor_excase != 19) {
				if (encoder_menu[19].valor_new >> 2 == 13)encoder_menu[19].valor_new = 0;
				sprintf(temp, "D/M/Y %02d>%02d-%d", encoder_menu[18].valor_new >> 2, encoder_menu[19].valor_new >> 2, encoder_menu[20].valor_new >> 2);
				sendString(temp, 0, 1);
				rec_eeprom(19);
				encoder_menu[19].valor_old = encoder_menu[19].valor_new >> 2;
			}
			encoder_menu[0].valor_excase = 19;
		} break; // case 19

		case 20: {
			//if (encoder_menu[0].valor_excase != 20) lcdclear();
			static char temp[16];
			sprintf(temp, "H/M/S %02d:%02d:%02d", tiempo.hor, tiempo.min, tiempo.sec);
			sendString(temp, 0, 0);
			if (encoder_menu[20].valor_old != encoder_menu[20].valor_new >> 2 || encoder_menu[0].valor_excase != 20) {
				if (encoder_menu[20].valor_new >> 2 <= 1985) encoder_menu[20].valor_new = 1985 << 2;
				sprintf(temp, "D/M/Y %02d-%02d>%d", encoder_menu[18].valor_new >> 2, encoder_menu[19].valor_new >> 2, encoder_menu[20].valor_new >> 2);
				sendString(temp, 0, 1);
				rec_eeprom(20);
				encoder_menu[20].valor_old = encoder_menu[20].valor_new >> 2;
			}
			encoder_menu[0].valor_excase = 20;
		} break; // case 20

		case 21: {  //mensaje_can[6]
			if (encoder_menu[0].valor_excase != 21) lcdclear();

			sendString(frames_PGN[Value2 - 15].address_PGN_hex, 0, 0);
			sendString("Trip", 9, 0);
			for (int i = 0; i < 8; i++) {
				char array_prov[3];
				itoa(mensaje_can[Value2 - 15].msg[i], array_prov, 16);
				if (mensaje_can[Value2 - 15].msg[i] > 16) sendString(array_prov, 2 * i, 1);
				else { sendString(array_prov, 2 * i + 1, 1); sendString("0", 2 * i, 1); }
			}
			rec_eeprom(21);
			encoder_menu[0].valor_excase = 21;
		} break; // case 21

		case 22: { // mensaje_can[7]
			if (encoder_menu[0].valor_excase != 22) lcdclear();
			sendString(frames_PGN[Value2 - 15].address_PGN_hex, 0, 0);
			sendString("Time/C", 9, 0);
			for (int i = 0; i < 8; i++) {
				char array_prov[3];
				itoa(mensaje_can[Value2 - 15].msg[i], array_prov, 16);
				if (mensaje_can[Value2 - 15].msg[i] > 16) sendString(array_prov, 2 * i, 1);
				else { sendString(array_prov, 2 * i + 1, 1); sendString("0", 2 * i, 1); }
			}
			rec_eeprom(22);
			encoder_menu[0].valor_excase = 22;
		} break; // case 22

		case 23: {  // can 8
			if (encoder_menu[0].valor_excase != 23) lcdclear();
			sendString(frames_PGN[Value2 - 15].address_PGN_hex, 0, 0);
			sendString("DM01", 9, 0);
			for (int i = 0; i < 8; i++) {
				char array_prov[3];
				itoa(mensaje_can[Value2 - 15].msg[i], array_prov, 16);
				if (mensaje_can[Value2 - 15].msg[i] > 16) sendString(array_prov, 2 * i, 1);
				else { sendString(array_prov, 2 * i + 1, 1); sendString("0", 2 * i, 1); }
			}
			rec_eeprom(23);
			encoder_menu[0].valor_excase = 23;
		} break; // case 23

		case 24: {
			if (encoder_menu[0].valor_excase != 24) lcdclear();
			sendString(frames_PGN[Value2 - 15].address_PGN_hex, 0, 0);
			sendString("AMB", 9, 0);
			for (int i = 0; i < 8; i++) {
				char array_prov[3];
				itoa(mensaje_can[Value2 - 15].msg[i], array_prov, 16);
				if (mensaje_can[Value2 - 15].msg[i] > 16) sendString(array_prov, 2 * i, 1);
				else { sendString(array_prov, 2 * i + 1, 1); sendString("0", 2 * i, 1); }
			}
			rec_eeprom(24);
			encoder_menu[0].valor_excase = 24;
		} break; // case 24

		case 25: { //Recepcción
			if (encoder_menu[0].valor_excase != 25) {
				lcdclear();
				sendString("R", 0, 0);
			}
			rec_eeprom(25);
			encoder_menu[0].valor_excase = 25;
		} break; // case 25

		case 26: {
			if (encoder_menu[26].valor_old != encoder_menu[26].valor_new >> 2 || encoder_menu[0].valor_excase != 26) {
				if ((encoder_menu[0].valor_excase != 26) & (1)) lcdclear();
				sendString(">DM1 SPN:", 0, 0);
				sendFloat((encoder_menu[26].valor_new >> 2), 3, 0, 10, 0);
				sendFloat((encoder_menu[27].valor_new >> 2), 3, 0, 11, 1);
				sendString(" DM1 FMI:", 0, 1);

				rec_eeprom(26);
				encoder_menu[26].valor_old = encoder_menu[26].valor_new >> 2;
				encoder_menu[0].valor_excase = 26;
			}
		} break; // case 26

		case 27: {
			if (encoder_menu[27].valor_old != encoder_menu[27].valor_new >> 2 || encoder_menu[0].valor_excase != 27) {
				if ((encoder_menu[0].valor_excase != 27) & (1)) lcdclear();
				if ((encoder_menu[27].valor_new >> 2) >31) encoder_menu[27].valor_new = 0;
				sendString(" DM1 SPN:", 0, 0);
				sendFloat((encoder_menu[27].valor_new >> 2), 3, 0, 11, 1);
				sendFloat((encoder_menu[26].valor_new >> 2), 3, 0, 10, 0);
				sendString(">DM1 FMI:", 0, 1);
				rec_eeprom(27);
				encoder_menu[27].valor_old = encoder_menu[27].valor_new >> 2;
				encoder_menu[0].valor_excase = 27;
			}
		} break; // case 27

		case 28: {
			if (encoder_menu[28].valor_old != encoder_menu[28].valor_new >> 2 || encoder_menu[0].valor_excase != 28) {
				if ((encoder_menu[0].valor_excase != 28) & (1)) lcdclear();
				if ((encoder_menu[28].valor_new >> 2) > 127) encoder_menu[28].valor_new = 0;
				sendString(">DM1 OC10:", 0, 0);
				sendFloat((encoder_menu[28].valor_new >> 2), 3, 0, 11, 0);
				sendFloat((encoder_menu[29].valor_new >> 2), 3, 0, 11, 1);
				sendString(" DM1 CM:", 0, 1);
				rec_eeprom(28);
				encoder_menu[28].valor_old = encoder_menu[28].valor_new >> 2;
				encoder_menu[0].valor_excase = 28;
			}
		} break; // case 28

		case 29: {
			if (encoder_menu[29].valor_old != encoder_menu[29].valor_new >> 2 || encoder_menu[0].valor_excase != 29) {
				if ((encoder_menu[0].valor_excase != 29) & (1)) lcdclear();
				if ((encoder_menu[29].valor_new >> 2) > 1) encoder_menu[29].valor_new = 0;
				sendString(" DM1 OC10:", 0, 0);
				sendFloat((encoder_menu[29].valor_new >> 2), 3, 0, 11, 1);
				sendFloat((encoder_menu[28].valor_new >> 2), 3, 0, 11, 0);
				sendString(">DM1 CM:", 0, 1);
				rec_eeprom(29);
				encoder_menu[29].valor_old = encoder_menu[29].valor_new >> 2;
				encoder_menu[0].valor_excase = 29;
			}
		} break; // case 29

		case 30: {
			if (encoder_menu[30].valor_old != encoder_menu[30].valor_new >> 2 || encoder_menu[0].valor_excase != 30) {
				// if ((encoder_menu[0].valor_excase != 30) & (1)) lcdclear();
				if ((encoder_menu[30].valor_new >> 2) > 3) encoder_menu[30].valor_new = 0;
				sendString(">MILS:0 ", 0, 0);
				sendString(" RSLS:0 ", 0, 1);
				sendString(" AWLS:0 ", 8, 0);
				sendString("  PLS:0 ", 8, 1);
				DTC(30);
			}
		} break; // case 30

		case 31: {
			if (encoder_menu[31].valor_old != encoder_menu[31].valor_new >> 2 || encoder_menu[0].valor_excase != 31) {
				//if ((encoder_menu[0].valor_excase != 31) & (1)) lcdclear();
				if ((encoder_menu[31].valor_new >> 2) > 3) encoder_menu[31].valor_new = 0;
				sendString(" MILS:0 ", 0, 0);
				sendString(">RSLS:0 ", 0, 1);
				sendString(" AWLS:0 ", 8, 0);
				sendString("  PLS:0 ", 8, 1);
				DTC(31);
			}
		} break; // case 31

		case 32: {
			if (encoder_menu[32].valor_old != encoder_menu[32].valor_new >> 2 || encoder_menu[0].valor_excase != 32) {
				//if ((encoder_menu[0].valor_excase != 32) & (1)) lcdclear();
				if ((encoder_menu[32].valor_new >> 2) > 3) encoder_menu[32].valor_new = 0;
				sendString(" MILS:0 ", 0, 0);
				sendString(" RSLS:0 ", 0, 1);
				sendString(">AWLS:0 ", 8, 0);
				sendString("  PLS:0 ", 8, 1);
				DTC(32);
			}
		} break; // case 32

		case 33: {
			if (encoder_menu[33].valor_old != encoder_menu[33].valor_new >> 2 || encoder_menu[0].valor_excase != 33) {
				// if ((encoder_menu[0].valor_excase != 33) & (1)) lcdclear();
				if ((encoder_menu[33].valor_new >> 2) > 3) encoder_menu[33].valor_new = 0;
				sendString(" MILS:0 ", 0, 0);
				sendString(" RSLS:0 ", 0, 1);
				sendString(" AWLS:0 ", 8, 0);
				sendString("> PLS:0 ", 8, 1);
				DTC(33);
			}
		} break; // case 33

		case 34: {  // PGN variable
			if (encoder_menu[34].valor_old != encoder_menu[34].valor_new >> 2 || encoder_menu[0].valor_excase != 34) {
				if ((encoder_menu[0].valor_excase != 34) & (encoder_menu[0].valor_excase != 34)) lcdclear();
				char temp1[sizeof(long)];
				sendString("VAR-ID:", 0, 0);
				long lMessageID_local = ((long)mensaje_can[9].priority << 26) + ((long)mensaje_can[9].PGN << 8) + ((long)mensaje_can[9].address);
				ltoa(lMessageID_local, temp1, 16);
				sendString(temp1, 8, 0);

				for (uint8_t i = 0; i < 8; i++) {
					char array_prov[3];
					itoa(mensaje_can[9].msg[i], array_prov, 16);
					if (mensaje_can[9].msg[i] > 16) sendString(array_prov, 2 * i, 1);
					else { sendString(array_prov, 2 * i + 1, 1); sendString("0", 2 * i, 1); }
				}

				rec_eeprom(34);
				encoder_menu[34].valor_old = encoder_menu[34].valor_new >> 2;
				encoder_menu[0].valor_excase = 34;
			}
		} break; // case 34

		case 35: {     // PGN
			if (encoder_menu[35].valor_old != encoder_menu[35].valor_new >> 2 || encoder_menu[0].valor_excase != 35) {
				if ((encoder_menu[0].valor_excase != 35) & (encoder_menu[0].valor_excase != 35)) lcdclear();
				// tope en PGN=65535
				if ((encoder_menu[35].valor_new >> 2) >= (267 + 8192)) encoder_menu[35].valor_new = (8192 + 266) << 2;
				ver_PGN(3, 0, 35);
			}
		} break; // case 35

		case 36: {     // D
			if (encoder_menu[36].valor_old != encoder_menu[36].valor_new >> 2 || encoder_menu[0].valor_excase != 36) {
				ver_PGN(11, 0, 36);
			}
		} break; // case 36

		case 37: {     // P
			if (encoder_menu[37].valor_old != encoder_menu[37].valor_new >> 2 || encoder_menu[0].valor_excase != 37) {
				ver_PGN(1, 1, 37);
			}
		} break; // case 37

		case 38: {     // T
			if (encoder_menu[38].valor_old != encoder_menu[38].valor_new >> 2 || encoder_menu[0].valor_excase != 38) {
				ver_PGN(5, 1, 38);
			}
		} break; // case 38

		case 39: { // Y/N
			if (encoder_menu[39].valor_old != encoder_menu[39].valor_new >> 2 || encoder_menu[0].valor_excase != 39) {
				if ((encoder_menu[0].valor_excase != 39) & (encoder_menu[0].valor_excase != 39)) lcdclear();
				//   if (encoder_menu[39].valor_new >>2) vTaskResume(xHandle[9]); else vTaskSuspend(xHandle[9]);
				ver_PGN(13, 1, 39);
			}
		} break; // case 39


		case 40: {
			if (encoder_menu[40].valor_old != encoder_menu[40].valor_new >> 2 || encoder_menu[0].valor_excase != 40) {
				if ((encoder_menu[0].valor_excase != 40) & (encoder_menu[0].valor_excase != 40)) sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 40

		case 41: {
			if (encoder_menu[41].valor_old != encoder_menu[41].valor_new >> 2 || encoder_menu[0].valor_excase != 41) {
				if ((encoder_menu[0].valor_excase != 41) & (encoder_menu[0].valor_excase != 41)) sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 41

		case 42: {
			if (encoder_menu[42].valor_old != encoder_menu[42].valor_new >> 2 || encoder_menu[0].valor_excase != 42) {
				if ((encoder_menu[0].valor_excase != 42) & (encoder_menu[0].valor_excase != 42)) sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 42

		case 43: {
			if (encoder_menu[43].valor_old != encoder_menu[43].valor_new >> 2 || encoder_menu[0].valor_excase != 43) {
				if ((encoder_menu[0].valor_excase != 43) & (encoder_menu[0].valor_excase != 43)) sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 43

		case 44: {
			if (encoder_menu[44].valor_old != encoder_menu[44].valor_new >> 2 || encoder_menu[0].valor_excase != 44) {
				if ((encoder_menu[0].valor_excase != 44) & (encoder_menu[0].valor_excase != 44)) sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 44

		case 45: {
			if (encoder_menu[45].valor_old != encoder_menu[45].valor_new >> 2 || encoder_menu[0].valor_excase != 45) {
				if ((encoder_menu[0].valor_excase != 45) & (encoder_menu[0].valor_excase != 45)) sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 45

		case 46: {
			if (encoder_menu[46].valor_old != encoder_menu[46].valor_new >> 2 || encoder_menu[0].valor_excase != 46) {
				if ((encoder_menu[0].valor_excase != 46) & (encoder_menu[0].valor_excase != 46)) sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 46

		case 47: {
			if (encoder_menu[47].valor_old != encoder_menu[47].valor_new >> 2 || encoder_menu[0].valor_excase != 47) {
				if ((encoder_menu[0].valor_excase != 47) & (encoder_menu[0].valor_excase != 47)) sendString("                ", 0, 0);
				valores_msg(Value2);
			}
		} break; // case 47

		case 48: {
			if (encoder_menu[48].valor_old != encoder_menu[48].valor_new >> 2 || encoder_menu[0].valor_excase != 48) {
				if ((encoder_menu[0].valor_excase != 48) & (encoder_menu[0].valor_excase != 48)) lcdclear();
				if ((encoder_menu[48].valor_new >> 2) > 1) encoder_menu[48].valor_new = 1 << 2;

				sendString(" Contrast: ", 0, 1);
				sendFloat((encoder_menu[49].valor_new >> 2), 3, 0, 10, 1);  // Contraste

				if ((encoder_menu[48].valor_new >> 2) == 1) sendString(">Banners: YES ", 0, 0);     // can A
				else sendString(">Banners: NO ", 0, 0);

				rec_eeprom(48);
				encoder_menu[48].valor_old = encoder_menu[48].valor_new >> 2;
				encoder_menu[0].valor_excase = 48;
			}
		} break; // case 48
		case 49: {
			if (encoder_menu[49].valor_old != encoder_menu[49].valor_new >> 2 || encoder_menu[0].valor_excase != 49) {
				if ((encoder_menu[0].valor_excase != 49) & (encoder_menu[0].valor_excase != 49)) lcdclear();
				if ((encoder_menu[49].valor_new >> 2) > 0xFF) encoder_menu[49].valor_new = 1 << 2;

				if ((encoder_menu[48].valor_new >> 2) == 1) sendString(" Banners: YES ", 0, 0);     // can A
				else sendString(" Banners: NO ", 0, 0);

				//encoder_menu[49].valor_new += 4;

				sendString(">Contrast: ", 0, 1);
				sendFloat((encoder_menu[49].valor_new >> 2), 3, 0, 10, 1);  // Contraste
																			//                                                           LCD.setContrast((encoder_menu[49].valor_new >> 2));

				rec_eeprom(49);
				encoder_menu[49].valor_old = encoder_menu[49].valor_new >> 2;
				encoder_menu[0].valor_excase = 49;
			}
		} break; // case 49

		} // FIN SWITCH

#define valor_max_byte 0xFA
#define valor_max_word 0xFAFF
#define valor_max_dword 0xFAFFFFFF

		  // CORE: actualización tramas CAN
		for (uint8_t k = 0; k < 6; k++) {
			uint8_t SPN_length = encoder_menu[k].SPN_length_int;
			if (SPN_length != 0) {  // sólo si se inicializa pulsando Der.
				uint32_t p = pow(2, SPN_length);
				uint8_t atoip = encoder_menu[k].posicion_int;
				uint8_t atoip2 = encoder_menu[k].posicion2_int;
				if (SPN_length == 32)
				{
					uint32_t muestra = map(analogRead(k + 54), 0, 1023, 0, valor_max_dword);
					mensaje_can[k].msg[atoip - 1] = (uint8_t)(muestra & 0xFF);
					mensaje_can[k].msg[atoip] = (uint8_t)((muestra & 0xFF00) >> 8);
					mensaje_can[k].msg[atoip + 1] = (uint8_t)((muestra & 0xFF0000) >> 16);
					mensaje_can[k].msg[atoip + 2] = (uint8_t)((muestra & 0xFF000000) >> 24);
				}
				else if (SPN_length == 16)
				{
					uint16_t muestra = map(analogRead(k + 54), 0, 1023, 0, valor_max_word);
					mensaje_can[k].msg[atoip - 1] = (uint8_t)(muestra & 0x00FF);
					mensaje_can[k].msg[atoip] = (uint8_t)(muestra >> 8);
				}
				else {
					uint32_t muestra = map(analogRead(k + 54), 0, 1023, 0, (SPN_length == 8) * valor_max_byte + (SPN_length == 2) * 3 + (SPN_length == 4) * 15 + (SPN_length == 1) * 1);
					mensaje_can[k].msg[atoip - 1] = (mensaje_can[Value2 - 1].msg[atoip - 1] & (0xFF - (p << (atoip2 - 1)))) | muestra << (atoip2 - 1);
				}
			}//if
		}// for

		 // boton DER.
		if (!digitalRead(encoderSwitchPin1)) {

			if (Value2 == 16) encoder_menu[16].valor_new = 255 << 2; // carga 0xFF en Destino
			if (Value2 == 49) encoder_menu[49].valor_new = 0xFF << 2; // carga 0xFF en Destino

																	  // PGN configurable
			if (Value2 == 35) encoder_menu[35].valor_new = 8192 << 2;      // PGN
			else if (Value2 == 36) encoder_menu[36].valor_new = 0 << 2;    // Source address
			else if (Value2 == 37) encoder_menu[37].valor_new = 6 << 2;    // prioridad
			else if (Value2 == 38) encoder_menu[38].valor_new = 4 << 2;    // 250ms

			if ((Value2 == 7) | (Value2 == 17)) pinMode(PINtoRESET, OUTPUT); // reset cambio serial or CAN bps

			if (Value2 == 0) encoderValue2 = 25 << 2; // recepcción si botón derecho en arranque

			uint16_t mPGN = atoi(PGN); // no asigna si ya existe excepto en el propio
			uint8_t mPGN_bool;
			if (((mensaje_can[0].PGN != mPGN)
				&  (mensaje_can[1].PGN != mPGN)
				&  (mensaje_can[2].PGN != mPGN)
				&  (mensaje_can[3].PGN != mPGN)
				&  (mensaje_can[4].PGN != mPGN)
				&  (mensaje_can[5].PGN != mPGN)) | (mensaje_can[Value2 - 1].PGN == mPGN)) mPGN_bool = 1; else mPGN_bool = 0;// existe? o sea mPGN_bool = 0

			char ch[sizeof("A")]; // dónde está siendo usado?
			if (mensaje_can[0].PGN == mPGN) strcpy(ch, "A");
			else if (mensaje_can[1].PGN == mPGN) strcpy(ch, "B");
			else if (mensaje_can[2].PGN == mPGN) strcpy(ch, "C");
			else if (mensaje_can[3].PGN == mPGN) strcpy(ch, "D");
			else if (mensaje_can[4].PGN == mPGN) strcpy(ch, "E");
			else if (mensaje_can[5].PGN == mPGN) strcpy(ch, "F");

			if (mPGN_bool) encoder_menu[Value2].frame_activo = encoder_menu[Value2].valor_new >> 2;

			if ((Value2 <= 6) & (encoder_menu[Value2].valor_new >> 2 > 0)) { // Montar nuevas tramas

				if (mPGN_bool)
				{
					mensaje_can[Value2 - 1].PGN = atoi(PGN);

					// Borrar msg´s a 0xFF si PGN es nuevo
					if (mensaje_can[Value2 - 1].PGN != PGN_anterior)
					{
						for (uint8_t i = 0; i < 8; i++) mensaje_can[Value2 - 1].msg[i] = 0xFF;
						PGN_anterior = mensaje_can[Value2 - 1].PGN;
					}

					mensaje_can[Value2 - 1].periodo = atoi(rate); // periodo ms
					encoder_menu[Value2 - 1].posicion_int = atoi(position);
					encoder_menu[Value2 - 1].posicion2_int = atoi(position2);
					encoder_menu[Value2 - 1].SPN_length_int = atoi(SPN_length);

					vTaskResume(xHandle[Value2 - 1]); //activa trama
					delay(1000);

					if ((encoder_menu[48].valor_new >> 2) == 1) scrollString(buffer, 0, 200); // Banner
				}
				else { lcdclear(); sendString("PGN USED IN", 0, 0); sendString(ch, 12, 0); delay(1000); }
			}//if

			if ((Value2 <= 6) & (Value2 > 0) & (encoder_menu[Value2].valor_new >> 2 == 0)) vTaskSuspend(xHandle[Value2 - 1]); // borrar trama

			encoder_menu[Value2].valor_old = 99;
		}

		//  botón IZQ.

		if ((!digitalRead(encoderSwitchPin2) & (Value2 < 7) & (Value2 > 0)) || (flag_1 == 1)) {

			if (flag_1 == 0) lcdclear();
			encoder_menu[8].valor_new = 2 << 2;
			flag_1 = 1;
			sendString("CAN-ID:", 0, 0);
			sendString(frames_PGN[Value2 - 1].address_PGN_hex, 8, 0);
			for (int i = 0; i < 8; i++) {
				char array_prov[3];
				itoa(mensaje_can[Value2 - 1].msg[i], array_prov, 16);
				if (mensaje_can[Value2 - 1].msg[i] > 16) sendString(array_prov, 2 * i, 1);
				else { sendString(array_prov, 2 * i + 1, 1); sendString("0", 2 * i, 1); }
			}
		}

		// Encendido led rojo -> activación DTCs
		if ((!digitalRead(encoderSwitchPin1) & ((Value2 >= 26) & (Value2 <= 33))) | lock) {
			lock = 1;
			//vTaskResume(xHandle[8]); //activa trama DM01
			digitalWrite(pin_led_rojo, LOW);
			((encoder_menu[30].valor_new >> 2) == 1) ? digitalWrite(A8, LOW) : digitalWrite(A8, HIGH);
			((encoder_menu[31].valor_new >> 2) == 1) ? digitalWrite(A9, LOW) : digitalWrite(A9, HIGH);
			((encoder_menu[32].valor_new >> 2) == 1) ? digitalWrite(A10, LOW) : digitalWrite(A10, HIGH);
			((encoder_menu[33].valor_new >> 2) == 1) ? digitalWrite(A11, LOW) : digitalWrite(A11, HIGH);
			mensaje_can[8].msg[0] = (encoder_menu[33].valor_new >> 2) + (encoder_menu[32].valor_new) + (encoder_menu[31].valor_new << 2) + (encoder_menu[30].valor_new << 4); //MIL LED on
			mensaje_can[8].msg[1] = 0xFF;                                             //Flash
			mensaje_can[8].msg[2] = (uint8_t)(encoder_menu[26].valor_new >> 2);       //SPN
			mensaje_can[8].msg[3] = (uint8_t)(encoder_menu[26].valor_new >> 10);      //SPN
			mensaje_can[8].msg[4] = (uint8_t)(encoder_menu[27].valor_new >> 2);       //FMI
			mensaje_can[8].msg[5] = (uint8_t)(encoder_menu[28].valor_new >> 2);       //OC
			mensaje_can[8].msg[5] |= (encoder_menu[29].valor_new << 5);               //CM
			mensaje_can[8].msg[6] = 0xFF;
			mensaje_can[8].msg[7] = 0xFF;
		}

		if (!digitalRead(encoderSwitchPin2) & (Value2 == 35)) encoder_menu[35].valor_new = 8203 << 2;      // PGN

																										   // Apagado led rojo -> desactivación DTCs
		if (!digitalRead(encoderSwitchPin2) & ((Value2 >= 26) & (Value2 <= 33))) {
			digitalWrite(pin_led_rojo, HIGH);
			digitalWrite(A8, HIGH); digitalWrite(A9, HIGH); digitalWrite(A10, HIGH); digitalWrite(A11, HIGH);
			//vTaskSuspend(xHandle[8]); // Apago trama DM01
			mensaje_can[8] = { 65226,1000,0,6,{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } }; // DM01
			lock = 0;
		}

		// We want this task to execute exactly every period milliseconds
		vTaskDelayUntil(&xLastWakeTime2, (50 / portTICK_PERIOD_MS));
	}
}

static void vPeriodicTask2(void *pvParameters)
{
	TickType_t xLastWakeTime2 = xTaskGetTickCount();  // en ms

	uint8_t *xmsg;

	// As per most tasks, this task is implemented in an infinite loop.
	for (;; )
	{

		if (alive_led) {
			// Alive
			digitalWrite(pin_led_verde, LOW);
			vTaskDelay(25 / portTICK_PERIOD_MS);
			digitalWrite(pin_led_verde, HIGH);
		}

		// Lectura cola CAN RX
		struct AMessage *pxRxedMessage;
		if (xQueue1 != 0)
		{
			// Receive a message on the created queue.  Block for 10 ticks if a message is not immediately available
			if (xQueueReceive(xQueue1, &(pxRxedMessage), (TickType_t)10))
			{
				// pcRxedMessage now points to the struct AMessage variable posted by vATask
				// Analyze the received PGN

				if (!alive_led) {
					digitalWrite(pin_led_verde, LOW);
				}

				if (pxRxedMessage->lPGN == 0xEA00) // peer-to-peer
				{
					switch (pxRxedMessage->nData[0] + (uint16_t)(pxRxedMessage->nData[1] << 8)) // en función del PGN recibido
					{
					case (msgEngineTemperature_PGN): {xmsg = msgEngineTemperature; } break;
					case (Engine_Total_Average_Fuel_Rate_PGN): {xmsg = Engine_Total_Average_Fuel_Rate; } break;
					case (Specific_Heat_Ratio_PGN): {xmsg = Specific_Heat_Ratio; } break;
					case (Engine_Cylinder_1_Combustion_Time_PGN): {xmsg = Engine_Cylinder_1_Combustion_Time; } break;
					case (Engine_Fuel_Flow_Rate_PGN): {xmsg = Engine_Fuel_Flow_Rate; } break;
					case (Engine_Cylinder_1_Ignition_Timing_PGN): {xmsg = Engine_Cylinder_1_Ignition_Timing; } break;
					case (Engine_Power_PGN): {xmsg = Engine_Power; } break;
					case (Engine_Actual_Exhaust_Oxygen_PGN): {xmsg = Engine_Actual_Exhaust_Oxygen; } break;
					case (Trip_Average_Fuel_Rate_PGN): {xmsg = Trip_Average_Fuel_Rate; } break;
					case (Trip_Cruise_Distance_PGN): {xmsg = Trip_Cruise_Distance; } break;
					case (Engine_Total_Hours_of_Operation_PGN): {xmsg = Engine_Total_Hours_of_Operation; } break;
					case (Engine_Trip_Fuel_PGN): {xmsg = Engine_Trip_Fuel; } break;
					} // fin switch

					if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE)
					{
						noInterrupts();
						j1939Transmit(pxRxedMessage->nData[0] + (uint16_t)(pxRxedMessage->nData[1] << 8) + (uint16_t)(pxRxedMessage->nData[2] << 16), pxRxedMessage->nPriority, encoder_menu[16].valor_new >> 2, pxRxedMessage->nSrcAddr, (char*)xmsg, 8);  // Transmit the message
						interrupts();
						xSemaphoreGive(xSemaphore);
					}

				} // fin peer-to-peer

				if ((encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 8)) {
					Serial.print("F:");
					Serial.print(pxRxedMessage->cuenta);
					Serial.print(" ");
				}

				if ((encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 7)) {
					Serial.print("F:");
					Serial2.print(pxRxedMessage->cuenta);
					Serial2.print(" ");
				}

				sprintf(sString, "PGN:%05u 0x%X P:%X Src:0x%X Dest:0x%X ", (int)pxRxedMessage->lPGN, (int)pxRxedMessage->lPGN, pxRxedMessage->nPriority, pxRxedMessage->nSrcAddr, pxRxedMessage->nDestAddr);
				if ((encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 7)) Serial2.print(sString);
				if ((encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 8)) Serial.print(sString);

				// Recepcíón
				// mostrar en LCD si menu=25
				if (Value2 == 25) {
					if (IsOdd(tiempo.sec) == true) sendString("R ", 0, 0); else sendString("  ", 0, 0); // parpadeo

					long lMessageID2 = ((long)pxRxedMessage->nPriority << 26) + ((long)pxRxedMessage->lPGN << 8) + (long)pxRxedMessage->nSrcAddr;
					ltoa(lMessageID2, sString, 16);
					sendString(sString, 2, 0);

					sendFloat(pxRxedMessage->time, 4, 0, 11, 0); // periodo

					if ((pxRxedMessage->time) < 1000) sendString(" ", 15, 0);

					// Evitar restos de datos anteriores
					for (int p = 0; p < 2 * (8 - pxRxedMessage->nDataLen); p++)
						sendString(" ", 16 - p, 1);
				}

				if (xMessage.nDataLen == 0) Serial.print("No Data.\n\r");
				else
				{
					if ((encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 8)) Serial.print("Data: ");
					if ((encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 7)) Serial2.print("Data: ");

					for (int nIndex = 0; nIndex < pxRxedMessage->nDataLen; nIndex++)
					{
						sprintf(sString, "0x%X ", pxRxedMessage->nData[nIndex]);
						if ((encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 8)) Serial.print(sString);
						if ((encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 7))  Serial2.print(sString);

						// mostrar en LCD si menu=25
						if (Value2 == 25) {
							sprintf(sString, "%X ", pxRxedMessage->nData[nIndex]);
							if (pxRxedMessage->nData[nIndex] > 16) sendString(sString, 2 * nIndex, 1);
							else { sendString(sString, 2 * nIndex + 1, 1); sendString("0", 2 * nIndex, 1); }
						}

					}// end for

					if ((encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 8)) {
						Serial.print("T:");
						Serial.print(time_old);
						Serial.print("\n\r");
					}

					if ((encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 4) | (encoder_menu[8].valor_new >> 2 == 7)) {
						Serial2.print("T:");
						Serial2.print(time_old);
						Serial2.print("\n\r");
					}

				}// end else
			}

			if (!alive_led) {
				digitalWrite(pin_led_verde, HIGH);
			}

		}
		// We want this task to execute exactly every 100 milliseconds
		vTaskDelayUntil(&xLastWakeTime2, (100 / portTICK_PERIOD_MS));
	}
}

void vPeriodicTask3(struct mystruct* can_tx) // Creación tramas CAN
{
	TickType_t xLastWakeTime = xTaskGetTickCount();

	// As per most tasks, this task is implemented in an infinite loop/
	for (;; )
	{
		// Ponderación del periodo
		if (can_tx->periodo == 50) can_tx->periodo = 49;
		else if (can_tx->periodo == 100) can_tx->periodo = 98;
		else if (can_tx->periodo == 200) can_tx->periodo = 196;
		else if (can_tx->periodo == 250) can_tx->periodo = 244;
		else if (can_tx->periodo == 500) can_tx->periodo = 488;
		else if (can_tx->periodo == 1000) can_tx->periodo = 977;
		else if (can_tx->periodo == 5000) can_tx->periodo = 4883;
		else if (can_tx->periodo == 10000) can_tx->periodo = 9766;

		if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE)
		{
			noInterrupts();
			j1939Transmit(can_tx->PGN, can_tx->priority, can_tx->address, (encoder_menu[16].valor_new >> 2), can_tx->msg, 8);  // Transmit the message
			interrupts();
			xSemaphoreGive(xSemaphore);
		}

		// We want this task to execute exactly every period milliseconds
		vTaskDelayUntil(&xLastWakeTime, (can_tx->periodo / portTICK_PERIOD_MS));
	}
}

// Serial TX
void vPeriodicTask4(void *pvParameters)
{
	TickType_t xLastWakeTime = xTaskGetTickCount();

	// As per most tasks, this task is implemented in an infinite loop/
	for (;;)
	{
		if ((encoder_menu[8].valor_new >> 2 == 1) | (encoder_menu[8].valor_new >> 2 == 3) | (encoder_menu[8].valor_new >> 2 == 6)) {
			for (int u = 0; u < 10; u++) {
				Serial.print("PGN:");
				if (mensaje_can[u].PGN<1000) Serial.print("  ");
				else if (mensaje_can[u].PGN<10000) Serial.print(" ");
				Serial.print(mensaje_can[u].PGN); Serial.print(" ID:");
				lMessageID = ((long)mensaje_can[u].priority << 26) + ((long)mensaje_can[u].PGN << 8) + ((long)mensaje_can[u].address);
				ltoa(lMessageID, frames_PGN[u].address_PGN_hex, 16);
				Serial.print(lMessageID, HEX); Serial.print(" Data: ");
				for (int i = 0; i < 8; i++) {
					Serial.print("0x"); if (mensaje_can[u].msg[i] < 17) Serial.print("0");
					Serial.print(mensaje_can[u].msg[i], HEX); if (i != 7) Serial.print(",");
				}Serial.println();
			}//for
			Serial.print("Frame: "); Serial.print(count);
			if (!(encoder_menu[8].valor_new >> 2 == 1) | (encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 5)) count++;
			Serial.print(" Time(ms):");
			Serial.println(xTaskGetTickCount()); Serial.println("");
		} // fin if

		if ((encoder_menu[8].valor_new >> 2 == 1) | (encoder_menu[8].valor_new >> 2 == 2) | (encoder_menu[8].valor_new >> 2 == 5)) {
			for (int u = 0; u < 10; u++) {
				Serial2.print("PGN:");
				if (mensaje_can[u].PGN<1000) Serial2.print("  ");
				else if (mensaje_can[u].PGN<10000) Serial2.print(" ");
				Serial2.print(mensaje_can[u].PGN); Serial2.print(" ID:");
				lMessageID = ((long)mensaje_can[u].priority << 26) + ((long)mensaje_can[u].PGN << 8) + ((long)mensaje_can[u].address);
				ltoa(lMessageID, frames_PGN[u].address_PGN_hex, 16);
				Serial2.print(lMessageID, HEX); Serial2.print(" Data: ");
				for (int i = 0; i < 8; i++) {
					Serial2.print("0x"); if (mensaje_can[u].msg[i] < 17) Serial2.print("0");
					Serial2.print(mensaje_can[u].msg[i], HEX); if (i != 7) Serial2.print(",");
				}Serial2.println();
			}Serial2.print("Frame: "); Serial2.print(count); count++; Serial2.print(" Time(ms):");
			Serial2.println(xTaskGetTickCount()); Serial2.println("");
		} // fin if

		tiempo.sec++; if (tiempo.sec == 60) { tiempo.sec = 0; tiempo.min++; }
		if (tiempo.min == 60) { tiempo.min = 0; tiempo.hor++; }

		// Paso día
		if (tiempo.hor == 24) {
			tiempo.hor = 0;  encoder_menu[18].valor_new += 4;
		}

		// Paso mes
		switch (encoder_menu[19].valor_new >> 2)
		{
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			if ((encoder_menu[18].valor_new >> 2) == 32) {
				encoder_menu[18].valor_new = 0;
				encoder_menu[19].valor_new += 4;
			}
			break;
		case 4: case 6: case 9: case 11:
			if ((encoder_menu[18].valor_new >> 2) == 31) {
				encoder_menu[18].valor_new = 0;
				encoder_menu[19].valor_new += 4;
			}
			break;
		case 2:
			if ((encoder_menu[20].valor_new >> 2) % 4 == 0)
			{
				if ((encoder_menu[18].valor_new >> 2) == 29) {
					encoder_menu[18].valor_new = 0;
					encoder_menu[19].valor_new += 4;
				}
				else
					if ((encoder_menu[18].valor_new >> 2) == 30) {
						encoder_menu[18].valor_new = 0;
						encoder_menu[19].valor_new += 4;
					}
			}
			break;
		}

		// paso año
		if ((encoder_menu[19].valor_new >> 2) == 13) {
			encoder_menu[19].valor_new = 1 << 2;
			encoder_menu[20].valor_new += 4;
		}

		// calendario reloj en tramas can
		mensaje_can[7].msg[0] = 4 * tiempo.sec;
		mensaje_can[7].msg[1] = tiempo.min;
		mensaje_can[7].msg[2] = tiempo.hor;
		mensaje_can[7].msg[3] = encoder_menu[19].valor_new >> 2;
		mensaje_can[7].msg[4] = 4 * (encoder_menu[18].valor_new >> 2);
		mensaje_can[7].msg[5] = (encoder_menu[20].valor_new >> 2) - 193;

		// Trip distance
		for (byte i = 0; i < 4; i++) mensaje_can[6].msg[i] = (uint8_t)(kilometros & 0x00FF << i * 8) >> 8 * i;
		kilometros++;

		// We want this task to execute exactly every period milliseconds
		vTaskDelayUntil(&xLastWakeTime, (1000 / portTICK_PERIOD_MS));
	}
}
//------------------------------------------------------------------------------

// Interrupción encoder 1
void loop() {}

void recibir_can() {

	if (xQueue1 != NULL)
	{
		xMessage.time = millis() - time_old;
		time_old = millis();
		xMessage.cuenta++;

		j1939Receive(&xMessage.lPGN, &xMessage.nPriority, &xMessage.nSrcAddr, &xMessage.nDestAddr, xMessage.nData, &xMessage.nDataLen);

		// Send a pointer to a struct AMessage object.  Don't block if the queue is already full
		pxMessage = &xMessage;
		xQueueSend(xQueue1, (void *)&pxMessage, (TickType_t)0);
	}
}

void updateEncoder1() { //derecho

	static uint8_t MSB, LSB, encoded, sum;

	MSB = digitalRead(5); //MSB = most significant bit
	LSB = digitalRead(6); //LSB = least significant bit
	encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
	sum = (lastEncoded1 << 2) | encoded; //adding it to the previous encoded value

	if ((sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) && encoder_menu[Value2].valor_new >= 1) encoder_menu[Value2].valor_new--;
	else if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoder_menu[Value2].valor_new++;

	lastEncoded1 = encoded; //store this value for next time
}

// Interrupción encoder 2
void updateEncoder2() {

	static uint8_t MSB, LSB, encoded, sum;

	MSB = digitalRead(encoderPin3); //MSB = most significant bit
	LSB = digitalRead(encoderPin4); //LSB = least significant bit
	encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
	sum = (lastEncoded2 << 2) | encoded; //adding it to the previous encoded value

	if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue2++;
	else if ((sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) && 1) encoderValue2--;

	if (encoderValue2 < 0)  encoderValue2 = (cases << 2); // revolver en menús
	else if (encoderValue2 >(cases << 2))  encoderValue2 = 0;

	lastEncoded2 = encoded; //store this value for next time
}
*/