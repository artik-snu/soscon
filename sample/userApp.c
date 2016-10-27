/**
 * @file	userApp.c
 * @brief	SAMSUNG cleaning robot control Api
 * @author	junu.hong@samsung.com
 * Copyright 2016 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#include <stdio.h>		// printf(), getchar()
#include <stdlib.h>		// memcpy()
#include <termios.h>	// termios
#include <unistd.h>		// tcgetattr(), usleep()
#include <fcntl.h>		// fcntl()

#include "rvc_api.h"	// RVC API


#define LOG_RED "\033[0;31m"
#define LOG_GREEN "\033[0;32m"
#define LOG_BROWN "\033[0;33m"
#define LOG_BLUE "\033[0;34m"
#define LOG_END "\033[0;m"


/**
 * @brief Defines a callback to be executed when the mode is changed
 * @param[in] mode Current mode 
 * @param[in] user_data User data to be passed to the callback function
 * @pre The callback must be registered using rvc_set_mode_evt_cb().
 * @see rvc_set_mode_evt_cb()
 * @see rvc_unset_mode_evt_cb()
 * @see typedef void (*rvc_mode_evt_cb)(rvc_mode_type_get_e mode, void* user_data)
 */
void __test_mode_evt_callback(unsigned char mode, void* user_data)
{
	printf("Mode event callback : %d\n", mode);
}

/**
 * @brief Defines a callback to be executed when the error occurs
 * @since_tizen KANTM 3.0 
 * @param[in] error Current error 
 * @param[in] user_data User data to be passed to the callback function
 * @pre The callback must be registered using rvc_set_error_evt_cb().
 * @see rvc_set_error_evt_cb()
 * @see rvc_unset_error_evt_cb()
 * @see typedef void (*rvc_error_evt_cb)(rvc_device_error_type_e error, void* user_data)
 */
void __test_error_evt_callback(unsigned char error_type, void* user_data)
{
	printf("Error event callback : %d\n", error_type);
}

/**
 * @brief Defines a callback to be executed when the cliff event occurs
 * @param[in] cliff_left The value of left-cliff sensor 
 * @param[in] cliff_center The value of center-cliff sensor
 * @param[in] cliff_right The value of right-cliff sensor
 * @param[in] user_data User data to be passed to the callback function
 * @pre The callback must be registered using rvc_set_cliff_evt_cb().
 * @see rvc_set_cliff_evt_cb()
 * @see rvc_unset_cliff_evt_cb()
 * @see typedef void (*rvc_cliff_evt_cb)(unsigned char cliff_left, unsigned char cliff_center, unsigned char cliff_right, void* user_data)
 */
void __test_cliff_evt_callback(unsigned char cliff_left, unsigned char cliff_center, unsigned char cliff_right, void* user_data)
{
	printf("Cliff is changed : (left) %s, (center) %s, (right) %s\n",
		cliff_left   ? "true" : "false",
		cliff_center ? "true" : "false",
		cliff_right  ? "true" : "false");
}

/**
 * @brief Defines a callback to be executed when the wheel is lifted
 * @since_tizen KANTM 3.0 
 * @param[in] lift_left Left wheel sensor for detecting lift
 * @param[in] lift_right Right wheel sensor for detecting lift
 * @param[in] user_data User data to be passed to the callback function
 * @pre The callback must be registered using rvc_set_lift_evt_cb().
 * @see rvc_set_lift_evt_cb()
 * @see rvc_unset_lift_evt_cb()
 * @see typedef void (*rvc_lift_evt_cb)(unsigned char lift_left, unsigned char lift_right, void* user_data)
 */
void __test_lift_evt_callback(unsigned char lift_left, unsigned char lift_right, void* user_data)
{
	printf("Lift is changed : (left) %s, (right) %s\n",
		lift_left  ? "true" : "false",
		lift_right ? "true" : "false");
}

