//-----------------------------------------------------------------------------
// Torque Game Engine
// 
// Copyright (c) 2003 Black Blood Studios
// Copyright (c) 2001 GarageGames.Com
// Portions Copyright (c) 2001 by Sierra Online, Inc.
//-----------------------------------------------------------------------------
// This release by: Xavier Amado (xavier@blackbloodstudios.com)
// Credits to : Vinci_Smurf, Loonatik, Matt Webster, Frank Bignone, Xavier Amado
//-----------------------------------------------------------------------------

#include "gui/core/guiCanvas.h"
#include "gui/controls/guiObjectView.h"
#include "console/consoleTypes.h"
#include "sceneGraph/sceneGraph.h"
extern SceneGraph* gClientSceneGraph;

static const F32 MaxOrbitDist = 50.0f;
static const S32 MaxAnimations = 6;

IMPLEMENT_CONOBJECT( GuiObjectView );

GuiObjectView::GuiObjectView() : GuiTSCtrl()
{
	mActive = true;

	mMouseState = None;

	// Can zoom and spin by default
	mZoom = true;
	mSpin = true;
	mLastMousePoint.set( 0, 0 );
	mLightDirection = VectorF(-0.57735f, -0.57735f, -0.57735f);
	mLightColor = ColorF(0.6f, 0.58f, 0.5f);
	mAmbientColor = ColorF(0.3f, 0.3f, 0.3f);
	mCameraRot.set(0,0,10);
	mCameraOffset.set(0,0,0);
}

GuiObjectView::~GuiObjectView()
{
	mMeshObjects.Clear();
}

// Script function handling for "setMouse"
ConsoleMethod( GuiObjectView, setMouse, void, 4, 4, "ObjectView.setMouse(canZoom, canSpin)" ) {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>( object );
	view->setMouseOptions(dAtob(argv[2]), dAtob(argv[3]));
}

// Script function handling for "setObject"
ConsoleMethod( GuiObjectView, setObject, void, 6, 6, "ObjectView.setObject(name, model, skin, lod)" )
{
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>( object );
	view->loadObject(argv[2], argv[3], argv[4], "", "", dAtoi(argv[5]));
	view->setCamera();
}

// Script function handling for "mountObject"
ConsoleMethod( GuiObjectView, mountObject, void, 8, 8, "ObjectView.mountObject(name, model, skin, parentName, nodeName, lod)" ) {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>( object );
	view->loadObject(argv[2], argv[3], argv[4], argv[5], argv[6] ,dAtoi(argv[7]));
}

// Script function handling for "unMountObject"
ConsoleMethod( GuiObjectView, unMountObject, void, 4, 4, "ObjectView.unMountObject(name, node)" ) {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>( object );
	view->unLoadObject(argv[2], argv[3]);
}

// Script function handling for "setEmpty"
ConsoleMethod( GuiObjectView, setEmpty, void, 2, 2, "ObjectView.setEmpty( )" ) {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>( object );
	view->Clear();
}

// Script function handling for "loadDSQ"
ConsoleMethod( GuiObjectView, loadDSQ, void, 4, 4, "ObjectView.loadDSQ(name, dsq)" ) {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>( object );
	view->loadDSQ(argv[2], argv[3]);
}

// Script function handling for "setSequence"
ConsoleMethod( GuiObjectView, setSequence, void, 6, 6, "ObjectView.setSequence(name, threadNum, seq, time)" ) {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>( object );
	view->setSequence(argv[2], dAtoi(argv[3]), argv[4], dAtof(argv[5]));
}

// Script function handling for "setCamera"
ConsoleMethod( GuiObjectView, setCamera, void, 6, 6, "ObjectView.setCamera(x, y, z, dist)" ){
	argc;   GuiObjectView* view = static_cast<GuiObjectView*>( object );
	view->setCamera(dAtof(argv[2]), dAtof(argv[3]), dAtof(argv[4]), dAtof(argv[5]));
}

