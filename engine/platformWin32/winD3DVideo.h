//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _WIND3DVIDEO_H_
#define _WIND3DVIDEO_H_

#ifndef _PLATFORMVIDEO_H_
#include "platform/platformVideo.h"
#endif

class D3DDevice : public DisplayDevice
{
      static bool smCanSwitchBitDepth;

      bool mRestoreGamma;
      U16  mOriginalRamp[256*3];

   public:
      D3DDevice();

      void initDevice();
      bool activate( U32 width, U32 height, U32 bpp, bool fullScreen );
      void shutdown();
      void destroy();
      bool setScreenMode( U32 width, U32 height, U32 bpp, bool fullScreen, bool forceIt = false, bool repaint = true );
      void swapBuffers();
      const char* getDriverInfo();
      bool getGammaCorrection(F32 &g);
      bool setGammaCorrection(F32 g);
      bool setVerticalSync( bool on );

      static DisplayDevice* create();

      static bool smStay16;
};

#endif // _H_WIND3DVIDEO