/**
 * @brief Defines a callback to be executed when the magnet status is changed
 * @param[in] magnet Indicate whether Robot detects magnet or not. (0: not detected, 1: detected)
 * @param[in] user_data User data to be passed to the callback function
 * @pre The callback must be registered using rvc_set_magnet_evt_cb().
 * @see rvc_set_magnet_evt_cb()
 * @see rvc_unset_magnet_evt_cb()
 * @see typedef void (*rvc_magnet_evt_cb)(unsigned char magnet, void* user_data)
 */
void __test_magnet_evt_callback(unsigned char magnet, void* user_data) 
{
	printf("Magnet event callback : %s\n", magnet ? "true" : "false");
}

/**
 * @brief Defines a callback to be executed when the bumper event occurs
 * @param[in] bumper_left Left bumper
 * @param[in] bumper_right Right bumper
 * @param[in] user_data User data to be passed to the callback function
 * @pre The callback must be registered using rvc_set_bumper_evt_cb().
 * @see rvc_set_bumper_evt_cb()
 * @see rvc_unset_bumper_evt_cb()
 * @see typedef void (*rvc_bumper_evt_cb)(unsigned char bumper_left, unsigned char bumper_right, void* user_data)
 */
void __test_bumper_evt_callback(unsigned char bumper_left, unsigned char bumper_right, void* user_data)
{
	printf("Bumper event callback, bumper_left : %d, bumper_right : %d\n", bumper_left, bumper_right);
}

/**
 * @brief Defines a callback to be executed when the suction status is changed
 * @param[in] state Suction state (the value would be RVC_SUCTION_UNKNOWN or RVC_SUCTION_SLIENT, RVC_SUCTION_NORMAL, RVC_SUCTION_TURBO)
 * @param[in] user_data User data to be passed to the callback function
 * @pre The callback must be registered using rvc_set_suction_evt_cb().
 * @see rvc_set_suction_evt_cb()
 * @see rvc_unset_suction_evt_cb()
 * @see typedef void (*rvc_suction_evt_cb)(rvc_suction_state_e state, void* user_data)
 */
void __test_suction_evt_callback(unsigned char suction, void* user_data)
{
	printf("Suction event callback, suction : %d\n", suction);
}

/**
 * @brief Defines a callback to be executed when the reservation status is changed
 * @param[in] reserve_type Type that reservation status is changed. the value would be RVC_RESERVE_TYPE_ONCE, RVC_RESERVE_TYPE_DAILY
 * @param[in] is_on Indicate whether reserve type(parameter 1) is set or unset. (0: unset, 1: set)
 * @param[in] reserve_hh Hour
 * @param[in] reserve_mm Minute
 * @param[in] user_data User data to be passed to the callback function
 * @pre The callback must be registered using rvc_set_reservation_evt_cb().
 * @see rvc_set_reservation_evt_cb()
 * @see rvc_unset_reservation_evt_cb()
 * @see typedef void (*rvc_reservation_evt_cb)(rvc_reserve_type_e reserve_type, unsigned char is_on, unsigned char reserve_hh, unsigned char reserve_mm, void* user_data)
 */
void __test_reservation_evt_callback(rvc_reserve_type_e reserve_type, unsigned char is_on, unsigned char reserve_hh, unsigned char reserve_mm, void* user_data)
{
	printf("reservation event callback, reserve_type : %d, is_on = %d, reserve_hh = %d, reserve_mm = %d\n", reserve_type, is_on, reserve_hh, reserve_mm);
}

