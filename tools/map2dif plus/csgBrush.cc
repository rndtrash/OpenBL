//-----------------------------------------------------------------------------
// Torque Game Engine
// 
// Copyright (c) 2003 GarageGames.Com
//-----------------------------------------------------------------------------

#include "dgl/gBitmap.h"
#include "map2dif plus/csgBrush.h"
#include "core/tokenizer.h"

extern int gQuakeVersion;

// For the moment these exist in InteriorMap.cc
// MDFFIX: clean this up
extern F32	baseaxis[18][3];
extern void textureAxisFromPlane(const VectorF& normal, F32* xv, F32* yv);
extern void quakeTextureVecs(const VectorF& normal, F32 offsetX, F32 offsetY, F32 rotate, F32 scaleX, F32 scaleY, PlaneF* u, PlaneF *v);

void CopyConvexToCSG(ConvexBrush* convex, CSGBrush* csg)
{
   for (U32 i = 0; i < convex->mFaces.mPolyList.size(); i++)
   {
      PlaneF normal = convex->mFaces.mPlaneList[convex->mFaces.mPolyList[i].plane];
      U32 planeEQIndex = gWorkingGeometry->insertPlaneEQ(Point3D(normal.x, normal.y, normal.z), normal.d);

      CSGPlane* rPlane;

      if (i < csg->mPlanes.size() && planeEQIndex == csg->mPlanes[i].planeEQIndex)
         rPlane = &csg->mPlanes[i];
      else
      {
         csg->mPlanes.increment();
         rPlane = &csg->mPlanes.last();
         rPlane->planeEQIndex = planeEQIndex;
         rPlane->pTextureName = NULL;
         rPlane->flags        = 0;
      }

      // Copy over the windings
      rPlane->winding.numIndices = convex->mFaces.mPolyList[i].vertexCount;
      rPlane->winding.numNodes = 0;
      rPlane->winding.numZoneIds = 0;

      for (U32 j = 0; j < convex->mFaces.mPolyList[i].vertexCount; j++)
      {
         U32 idx = convex->mFaces.mIndexList[convex->mFaces.mPolyList[i].vertexStart + j];
         Point3F pt = convex->mFaces.mVertexList[idx];

         U32 vdx = gWorkingGeometry->insertPoint(Point3D(pt.x, pt.y, pt.z));
         rPlane->winding.indices[j] = vdx;
      }
   }

   // Set the brush bounds
   csg->mMinBound.set(1e10, 1e10, 1e10);
	csg->mMaxBound.set(-1e10, -1e10, -1e10);

   Box3F bounds = convex->mBounds;
   csg->mMinBound.setMin(Point3D(bounds.min.x, bounds.min.y, bounds.min.z));
   csg->mMaxBound.setMax(Point3D(bounds.max.x, bounds.max.y, bounds.max.z));

   // Set the map bounds
   gWorkingGeometry->mMinBound.setMin(Point3D(bounds.min.x, bounds.min.y, bounds.min.z));
   gWorkingGeometry->mMaxBound.setMax(Point3D(bounds.max.x, bounds.max.y, bounds.max.z));
}

void CopyCSGToConvex(CSGBrush* csg, ConvexBrush* convex)
{
   // Add all of the planes
   // These will match up
   for (U32 i = 0; i < csg->mPlanes.size(); i++)
   {
      CSGPlane* rPlane = &csg->mPlanes[i];
      const PlaneEQ& rPlaneEQ = gWorkingGeometry->getPlaneEQ(rPlane->planeEQIndex);
      PlaneF plane(rPlaneEQ.normal.x, rPlaneEQ.normal.y, rPlaneEQ.normal.z, rPlaneEQ.dist);

      convex->addPlane(plane);
   }

   // Now process it
   convex->processBrush();
}

U32 addTexGen(PlaneF tGenX, PlaneF tGenY)
{
   // add it...
   bool found = false;
   for (U32 i = 0; i < gWorkingGeometry->mTexGenEQs.size(); i++) 
   {
      if (gWorkingGeometry->mTexGenEQs[i].planeX == tGenX && gWorkingGeometry->mTexGenEQs[i].planeX.d == tGenX.d &&
            gWorkingGeometry->mTexGenEQs[i].planeY == tGenY && gWorkingGeometry->mTexGenEQs[i].planeY.d == tGenY.d)
      return i;
   }

   gWorkingGeometry->mTexGenEQs.increment();
   gWorkingGeometry->mTexGenEQs.last().planeX = tGenX;
   gWorkingGeometry->mTexGenEQs.last().planeY = tGenY;
   return gWorkingGeometry->mTexGenEQs.size() - 1;
}