// Script function handling for "hideNode"
ConsoleMethod(GuiObjectView, hideNode, void, 4, 4, "ObjectView.hideNode(object name, node name)") {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>(object);
	view->hideNode(argv[2], argv[3]);
}

// Script function handling for "unHideNode"
ConsoleMethod(GuiObjectView, unHideNode, void, 4, 4, "ObjectView.unHideNode(object name, node name)") {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>(object);
	view->unHideNode(argv[2], argv[3]);
}

// Script function handling for "setNodeColor"
ConsoleMethod(GuiObjectView, setNodeColor, void, 5, 5, "ObjectView.setIflFrame(object name, nodeName, node color)") {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>(object);

	F32 r, g, b, a;
	dSscanf(argv[4], "%f %f %f %f", &r, &g, &b, &a);

	view->setNodeColor(argv[2], argv[3], ColorF(r, g, b, a));
}

// Script function handling for "setNodeColor"
ConsoleMethod(GuiObjectView, setIflFrame, void, 5, 5, "ObjectView.setIflFrame(object name, iflName, frame)") {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>(object);
	int frame = dAtoi(argv[4]);
	view->setIflFrame(argv[2], argv[3], frame);
}

// Script function handling for "dumpView"
ConsoleMethod(GuiObjectView, dumpView, void, 2, 2, "ObjectView.dumpView") {
	GuiObjectView* view = static_cast<GuiObjectView*>(object);
	view->dumpView();
}

// Script function handling for "setOrbitDist"
ConsoleMethod(GuiObjectView, setOrbitDist, void, 3, 3, "ObjectView.setOrbitDist(float dist)") {
	argc;
	F32 dist = dAtof(argv[2]);
	GuiObjectView* view = static_cast<GuiObjectView*>(object);
	view->setOrbitDist(dist);
}

// Script function handling for "setCameraRot"
ConsoleMethod(GuiObjectView, setCameraRot, void, 5, 5, "ObjectView.setCameraRot(float x, float y, float z)") {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>(object);

	F32 x = dAtof(argv[2]);
	F32 y = dAtof(argv[3]);
	F32 z = dAtof(argv[4]);

	view->setCameraRot(x, y, z);
}

// Script function handling for "setThreadPos"
ConsoleMethod(GuiObjectView, setThreadPos, void, 5, 5, "ObjectView.setThreadPos(name, threadNum, threadPos)") {
	argc;
	GuiObjectView* view = static_cast<GuiObjectView*>(object);
	int threadNum = dAtoi(argv[3]);
	float threadPos = dAtof(argv[4]);
	view->setThreadPos(argv[2], threadNum, threadPos);
}

ConsoleMethod( GuiObjectView, setCameraOffset, void, 5, 5, "ObjectView.setCameraOffset(x, y, z)" ){
	argc;   GuiObjectView* view = static_cast<GuiObjectView*>( object );
	view->setCameraOffset(dAtof(argv[2]), dAtof(argv[3]), dAtof(argv[4]));
}

void GuiObjectView::consoleInit()
{

}

void GuiObjectView::initPersistFields()
{
	Parent::initPersistFields();
	addField("lightDirection",   TypePoint3F, Offset(mLightDirection,   GuiObjectView));
	addField("lightColor",   TypeColorF, Offset(mLightColor,   GuiObjectView));
	addField("ambientColor",   TypeColorF, Offset(mAmbientColor,   GuiObjectView));
}

bool GuiObjectView::onWake()
{
	if ( !Parent::onWake() )
		return( false );
      
	mCameraMatrix.identity();
	mCameraRot.set( 0, 0, 3.9 );
	mCameraPos.set( 0, 1.75, 1.25 );
	mCameraMatrix.setColumn( 3, mCameraPos );
	mOrbitPos.set( 0, 0, 0 );
	mOrbitDist = 9.5f;

	return( true );
}

