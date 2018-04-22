// Tonokip RepRap firmware rewrite based off of Hydra-mmm firmware.
// License: GPL

#ifndef MARLIN_H
#define MARLIN_H

#define  FORCE_INLINE __attribute__((always_inline)) inline

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>


#include "fastio.h"
#include "Configuration.h"
#include "pins.h"

#define  HardwareSerial_h // trick to disable the standard HWserial

#if (ARDUINO >= 100)
# include "Arduino.h"
#else
# include "WProgram.h"
#endif

// Arduino < 1.0.0 does not define this, so we need to do it ourselves
#ifndef analogInputToDigitalPin
# define analogInputToDigitalPin(p) ((p) + 0xA0)
#endif

#include "MarlinSerial.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include "WString.h"

#define MYSERIAL MSerial

#define SERIAL_PROTOCOL(x) (MYSERIAL.print(x))
#define SERIAL_PROTOCOL_F(x,y) (MYSERIAL.print(x,y))
#define SERIAL_PROTOCOLPGM(x) (serialprintPGM(PSTR(x)))
#define SERIAL_PROTOCOLLN(x) (MYSERIAL.print(x),MYSERIAL.write('\n'))
#define SERIAL_PROTOCOLLNPGM(x) (serialprintPGM(PSTR(x)),MYSERIAL.write('\n'))

#define SERIAL_CHAR(x) ((void)MYSERIAL.write(x))

extern const char errormagic[] PROGMEM;
extern const char echomagic[] PROGMEM;

#define SERIAL_ERROR_START (serialprintPGM(errormagic))
#define SERIAL_ERROR(x) SERIAL_PROTOCOL(x)
#define SERIAL_ERRORPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ERRORLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ERRORLNPGM(x) SERIAL_PROTOCOLLNPGM(x)

#define SERIAL_ECHO_START (serialprintPGM(echomagic))
#define SERIAL_ECHO(x) SERIAL_PROTOCOL(x)
#define SERIAL_ECHOPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ECHOLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ECHOLNPGM(x) SERIAL_PROTOCOLLNPGM(x)

#define SERIAL_ECHOPAIR(name,value) (serial_echopair_P(PSTR(name),(value)))

#define SERIAL_EOL SERIAL_ECHOLN("")

void serial_echopair_P(const char *s_P, float v);
void serial_echopair_P(const char *s_P, double v);
void serial_echopair_P(const char *s_P, unsigned long v);


//Things to write to serial from Program memory. Saves 400 to 2k of RAM.
FORCE_INLINE void serialprintPGM(const char *str)
{
  char ch=pgm_read_byte(str);
  while(ch)
  {
    MYSERIAL.write(ch);
    ch=pgm_read_byte(++str);
  }
}


void get_command();
void process_commands();

void manage_inactivity();

// Hangprinter needs all motors to be constantly enabled so it doesn't fall down */
#if defined(X_ENABLE_PIN) && X_ENABLE_PIN > -1
#define  enable_x() WRITE(X_ENABLE_PIN, X_ENABLE_ON)
#define disable_x() /* nothing for Hangprinter... { WRITE(X_ENABLE_PIN,!X_ENABLE_ON); axis_known_position[X_AXIS] = false; } */
#else
#define enable_x() ;
#define disable_x() ;
#endif

#if defined(Y_ENABLE_PIN) && Y_ENABLE_PIN > -1
#define  enable_y() WRITE(Y_ENABLE_PIN, Y_ENABLE_ON)
#define disable_y() /* nothing for Hangprinter... { WRITE(Y_ENABLE_PIN,!Y_ENABLE_ON); axis_known_position[Y_AXIS] = false; } */
#else
#define enable_y() ;
#define disable_y() ;
#endif

#if defined(Z_ENABLE_PIN) && Z_ENABLE_PIN > -1
#define  enable_z() WRITE(Z_ENABLE_PIN, Z_ENABLE_ON)
#define disable_z() /* nothing for Hangprinter... { WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; } */
#else
#define enable_z() ;
#define disable_z() ;
#endif

