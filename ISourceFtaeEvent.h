/*
	Copyright (c) Ocean Data Systems Ltd.
	All rights reserved

	USE, DISCLOSURE, OR REPRODUCTION IS PROHIBITED WITHOUT THE PRIOR
	EXPRESS WRITTEN PERMISSION OF OCEAN DATA SYSTEMS LTD.
*/

/**
	@file
	@brief Interface for HDA Alarm data source. This is the object type that can be created from
	Drv_XYZ_HdaAlarm.dll
*/

#pragma once

#include "IDataSource.h"
#include"IServerFtae.h"
#include"IBrowserEvent.h"
#include"IDsConfiguratorEvent.h"
#pragma pack(8)

/**
	@addtogroup ods_sdk
	@{
*/

namespace ODS
{
	/**
	*/
	class ISourceFtaeEvent : public IDataSource
	{
	public:

		/** flag of the browser filter configuration ability UI*/
		static const int PROPERTY_ID_HAS_FILTER_CFG_UI = 11;

		/** flag of external filter supporting*/
		static const int PROPERTY_ID_EXTERNAL_FILTER_SUPPORT = 12;

		/** flag of custom filter supporting*/
		static const int PROPERTY_ID_CUSTOM_FILTER_SUPPORT = 13;


		virtual IDsConfiguratorEvent* GetConfigurator() = 0;
		/**<
			@brief Retrieve interface to configurator

			@return Pointer to interface
		*/

		virtual IServerFtae* GetServer() = 0;
		/**<
			@brief Retrieve interface to server for data exchange operations

			@return Pointer to interface
		*/

		virtual IBrowserEvent* GetBrowser() = 0;
		/**<
			@brief Retrieve interface to browser to browse what is inside a data source

			@return Pointer to interface
		*/
	};

}

/**
	@}
*/
#pragma pack()
