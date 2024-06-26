//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include <pthread.h>
#include "platform/platformThread.h"
#include "platform/platformSemaphore.h"
#include "platform/platformMutex.h"
#include <stdlib.h>

struct MacThreadData
{
   ThreadRunFunction       mRunFunc;
   S32                     mRunArg;
   Thread *                mThread;
   //void *                  mSemaphore;
   pthread_mutex_t         mMutt;
};

//-----------------------------------------------------------------------------
// Function:    ThreadRunHandler
// Summary:     Calls Thread::run() with the thread's specified run argument.
//               Neccesary because Thread::run() is provided as a non-threaded
//               way to execute the thread's run function. So we have to keep
//               track of the thread's lock here.
static void *ThreadRunHandler(void * arg)
{
   MacThreadData * threadData = reinterpret_cast<MacThreadData*>(arg);

   threadData->mThread->run(threadData->mRunArg);
   //Semaphore::releaseSemaphore(threadData->mSemaphore);
   pthread_mutex_unlock(&(threadData->mMutt));
   return NULL;
   // this is where the created pthread will die.
}
   

//-----------------------------------------------------------------------------
Thread::Thread(ThreadRunFunction func, S32 arg, bool start_thread)
{
   MacThreadData * threadData = new MacThreadData();
   threadData->mRunFunc = func;
   threadData->mRunArg = arg;
   threadData->mThread = this;
//   threadData->mSemaphore = Semaphore::createSemaphore();
   pthread_mutex_init(&(threadData->mMutt),NULL);
   
   
   mData = reinterpret_cast<void*>(threadData);
   if (start_thread)
     start();
}

Thread::~Thread()
{
   join();

   MacThreadData * threadData = reinterpret_cast<MacThreadData*>(mData);
   //Semaphore::destroySemaphore(threadData->mSemaphore);
   pthread_mutex_destroy(&(threadData->mMutt));
   delete threadData;
}

void Thread::start()
{
   if(isAlive())
     return;

   MacThreadData * threadData = reinterpret_cast<MacThreadData*>(mData);
   // cause start to block out other pthreads from using this Thread, at least until ThreadRunHandler exits.
   //Semaphore::acquireSemaphore(threadData->mSemaphore);
   pthread_mutex_lock(&(threadData->mMutt));

   pthread_t threadID;
   pthread_create(&threadID, NULL, ThreadRunHandler, mData);
}

bool Thread::join()
{
   if(!isAlive())
     return(false);

   MacThreadData * threadData = reinterpret_cast<MacThreadData*>(mData);
//   return(Semaphore::acquireSemaphore(threadData->mSemaphore));
   pthread_mutex_lock(&(threadData->mMutt));
   return true;
}

void Thread::run(S32 arg)
{
   MacThreadData * threadData = reinterpret_cast<MacThreadData*>(mData);
   if(threadData->mRunFunc)
     threadData->mRunFunc(arg);
}

bool Thread::isAlive()
{
   MacThreadData * threadData = reinterpret_cast<MacThreadData*>(mData);
   
   //bool signal = Semaphore::acquireSemaphore(threadData->mSemaphore, false);
   if( pthread_mutex_trylock(&(threadData->mMutt)) == 0 ) 
   {
     //Semaphore::releaseSemaphore(threadData->mSemaphore);
     pthread_mutex_unlock(&(threadData->mMutt));
     return false; // we got the lock, it aint alive.
   }
   else
     return true; // we could not get the lock, it must be alive.
}

U32 Thread::getCurrentThreadId()
{
 
   return (U32)pthread_self();
}


static void *gMut1 = Mutex::createMutex();

#if defined(TORQUE_MAC_THREAD_TESTS)
// -- the following is a set of tests to check thread sync facilities.
//  the 2 different ways of starting threads, by subclassing Thread,
//  and by using a (ThreadRunFunc*)(), are both demonstrated here.
#include "gui/core/guiControl.h"

