#pragma once
#include "HRM.h"
//#include "MyIvyConfiguration.h"
#include "HRMImguiWidget.h"
#include "HRM_Mission.h"





class HRM_Editor
{
public:

	std::string m_ds = "";

	std::string m_system_path = "";
	std::string m_scenery_file = "";
	std::string m_config_path = "";

	int m_scenery_number = 1;
	std::string m_scenery_name = "";

	HRM_Mission *mp_current_mission = NULL;
	HRM_Mission *mp_last_mission = NULL;

	std::shared_ptr<HRMImguiWidget> imguiPtr;

	XPLMObjectRef m_obj_ref = NULL;
	XPLMInstanceRef m_inst_ref = NULL;

	//HRM_Object *p_myobject = NULL;

	std::vector<HRM_Mission *> m_street_missions;
	std::vector<HRM_Mission *> m_urban_missions;
	std::vector<HRM_Mission *> m_sar_missions;
	std::vector<HRM_Mission *> m_sling_missions;

	

	bool m_initialized = false;
	int m_plugin_enabled = 0;
	int m_aircraft_loaded = 0;

	float m_data_rate = 0.2;

	//MyIvyConfiguration *m_ivyConfig = NULL;
	
	int m_PluginMenu = 0;
	XPLMMenuID m_PluginMenuID = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// DATAREFS

	XPLMDataRef m_d_latitude;
	XPLMDataRef m_d_longitude;
	XPLMDataRef m_f_heading;

	/////////////////////////////////////////////////////////////////////////////////
	// Dataref Variables

	double m_ld_latitude;
	double m_ld_longitude;
	float m_lf_heading;

public:
	HRM_Editor();
	~HRM_Editor();

	void PluginStart();
	void PluginStop();
	void PluginEnable();
	void PluginDisable();

	void UpdatePosition();


	void PluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam);
	//void IvyDrawOutputWindow(XPLMWindowID in_window_id, void * in_refcon);
	void PluginMenuHandler(void * in_menu_ref, void * in_item_ref);

	void SaveMissions();
	void ReadMissions();

	void PluginKeyCallback(XPLMWindowID inWindowID, char inKey, XPLMKeyFlags inFlags, char inVirtualKey, void * inRefcon, int losingFocus);
	int PluginMouseClickCallback(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void * inRefcon);

	float PluginFlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon);
};

