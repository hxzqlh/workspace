#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"

#ifdef __cplusplus
};
#endif

int yuv_to_jpeg (unsigned char * picture_buf, unsigned iw, unsigned ih, 
    const char * jpg_file, unsigned ow, unsigned oh)
{

    AVFormatContext* pFormatCtx;
    AVOutputFormat* fmt;
    AVStream* video_st;
    AVCodecContext* pCodecCtx;
    AVCodec* pCodec;

    AVFrame* picture;
    AVPacket pkt;
    int y_size;
    int got_picture=0;
    int size;

    int ret=0;

    av_register_all();

    //Method 1
    pFormatCtx = avformat_alloc_context();
    //Guess format
    fmt = av_guess_format("mjpeg", NULL, NULL);
    pFormatCtx->oformat = fmt;
    //Output URL
    if (avio_open(&pFormatCtx->pb,jpg_file, AVIO_FLAG_READ_WRITE) < 0){
        printf("Couldn't open output file.");
        return -1;
    }

    //Method 2. More simple
    //avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, jpg_file);
    //fmt = pFormatCtx->oformat;

    video_st = avformat_new_stream(pFormatCtx, 0);
    if (video_st==NULL){
        return -1;
    }

    pCodecCtx = video_st->codec;
    pCodecCtx->codec_id = fmt->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;

    pCodecCtx->width = iw;  
    pCodecCtx->height = ih;

    pCodecCtx->time_base.num = 1;  
    pCodecCtx->time_base.den = 25;   
    //Output some information
    av_dump_format(pFormatCtx, 0, jpg_file, 1);

    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec){
        printf("Codec not found.");
        return -1;
    }
    if (avcodec_open2(pCodecCtx, pCodec,NULL) < 0){
        printf("Could not open codec.");
        return -1;
    }
    picture = av_frame_alloc();
    avpicture_fill ((AVPicture *)picture, NULL, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);

    //Write Header
    avformat_write_header(pFormatCtx,NULL);

    y_size = pCodecCtx->width * pCodecCtx->height;
    av_new_packet (&pkt,y_size*3);
    picture->data[0] = picture_buf;              // Y
    picture->data[1] = picture_buf+ y_size;      // U 
    picture->data[2] = picture_buf+ y_size*5/4;  // V

    //Encode
    ret = avcodec_encode_video2(pCodecCtx, &pkt,picture, &got_picture);
    if(ret < 0){
        printf("Encode Error.\n");
        return -1;
    }
    if (got_picture==1){
        pkt.stream_index = video_st->index;
        ret = av_write_frame(pFormatCtx, &pkt);
    }

    av_free_packet(&pkt);
    //Write Trailer
    av_write_trailer(pFormatCtx);

    printf("Encode Successful.\n");

    if (video_st){
        avcodec_close(video_st->codec);
        av_free(picture);
    }
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);

    return 0;
}

int main (int argc, char * argv [])
{
    if (argc < 5) {
        fprintf (stderr, "%s yuv width height jpeg\n", argv[0]);
        return -1;
    }   

    FILE * fp = fopen (argv[1], "rb");
    unsigned iw = atoi (argv[2]);
    unsigned ih = atoi (argv[3]);
    unsigned yuv_size = iw * ih * 3 /2;
     
    unsigned char * pic_buf = (unsigned char *) malloc (yuv_size);
    fread (pic_buf, 1, yuv_size, fp);
    int ret = yuv_to_jpeg (pic_buf, iw, ih, argv[4], iw, ih);

    free (pic_buf);

    return 0;
}
