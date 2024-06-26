//-----------------------------------------------------------------------------
// Torque Game Engine 
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "appTime.h"
#include <string>

namespace DTS
{

   // Default time...base pose
   AppTime AppTime::smDefaultTime(0,0);

   // equality tolerance for f64 component
   F64 AppTime::smTOL = 0.0001;

   // when converting to string, display int? display float?
   bool AppTime::smPrintInt = false;
   bool AppTime::smPrintFloat = true;

   const char * AppTime::getStr() const
   {
      if (!mBuffer)
#ifdef __APPLE__
         // mac doesn't like the cast to const char
         mBuffer = new char[64];
#else
         // non mac doesn't like the non-constant assignment
         const_cast<char*>(mBuffer) = new char[64];
#endif
      if (smPrintInt && smPrintFloat)
         sprintf(mBuffer,"%f:%i",F32(f64),u32);
      else if (smPrintInt)
         sprintf(mBuffer,"%i",u32);
      else if (smPrintFloat)
         sprintf(mBuffer,"%f",F32(f64));
      else
         return "";
      return mBuffer;
   }


}; // namespace DTS