class TestThread : public Thread
{
public:
   volatile bool dienow;
   TestThread(ThreadRunFunction func, S32 arg, bool start_thread) : Thread(func,arg,start_thread)
   {
     dienow = false;
   }
   virtual void run(S32 arg)
   {
     int r;
     U32 time = Platform::getRealMilliseconds();
     Con::printf("thread %i starting",arg);
     while(!this->dienow) // will be set true by mothership thread
     {
       Mutex::lockMutex(gMut1);
       GuiControl *ctrl = new GuiControl();
       r = ((float)rand() / RAND_MAX ) * 10;
       Platform::sleep(r);
       delete ctrl;
       Mutex::unlockMutex(gMut1);
     }
     time = Platform::getRealMilliseconds() - time;
     Con::printf("thread %i exec time: %i",arg,time);
   }
};



void mothership(S32 arg)
{
   Con::printf("mothership started with arg %i",arg);
   int r;
   U32 time = Platform::getRealMilliseconds();
   TestThread* thread[arg];
   // create some threads, randomly sleep or delete one.
   Mutex::lockMutex(gMut1);
   for(int i=0; i < arg; i++)
   {
   
     Con::printf("starting thread %i",i+1);
     thread[i] = new TestThread((ThreadRunFunction)NULL, i+1, true);
     r = ((float)rand() / RAND_MAX ) * 10;
     Platform::sleep(r);

   }
   Mutex::unlockMutex(gMut1);
     
   for(int i=0; i < arg; i++)
   {
     r = ((float)rand() / RAND_MAX ) * 10;
     Platform::sleep(r);
     thread[i]->dienow=true;
     delete thread[i];
   }
   time = Platform::getRealMilliseconds() - time;
   Con::printf("mothership exec time: %i",time);
}


ConsoleFunction(TestThreads,void,1,3,"TestThreads([number of motherships], [number of threads per mothership]); Launches threads, all competing for the same mutex.")
{
   int nThreads = 1;
   int nMotherships = 1;
   if(argc>=2) {
     nThreads = dAtoi(argv[1]);
   }

   if(argc>=3) {
      nMotherships = dAtoi(argv[2]);
   }

   bool semStateTest1, semStateTest2, semIndepTest;

   // check whether we can acquire a newly made semaphore
   semStateTest1 = true;
   void* sem = Semaphore::createSemaphore();
   void* sem2 = Semaphore::createSemaphore();
   if(Semaphore::acquireSemaphore(sem,false)) {
      semStateTest1 = true;
   } else 
      semStateTest1 = false;

   if(Semaphore::acquireSemaphore(sem2,false)) {
      semStateTest2 = true;
   } else 
      semStateTest2 = false;
   
   // if we failed to acquire new semaphores, test whether semaphores are independant.
   semIndepTest = true; 
   if(!semStateTest1 && !semStateTest2) {
      Semaphore::releaseSemaphore(sem); // release one,
      if(Semaphore::acquireSemaphore(sem2,false)) // try to acquire the other ( that we know we can't acquire yet )
         semIndepTest = false; // we really should not be able to get this semaphore
      else 
         semIndepTest = true;
   }

   Con::errorf("-------------- Semaphore test Results ------------");
   if(!semStateTest1)
      Con::errorf("New Semaphore Aqcuire test 1 failed.");
   else
      Con::printf("New Semaphore Aqcuire test 1 passed.");

   if(!semStateTest2)
      Con::errorf("New Semaphore Aqcuire test 2 failed.");
   else
      Con::printf("New Semaphore Aqcuire test 2 passed.");

   if(!semIndepTest)
      Con::errorf("Semaphores are NOT INDEPENDANT!!! - This is bad.");
   else
      Con::errorf("Semaphore Independance test passed.");

   
   Con::printf("starting concurrent threads...");
   Mutex::lockMutex(gMut1);
   for(int i=0; i < nMotherships; i++) {
     // memory leak here: because we dont keeep refs to the mothership Threads, we cannot delete them.
     Con::printf("starting a mothership");
     Thread *t = new Thread((ThreadRunFunction)mothership,nThreads,true);
   }
   Mutex::unlockMutex(gMut1);
}

#endif // TORQUE_MAC_THREAD_TESTS
