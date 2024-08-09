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

	S8              angleID;
	S8              colorID;
	S8              printID;
	S8              colorFxID;
	S8              shapeFxID;
	bool            isBasePlate;
	bool            isPlanted;
	S32             client;
	S32             stackBL_ID;

	Point3F         worldPos;
			          
	S32             topArea;
	S32             bottomArea;
	S32             northArea;
	S32             eastArea;
	S32             southArea;
	S32             westArea;

public:

	enum fxDTSBrickMasks {
		DataBlockMask = 1 << 0,
		PositionMask  = 1 << 2,
	};

	// SceneObject overrides
	void renderObject(SceneState* state, SceneRenderImage* image);
	bool prepRenderImage(SceneState* state, const U32 stateKey, const U32 startZone, const bool modifyBaseZoneState = false);
	void addToScene();      // Adds object to the client or server container depending on the object
	bool onAdd();
	void onRemove();
	void setTransform(const MatrixF& mat);

	// NetObject overrides
	U32 packUpdate(NetConnection*, U32, BitStream*);
	void unpackUpdate(NetConnection*, BitStream*);

	// GameBase overrides
	bool setDataBlock(fxDTSBrickData* dptr);
	bool onNewDataBlock(fxDTSBrickData* dptr);

	AngAxisF ang;

	// fxDTSBrick stuff
	fxDTSBrick();
	~fxDTSBrick();

	// usually mDataBlock would be private, but the consolemethod needs to access it
	fxDTSBrickData* mDataBlock;

	static void initPersistFields();

	static StringTableEntry sprayCanDivisionName[16];
	static S32              sprayCanDivisionSlot[64];
	static ColorF           colorIDTable[64];
	static S32              brickCount;

	DECLARE_CONOBJECT(fxDTSBrick);
};

#endif // _FXDTSBRICK_H_
