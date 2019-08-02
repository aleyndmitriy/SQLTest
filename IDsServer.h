#pragma once
#include <tchar.h>
#include "IExtensible.h"

#pragma pack(8)

/**
	@addtogroup ods_sdk
	@{
*/

namespace ODS
{

	/**
		This interface represents Data Source server
	*/
	class IDsServer : public IExtensible
	{
	public:

		virtual int Init(TCHAR* szCfgString) = 0;
		/**<
			@brief Initialization with configuration string that was saved
				   in the framework, or NULL if nothing was saved

			@param[in] szCfgString Pointer to null terminated string

			@return Error code
		*/


		virtual int Shut() = 0;
		/**<
			@brief Shut

			@return Error code
		*/
	};

}

/**
	@}
*/

#pragma pack()
