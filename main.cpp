//
//  main.cpp
//  librtmp
//
//  Created by mac on 2020/7/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "librtmp/rtmp.h"
#include "flvmuxer/xiecc_rtmp.h"

int h264_startswith_annexb(uint8_t* h264_raw_data, int h264_raw_size, int* pnb_start_code)
{
    if (!h264_raw_data) {
        return 0;
    }

    if (h264_raw_size <= 0) {
        return 0;
    }

    uint8_t* bytes = h264_raw_data;   //stream->data() + stream->pos();
    uint8_t* p = bytes;

    for (;;) {
        if((int)(p - bytes + 3) > h264_raw_size - (p - bytes))
        {
            return 0;
        }

        // not match
        if (p[0] != (char)0x00 || p[1] != (char)0x00) {
            return 0;
        }

        // match N[00] 00 00 01, where N>=0
        if (p[2] == (char)0x01) {
            if (pnb_start_code) {
                *pnb_start_code = (int)(p - bytes) + 3;
            }
            return 1;
        }

        p++;
    }
}


int read_h264_frame(uint8_t* data, int size, uint8_t** pp, int* pnb_start_code, int fps,
    uint8_t** frame, int* frame_size, uint64_t* dts, uint64_t* pts)
{
    uint8_t* p = *pp;

    // @remark, for this demo, to publish h264 raw file to SRS,
    // we search the h264 frame from the buffer which cached the h264 data.
    // please get h264 raw data from device, it always a encoded frame.
    //暂时认为都有,先不做校验
    if (!h264_startswith_annexb(p, size - (p - data), pnb_start_code)) {
        printf("%s","h264 raw data invalid.");
        return -1;
    }

    // @see srs_write_h264_raw_frames
    // each frame prefixed h.264 annexb header, by N[00] 00 00 01, where N>=0,
    // for instance, frame = header(00 00 00 01) + payload(67 42 80 29 95 A0 14 01 6E 40)
    *frame = p;
    p += *pnb_start_code;

    for (; p < data + size; p++) {
        if (h264_startswith_annexb(p, size - (p - data), NULL)) {
            break;
        }
    }

    *pp = p;
    *frame_size = p - *frame;
    if (*frame_size <= 0) {
        printf("%s","h264 raw data invalid.");
        return -1;
    }

    // @remark, please get the dts and pts from device,
    // we assume there is no B frame, and the fps can guess the fps and dts,
    // while the dts and pts must read from encode lib or device.
    *dts += 1000 / fps;
    *pts = *dts;

    return 0;
}

int main(int /*argc*/, char* /*argv*/[])
{
    int result = rtmp_open_for_write("rtmp://49.233.136.247/live/pcg", 0, 0);
    if(result<=0){
        printf("%s","open failed");
        return 0;
    }
    bool bConnected = rtmp_is_connected();
    if(!bConnected){
        printf("%s","connect failed");
        return 0;
    }
    
    //读取H264 raw file
    const char* raw_file = "/Users/mac/pcgdev/cpp/librtmp_mac/aaa.h264";
    FILE* fp = fopen(raw_file,"rb");
    if(fp == NULL){
        printf("open h264 raw file %s failed!",raw_file);
        return 0;
        //goto rtmp_destroy;
    }
    fseek(fp,0,SEEK_END);
    long file_size = ftell(fp);
    uint8_t* h264_raw = (uint8_t*)malloc(file_size);
    if(!h264_raw){
        printf("alloc raw buffer failed for file %s!",raw_file);
        return 0;
    }
    fseek(fp, 0, SEEK_SET);
    
    size_t nb_read = 0;
    if((nb_read = fread(h264_raw,sizeof(char),file_size,fp))!= file_size){
        printf("buffer failed!");
        return 0;
    }
    
    uint8_t* p = h264_raw;
    uint64_t dts = 0;
    uint64_t pts = 0;
    int count = 0;
    int fps = 15;
    for(; p<h264_raw+file_size;){
        uint8_t* data = NULL;
        int size =0;
        int nb_start_code = 0;
        if (read_h264_frame(h264_raw, (int)file_size, &p, &nb_start_code, fps, &data, &size, &dts, &pts) < 0) {
            printf("read a frame from file buffer failed.");
            goto rtmp_destroy;
        }
        //Write h264 nal units with this function
        rtmp_sender_write_video_frame(data, size, dts*1000, 0, 0);
        
        if (count++ == 9) {
            usleep(1000 *1000* count / fps);
            //system_sleep(1000 * count / fps);//in milisecond
            count = 0;
        }
        
    }
    //Write h264 nal units with this function
    //rtmp_sender_write_video_frame(uint8_t *data, int size, uint64_t dts_us, <#int key#>, <#uint32_t abs_ts#>)
    
    
    
    rtmp_destroy:
    rtmp_close();
    fclose(fp);
    free(h264_raw);
    printf("%s","hello world");
    
    return 0;
}