/**
 * @brief Defines a callback to be executed when the left/right wheel velocity is changed
 * @details wheel_vel_left = v - 0.5 * wheel_base * w, wheel_vel_right = v + 0.5 * wheel_base * w
 * v => linear velocity, w => angular velocity
 * [(wheel_vel_left) 100, (wheel_vel_right) 100] is same as [(linear vel) 100, (ang vel) 0]
 * @since_tizen 3.0 
 * @param[in] wheel_vel_left Left wheel velocity (measure : mm/s)
 * @param[in] wheel_vel_right Right wheel velocity (measure : mm/s)
 * @param[in] user_data User data to be passed to the callback function
 * @pre The callback must be registered using rvc_set_wheel_vel_evt_cb().
 * @see rvc_set_wheel_vel_evt_cb()
 * @see rvc_unset_wheel_vel_evt_cb()
 * @see typedef void (*rvc_wheel_vel_evt_cb)(signed short wheel_vel_left, signed short wheel_vel_right, void* user_data)
 */
void __test_wheel_vel_evt_callback(signed short wheel_vel_left, signed short wheel_vel_right, void* user_data)
{
	printf("wheel event callback, wheel_vel_left : %d, wheel_vel_right = %d\n", wheel_vel_left, wheel_vel_right);
}

/**
 * @brief Defines a callback to be executed when the linear or angular velocity is changed
 * @since_tizen 3.0 
 * @param[in] lin Linear velocity 
 * @param[in] ang Angular velocity 
 * @param[in] user_data User data to be passed to the callback function
 * @pre The callback must be registered using rvc_set_lin_ang_evt_cb().
 * @see rvc_set_lin_ang_evt_cb()
 * @see rvc_unset_lin_ang_evt_cb()
 * @see typedef void (*rvc_lin_ang_evt_cb)(float lin, float ang, void* user_data)
 */
void __test_lin_ang_evt_callback(float lin, float ang, void* user_data)
{
	printf("lin/ang event callback, lin : %.2f, ang = %.2f\n", lin, ang);
}