bool parseBrush (CSGBrush& rBrush, Tokenizer* pToker, EditGeometry& geom)
{
   while (pToker->getToken()[0] == '(') {
      // Enter the plane...
      F64 points[3][3];
      for (S32 i = 0; i < 3; i++) {
         if (pToker->getToken()[0] != '(')
            goto EntityBrushlistError;

         for (S32 j = 0; j < 3; j++) {
            pToker->advanceToken(false);
            points[i][j] = dAtof(pToker->getToken());
         }

         pToker->advanceToken(false);
         if (pToker->getToken()[0] != ')')
            goto EntityBrushlistError;
         pToker->advanceToken(false);
      }

      CSGPlane& rPlane = rBrush.constructBrushPlane(Point3D(points[0][0], points[0][1], points[0][2]),
                                                    Point3D(points[1][0], points[1][1], points[1][2]),
                                                    Point3D(points[2][0], points[2][1], points[2][2]));

      // advanced already...
      if (pToker->tokenAvailable() == false)
         goto EntityBrushlistError;
      rPlane.pTextureName = geom.insertTexture(pToker->getToken());

      U32 bmIndex = geom.getMaterialIndex(rPlane.pTextureName);
      const GBitmap* pBitmap = geom.mTextures[bmIndex];

      PlaneF tGenX;
      PlaneF tGenY;

      if (gQuakeVersion == 2)
      {
         //hl ( 564 -396 -672 ) ( 564 -372 -672 ) ( 512 -372 -672 ) WALL_BLOCK01 [ 1 0 0 -213 ] [ 0 -1 0 -117 ] 0 1 1

         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         if (pToker->getToken()[0] != '[')         goto EntityBrushlistError;
   
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         tGenX.x = dAtof(pToker->getToken());
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         tGenX.y = dAtof(pToker->getToken());
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         tGenX.z = dAtof(pToker->getToken());
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         tGenX.d = dAtof(pToker->getToken());

         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         if (pToker->getToken()[0] != ']')         goto EntityBrushlistError;
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         if (pToker->getToken()[0] != '[')         goto EntityBrushlistError;

         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         tGenY.x = dAtof(pToker->getToken());
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         tGenY.y = dAtof(pToker->getToken());
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         tGenY.z = dAtof(pToker->getToken());
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         tGenY.d = dAtof(pToker->getToken());
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         if (pToker->getToken()[0] != ']')         goto EntityBrushlistError;

         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         F32 scaleX = dAtof(pToker->getToken());
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         F32 scaleY = dAtof(pToker->getToken());

         tGenX.x /= scaleX * F32(pBitmap->getWidth());
         tGenX.y /= scaleX * F32(pBitmap->getWidth());
         tGenX.z /= scaleX * F32(pBitmap->getWidth());
         tGenX.d /= F32(pBitmap->getWidth());

         tGenY.x /= scaleY * F32(pBitmap->getHeight());
         tGenY.y /= scaleY * F32(pBitmap->getHeight());
         tGenY.z /= scaleY * F32(pBitmap->getHeight());
         tGenY.d /= F32(pBitmap->getHeight());
      }
      else if (gQuakeVersion == 3)
      {
         //q1 ( 88 -40 8 ) ( -56 -40 8 ) ( -56 -64 8 ) block10d 0 0 0 1.000000 1.000000
         //q2 ( 88 -40 8 ) ( -56 -40 8 ) ( -56 -64 8 ) block10d 0 0 0 1.000000 1.000000
         //q3 ( 88 -40 8 ) ( -56 -40 8 ) ( -56 -64 8 ) block10d 0 0 0 0.500000 0.500000 0 0 0

         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         F32 shiftU = dAtof (pToker->getToken());
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         F32 shiftV = dAtof (pToker->getToken());

         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         F32 rot = dAtof (pToker->getToken());

         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         F32 scaleX = dAtof(pToker->getToken());
         if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         F32 scaleY = dAtof(pToker->getToken());

         // Skip last 3 tokens
         //if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         //if (pToker->advanceToken(false) == false) goto EntityBrushlistError;
         //if (pToker->advanceToken(false) == false) goto EntityBrushlistError;

         pToker->advanceToken(false);
         pToker->advanceToken(false);
         pToker->advanceToken(false);

         // Compute the normal
         VectorF normal;
         VectorF t1, t2;

         t1 = VectorF (points[0][0], points[0][1], points[0][2]) - VectorF (points[1][0], points[1][1], points[1][2]);
         t2 = VectorF (points[2][0], points[2][1], points[2][2]) - VectorF (points[1][0], points[1][1], points[1][2]);
         mCross (t1, t2, normal);
         normal.normalize ();

         quakeTextureVecs (normal, shiftU, shiftV, rot, scaleX, scaleY, &tGenX, &tGenY);

         F32 width = F32(pBitmap->getWidth());
         F32 height = F32(pBitmap->getHeight());

         tGenX.x /= F32(pBitmap->getWidth());
         tGenX.y /= F32(pBitmap->getWidth());
         tGenX.z /= F32(pBitmap->getWidth());
         tGenX.d /= F32(pBitmap->getWidth());

         tGenY.x /= F32(pBitmap->getHeight());
         tGenY.y /= F32(pBitmap->getHeight());
         tGenY.z /= F32(pBitmap->getHeight());
         tGenY.d /= F32(pBitmap->getHeight());
      }
      else
      {
         goto EntityBrushlistError;
      }

      // add it...
      bool found = false;
      for(U32 i = 0; !found && (i < geom.mTexGenEQs.size()); i++) 
         if(!dMemcmp(&geom.mTexGenEQs[i].planeX, &tGenX, sizeof(PlaneF)) &&
            !dMemcmp(&geom.mTexGenEQs[i].planeY, &tGenY, sizeof(PlaneF)))
         {
            found = true;
            rPlane.texGenIndex = i;
         }

      //
      if(!found)
      {
         geom.mTexGenEQs.increment();
         geom.mTexGenEQs.last().planeX = tGenX;
         geom.mTexGenEQs.last().planeY = tGenY;
         rPlane.texGenIndex = geom.mTexGenEQs.size() - 1;
      }

      pToker->advanceToken(true);
   }

   return true;

EntityBrushlistError:
   return false;
}

