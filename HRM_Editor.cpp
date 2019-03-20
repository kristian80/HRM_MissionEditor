#include "HRM_Editor.h"



HRM_Editor::HRM_Editor() :
	m_street_missions(),
	m_urban_missions(),
	m_sar_missions(),
	m_sling_missions()
{
	
}


HRM_Editor::~HRM_Editor()
{
	
}



void HRM_Editor::PluginStart()
{
	

	m_ds = XPLMGetDirectorySeparator();

	char buffer[2048];
	XPLMGetSystemPath(buffer);
	m_system_path = buffer;

	m_scenery_file = m_system_path + m_ds + "Resources" + m_ds + "plugins" + m_ds + "HRM_MissionEditor" + m_ds + "scenery_1.xml";

	

	// m_ivyConfig = new MyIvyConfiguration();

	srand(time(NULL));

	

	XPLMRegisterFlightLoopCallback(WrapFlightLoopCallback, 0.1, 0);

	// Menu;
	m_PluginMenu = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "HRM Editor", 0, 1);
	m_PluginMenuID = XPLMCreateMenu("HRM Editor", XPLMFindPluginsMenu(), m_PluginMenu, WrapMenuHandler, 0);
	XPLMAppendMenuItem(m_PluginMenuID, "Toggle Control Window", "Item1", 1);

	int left, top, right, bot;
	XPLMGetScreenBoundsGlobal(&left, &top, &right, &bot);
	

	int width = 1000;
	int height = 550;
	int left_pad = 175;
	int top_pad = 75;
	int x = left + left_pad;
	int y = top - top_pad;
	// WindowDecoration decorate
	// WindowDecorationNone = 0
	// WindowDecorationRoundRectangle = 1
	// WindowDecorationSelfDecorated = 2
	// WindowDecorationSelfDecoratedResizable = 3
	int decorate = 1;
	imguiPtr = std::make_shared<HRMImguiWidget>(this, 
		                                        300, 
												550, 
												300 + 1200, 
												550 - 400, 
												decorate);  // calls constructor
	//imguiPtr-> // calls ImgWindow::init from the base class which in turn virtually calls the overridden function
	
	//XPLMCreateProbe()

	HRMDebugString("Lookup Datarefs");

	m_d_latitude = XPLMFindDataRef("sim/flightmodel/position/latitude");
	m_d_longitude = XPLMFindDataRef("sim/flightmodel/position/longitude");
	m_f_heading = XPLMFindDataRef("sim/flightmodel/position/psi");


	m_ld_latitude = XPLMGetDatad(m_d_latitude);
	m_ld_longitude = XPLMGetDatad(m_d_longitude);
	m_lf_heading = XPLMGetDataf(m_f_heading);

	
	/*HRM_Object *p_myobject = new HRM_Object();

	p_myobject->LoadObject("3D_people_library/commercial_pilot_2.obj");

	p_myobject->SetPosition(m_ld_latitude, m_ld_longitude, m_lf_heading);

	m_object_vector.push_back(p_myobject);*/

	m_initialized = true;
}



void HRM_Editor::PluginStop()
{

	
	if (m_initialized == false) return;

	//XPLMUnregisterFlightLoopCallback(m_FlightLoopCB, 0);

	XPLMDestroyMenu(m_PluginMenuID);


	//m_ivyConfig->WriteConfig();

	//if (m_ivyConfig) delete m_ivyConfig;

	

}

void HRM_Editor::PluginEnable()
{
	m_plugin_enabled = 1;
}

void HRM_Editor::PluginDisable()
{
	
	m_plugin_enabled = 0;
}

void HRM_Editor::UpdatePosition()
{
	m_ld_latitude = XPLMGetDatad(m_d_latitude);
	m_ld_longitude = XPLMGetDatad(m_d_longitude);
	m_lf_heading = XPLMGetDataf(m_f_heading);

	for (auto p_mission : m_street_missions)	p_mission->SetPosition(m_ld_latitude, m_ld_longitude, m_lf_heading);
	for (auto p_mission : m_urban_missions)	p_mission->SetPosition(m_ld_latitude, m_ld_longitude, m_lf_heading);
	for (auto p_mission : m_sar_missions)	p_mission->SetPosition(m_ld_latitude, m_ld_longitude, m_lf_heading);
	for (auto p_mission : m_sling_missions)	p_mission->SetPosition(m_ld_latitude, m_ld_longitude, m_lf_heading);
}



void HRM_Editor::PluginReceiveMessage(XPLMPluginID inFromWho, int inMessage, void * inParam)
{
	HRMDebugString("Message Received");

	if (m_initialized == false) return;

	if (inFromWho == XPLM_PLUGIN_XPLANE)
	{
		if (inMessage == XPLM_MSG_PLANE_LOADED)
		{
			HRMDebugString("Plane loaded");

			m_aircraft_loaded = 1;
			
		}
		if (inMessage == XPLM_MSG_AIRPORT_LOADED)
		{
			HRMDebugString("Airport loaded");
		}
		

		if (inMessage == XPLM_MSG_SCENERY_LOADED)
		{
			HRMDebugString("Scenery loaded");
			

		}
	}
}



void HRM_Editor::PluginMenuHandler(void * in_menu_ref, void * inItemRef)
{
	HRMDebugString("Menu Handler");

	if (m_initialized == false) return;

	if (strcmp((char *) inItemRef, "Item1") == NULL)
	{
		if (!imguiPtr->GetVisible())
		{
			imguiPtr->Visible(true);

			//CreateLogbookWidget(int(m_ivyConfig->m_log_window_pos_x), int(m_ivyConfig->m_log_window_pos_y), int(m_ivyConfig->m_log_window_width), int(m_ivyConfig->m_log_window_height));
			//m_MenuLogbookShow = 1;
		}
		else
		{
			imguiPtr->Visible(false);

			//m_MenuLogbookShow = 0;
			//XPDestroyWidget(m_LogbookWidget, 1);
		}
	}
}

void HRM_Editor::SaveMissions()
{

	boost::property_tree::xml_writer_settings<char> settings(' ', 4);

	boost::property_tree::ptree pt;
	int mission_counter = 0;

	for (auto p_mission : m_street_missions)	p_mission->SaveMission(pt, mission_counter);
	for (auto p_mission : m_urban_missions)		p_mission->SaveMission(pt, mission_counter);
	for (auto p_mission : m_sar_missions)		p_mission->SaveMission(pt, mission_counter);
	for (auto p_mission : m_sling_missions)		p_mission->SaveMission(pt, mission_counter);

	boost::property_tree::write_xml(m_scenery_file, pt, std::locale(), settings);
}

void HRM_Editor::LoadMissions()
{
	boost::property_tree::ptree pt;
	try
	{
		boost::property_tree::read_xml(m_scenery_file, pt);
	}
	catch (...)
	{
		HRMDebugString("Could not open XML");
		return;
	}

	int mission_counter = 0;

}


void HRM_Editor::PluginKeyCallback(XPLMWindowID inWindowID, char inKey, XPLMKeyFlags inFlags, char inVirtualKey, void * inRefcon, int losingFocus)
{
}

int HRM_Editor::PluginMouseClickCallback(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void * inRefcon)
{
	return 0;
}



float HRM_Editor::PluginFlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon)
{
	
	

	return m_data_rate;
}

