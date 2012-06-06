#ifndef _MULTICASTCOMMON_H_
#define _MULTICASTCOMMON_H_
/*
 *  MulticastCommon.h
 *  Multicast
 *
 *  Created by Loren Olson on 11/11/05.
 *
 */

/*
 * The following SWAP macros handle Network byteorder issues.
 * On Windows, we don't need to do anything, but on the Mac,
 * all the data needs to be swapped.
 *
 * Note that Mac OS X has inherited the NXSwap* routines from Next!
 */

#ifdef WIN32	/***** Begin Windows specific code *****/

#define SWAP_SHORT(x)		(x)
#define SWAP_LONG(x)		(x)
#define SWAP_FLOAT(x)		(x)
#define SWAP_DOUBLE(x)		(x)

#else			/***** Begin Mac OS X specific code *****/

static __inline__
double ameSwapDouble( double inv )
{
    union doubleconv {
		double				ud;
		unsigned char		uc[8];
    } *inp, outv;
    
    inp = (union doubleconv *)&inv;
    
    outv.uc[0] = inp->uc[7];
    outv.uc[1] = inp->uc[6];
    outv.uc[2] = inp->uc[5];
    outv.uc[3] = inp->uc[4];
    outv.uc[4] = inp->uc[3];
    outv.uc[5] = inp->uc[2];
    outv.uc[6] = inp->uc[1];
    outv.uc[7] = inp->uc[0];
    
    return( outv.ud );
}

static __inline__
float ameSwapFloat( float inv )
{
    union floatconv {
		float				uf;
		unsigned char		uc[4];
    } *inp, outv;
    
    inp = (union floatconv *)&inv;
    
    outv.uc[0] = inp->uc[3];
    outv.uc[1] = inp->uc[2];
    outv.uc[2] = inp->uc[1];
    outv.uc[3] = inp->uc[0];
    
    return( outv.uf );
}


#define SWAP_SHORT(x)		NXSwapShort(x)
#define SWAP_LONG(x)		NXSwapLong(x)
#define SWAP_FLOAT(x)		ameSwapFloat(x)
#define SWAP_DOUBLE(x)		ameSwapDouble(x)

#endif			/***** End Mac OS X specific code *****/


#endif          /* _MULTICASTCOMMON_H_ */