// Function to determine the ways the mouse can interact with the gui object
void GuiObjectView::setMouseOptions( bool zoom, bool spin  )
{
	mZoom = zoom;
	mSpin = spin;
}

// Mouse is down, lock mouse input and get the mouse pointer coordinates and set mode to spin
void GuiObjectView::onMouseDown( const GuiEvent &event )
{
	if ( !mActive || !mVisible || !mAwake || !mSpin )
		return;

	mMouseState = Rotating;

	mLastMousePoint = event.mousePoint;
	mouseLock();
}

// Mouse is up, unlock mouse input
void GuiObjectView::onMouseUp( const GuiEvent &/*event*/ )
{
	mouseUnlock();
	mMouseState = None;
}

// If mouse is dragged, adjust camera position accordingly. Makes model rotate
void GuiObjectView::onMouseDragged( const GuiEvent &event )
{
	if ( mMouseState != Rotating )
		return;

	Point2I delta = event.mousePoint - mLastMousePoint;
	mLastMousePoint = event.mousePoint;

	mCameraRot.x += ( delta.y * 0.01 );
	mCameraRot.z += ( delta.x * 0.01 );
}

// Right mouse is down, lock mouse input and get the mouse pointer coordinates and set mode to zoom
void GuiObjectView::onRightMouseDown( const GuiEvent &event )
{
	if ( !mActive || !mVisible || !mAwake || !mZoom )
		return;

	mMouseState = Zooming;

	mLastMousePoint = event.mousePoint;
	mouseLock();
}

// Right mouse is up, unlock mouse input
void GuiObjectView::onRightMouseUp( const GuiEvent &/*event*/ )
{
	mouseUnlock();
	mMouseState = None;
}

// If mouse is dragged, adjust camera position accordingly. Makes model zoom
void GuiObjectView::onRightMouseDragged( const GuiEvent &event )
{
	if ( mMouseState != Zooming )
		return;

	S32 delta = event.mousePoint.y - mLastMousePoint.y;
	mLastMousePoint = event.mousePoint;

	mOrbitDist += ( delta * 0.01 ); 
}

//-----------------------------------------------------------
void GuiObjectView::setCamera()
{
	// Make sure there is a main object in the scene
	if (mMeshObjects.mMainObject)
	{
		// Initialize camera values:
		mOrbitPos = mMeshObjects.mMainObject->getShape()->center;
		mMinOrbitDist = mMeshObjects.mMainObject->getShape()->radius;
		mOrbitDist = mMinOrbitDist + 1;
	}
}
void GuiObjectView::setCamera(F32 rX, F32 rY, F32 rZ, F32 dist)
{
	// Make sure there is a main object in the scene
	if (mMeshObjects.mMainObject)
	{
		F32 degToRad = (M_PI / 180);		
		mCameraRot.x = rX * degToRad;
		mCameraRot.y = rY * degToRad;
		mCameraRot.z = rZ * degToRad;
		// Initialize camera values:
		mOrbitPos = mMeshObjects.mMainObject->getShape()->center;
		mMinOrbitDist = mMeshObjects.mMainObject->getShape()->radius; 
		mOrbitDist = dist > mMinOrbitDist ? dist : mMinOrbitDist;
	}
}

void GuiObjectView::setCameraOffset(F32 oX, F32 oY, F32 oZ){
	// Make sure there is a main object in the scene
	if (mMeshObjects.mMainObject)
	{
		mCameraOffset.x = oX ;
		mCameraOffset.y = oY ;
		mCameraOffset.z = oZ ;
	}
}

