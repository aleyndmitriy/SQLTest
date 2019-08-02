
#pragma once

#include <atlstr.h>

namespace ODS
{
	// definition from <atlstr.h> :
	//typedef CStringT< TCHAR, StrTraitATL< TCHAR, ChTraitsCRT< TCHAR > > > CAtlString;

	typedef ATL::CAtlString OdsString;
}
