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
    int result = rtmp_open_for_write("rtmp://49.233.136.247/live/pcg", 1280, 720);
    if(result<=0){
        printf("%s","open failed");
        return 0;
    }
    bool bConnected = rtmp_is_connected();
    if(!bConnected){
        printf("%s","connect failed");
        return 0;
    }
    //Write h264 nal units with this function
    //rtmp_sender_write_video_frame(<#uint8_t *data#>, <#int size#>, <#uint64_t dts_us#>, <#int key#>, <#uint32_t abs_ts#>)
    
    rtmp_close();
    
    
    printf("%s","hello world");
    
    return 0;
}
