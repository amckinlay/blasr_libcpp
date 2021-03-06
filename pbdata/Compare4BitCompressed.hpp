/*
 * ============================================================================
 *
 *       Filename:  Compare4BitCompressed.h
 *
 *    Description:  Imported from BLASR algorithm/compare
 *
 *        Version:  1.0
 *        Created:  12/19/2013 05:22:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yuan Li (yli), yli@pacificbiosciences.com
 *        Company:  Pacific Biosciences
 *
 * ============================================================================
 */
#ifndef _BLASR_COMPARE_4BIT_COMPRESSED_HPP_
#define _BLASR_COMPARE_4BIT_COMPRESSED_HPP_

#include "defs.h"

template <typename TNuc>
class Compare4BitCompressed
{
public:
    const static unsigned char MaskCount = 0xf;

    static int Compare(TNuc *lhs, TNuc *rhs)
    {
        return ThreeBit[*lhs & MaskCount] - ThreeBit[*rhs & MaskCount];
    }

    static int Compare(TNuc *lhs, TNuc *rhs, int length)
    {
        int i;
        int res;
        i = 0;
        TNuc *lhsptr;
        TNuc *rhsptr;
        lhsptr = lhs;
        rhsptr = rhs;
        char *lhsend = lhs + length;
        res = 0;
        while (lhsptr != lhsend and res == 0) {
            res = ThreeBit[*lhsptr & MaskCount] - ThreeBit[*rhsptr & MaskCount];
            ++lhsptr;
            ++rhsptr;
        }
        return res;
    }

    static int Equal(TNuc a, TNuc b) { return (a & MaskCount) == (b & MaskCount); }

    static int LessThan(TNuc *a, int aLen, TNuc *b, int bLen)
    {
        int minabLen = MIN(aLen, bLen);
        if (minabLen <= 0) return 0;
        //		int cmpRes;

        int cmpRes = Compare(a, b, minabLen);
        if (cmpRes < 0) {
            return 1;
        } else {
            return 0;
        }
    }

    static int LessThanEqual(TNuc *a, int aLen, TNuc *b, int bLen)
    {
        int minabLen = MIN(aLen, bLen);
        if (minabLen <= 0) return 1;
        int cmpRes = Compare(a, b, minabLen);
        if (cmpRes <= 0) {
            return 1;
        } else {
            return 0;
        }
    }

    static int Equals(TNuc *a, int aLen, TNuc *b, int bLen)
    {
        int minabLen = MIN(aLen, bLen);
        if (minabLen < 0) return 0;
        if (minabLen == 0) return 1;

        int cmpRes = Compare(a, b, minabLen);
        if (cmpRes == 0 and aLen <= bLen) {
            return 1;
        } else {
            return 0;
        }
    }
};
#endif
