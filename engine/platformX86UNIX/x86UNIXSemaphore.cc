//-----------------------------------------------------------------------------
// Torque Game Engine 
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------



#include "platformX86UNIX/platformX86UNIX.h"
#include "platform/platformSemaphore.h"
#include <pthread.h>
#include <errno.h>
//
// Emulate the same behavior as the windows semaphore functions. We use
// the phread functions to accomplish this as we are using them already.
//
struct sem_private {
  pthread_mutex_t    mutex;
  pthread_cond_t     con;
  int                count;
};

void * Semaphore::createSemaphore(U32 initialCount)
{
  sem_private *semaphore = new sem_private;
  // Initialize the mutex
  if (pthread_mutex_init(&(semaphore->mutex), NULL))
    AssertFatal (false, "Semeaphore::createSemaphore: unable to create mutex");
  // Initialize the condition
  if (pthread_cond_init(&(semaphore->con), NULL))
    AssertFatal (false, "Semeaphore::createSemaphore: unable to create condition");
  semaphore->count = initialCount;
  return (semaphore);
}

void Semaphore::destroySemaphore(void * semaphore)
{
  AssertFatal (semaphore, "Semaphore::destroySemaphore: invalid semaphore.");
  sem_private *sem = static_cast<sem_private *>(semaphore);
  pthread_mutex_destroy(&(sem->mutex));
  pthread_cond_destroy(&(sem->con));
  delete sem;
}

bool Semaphore::acquireSemaphore(void * semaphore, bool block)
{
  AssertFatal (semaphore, "Semaphore::acqireSemaphore: invalid semaphore.");
  sem_private *sem = static_cast<sem_private *>(semaphore);
  if (block)
    {
      int result;
      pthread_mutex_lock(&(sem->mutex));
      while (sem->count <= 0)
	{
	  result = pthread_cond_wait(&(sem->con), &(sem->mutex));
	  if (result && errno != EINTR)
	    break;
	}
      sem->count--;
      pthread_mutex_unlock(&(sem->mutex));
      return (true);
    }
  else
    {
      struct timespec tm;
      int result;
      result = pthread_mutex_trylock(&(sem->mutex));
      if (result == 0)
	{
	  tm.tv_sec  = 0;
	  tm.tv_nsec = 0;
	  while(sem->count <= 0)
	    {
	      result = pthread_cond_timedwait(&(sem->con), &(sem->mutex), &tm);
	      if (result && (errno != EINTR))
		break;
	    }
	  sem->count--;
	  pthread_mutex_unlock(&(sem->mutex));
	  return true;
	}
      return false;
    }
}

void Semaphore::releaseSemaphore(void * semaphore)
{
   AssertFatal(semaphore, "Semaphore::releaseSemaphore: invalid semaphore.");
   sem_private *sem = static_cast<sem_private *>(semaphore);
   pthread_mutex_lock(&(sem->mutex));
   sem->count++;
   pthread_mutex_unlock(&(sem->mutex));
   pthread_cond_broadcast(&(sem->con));
}