#if defined(E0_ENABLE_PIN) && (E0_ENABLE_PIN > -1)
#define enable_e0() WRITE(E0_ENABLE_PIN, E_ENABLE_ON)
#define disable_e0() WRITE(E0_ENABLE_PIN,!E_ENABLE_ON)
#else
#define enable_e0()  /* nothing */
#define disable_e0() /* nothing */
#endif

#if defined(E1_ENABLE_PIN) && (E1_ENABLE_PIN > -1)
#define enable_e1() WRITE(E1_ENABLE_PIN, E_ENABLE_ON)
#define disable_e1() /* nothing for Hangprinter... WRITE(E1_ENABLE_PIN,!E_ENABLE_ON) */
#else
#define enable_e1()  /* nothing */
#define disable_e1() /* nothing */
#endif

#define enable_e2()  /* nothing */
#define disable_e2() /* nothing */

#define enable_e3()  /* nothing */
#define disable_e3() /* nothing */

// Note that E_AXIS and E_CARTH are different numbers
// arrays that need to use E_CARTH instead of E_AXIS is difference[4], destination[4], current_position[4]. tobben 10 sep 2015
enum AxisEnum {A_AXIS=0, B_AXIS=1, C_AXIS=2, D_AXIS=3, E_AXIS=4};
enum CarthesianEnum {X_AXIS=0, Y_AXIS=1, Z_AXIS=2, E_CARTH=3};

void FlushSerialRequestResend();
void ClearToSend();

void get_coordinates();
void calculate_line_lengths(float cartesian[3], float line_lengths[4]);
extern float line_lengths[DIRS];
void prepare_move();
void kill();
void Stop();

bool IsStopped();

void enquecommand(const char *cmd); //put an ASCII command at the end of the current buffer.
void enquecommand_P(const char *cmd); //put an ASCII command at the end of the current buffer, read from flash

void refresh_cmd_timeout(void);

#ifdef FAST_PWM_FAN
void setPwmFrequency(uint8_t pin, int val);
#endif

#ifndef CRITICAL_SECTION_START
#define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli();
#define CRITICAL_SECTION_END    SREG = _sreg;
#endif //CRITICAL_SECTION_START

extern float homing_feedrate[];
extern bool axis_relative_modes[];
extern int feedmultiply;
extern int extrudemultiply; // Sets extrude multiply factor (in percent) for all extruders
extern bool volumetric_enabled;
extern int extruder_multiply[1]; // sets extrude multiply factor (in percent) for each extruder individually
extern float filament_size[1]; // cross-sectional area of filament (in millimeters), typically around 1.75 or 2.85, 0 disables the volumetric calculations for the extruder.
extern float volumetric_multiplier[1]; // reciprocal of cross-sectional area of filament (in square millimeters), stored this way to reduce computational burden in planner
extern float current_position[4]; // Holds the current position in gcode Carthesian xyze coords
extern float add_homing[DIRS];
#ifdef DELTA
extern float endstop_adj[DIRS];
extern float delta_radius;
extern float delta_diagonal_rod;
extern float delta_segments_per_second;
void recalc_delta_settings(float radius, float diagonal_rod);
#endif
extern float min_pos[DIRS];
extern float max_pos[DIRS];
extern bool axis_known_position[DIRS];
extern float zprobe_zoffset;
extern int fanSpeed;

#ifdef FAN_SOFT_PWM
extern unsigned char fanSpeedSoftPwm;
#endif

extern unsigned long starttime;
extern unsigned long stoptime;

// Handling multiple extruders pins
extern uint8_t active_extruder;

#endif

extern void calculate_volumetric_multipliers();

extern float anchor_A_x;
extern float anchor_A_y;
extern float anchor_A_z;
extern float anchor_B_x;
extern float anchor_B_y;
extern float anchor_B_z;
extern float anchor_C_x;
extern float anchor_C_y;
extern float anchor_C_z;
extern float anchor_D_z;
