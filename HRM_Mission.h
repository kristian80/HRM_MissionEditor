#pragma once
#include "HRM.h"
#include "HRM_Object.h"


class HRM_Mission
{

public:
	std::string m_name = "NEW MISSION";

	std::vector<HRM_Object *> m_object_vector;

	HRM_Mission();
	~HRM_Mission();
};

