#ifndef _FXDTSBRICK_H_
#define _FXDTSBRICK_H_

#ifndef _SCENEOBJECT_H_
#include "sim/sceneObject.h"
#endif

#ifndef _FXDTSBRICKDATA_H_
#include "game/fx/fxDTSBrickData.h"
#endif

class fxDTSBrick : public SceneObject
{
private:
	typedef SceneObject Parent;

	fxDTSBrickData* mDataBlock;
	S8              angleID;
	S8              colorID;
	S8              printID;
	S8              colorFxID;
	S8              shapeFxID;
	bool            isBasePlate;
	bool            isPlanted;
	S32             client;
	S32             stackBL_ID;

public:
	fxDTSBrick();
	~fxDTSBrick();

	static void initPersistFields();

	static StringTableEntry sprayCanDivisionName[16];
	static S32              sprayCanDivisionSlot[64];
	static ColorF           colorIDTable[64];

	DECLARE_CONOBJECT(fxDTSBrick);
};

#endif // _FXDTSBRICK_H_