U32 CSGBrush::addPlane(CSGPlane plane)
{
   for (U32 i = 0; i < mPlanes.size(); i++)
   {
      if (mPlanes[i].flags == plane.flags &&
          mPlanes[i].owningEntity == plane.owningEntity &&
          mPlanes[i].planeEQIndex == plane.planeEQIndex &&
          mPlanes[i].pTextureName == plane.pTextureName &&
          mPlanes[i].texGenIndex == plane.texGenIndex)
          return i;
   }

   mPlanes.increment();
   mPlanes.last() = plane;
   return mPlanes.size() - 1;
}

void CSGPlane::construct(const Point3D& Point1, const Point3D& Point2, const Point3D& Point3)
{
   // |yi zi 1|     |xi zi 1|     |xi yi 1|     |xi yi zi|
   // |yj zj 1| x + |xj zj 1| y + |xj yj 1| z = |xj yj zj|
   // |yk zk 1|     |xk zk 1|     |xk yk 1|     |xk yk zk|
   //
   Point3D normal;
   F64  dist;

   normal.x = Point1.y * Point2.z - Point1.y * Point3.z +
              Point3.y * Point1.z - Point2.y * Point1.z +
              Point2.y * Point3.z - Point3.y * Point2.z;
   normal.y = Point1.x * Point2.z - Point1.x * Point3.z +
              Point3.x * Point1.z - Point2.x * Point1.z +
              Point2.x * Point3.z - Point3.x * Point2.z;
   normal.z = Point1.x * Point2.y - Point1.x * Point3.y +
              Point3.x * Point1.y - Point2.x * Point1.y +
              Point2.x * Point3.y - Point3.x * Point2.y;
   dist     = Point1.x * Point2.y * Point3.z - Point1.x * Point2.z * Point3.y +
              Point1.y * Point2.z * Point3.x - Point1.y * Point2.x * Point3.z +
              Point1.z * Point2.x * Point3.y - Point1.z * Point2.y * Point3.x;

   normal.x = -normal.x;
   normal.z = -normal.z;
   
   //
   planeEQIndex = gWorkingGeometry->insertPlaneEQ(normal, dist);
   flags = 0;
}

