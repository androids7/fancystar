/* Exit Games Common - C Client Lib
 * Copyright (C) 2004-2012 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

/* Summary
   Constants used by the library.
 */

#ifndef __CONSTANTS_H 
	#define __CONSTANTS_H

	#if !defined(_EG_MARMALADE_PLATFORM) && !defined(_EG_BREW_PLATFORM) && !defined(_EG_IPHONE_PLATFORM) && !defined(_EG_IMAC_PLATFORM) && !defined(_EG_MEEGO_PLATFORM) && !defined(_EG_WIN32_PLATFORM) && !defined(_EG_WIN_MOBILE_PLATFORM) && !defined(_EG_ANDROID_PLATFORM)
		#include "platform_definition.h"
	#endif

	#if defined(_EG_IPHONE_PLATFORM) || defined(_EG_IMAC_PLATFORM)
		#define _EG_APPLE_PLATFORM true
	#endif

	#if defined(_EG_WIN32_PLATFORM) || defined(_EG_WIN_MOBILE_PLATFORM)
		#define _EG_WINDOWS_PLATFORM true
	#endif

	#if defined(_EG_MARMALADE_PLATFORM) || defined(_EG_APPLE_PLATFORM) || defined(_EG_MEEGO_PLATFORM) || defined(_EG_ANDROID_PLATFORM)
		#define _EG_UNIX_PLATFORM true
	#endif

	#if defined _EG_WINDOWS_PLATFORM || (defined _EG_MARMALADE_PLATFORM && defined _MSC_VER && !defined __clang__ && !defined __gcc__ && defined I3D_ARCH_X86)
		#define _EG_MS_COMPILER
	#endif

	#ifndef __cplusplus
		#define false 0
		#define true 1
	#endif

	#ifndef _nByte_defined_
		#define _nByte_defined_
		typedef unsigned char nByte; // unsigned 8  bit value type.
	#endif

	

	// To activate the debug output sent to your SETDEBUG_CB and PHOTONDEBUG_CB functions.
	//#define EG_DEBUGGER 

	//To print out the major checkpoints of Neutron
    //including run, execute, tick, etc
	//#define EG_CHECKPOINTS

	//To include the update methods (tick and run) in your printouts
    //#define EG_UPDATE_FUNCTIONS

	//To print the serialized and deserialized data use internal debug.
	//#define EG_INTERNAL_DEBUG 

	// Prints out which operation is being run
	//#define EG_OP_CODE_DEBUG

	// To print out the values of your serialized data
    // Note: This can be a lot of data...
	//#define EG_SERIALIZE_DEBUG

    // To print out each header
    //#define EG_HEADER_DEBUG

	// Prints timestamp
	//#define EG_TIMESTAMP_IN_DEBUG



	// Just Dummy String which stores value as ""
	#define EMPTY_STRING L""

	#define SHORT_SIZE		2
	#define INTEGER_SIZE	4
	#define LONG_SIZE		8
	#define FLOAT_SIZE		4
	#define DOUBLE_SIZE		8

	#define EG_DEFAULT_OBJECTSIZE 1

	#define EG_BYTE					((nByte)'b')
	#define EG_SHORT				((nByte)'k')
	#define EG_INTEGER				((nByte)'i')
	#define EG_LONG					((nByte)'l')
	#define EG_FLOAT				((nByte)'f') // Photon only
	#define EG_DOUBLE				((nByte)'d') // Photon only
	#define EG_BOOLEAN				((nByte)'o')
	#define EG_STRING				((nByte)'s')
	#define EG_VECTOR				((nByte)'v') // Neutron only, not allowed for arrays!
	#define EG_HASHTABLE			((nByte)'h')
	#define EG_INTERNAL_BYTEARRAY	((nByte)'x') // Neutron internal only
	#define EG_INTERNAL_INTARRAY	((nByte)'n') // Neutron internal only
	#define EG_INTERNAL_STRINGARRAY	((nByte)'a') // Neutron internal only
	#define EG_ARRAY				((nByte)'y') // Photon internal only
	#define EG_GPOPERATION			((nByte)'g') // This is internally used for gp operations.
	#define EG_PHOTON_COMMAND		((nByte)'p') // This is internally used for photon commands.
	#define EG_OBJECT				((nByte)'z') // for EG_Object (C) / Object (C++) / NSObject (objC), only allowed for arrays!
	#define EG_NULL					((nByte)'*') // Photon internal only, for adding NULL as a value to a HashTable
	#define EG_CUSTOM				((nByte)'c') // Photon internal only, for custom data, (de-)serialized by callbacks, implemented by the application

	// 24hours in ms
	#define EG_TIME_OVERFLOW 86400000
#endif