//------------------------------------------------------------------------------
bool GuiObjectView::processCameraQuery( CameraQuery* query )
{
	// Make sure the orbit distance is within the acceptable range:
	mOrbitDist = ( mOrbitDist < mMinOrbitDist ) ? mMinOrbitDist : ( ( mOrbitDist > MaxOrbitDist ) ? MaxOrbitDist : mOrbitDist );
	// Adjust the camera so that we are still facing the model:
	Point3F vec;
	MatrixF xRot, yRot, zRot, t;
	xRot.set( EulerF( mCameraRot.x, 0, 0 ) );
	yRot.set( EulerF( 0, 0, 0 ) );
	zRot.set( EulerF( 0, 0, mCameraRot.z ) );
	mCameraMatrix.mul( zRot, xRot );
	mCameraMatrix.mul( mCameraMatrix, yRot );
	mCameraMatrix.getColumn( 1, &vec );
	vec *= mOrbitDist;
	mCameraPos = mOrbitPos - vec;
	mCameraPos += VectorF(mCameraOffset.x, mCameraOffset.y, mCameraOffset.z);
	query->nearPlane = 0.1;
	query->farPlane = 2100.0;
	query->fov = 3.1415 / 3.5;
	mCameraMatrix.setColumn( 3, mCameraPos );
	query->cameraMatrix = mCameraMatrix;
	return( true );
}