//------------------------------------------------------------------------------
// Needs to insert new plane equation.  parameter is positive amount 
// to extrude outward.
void CSGPlane::extrude(F64 byAmount)
{
   const PlaneEQ & eq = gWorkingGeometry->getPlaneEQ(planeEQIndex);
   planeEQIndex = gWorkingGeometry->insertPlaneEQ(eq.normal, eq.dist - byAmount);
}

//------------------------------------------------------------------------------

bool CSGPlane::createBaseWinding(const Vector<U32>& rPoints)
{
   return ::createBaseWinding(rPoints, getNormal(), &winding);
}


//------------------------------------------------------------------------------

// Try a more accurate version of this.  
PlaneSide CSGPlane::sideCheckEpsilon(const Point3D& testPoint, F64 epsilon) const
{
   F64 distance = distanceToPlane(testPoint);
   if (distance < - epsilon)
      return PlaneBack;
   else if (distance > epsilon)
      return PlaneFront;
   else
      return PlaneOn;
}

// Need more info for asserts.  
const char* CSGPlane::sideCheckInfo(const Point3D & P, const char * msg, F64 E) const
{
   F64            D = distanceToPlane(P);
   const char *   txt = D < -E ? "BACK" : (D > E ? "FRONT" : "ON");
   return avar( "%s:  Side=%s E=%lf D=%1.20lf P=(%lf,%lf,%lf)", msg, txt, E, D, P.x, P.y, P.z );
}

// See if this plane lies along an edge of the supplied winding.  Return the 
// points if so (they're needed), else return NULL for false.  
//
// Maybe need a better point here.  
Point3D * CSGPlane::sharesEdgeWith(const Winding & W) const
{
   static Point3D    edgePts[2];
   for( U32 i = 0; i < W.numIndices; i++ )
   {
      edgePts[0] = gWorkingGeometry->getPoint(W.indices[i]);
      // if( sideCheckEpsilon( edgePts[0] ) == PlaneOn )
      if( whichSide( edgePts[0] ) == PlaneOn )
      {
         edgePts[1] = gWorkingGeometry->getPoint(W.indices[(i + 1) % W.numIndices]);
         // if( sideCheckEpsilon( edgePts[1] ) == PlaneOn )
         if( whichSide( edgePts[1] ) == PlaneOn )
            return edgePts;
      }
   }
   return NULL;
}


// Assuming this is a brush that has already been selfClip()d, make 
//    and return a new brush that is extruded version of this 
//       one (tho not clipped) in given direction.  
// 
// If doBoth, then extrude also in negative of direction.  
//
// Adds additional planes on those edges which are boundaries with 
//    respect to the direction axis.  That is, those edges which will
//    will be on the outside of the shape perimeter when viewing
//    along the given direction axis, and so planes need to be added
//    to limit unwanted extrusion outside of the intended axis.  
// 
CSGBrush* CSGBrush::createExtruded(const Point3D & extrudeDir, bool doBoth) const
{
   CSGBrush * newBrush = gBrushArena.allocateBrush();
   
   newBrush->copyBrush( this );

   for( U32 i = 0; i < mPlanes.size(); i++ )
   {
      // Get extrusion component along normal.  
      const Point3D &   curNormal = mPlanes[i].getNormal();
      F64               extrudeComponent = mDot( extrudeDir, curNormal );

      if( mFabsD(extrudeComponent) > 0.001 ) 
      {
         const Winding &   curWinding = mPlanes[i].winding;
         
         // Look for planes that are opposite with respect to this 
         // direction and which have a shared edge.  We create 'clamping'
         // planes along such edges to avoid spikes.  
         for( U32 j = i + 1; j < mPlanes.size(); j++ )
         {
            if( mDot(mPlanes[j].getNormal(),extrudeDir) * extrudeComponent < -0.0001 )
            {
               if( Point3D * edgePts = mPlanes[j].sharesEdgeWith(curWinding) )
               {
                  Point3D  other = edgePts[0] + extrudeDir;
                  CSGPlane & rPlane = (extrudeComponent > 0) 
                                       ?
                     newBrush->constructBrushPlane( other, edgePts[0], edgePts[1] )
                                       :
                     newBrush->constructBrushPlane( edgePts[1], edgePts[0], other );
                        
                  rPlane.pTextureName = mPlanes[0].pTextureName;
                  rPlane.xShift = 0;
                  rPlane.yShift = 0;
                  rPlane.rotation = 0;
                  rPlane.xScale = rPlane.yScale = 1.0;
               }
            }
         }
         
         if( extrudeComponent > 0 || doBoth )
            newBrush->mPlanes[i].extrude( mFabsD( extrudeComponent ) );
      }
   }
   
   AssertFatal( newBrush->mPlanes.size() >= mPlanes.size(), 
         "CSGBrush::createExtruded(): new brush shouldn't be smaller" );
         
   return newBrush;
}


