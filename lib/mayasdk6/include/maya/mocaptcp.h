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
 * captcp.h -	Definitions for handling serial lines for motion
 *		capture servers for Maya
 */

#if !defined(__CAPTCP_H__)
#define __CAPTCP_H__ 1

/*
 * Figure out if we should use prototypes or not
 */

#if !defined(_PROTO)
#if defined(PROTOTYPES) || defined(FUNCPROTO) || defined(__STDC__) || defined(__EXTENSIONS__) || defined(__cplusplus)
#define _PROTO(x)	x
#else
#define _PROTO(x)	()
#endif
#endif //(_PROTO)

/*
 * Capture library TCP/IP routines function declarations/prototypes
 */

/*
 * Open a connection to a socket on a host and tcp port.
 */
int CapTcpOpen _PROTO((char *name));

#endif /* __CAPTCP_H__ */