int main(int argc, char *argv[])
{
	int bShouldQuit = 0;
	int mode = 0;
	int voice_mode = 0;
	int hour = 0, min = 0;
	int reserve_type = 0;
	int manual_dir = 0;
	int info_type = 0;
	int cb_type = 0;
	int suction_sts = 0;
	int ret_1 = 0;
	int ret_2 = 0;
	int ret_3 = 0;
	signed short wheel_vel_left=0, wheel_vel_right=0;
	float ang_vel=0.f, lin_vel=0.f;
	int timer = 0;
    unsigned char is_on = 0;

	// initialize RVC API
	if (rvc_initialize() < 0)
	{
		printf("rvc_initialize() failed.\n");
		return -1;
	}

	char cmd = 0;

	while (!bShouldQuit)
	{	
		if (cmd != '\n')
		{
			printf("RVC Hackathon API Test App\n\n");
			printf("Options..\n");
			printf(LOG_GREEN "1	- set callbacks					\n" LOG_END);
			printf("2	- set time								\n");
			printf("3	- set reserve								\n");
			printf("4	- set mode auto cleaning				\n");
			printf("5	- set mode stop							\n");
			printf(LOG_GREEN "6	- unset callbacks				\n" LOG_END);
			printf("7	- set mode spot							\n");
			printf("8	- set voice								\n");
			printf("9	- set mode docking						\n");
			printf("0	- set mode manual						\n");
			printf("a	- get information						\n");
			printf("b	- set suction sts						\n");
			printf("c	- set wheel ang/lin vel					\n");
			printf("d	- set wheel left/right vel				\n");
			printf("e	- excute test planning mode w/ lin/ang	\n");
			printf("f	- excute test planning mode w/ l/r wheel\n");
			printf("q	- quit.									\n");

		}

		scanf("%c", &cmd);

		switch (cmd)
		{
		case '1':
			printf("set callback\n");
			printf("1) Mode		2) error	3) bumper	4) cliff	5) lift		6) magnet	7) suction	8) reserve	9) l/r wheel vel  10) lin/ang vel\n");
			scanf("%d", &cb_type);

			switch(cb_type)
			{
			case 1:
				// Sets callback '__test_mode_evt_callback()' to be executed when the mode is changed.
				rvc_set_mode_evt_cb(__test_mode_evt_callback, NULL);
				break;
			case 2:
				// Sets callback '__test_error_evt_callback' to be executed when the error occurs.
				rvc_set_error_evt_cb(__test_error_evt_callback, NULL);
				break;
			case 3:
				// Sets callback '__test_bumper_evt_callback' to be executed when the bumper event occurs.
				rvc_set_bumper_evt_cb(__test_bumper_evt_callback, NULL);
				break;
			case 4:
				// Sets callback '__test_cliff_evt_callback' to be executed when the cliff detects.
				rvc_set_cliff_evt_cb(__test_cliff_evt_callback, NULL);
				break;
			case 5:
				// Sets callback '__test_lift_evt_callback' to be executed when the wheel lift detects.
				rvc_set_lift_evt_cb(__test_lift_evt_callback, NULL);
				break;
			case 6:
				// Sets callback '__test_magnet_evt_callback' to be executed when the magnet detects.
				rvc_set_magnet_evt_cb(__test_magnet_evt_callback, NULL);
				break;
			case 7:
				// Sets callback '__test_suction_evt_callback' to be executed when the suction status is changed.
				rvc_set_suction_evt_cb(__test_suction_evt_callback, NULL);
				break;
			case 8:
				// Sets callback '__test_reservation_evt_callback' to be executed when the reservation status is changed.
				rvc_set_reservation_evt_cb(__test_reservation_evt_callback, NULL);
				break;
			case 9:
				// Sets callback '__test_wheel_vel_evt_callback' to be executed when the reservation status is changed.
				rvc_set_wheel_vel_evt_cb(__test_wheel_vel_evt_callback, NULL);
				break;

			case 10:
				// Sets callback '__test_lin_ang_evt_callback' to be executed when the reservation status is changed.
				rvc_set_lin_ang_evt_cb(__test_lin_ang_evt_callback, NULL);
				break;
			}
			
			break;

		case '2':
			printf("set current time\n");
			printf("input hour\n");
			scanf("%d", &hour);
			printf("input min\n");
			scanf("%d", &min);

			// Sets current time.
			rvc_set_time((unsigned char)hour, (unsigned char)min);

			break;

		case '3':
			printf("set reserve / cancel reserve\n");
			printf("1) set reserve		2) cancel reserve\n");
			scanf("%d", &mode);
			if (mode == 1)
			{
				printf("input reserve type\n");
				scanf("%d", &reserve_type);
				printf("input hour\n");
				scanf("%d", &hour);
				printf("input min\n");
				scanf("%d", &min);

				// Sets reservation for auto-cleaning.
				rvc_set_reserve((unsigned char)reserve_type, (unsigned char)hour, (unsigned char)min);
			}
			else if (mode == 2)
			{
				printf("input reserve type for cancel, 0: RVC_RESERVE_TYPE_ONCE, 1: RVC_RESERVE_TYPE_DAILY\n");
				scanf("%d", &reserve_type);

				// Cancels the reservation.
				rvc_set_reserve_cancel((unsigned char)reserve_type);
			}
		
			break;

		case '4':
			printf("Sets auto-cleaning mode\n");

			// Starts auto-cleaning.
			rvc_set_mode(RVC_MODE_SET_CLEANING_AUTO);
			break;

		case '5':
			printf("Sets pause mode\n");

			// Sets mode to stop.
			rvc_set_mode(RVC_MODE_SET_PAUSE);
			break;

		case '6':
			printf("unset callback\n");

			// Unsets callbacks as below.
			rvc_unset_mode_evt_cb();
			rvc_unset_error_evt_cb();
			rvc_unset_wheel_vel_evt_cb();
			rvc_unset_pose_evt_cb();
			rvc_unset_bumper_evt_cb();
			rvc_unset_cliff_evt_cb();
			rvc_unset_lift_evt_cb();
			rvc_unset_magnet_evt_cb();
			rvc_unset_suction_evt_cb();
			rvc_unset_reservation_evt_cb();
			break;

		case '7':
			printf("set spot mode\n");

			// Starts spot-cleaning.
			rvc_set_mode(RVC_MODE_SET_CLEANING_SPOT);
			break;

		case '8':
			printf("1) BEEP		2) MUTE	\n");
			scanf("%d", &voice_mode);

			switch(voice_mode)
			{
				case 1:
					// Sets a type of voice to beep.
					rvc_set_voice(RVC_VOICE_TYPE_BEEP);
					break;
				case 2:
					// Sets a type of voice to mute.
					rvc_set_voice(RVC_VOICE_TYPE_NONE);
					break;

				default:
					printf("operation not found\n");
			}

			break;

		case '9':
			printf("set mode docking\n");

			// Sends a command to return for charging.
			rvc_set_mode(RVC_MODE_SET_DOCKING);
			break;

		case '0':
			printf("set mode manual\n");
			printf("input dir : 1) FORWARD		2) LEFT		3) RIGHT\n");
			scanf("%d", &manual_dir);
			if (manual_dir == 1)
				// Sends a command to move forward.
				rvc_set_control(RVC_CONTROL_DIR_FORWARD);
			else if (manual_dir == 2)
				// Sends a command to rotate left.
				rvc_set_control(RVC_CONTROL_DIR_LEFT);
			else if (manual_dir == 3)
				// Sends a command to rotate right.
				rvc_set_control(RVC_CONTROL_DIR_RIGHT);
			break;

		case 'a':
			printf("get information. \n");
			printf("1) mode		2) error	3) suction	4) pose		5) cliff	6) lift		7) magnet	8) bumper	9) wheel vel	10) reserve info	11)lin/ang vel \n");
			scanf("%d", &info_type);
			switch(info_type)
			{
			case 1:
				// Gets the current mode(auto-cleaning, pause, idle, docking...).
				rvc_get_mode(&ret_1);
				printf("%d\n", ret_1);
				break;
			case 2:
				// Gets the error(none, lift, cliff...).
				rvc_get_error(&ret_1);
				printf("%d\n", ret_1);
				break;
			case 3:
				// Gets the suction state(normal, turbo, silent...).
				rvc_get_suction_state(&ret_1);
				printf("%d\n", ret_1);
				break;
			case 4:
				// Gets the current pose of RVC.
				rvc_get_pose(&ret_1, &ret_2, &ret_3);
				printf("(%d, %d, %d)\n", ret_1, ret_2, ret_3);
				break;
			case 5:
				// Get the cliff sensor values.
				rvc_get_cliff(&ret_1, &ret_2, &ret_3);
				printf("(%d, %d, %d)\n", ret_1, ret_2, ret_3);
				break;
			case 6:
				// Gets the lift-sensor values.
				rvc_get_lift(&ret_1, &ret_2);
				printf("(%d, %d)\n", ret_1, ret_2);
				break;
			case 7:
				// Gets the magnet-sensor value.
				rvc_get_magnet(&ret_1);
				printf("%d\n", ret_1);
				break;
			case 8:
				// Gets the bumper-sensor values.
				rvc_get_bumper(&ret_1, &ret_2);
				printf("(%d, %d)\n", ret_1, ret_2);
			case 9:
				// Gets the left/right wheel velocity. 
				rvc_get_wheel_vel(&wheel_vel_left, &wheel_vel_right);
				printf("(%d, %d)\n", wheel_vel_left, wheel_vel_right);
				break;
			case 10:
				printf("input reserve type :	1) ONCE  	2) DAILY >");
				scanf("%d", &reserve_type);
				if (reserve_type == 1)
				{
					// Gets once reservation info.
					rvc_get_reserve(RVC_RESERVE_TYPE_ONCE, &is_on, &hour, &min);
					printf("reserve type (%d:%d)\n", (unsigned char)hour, (unsigned char)min);
				} 
				else if (reserve_type == 2)
				{
					// Gets daily reservation info.
					rvc_get_reserve(RVC_RESERVE_TYPE_DAILY, &is_on, &hour, &min);
					printf("reserve type (%d:%d)\n", (unsigned char)hour, (unsigned char)min);
				}
				break;
			case 11:
				// Gets the linear and angular velocity from RVC.
				rvc_get_lin_ang_vel(&lin_vel, &ang_vel);
				printf("lin_vel = %.2f, ang_vel = %.2f\n", lin_vel, ang_vel);
				break;
			}
			break;

		case 'b':
			printf("set suction sts \n");
			printf("1) SUCTION_SILENT	2) SUCTION_NORMAL	3) SUCTION_TURBO \n");
			scanf("%d", &suction_sts);

			switch(suction_sts)
			{
			
			case 1:
				// Sets the suction state to RVC_SUCTION_SLIENT
				rvc_set_suction_state(RVC_SUCTION_SLIENT);
				break;
			case 2:
				// Sets the suction state to RVC_SUCTION_NORMAL
				rvc_set_suction_state(RVC_SUCTION_NORMAL);
				break;
			case 3:
				// Sets the suction state to RVC_SUCTION_TURBO
				rvc_set_suction_state(RVC_SUCTION_TURBO);
				break;

			default:
				// Sets the suction state to NONE
				rvc_set_suction_state(RVC_SUCTION_UNKNOWN);
			}
			break;

		case 'c':
			printf("set wheel lin/ang vel\n");
			printf("input lin_vel>");
			scanf("%f", &lin_vel);
			printf("input ang_vel>");
			scanf("%f", &ang_vel);

			// Sets the linear and angular velocity.
			rvc_set_lin_ang(lin_vel, ang_vel);
			break;

		case 'd':
			printf("set wheel left/right vel\n");
			printf("input wheel_vel_left>");
			scanf("%d", &wheel_vel_left);
			printf("input wheel_vel_right>");
			scanf("%f", &wheel_vel_right);

			// Sets the left/right wheel velocity.
			rvc_set_wheel_vel(wheel_vel_left, wheel_vel_right);
			break;


		case 'e':
			printf("execute test planning mode w/ lin/ang vel\n");
			while(1)
			{
				if(timer < 800)
				{
					// Set the linear and angular velocity.
					rvc_set_lin_ang(10.f, 0.f);
				}
				else if(timer < 1200)
				{
					rvc_set_lin_ang(-10.f, 0.f);
				}
				else if(timer < 1600)
				{
					rvc_set_lin_ang(50.f, 0.3f);
				}
				else
				{
					timer = 0;
					printf("timer is over\n", timer);

					// Sends a command to stop RVC
					rvc_set_mode(RVC_MODE_SET_PAUSE);
					break;
				}
				timer++;

				if (timer % 100 == 0)
				{
					printf("timer = %d\n", timer);
				}
				//usleep(50000); // sleep for 50ms
			}

		case 'f':
			printf("execute test planning mode w/ left/right wheel vel\n");
			while(1)
			{
				if(timer < 800)
				{
					// Set the linear and angular velocity.
					rvc_set_wheel_vel(10, 10);
				}
				else if(timer < 1200)
				{
					rvc_set_wheel_vel(-10, -10);
				}
				else if(timer < 1600)
				{
					rvc_set_wheel_vel(50, 20);
				}
				else
				{
					timer = 0;
					printf("timer is over\n", timer);

					// Sends a command to stop RVC
					rvc_set_mode(RVC_MODE_SET_PAUSE);
					break;
				}
				timer++;

				if (timer % 100 == 0)
				{
					printf("timer = %d\n", timer);
				}
				//usleep(50000); // sleep for 50ms
			}
			

			break;

		case 'q':
			bShouldQuit = 1;
			break;
		}
	}

	// Deinitializes RVC API.
	printf("Before RvcApiQuit()\n");
	rvc_deinitialize();
	printf("After  RvcApiQuit()\n");

	return 0;
}
