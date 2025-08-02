// Stub implementations for CPU detection functions
#include "config.h"
#include "cpu.h"

NAMESPACE_BEGIN(CryptoPP)

bool CRYPTOPP_SECTION_INIT g_x86DetectionDone = false;
bool CRYPTOPP_SECTION_INIT g_hasSSE2 = false;
bool CRYPTOPP_SECTION_INIT g_hasSSSE3 = false;
bool CRYPTOPP_SECTION_INIT g_hasSSE41 = false;
bool CRYPTOPP_SECTION_INIT g_hasSSE42 = false;
bool CRYPTOPP_SECTION_INIT g_hasAESNI = false;
bool CRYPTOPP_SECTION_INIT g_hasCLMUL = false;
bool CRYPTOPP_SECTION_INIT g_hasAVX = false;
bool CRYPTOPP_SECTION_INIT g_hasAVX2 = false;
bool CRYPTOPP_SECTION_INIT g_hasBMI2 = false;
bool CRYPTOPP_SECTION_INIT g_hasSSE = false;
bool CRYPTOPP_SECTION_INIT g_hasRDRAND = false;
bool CRYPTOPP_SECTION_INIT g_hasRDSEED = false;
bool CRYPTOPP_SECTION_INIT g_hasPadlockRNG = false;
bool CRYPTOPP_SECTION_INIT g_hasPadlockACE = false;
bool CRYPTOPP_SECTION_INIT g_hasPadlockACE2 = false;
bool CRYPTOPP_SECTION_INIT g_hasPadlockPHE = false;
bool CRYPTOPP_SECTION_INIT g_hasPadlockPMM = false;
word32 CRYPTOPP_SECTION_INIT g_cacheLineSize = CRYPTOPP_L1_CACHE_LINE_SIZE;

// Stub implementation
bool CpuId(word32, word32, word32 *data)
{
    if (data) std::memset(data, 0, 4*sizeof(word32));
    return false;
}

// Stub implementation
word64 XGetBV(word32)
{
    return 0;
}

// Stub implementation
void DetectX86Features()
{
    g_x86DetectionDone = true;
}

// Stub implementation
void CpuId(word32, word32, word32 &eax, word32 &ebx, word32 &ecx, word32 &edx)
{
    eax = ebx = ecx = edx = 0;
}

NAMESPACE_END