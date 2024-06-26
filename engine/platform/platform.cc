#include "platform/platform.h"
#include "console/console.h"
#include "console/consoleTypes.h"
#include  "platform/platformMutex.h"

S32 sgBackgroundProcessSleepTime = 25;
S32 sgTimeManagerProcessInterval = 0;

void Platform::initConsole()
{
   Con::addVariable("Pref::backgroundSleepTime", TypeS32, &sgBackgroundProcessSleepTime);
   Con::addVariable("Pref::timeManagerProcessInterval", TypeS32, &sgTimeManagerProcessInterval);
}

S32 Platform::getBackgroundSleepTime()
{
   return sgBackgroundProcessSleepTime;
}