//------------------------------------------------------------------------------
bool CSGBrush::intersectPlanes(U32 i, U32 j, U32 k,
                               Point3D* pOutput)
{
   AssertFatal(i < mPlanes.size() && j < mPlanes.size() && k < mPlanes.size() &&
               i != j && i != k && j != k, "CSGBrush::intersectPlanes: bad plane indices");

   CSGPlane& rPlane1 = mPlanes[i];
   CSGPlane& rPlane2 = mPlanes[j];
   CSGPlane& rPlane3 = mPlanes[k];

   const PlaneEQ& rPlaneEQ1 = gWorkingGeometry->getPlaneEQ(rPlane1.planeEQIndex);
   const PlaneEQ& rPlaneEQ2 = gWorkingGeometry->getPlaneEQ(rPlane2.planeEQIndex);
   const PlaneEQ& rPlaneEQ3 = gWorkingGeometry->getPlaneEQ(rPlane3.planeEQIndex);

   F64 bc  = (rPlaneEQ2.normal.y * rPlaneEQ3.normal.z) - (rPlaneEQ3.normal.y * rPlaneEQ2.normal.z);
   F64 ac  = (rPlaneEQ2.normal.x * rPlaneEQ3.normal.z) - (rPlaneEQ3.normal.x * rPlaneEQ2.normal.z);
   F64 ab  = (rPlaneEQ2.normal.x * rPlaneEQ3.normal.y) - (rPlaneEQ3.normal.x * rPlaneEQ2.normal.y);
   F64 det = (rPlaneEQ1.normal.x * bc) - (rPlaneEQ1.normal.y * ac) + (rPlaneEQ1.normal.z * ab);

   if (mFabs(det) < 1e-7) {
      // Parallel planes
      return false;
   }

   F64 dc     = (rPlaneEQ2.dist * rPlaneEQ3.normal.z) - (rPlaneEQ3.dist * rPlaneEQ2.normal.z);
   F64 db     = (rPlaneEQ2.dist * rPlaneEQ3.normal.y) - (rPlaneEQ3.dist * rPlaneEQ2.normal.y);
   F64 ad     = (rPlaneEQ3.dist * rPlaneEQ2.normal.x) - (rPlaneEQ2.dist * rPlaneEQ3.normal.x);
   F64 detInv = 1.0 / det;

   pOutput->x = ((rPlaneEQ1.normal.y * dc) - (rPlaneEQ1.dist     * bc) - (rPlaneEQ1.normal.z * db)) * detInv;
   pOutput->y = ((rPlaneEQ1.dist     * ac) - (rPlaneEQ1.normal.x * dc) - (rPlaneEQ1.normal.z * ad)) * detInv;
   pOutput->z = ((rPlaneEQ1.normal.y * ad) + (rPlaneEQ1.normal.x * db) - (rPlaneEQ1.dist     * ab)) * detInv;

   return true;
}


//------------------------------------------------------------------------------
bool CSGBrush::selfClip()
{
   // MDFFIX: Really hacky way of doing it for now but it will work
   ConvexBrush brush;

   CopyCSGToConvex(this, &brush);

   CopyConvexToCSG(&brush, this);

   return true;
}

