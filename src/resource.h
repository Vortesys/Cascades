/* * * * * * * *\
	RESOURCE.H -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		NT Style's resource collection.
		** NO DEPENDENCIES **
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#ifndef RC_INVOKED
#pragma once
#endif

/* Icon Library */
#define IDI_MAIN		1

/* Dialogs */
#define DLG_MAIN	1

/* Dialog Controls */
// Buttons
#define IDD_OK				1
#define IDD_CANCEL			2
#define IDD_BROWSE			3
#define IDD_BROWSE2			4
#define IDD_CHICON			5
// Radio Buttons
#define IDD_WORKDIR			100
#define IDD_COMMGROUP		101
// Input Controls
#define IDD_NAME			200
#define IDD_PATH			201
#define IDD_WORKPATH		202
#define IDD_HOTKEY			203
// Static Controls
#define IDD_STAT_ICON		300
#define IDD_STAT_WORKDIR	301

/* String Table */
#define IDS_APPTITLE		1
#define IDS_WEBSITE			5
// General Dialog
#define IDS_DLG_OK			200
#define IDS_DLG_CANCEL		201
#define IDS_DLG_BROWSE		202
// Icon Section
#define IDS_DLG_ICON		203
#define IDS_DLG_CHICON		204
// Properties Section
#define IDS_DLG_PROPS		205
#define IDS_DLG_NAME		206
#define IDS_DLG_DIRECTORY	207
#define IDS_DLG_COMMGRP		208
// Advanced Section
#define IDS_DLG_ADVANCED	209
#define IDS_DLG_WORKDIR		210
#define IDS_DLG_HOTKEY		211
// Dialog Title
#define IDS_DLT_GRP_PROPS	300
#define IDS_DLT_GRP_NEW		301
#define IDS_DLT_ITEM_PROPS	302
#define IDS_DLT_ITEM_NEW	303
#define IDS_DLT_POWER		304