//------------------------------------------------------------------------------
void GuiObjectView::renderWorld( const RectI &updateRect )
{
	if (!(bool)mMeshObjects.mMainObject)
		return;
	    
	glClear( GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat lightColor[]   = { mLightColor.red, mLightColor.green, mLightColor.blue, 1.f };
	GLfloat ambientColor[] = { mAmbientColor.red, mAmbientColor.green, mAmbientColor.blue, 1.f };
	GLfloat lightDir[]     = { mLightDirection.x, mLightDirection.y, mLightDirection.z, 0.f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
	glLightfv(GL_LIGHT0, GL_COLOR, lightColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

	for (S32 i=0; i<33; i++)
	{
		if (mMeshObjects.mMesh[i].mesh)
		{
			// Animate and render
			if(mMeshObjects.mMesh[i].mode == 1)
			{
				S32 time = Platform::getVirtualMilliseconds();
				S32 dt = time - mMeshObjects.mMesh[i].lastRenderTime;
				mMeshObjects.mMesh[i].lastRenderTime = time;
				F32 fdt = dt;

				for (S32 j = 0; j < sizeof(mMeshObjects.mMesh[i].threads) / sizeof(mMeshObjects.mMesh[i].threads[0]); j++)
				{
					if (mMeshObjects.mMesh[i].threads[j] != NULL)
						mMeshObjects.mMesh[i].mesh->advanceTime(fdt / 1000.f, mMeshObjects.mMesh[i].threads[j]);
				}

				mMeshObjects.mMesh[i].mesh->animate();
			}

			// If this is a mounted object transform to the correct position
			if (mMeshObjects.mMesh[i].parentNode != -1)
			{
				MatrixF mat;
				getObjectTransform( &mat, i ); 
				glPushMatrix();
				dglMultMatrix( &mat );
				mMeshObjects.mMesh[i].mesh->render();
				glPopMatrix();
			}
			else
			{
				mMeshObjects.mMesh[i].mesh->render();
			}
		}
	}
	
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable( GL_DEPTH_TEST );
	dglSetClipRect( updateRect );
	dglSetCanonicalState();
}

//-----------------------------------------------------------
void GuiObjectView::getObjectTransform( MatrixF *mat , S32 index)
{
	
	MatrixF subTrans = mMeshObjects.mMesh[index].mesh->mNodeTransforms[mMeshObjects.mMesh[index].node];
	Point3F subOffset = -subTrans.getPosition();

	S32 pIndex = mMeshObjects.mMesh[index].parentIndex;
	MatrixF parentTrans = mMeshObjects.mMesh[pIndex].mesh->mNodeTransforms[mMeshObjects.mMesh[index].parentNode];
	parentTrans.mulP( subOffset );
	parentTrans.setPosition( subOffset );
	*mat = parentTrans;
}

//-----------------------------------------------------------
void GuiObjectView::loadObject(const char* name, const char* shape, const char* skin, const char* parentName, const char* nodeName, S32 detail)
{ 
	bool main = (dStrcmp(nodeName,"") == 0);
	S32 index = 0;

	if (main)
	{
		// This is the main object clear out any meshs already loaded
		Clear();

		mMeshObjects.load(0, name, shape, skin, 0, -1, detail);
	}
	else
	{
		// Make sure there is a mMainObject
		if (!mMeshObjects.mMainObject)
		{
			Con::printf("Error: Main object not found");
			return;
		}

		// Check for target node
		S32 pNode;
		S32 pIndex;
		for(S32 i=0; i<33; i++)
		{
			if(dStrcmp(mMeshObjects.mMesh[i].name, parentName) == 0)
			{
				pNode = mMeshObjects.mMesh[i].mesh->getShape()->findNode(nodeName);
				pIndex = i;
				if(pNode == -1)
				{
					Con::printf("Error: Unable to find node %s", nodeName);
					return;
				}

				continue;
			}
		}

		// Check to see if something is already mounted to this object's mountPoint, or if the object name already exists.
		for (S32 i=0; i<33; i++)
		{
			if (mMeshObjects.mMesh[i].parentNode == pNode && mMeshObjects.mMesh[i].parentIndex == pIndex || dStrcmp(mMeshObjects.mMesh[i].name, name) == 0)
			{
				Con::printf("Unloading object %s at %s", mMeshObjects.mMesh[i].name, nodeName);
				mMeshObjects.unLoad(i);
			}
		}

		// This is a mounted object find an open spot for this mesh
		index = mMeshObjects.findOpen();
		if (index == -1)
		{
			Con::printf("Error: Maximum mountable objects reached. Please unMount an object");
			return;
		}

		mMeshObjects.load(index, name, shape, skin, pIndex, pNode, detail);
	}
}

//-----------------------------------------------------------
void GuiObjectView::unLoadObject(const char* name, const char* node)
{ 
	if (dStrcmp(name,"") != 0)
	{
		S32 index = mMeshObjects.findMeshByName(name);
		if (index != -1)
		{
			mMeshObjects.unLoad(index);
			Con::printf("Unloading object %s", name);
		}
		else
		{
			Con::printf("Error: Unable to find object %s", name);
		}
	}
	else if (dStrcmp(node,"") != 0)
	{
		S32 index = mMeshObjects.findMeshByNode(node);
		if (index != -1)
		{
			mMeshObjects.unLoad(index);
			Con::printf("Unloading object at %s", node);
		}
		else
		{
			Con::printf("Error: Unable to find object at %s", node);
		}
	}
}

void GuiObjectView::loadDSQ(const char* name, const char* dsq)
{
	S32 index = mMeshObjects.findMeshByName(name);
	if (index != -1)
	{
		mMeshObjects.mMesh[index].loadDSQ(dsq);
	}
	else
	{
		Con::printf("Error: Could not find object %s", name);
	}
}
//-----------------------------------------------------------

void GuiObjectView::setSequence(const char* name, S32 threadNum, const char* seq, F32 time)
{
	S32 index = mMeshObjects.findMeshByName(name);
	if (index != -1)
	{
		mMeshObjects.mMesh[index].setSequence(threadNum, seq, time);
	}
	else
	{
		Con::printf("Error: Could not find object %s", name);
	}
}

//-----------------------------------------------------------
void GuiObjectView::Clear()
{ 
	mMeshObjects.Clear(); 
}

//-----------------------------------------------------------
void GuiObjectView::hideNode(const char* parentName, const char* nodeName)
{
	// make sure we have a parent
	S32 index = mMeshObjects.findMeshByName(parentName);
	if (index != -1)
	{
		// make sure we can find nodes 
		S32 nodeIndex = mMeshObjects.mMainObject->getShape()->findObject(nodeName);
		if (nodeIndex != -1)
		{
			mMeshObjects.mMainObject->mHiddenNodes[nodeIndex] = true;
		}
		else
		{
			Con::printf("Error: GuiObjectView::hideNode() - node \"%s\" not found", nodeName);
			return;
		}
	}
	else
	{
		Con::printf("Error: GuiObjectView::hideNode() - Could not find object %s", parentName);
		return;
	}

	// Case for "null shape instance" is missing, I currently don't know the appropriate location for it.
}

//-----------------------------------------------------------
void GuiObjectView::unHideNode(const char* parentName, const char* nodeName)
{
	// make sure we have a parent
	S32 index = mMeshObjects.findMeshByName(parentName);
	if (index != -1)
	{
		// make sure we can find nodes 
		S32 nodeIndex = mMeshObjects.mMainObject->getShape()->findObject(nodeName);
		if (nodeIndex != -1)
		{
			mMeshObjects.mMainObject->mHiddenNodes[nodeIndex] = false;
		}
		else
		{
			Con::printf("Error: GuiObjectView::unhideNode() - node \"%s\" not found", nodeName);
			return;
		}
	}
	else
	{
		Con::printf("Error: GuiObjectView::unhideNode() - Could not find object %s", parentName);
		return;
	}

	// Case for "null shape instance" is missing, I currently don't know the appropriate location for it.
}

//-----------------------------------------------------------

void GuiObjectView::setNodeColor(const char* parentName, const char* nodeName, ColorF color)
{
	// make sure we have a parent
	S32 index = mMeshObjects.findMeshByName(parentName);
	if (index != -1)
	{
		// make sure we can find nodes 
		S32 nodeIndex = mMeshObjects.mMainObject->getShape()->findObject(nodeName);
		if (nodeIndex != -1)
		{
			mMeshObjects.mMainObject->mNodeColorData[nodeIndex] = { true, color };
		}
		else
		{
			Con::printf("Error: GuiObjectView::setNodeColor() - node \"%s\" not found", nodeName);
			return;
		}
	}
}

//-----------------------------------------------------------

void GuiObjectView::setIflFrame(const char* objectName, const char* iflName, S32 frame)
{
	char iflFileNameBuffer[256];

	S32 index = mMeshObjects.findMeshByName(objectName);
	if (index != -1)
	{
		dStrcpy(iflFileNameBuffer, iflName);
		dStrcat(iflFileNameBuffer, ".ifl");
	
		if (mMeshObjects.mMainObject->getShape()->materialList->size() == 0)
		{
			Con::errorf("Null MaterialList found!");
		}
		else
		{
			S32 iflIndex = mMeshObjects.mMainObject->getShape()->findName(iflFileNameBuffer);
			iflIndex = mMeshObjects.mMainObject->getShape()->findIflMaterial(iflIndex);
			mMeshObjects.mMainObject->mIflMaterialInstances[iflIndex].frame = frame;
		}
	}
}

//-----------------------------------------------------------

void GuiObjectView::dumpView()
{
	Con::warnf("mCameraRot = %f %f %f", this->mCameraRot.x, this->mCameraRot.y, this->mCameraRot.z);
	Con::warnf("mOrbitPos = %f %f %f", this->mOrbitPos.x, this->mOrbitPos.y, this->mOrbitPos.z);
	Con::warnf("mOrbitDist = %f", this->mOrbitDist);
	return;
}

//-----------------------------------------------------------

void GuiObjectView::setThreadPos(const char* name, S32 threadNum, F32 threadPos)
{
	S32 index = mMeshObjects.findMeshByName(name);
	if (index != -1)
	{
		mMeshObjects.mMesh[index].setThreadPos(threadNum, threadPos);
	}
	else
	{
		Con::printf("Error: Could not find object %s", name);
	}
}

//-----------------------------------------------------------

void GuiObjectView::setCameraRot(float x, float y, float z)
{
	// Make sure there is a main object in the scene
	if (mMeshObjects.mMainObject)
	{
		this->mCameraRot.x = x;
		this->mCameraRot.y = y;
		this->mCameraRot.z = z;
	}
}

//-----------------------------------------------------------

void GuiObjectView::setOrbitDist(float dist)
{
	// Make sure there is a main object in the scene
	if (mMeshObjects.mMainObject)
	{
		this->mOrbitDist = dist;
	}
}

//-----------------------------------------------------------
GuiObjectView::meshObjects::meshObjects()
{
	mMainObject = NULL;
	mDetail = 0;
}

GuiObjectView::meshObjects::~meshObjects()
{
	
}

//-----------------------------------------------------------
void GuiObjectView::meshObjects::load(S32 index, const char* name, const char* shape, const char* skin, S32 pIndex, S32 pNode, S32 detail)
{
	char fileBuffer[256];
	
	// Load the shape
	dSprintf(fileBuffer, sizeof( fileBuffer ), "%s", shape);

	// Load the shape into the ResourceManager
	Resource<TSShape> hShape = ResourceManager->load(fileBuffer);
	if (!bool(hShape))
	{
		Con::printf("Error: Unable to load: %s", shape);
		return;
	}

	// Copy the shape to mMesh
	mMesh[index].mesh = new TSShapeInstance(hShape, true);
	AssertFatal(mMesh[index].mesh, "ERROR!  Failed to load object model!");

	// Load the skin
	if(dStrcmp(skin,"") != 0)
	{
		dSprintf(fileBuffer, sizeof( fileBuffer ), "%s", skin);
		TextureHandle texture = TextureHandle(fileBuffer, MeshTexture, false);
		TSMaterialList* materialList = mMesh[index].mesh->getMaterialList();
		materialList->mMaterials[0] = texture;
	}

	// If a parent Index exists store it.
	if(pIndex)
		mMesh[index].parentIndex = pIndex;

	if (pNode == -1)
	{
		// If this is the main object setup the pointer and global detail level
		mMainObject = mMesh[index].mesh;
		mDetail = detail;	
	}
	else
	{
		// If this is a mounted object set the mountPoint node and parentNode
		mMesh[index].node = mMesh[index].mesh->getShape()->findNode("mountPoint");
		mMesh[index].parentNode = pNode;
	}

	// Set the name
	dSprintf(mMesh[index].name, sizeof( mMesh[index].name ), "%s", name);

	// Set the detail level
	mMesh[index].detail = (detail != -1) ? detail : mDetail;

	// Check the detail level to make sure LOD is valid
	U32 dlNum = mMesh[index].mesh->getNumDetails();
	if(mMesh[index].detail >= dlNum)
	{
		mMesh[index].detail = dlNum - 1;
	}
	mMesh[index].mesh->setCurrentDetail(mMesh[index].detail);

	Con::printf("Loading object %s", shape);
}

//-----------------------------------------------------------
void GuiObjectView::meshObjects::unLoad(S32 index)
{
	if (mMesh[index].mesh)
	{
		dStrcpy(mMesh[index].name, "");
		mMesh[index].mode = 0;
		mMesh[index].node = -1;
		mMesh[index].parentIndex = 0;
		mMesh[index].parentNode = -1;
		mMesh[index].detail = -1;
		mMesh[index].lastRenderTime = 0;

		for (S32 i = 0; i < sizeof(mMesh[index].threads) / sizeof(mMesh[index].threads[0]); i++)
		{
			if (mMesh[index].threads[i])
			{
				mMesh[index].mesh->destroyThread(mMesh[index].threads[i]);
				mMesh[index].threads[i] = 0;
			}
		}
		
		delete mMesh[index].mesh;
		mMesh[index].mesh = NULL;
	}
}

//-----------------------------------------------------------
S32 GuiObjectView::meshObjects::findOpen()
{
	for (S32 i = 0; i<33; i++)
	{
		if (!mMesh[i].mesh)
		{
			return i;
		}
	}

	return -1;
}

//-----------------------------------------------------------
S32 GuiObjectView::meshObjects::findMeshByName(const char* name)
{
	for (S32 i = 0; i<33; i++)
	{
		if (dStrcmp(mMesh[i].name, name) == 0)
		{
			return i;
		}
	}

	return -1;
}

//-----------------------------------------------------------
S32 GuiObjectView::meshObjects::findMeshByNode(const char* node)
{
	S32 pNode = mMainObject->getShape()->findNode(node);
	if (pNode != -1)
	{
		for (S32 i = 0; i<33; i++)
		{
			if (mMesh[i].parentNode == pNode)
			{
				return i;
			}
		}
	}

	return -1;
}

//-----------------------------------------------------------
void GuiObjectView::meshObjects::Clear()
{
	for (S32 i = 0; i < 33; i++)
	{
		unLoad(i);
	}
	
	mMainObject = NULL;
	mDetail = -1;

}

//-----------------------------------------------------------
GuiObjectView::meshObjects::meshs::meshs()
{
	mesh = NULL;
	mode = 0;
	node = -1;
	parentIndex = 0;
	parentNode = -1;
	detail = 0;
	lastRenderTime = 0;
	threads[0] = 0;
	threads[1] = 0;
	threads[2] = 0;
	threads[3] = 0;
}

GuiObjectView::meshObjects::meshs::~meshs()
{
	if (mesh)
	{
		delete mesh;
		mesh = NULL;
	}

	for (S32 i = 0; i < sizeof(threads) / sizeof(threads[0]); i++)
	{
		mesh->destroyThread(threads[i]);
		threads[i] = 0;
	}
}

//-----------------------------------------------------------
void GuiObjectView::meshObjects::meshs::loadDSQ(const char* dsq)
{
	Stream * f;
	char fileBuffer[256];

	dSprintf(fileBuffer, sizeof( fileBuffer ), "%s", dsq);

	f = ResourceManager->openStream(fileBuffer);
	if (f)
	{
		if (!mesh->getShape()->importSequences(f) || f->getStatus()!=Stream::Ok)
		{
			Con::errorf(ConsoleLogEntry::General,"Load sequence %s failed",dsq);
			return;
		}

		ResourceManager->closeStream(f);
		Con::printf("Loading dsq %s", dsq);
	}
	else
	{
		Con::printf("Error: Unable to open %s", dsq);
	}
}

//-----------------------------------------------------------
void GuiObjectView::meshObjects::meshs::setSequence(S32 threadNum, const char* seq, F32 time)
{
	S32 sequence = mesh->getShape()->findSequence(seq);

	if( sequence != -1 )
	{
		if (threads[threadNum] != NULL)
		{
			mesh->destroyThread(threads[threadNum]);
		}
		// If you found the sequence add the thread and set sequence and scale
		threads[threadNum] = mesh->addThread();
		lastRenderTime = Platform::getVirtualMilliseconds();
		mesh->setSequence(threads[threadNum], sequence, 0);
		time = mClampF(time, -1.0f, 1.0f);
		mesh->setTimeScale(threads[threadNum], time );
		mesh->setPos(threads[threadNum], 0.5);
		mode = 1;
	}
	else
	{
		Con::printf("Error: Could not locate sequence %s", seq);
	}
}


//-----------------------------------------------------------
void GuiObjectView::meshObjects::meshs::setThreadPos(S32 threadNum, F32 threadPos)
{
	TSThread* meshThread = threads[threadNum];

	if (meshThread != NULL)
	{
		mesh->setPos(meshThread, threadPos);
	}
	else
	{
		Con::printf("Error: Could not locate sequence %s");
	}
}