void CSGBrush::copyBrush(const CSGBrush* pCopy)
{
   mPlanes      = pCopy->mPlanes;
   mIsAmbiguous = pCopy->mIsAmbiguous;
   mBrushType   = pCopy->mBrushType;
   mMinBound    = pCopy->mMinBound;
   mMaxBound    = pCopy->mMaxBound;

   brushId      = pCopy->brushId;
   materialType = pCopy->materialType;
}

//------------------------------------------------------------------------------
bool CSGBrush::disambiguate()
{
   AssertFatal(mIsAmbiguous == false, "error, already disambiguated?");

   for (U32 i = 0; i < mPlanes.size(); i++) {
      for (U32 j = i + 1; j < mPlanes.size();) {
         // Compare i to j.  if j == i, with different tex parameters, increment
         //  ambiguous mInteriorRes->mBrushes (once only), and remove the plane.
         //
         CSGPlane& rPlane1 = mPlanes[i];
         CSGPlane& rPlane2 = mPlanes[j];
         if (rPlane1.planeEQIndex == rPlane2.planeEQIndex) {
            // Possible ambiguous plane pairing...
            //
            if (rPlane1.pTextureName != rPlane2.pTextureName ||
                rPlane1.xShift       != rPlane2.xShift       ||
                rPlane1.yShift       != rPlane2.yShift       ||
                rPlane1.rotation     != rPlane2.rotation     ||
                rPlane1.xScale       != rPlane2.xScale       ||
                rPlane1.yScale       != rPlane2.yScale) {
               mIsAmbiguous = true;
            } else {
               // Same texture parameters, should be fine, just erase it...
               //
            }

            mPlanes.erase(j);
         } else {
            // Plane is fine...
            j++;
         }
      }
   }

   return mIsAmbiguous;
}


//------------------------------------------------------------------------------
/*CSGPlane& CSGBrush::constructBrushPlane(const Point3I& rPoint1,
                                        const Point3I& rPoint2,
                                        const Point3I& rPoint3)
{
   mPlanes.increment();
   CSGPlane& rPlane = mPlanes.last();
   rPlane.flags = 0;
   rPlane.owningEntity = NULL;
   rPlane.winding.numNodes = 0;
   rPlane.winding.numZoneIds = 0;

   Point3D Point1(rPoint1.x, rPoint1.y, rPoint1.z);
   Point3D Point2(rPoint2.x, rPoint2.y, rPoint2.z);
   Point3D Point3(rPoint3.x, rPoint3.y, rPoint3.z);

   rPlane.construct(Point1, Point2, Point3);
   
   if (rPlane.getNormal().x == 0 && rPlane.getNormal().y == 0 && rPlane.getNormal().z == 0) {
      AssertISV(false, "Error, degenerate plane (colinear points)");
      mPlanes.decrement();
      return *((CSGPlane*)NULL);
   }
   
   return rPlane;
}*/
CSGPlane& CSGBrush::constructBrushPlane(const Point3D& Point1,
                                        const Point3D& Point2,
                                        const Point3D& Point3)
{
   mPlanes.increment();
   CSGPlane& rPlane = mPlanes.last();
   rPlane.flags = 0;
   rPlane.owningEntity = NULL;
   rPlane.winding.numNodes = 0;
   rPlane.winding.numZoneIds = 0;

   rPlane.construct(Point1, Point2, Point3);

   if (rPlane.getNormal().x == 0 && rPlane.getNormal().y == 0 && rPlane.getNormal().z == 0) {
      AssertISV(false, "Error, degenerate plane (colinear points)");
      mPlanes.decrement();
      return *((CSGPlane*)NULL);
   }

   return rPlane;
}

bool CSGBrush::isEquivalent(const CSGBrush& testBrush) const
{
   for (U32 i = 0; i < mPlanes.size(); i++) {
      U32 j;
      for (j = 0; j < testBrush.mPlanes.size(); j++) {
         if (testBrush.mPlanes[j].planeEQIndex == mPlanes[i].planeEQIndex)
            break;
      }

      if (j == testBrush.mPlanes.size())
         return false;
   }

   return true;
}
