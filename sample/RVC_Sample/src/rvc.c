#include <tizen.h>
#include <service_app.h>
#include <rvc_api.h>

#include "rvc.h"

#include "pthread.h"
#include "Ecore.h"

/**
* The following code shows how to register the rvc callback.
* @code

void rvc_mode_callback(rvc_mode_type_get_e mode, void* data){
	// Todo: add your code here.
}

void rvc_error_callback(rvc_device_error_type_e error, void* data){
	// Todo: add your code here.
}

void rvc_wheel_callback(signed short wheel_vel_left, signed short wheel_vel_right, void* data){
	// Todo: add your code here.
}

void rvc_pose_callback(float pose_x, float pose_y, float pose_q, void* data){
	// Todo: add your code here.
}

void rvc_bumper_callback(unsigned char bumper_left, unsigned char bumper_right, void* data){
	// Todo: add your code here.
}

void rvc_cliff_callback(unsigned char cliff_left, unsigned char cliff_center, unsigned char cliff_right, void* data){
	// Todo: add your code here.
}

void rvc_lift_callback(unsigned char lift_left, unsigned char lift_right, void* data){
	// Todo: add your code here.
}

void rvc_magnet_callback(unsigned char magnet, void* data){
	// Todo: add your code here.
}

void rvc_suction_callback(rvc_suction_state_e state, void* data){
	// Todo: add your code here.
}

//void rvc_batt_callback(rvc_batt_level_e level, void* data){
//	// Todo: add your code here.
//}

void rvc_voice_callback(rvc_voice_type_e type, void* data){
	// Todo: add your code here.
}

void rvc_batt_low_callback(void* data){
	// Todo: add your code here.
}

void rvc_register_callback(){
	rvc_set_mode_evt_cb(rvc_mode_callback, NULL);
	rvc_set_error_evt_cb(rvc_error_callback, NULL);
	rvc_set_wheel_vel_evt_cb(rvc_wheel_callback, NULL);
	rvc_set_pose_evt_cb(rvc_pose_callback, NULL);
	rvc_set_bumper_evt_cb(rvc_bumper_callback, NULL);
	rvc_set_cliff_evt_cb(rvc_cliff_callback, NULL);
	rvc_set_lift_evt_cb(rvc_lift_callback, NULL);
	rvc_set_magnet_evt_cb(rvc_magnet_callback, NULL);
	rvc_set_suction_evt_cb(rvc_suction_callback, NULL);
	rvc_set_batt_evt_cb(rvc_batt_callback, NULL);
	rvc_set_voice_evt_cb(rvc_voice_callback, NULL);
	rvc_set_batt_low_evt_cb(rvc_batt_low_callback, NULL);
}

void rvc_unregister_callback(){
	rvc_unset_mode_evt_cb();
	rvc_unset_error_evt_cb();
	rvc_unset_wheel_vel_evt_cb();
	rvc_unset_pose_evt_cb();
	rvc_unset_bumper_evt_cb();
	rvc_unset_cliff_evt_cb();
	rvc_unset_lift_evt_cb();
	rvc_unset_magnet_evt_cb();
	rvc_unset_suction_evt_cb();
	rvc_unset_batt_evt_cb();
	rvc_unset_voice_evt_cb();
	rvc_unset_batt_low_evt_cb();
}
* @endcode
*/

/**
* The following code shows how to get the rvc info.
* @code
void get_rvc_info(){
    rvc_mode_type_get_e mode;
	rvc_get_mode(&mode);

	rvc_device_error_type_e error;
	rvc_get_error(&error);

	signed short wheel_vel_left, wheel_vel_right;
	rvc_get_wheel_vel(&wheel_vel_left, &wheel_vel_right);

	unsigned char bumper_left, bumper_right;
	rvc_get_bumper(&bumper_left, &bumper_right);

	float pose_x, pose_y, pose_q;
	rvc_get_pose(&pose_x, &pose_y, &pose_q);

	unsigned char cliff_left, cliff_center, cliff_right;
	rvc_get_cliff(&cliff_left, &cliff_center, &cliff_right);

	unsigned char lift_left, lift_right;
	rvc_get_lift(&lift_left, &lift_right);

	unsigned char magnet;
	rvc_get_magnet(&magnet);

	rvc_suction_state_e suction;
	rvc_get_suction_state(&suction);

	unsigned char is_on, hour, minute;
	rvc_get_reserve(RVC_RESERVE_TYPE_ONCE, &is_on, &hour, &minute);
	rvc_get_reserve(RVC_RESERVE_TYPE_DAILY, &is_on, &hour, &minute);

	float lin_vel, ang_vel;
	rvc_get_lin_ang_vel(&lin_vel, &ang_vel);

	rvc_batt_level_e level;
	rvc_get_battery_level(&level);

	rvc_voice_type_e type;
	rvc_get_voice_type(&type);
}
* @endcode
*/

