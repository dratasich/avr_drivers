/*
 * soundmodule.h
 * ==============================================
 * Denise Ratasich
 * 10.11.2012
 */

#ifndef __SOUNDMODULE_H__
#define __SOUNDMODULE_H__	

#include <avr/io.h>	// e.g. uint8_t, uint16_t

// soundmodule ports/pins
// uses Timer 3
#define SM_PORT		PORTF
#define SM_PIN		PINF
#define SM_DDR		DDRF
#define SM_CLK		PF0
#define SM_DATA		PF1
#define SM_BUSY		PF2

//sound definitions for driving in remote control mode
#define SOUND_DRIVE_FORWARD		1
#define SOUND_DRIVE_BACKWARD	1
#define SOUND_DRIVE_LEFT		0
#define SOUND_DRIVE_RIGHT		0
#define SOUND_TURN_LEFT			14
#define SOUND_TURN_RIGHT		14
#define SOUND_STOP_SOFT			15
#define SOUND_STOP_IMMEDIATELY	19

//sound definitions for moving autonomous
#define SOUND_MOVE_START		1
#define SOUND_TURN_HEAD			3
#define SOUND_TURN_RKDK_90		5
#define SOUND_TURN_RKDK_180		6
#define SOUND_RC_START			10
#define SOUND_CLOSE_WALL_FRONT	14
#define SOUND_CLOSE_WALL_LEFT	15
#define SOUND_CLOSE_WALL_RIGHT	18
#define SOUND_MEASURE_DISTANCE	0
#define SOUND_EMERGENCY_STOP	19
#define SOUND_NR_MAX			SOUND_EMERGENCY_STOP

//sound definitions for wait and react
#define SOUND_NEAR_LEFT			0
#define SOUND_NEAR_RIGHT		0
#define SOUND_FAR_FRONT			1
#define SOUND_MID_FRONT			3
#define SOUND_NEAR_FRONT		19
#define SOUND_PERSON_LOST		12// 20
#define SOUND_WALK_A_STEP		18// 10// 8 //12
#define SOUND_WALK_STOP			16 //6 od 7

//sound definitions for move labyrinth
#define SOUND_INIT			2
#define SOUND_START			1
#define SOUND_TURN			9
#define SOUND_FORWARD			18

void soundmodule_init(void);
void soundmodule_playSound(uint8_t soundNr);
uint8_t soundmodule_isReady(void);
void soundmodule_sendCmd(uint16_t cmd);
// Timer3 overflow ISR

#endif
