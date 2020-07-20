//
//  main.cpp
//  librtmp
//
//  Created by mac on 2020/7/20.
//

#include <stdio.h>
#include "librtmp/rtmp.h"
#include "flvmuxer/xiecc_rtmp.h"

int main(int /*argc*/, char* /*argv*/[])
{
    RTMP* rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);
    RTMPResult ret = RTMP_SetupURL(rtmp, (char*)"rtmp://49.233.136.247/live/pcg");
    
    printf("%s","hello world");
    
    return 0;
}
