#ifndef _FXBRICKDTS_H_
#define _FXBRICKDTS_H_

#ifndef _SCENEOBJECT_H_
#include "sim/sceneObject.h"
#endif

class fxDTSBrick : public SceneObject
{
private:
public:
	static StringTableEntry sprayCanDivisionName[16];
	static S32              sprayCanDivisionSlot[64];
	static ColorF           colorIDTable[64];
};

#endif // _FXBRICKDTS_H_