/**
* The following code shows how to set command the rvc.
* @code
void set_rvc_command(){
    rvc_set_mode(RVC_MODE_SET_PAUSE);
    rvc_set_mode(RVC_MODE_SET_DOCKING);
    rvc_set_mode(RVC_MODE_SET_CLEANING_AUTO);
    rvc_set_mode(RVC_MODE_SET_CLEANING_SPOT);

    rvc_set_time(23, 00);

    rvc_set_voice(RVC_VOICE_TYPE_BEEP);
    rvc_set_voice(RVC_VOICE_TYPE_NONE);
    rvc_set_voice(RVC_VOICE_TYPE_WOMAN);
    rvc_set_voice(RVC_VOICE_TYPE_MAN);

    rvc_set_suction_state(RVC_SUCTION_SLIENT);
    rvc_set_suction_state(RVC_SUCTION_NORMAL);
    rvc_set_suction_state(RVC_SUCTION_TURBO);

    rvc_set_reserve(RVC_RESERVE_TYPE_ONCE, 23, 00);
    rvc_set_reserve(RVC_RESERVE_TYPE_DAILY, 23, 00);

    rvc_set_reserve_cancel(RVC_RESERVE_TYPE_ONCE);
    rvc_set_reserve_cancel(RVC_RESERVE_TYPE_DAILY);

    for(int i = 0 ; i < 100; i++){
        rvc_set_control(RVC_CONTROL_DIR_FORWARD);
        sleep(50000);
	}

    for(int i = 0 ; i < 100; i++){
        rvc_set_control(RVC_CONTROL_DIR_LEFT);
        sleep(50000);
	}

	for(int i = 0 ; i < 100; i++){
        rvc_set_control(RVC_CONTROL_DIR_RIGHT);
        sleep(50000);
	}

	for(int i = 0 ; i < 100; i++){
        rvc_set_wheel_vel(2, 2);
        sleep(50000);
	}

    for(int i = 0 ; i < 100; i++){
        rvc_set_lin_ang(10.f, 0.f);
        sleep(50000);
	}
}
* @endcode
*/

pthread_t p_thread;

void *t_func(void *data)
{

	bool b_should_pause = false;
	int timer = 0;

	while(!b_should_pause)
	{
		if (timer < 100)
		{
			rvc_set_control(RVC_CONTROL_DIR_RIGHT);
		}
		else if (timer < 150)
		{
			rvc_set_control(RVC_CONTROL_DIR_LEFT);
		}
		else // when time is over
		{
			timer = 0;

			// Sends a command to stop RVC
			rvc_set_mode(RVC_MODE_SET_PAUSE);

			b_should_pause = true;
		}
		timer++;
	}
}

bool service_app_create(void *data)
{
	if(rvc_initialize()!=RVC_USER_ERROR_NONE)
	{
		return false;
	}


	// Todo: add your code here.
	int thread_id = pthread_create(&p_thread, NULL, t_func, NULL);

	if (thread_id < 0)
	{
		return false;
	}

	return true;
}

void service_app_terminate(void *data)
{
	rvc_deinitialize();

	// Todo: add your code here.
	int p_thread_status;

	pthread_join(p_thread, (void **)&p_thread_status);

    return;
}

void service_app_control(app_control_h app_control, void *data)
{
    // Todo: add your code here.
    return;
}

static void
service_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	return;
}

static void
service_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
service_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
service_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[])
{
    char ad[50] = {0,};
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);

	return service_app_main(argc, argv, &event_callback, ad);
}
