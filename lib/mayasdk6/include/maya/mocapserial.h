
/*-
 ==========================================================================
 Copyright  (C)  Alias Systems,  a division  of  Silicon Graphics  Limited.
 All rights  reserved.  These coded  instructions,  statements and computer
 programs  contain  unpublished information  proprietary to  Alias Systems,
 a  division  of  Silicon  Graphics  Limited,  which  is  protected by  the
 Canadian  and  US federal copyright law and  may not be disclosed to third
 parties or  copied  or  duplicated,  in  whole  or in part,  without prior
 written  consent  of Alias Systems, a division of Silicon Graphics Limited
 ==========================================================================
*/

/*
 * mocapserial.h - Definitions for handling serial lines for motion
 *		 capture servers for Maya
 */

#if !defined(__CAPSERIAL_H__)
#define __CAPSERIAL_H__ 1

#ifndef _WIN32
#include <termios.h>
#else
#define termios _DCB
#endif

/*
 * Enumerated types
 */

typedef enum CapParity
{
  CAP_PARITY_EVEN,
  CAP_PARITY_ODD,
  CAP_PARITY_NONE,
  _CAP_PARITY_LAST
} CapParity;

typedef enum CapSerialMode
{
  CAP_SERIAL_ASCII,
  CAP_SERIAL_BINARY,
  _CAP_MODE_LAST
} CapSerialMode;

/*
 * Figure out if we should use prototypes or not
 */

#if !defined(_PROTO)
#if defined(PROTOTYPES) || defined(FUNCPROTO) || defined(__STDC__) || defined(__EXTENSIONS__) || defined(__cplusplus)
#define _PROTO(x)	x
#else
#define _PROTO(x)	()
#endif
#endif // !defined(_PROTO)

/*
 * Capture library serial routines function declarations/prototypes
 */

/*
 * Open a serial port and return a file descriptor
 */
int	CapSerialOpen _PROTO((char *tty, int baud, int data, CapParity parity,
			  int stop, CapSerialMode mode,
			  struct termios *save_attr));

/*
 * Reset a serial port to its default attributes
 */
int	CapSerialReset _PROTO((int tty_fd, struct termios *attr));

/*
 * Read n bytes (even from a serial line)
 */
int	CapSerialRead _PROTO((int tty_fd, void *buf, size_t nbytes));

/*
 * Read n bytes (even from a serial line) with a possible timeout
 */
int	CapSerialReadTimeout _PROTO((int tty_fd, void *buf,
                                        size_t nbytes, int msec));

/*
 * Write n bytes (even from a serial line)
 */
int	CapSerialWrite _PROTO((int tty_fd, void *buf, size_t nbytes));

#endif /* __CAPSERIAL_H__ */
