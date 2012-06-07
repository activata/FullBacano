#include <AudioUnit/AudioUnit.r>

#include "FullBacanoVersion.h"

// Note that resource IDs must be spaced 2 apart for the 'STR ' name and description
#define kAudioUnitResID_FullBacano				1000

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FullBacano~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define RES_ID			kAudioUnitResID_FullBacano
#define COMP_TYPE		kAudioUnitType_Effect
#define COMP_SUBTYPE	FullBacano_COMP_SUBTYPE
#define COMP_MANUF		FullBacano_COMP_MANF	

#define VERSION			kFullBacanoVersion
#define NAME			"Activata: FullBacano"
#define DESCRIPTION		"FullBacano 1.0"
#define ENTRY_POINT		"FullBacanoEntry"

#include "AUResources.r"