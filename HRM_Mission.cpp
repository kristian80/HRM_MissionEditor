#include "HRM_Mission.h"



HRM_Mission::HRM_Mission() :
	m_object_vector()
{
}


HRM_Mission::~HRM_Mission()
{
}

void HRM_Mission::SetPosition(double zero_latitude, double zero_longitude, double zero_heading)
{
	m_ld_latitude = zero_latitude;
	m_ld_longitude = zero_longitude;
	m_lf_heading = zero_heading;
}

void HRM_Mission::RemoveMission(void)
{
	for (auto p_obj : m_object_vector)
	{
		p_obj->DestroyInstance();
	}
}

void HRM_Mission::DrawMission(void)
{
	for (auto p_obj : m_object_vector)
	{
		p_obj->LoadObject();
		p_obj->SetPositionCart(m_ld_latitude, m_ld_longitude, m_lf_heading);
	}
}

void HRM_Mission::SaveMission(boost::property_tree::ptree & pt, int & mission_counter)
{
}
