/*
 *  bitscan.h
 *	taken from http://chessprogramming.wikispaces.com/BitScan
 *
 */

//These processor instructions work only for 64-bit processors
#ifdef _MSC_VER
    #include <intrin.h>
    #ifdef _WIN64
        #pragma intrinsic(_BitScanForward64)
        #pragma intrinsic(_BitScanReverse64)
        #define USING_INTRINSICS
    #endif
#elif defined(__GNUC__) && defined(__LP64__)
    static INLINE unsigned char _BitScanForward64(unsigned int* const Index, const U64 Mask)
    {
        U64 Ret;
        __asm__
        (
            "bsfq %[Mask], %[Ret]"
            :[Ret] "=r" (Ret)
            :[Mask] "mr" (Mask)
        );
        *Index = (unsigned int)Ret;
        return Mask?1:0;
    }
    static INLINE unsigned char _BitScanReverse64(unsigned int* const Index, const U64 Mask)
    {
        U64 Ret;
        __asm__
        (
            "bsrq %[Mask], %[Ret]"
            :[Ret] "=r" (Ret)
            :[Mask] "mr" (Mask)
        );
        *Index = (unsigned int)Ret;
        return Mask?1:0;
    }
    #define USING_INTRINSICS
#endif