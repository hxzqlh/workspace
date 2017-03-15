#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"

#ifdef __cplusplus
};
#endif

// http://guguclock.blogspot.jp/2009/12/ffmpeg-swscale.html
int resize_yuv (unsigned char * in, unsigned iw, unsigned ih,
                unsigned char * out, unsigned ow, unsigned oh) 
{
    struct SwsContext *img_convert_ctx;
    uint8_t *inbuf[4];
    uint8_t *outbuf[4];

    int inlinesize[4] = {iw, iw/2, iw/2, 0};
    int outlinesize[4] = {ow, ow/2, ow/2, 0};

    inbuf[0] = in;
    inbuf[1] = in + iw * ih;
    inbuf[2] = in + iw * ih * 5 /4;
    inbuf[3] = NULL;

    outbuf[0] = out;
    outbuf[1] = out + ow * oh;
    outbuf[2] = out + ow * oh * 5 /4;
    outbuf[3] = NULL;

    img_convert_ctx = sws_getContext (iw, ih, AV_PIX_FMT_YUV420P,
            ow, oh, AV_PIX_FMT_YUV420P, SWS_POINT,
            NULL, NULL, NULL);

    if(img_convert_ctx == NULL) {
        fprintf(stderr, "Cannot initialize the conversion context!\n");
        return -1;
    }

    sws_scale (img_convert_ctx, inbuf, inlinesize,
            0, ih, outbuf, outlinesize);

    sws_freeContext (img_convert_ctx);

    /*
    pic = av_frame_alloc();
    avpicture_fill ((AVPicture *)pic, pic_buf, AV_PIX_FMT_YUV420P, iw, ih);
 
    y_size = iw * ih;
    pic->data[0] = pic_buf;              // Y
    pic->data[1] = pic_buf+ y_size;      // U 
    pic->data[2] = pic_buf+ y_size*5/4;  // V

    sws_pic = av_frame_alloc ();
    avpicture_fill ((AVPicture *)sws_pic, pic_buf, AV_PIX_FMT_YUV420P, ow, oh);

    sws_ctx = sws_getContext (iw, ih, AV_PIX_FMT_YUV420P, 
                    ow, oh, AV_PIX_FMT_YUV420P, SWS_POINT, 
                    NULL, NULL, NULL);
    sws_scale (sws_ctx, pic->data, pic->linesize, 0, ih, sws_pic->data, sws_pic->linesize);

    memcpy (out, sws_pic->data[0], ow * oh);
    memcpy (out + ow * oh, sws_pic->data[1], ow * oh /4);
    memcpy (out + ow * oh * 5 / 4, sws_pic->data[2], ow * oh /4);

    if (sws_ctx) 
        sws_freeContext(sws_ctx);
    */

    return 0;
}

// http://blog.csdn.net/leixiaohua1020/article/details/25346147
int yuv_to_jpg (unsigned char * yuv, unsigned iw, unsigned ih, const char * jpg_file)
{
    AVFormatContext *   pFormatCtx;
    AVOutputFormat *    fmt;
    AVStream *          video_st;
    AVCodecContext *    pCodecCtx;
    AVCodec *           pCodec;
    AVFrame*            picture;
    AVPacket            pkt;

    unsigned            y_size  = 0;
    unsigned            size    = 0;
    int                 got_pic = 0;
    int                 ret     = 0;

    av_register_all ();

    //Method 1
    pFormatCtx = avformat_alloc_context ();

    //Guess format
    fmt = av_guess_format ("mjpeg", NULL, NULL);
    pFormatCtx->oformat = fmt;

    //Output URL
    if (avio_open (&pFormatCtx->pb,jpg_file, AVIO_FLAG_READ_WRITE) < 0){
        printf("Couldn't open output file.");
        return -1;
    }

    video_st = avformat_new_stream (pFormatCtx, 0);
    if (video_st==NULL)
        return -1;

    pCodecCtx                   = video_st->codec;
    pCodecCtx->codec_id         = fmt->video_codec;
    pCodecCtx->codec_type       = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt          = AV_PIX_FMT_YUVJ420P;
    pCodecCtx->width            = iw;  
    pCodecCtx->height           = ih;
    pCodecCtx->time_base.num    = 1;  
    pCodecCtx->time_base.den    = 25;   

    pCodec = avcodec_find_encoder (pCodecCtx->codec_id);
    if (!pCodec) {
        printf("Codec not found.");
        return -1;
    }
    if (avcodec_open2 (pCodecCtx, pCodec,NULL) < 0){
        printf("Could not open codec.");
        return -1;
    }

    picture = av_frame_alloc ();  
    // size = iw * ih * 3 / 2
    size = avpicture_fill ((AVPicture *) picture, NULL, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);  
    
    //Read YUV  
    y_size           = pCodecCtx->width * pCodecCtx->height;  
    picture->data[0] = yuv;                   // Y  
    picture->data[1] = yuv + y_size;          // U   
    picture->data[2] = yuv + y_size * 5 / 4;  // V  
    
    //Write Header
    avformat_write_header (pFormatCtx,NULL);

    //Encode
    av_new_packet (&pkt, y_size * 3);  
    ret = avcodec_encode_video2 (pCodecCtx, &pkt, picture, &got_pic);
    if(ret < 0){
        printf("Encode Error.\n");
        return -1;
    }

    if (got_pic==1) {
        pkt.stream_index = video_st->index;
        ret = av_write_frame (pFormatCtx, &pkt);
    }

    av_free_packet (&pkt);
   
     //Write Trailer
    av_write_trailer (pFormatCtx);

    printf ("Encode Successful.\n");

    if (video_st) {
        avcodec_close (video_st->codec);
        av_free (picture);
    }

    avio_close (pFormatCtx->pb);
    avformat_free_context (pFormatCtx);

    return 0;
}

int main (int argc, char * argv [])
{
    if (argc < 7) {
        fprintf (stderr, "%s yuv iw ih jpeg ow oh\n", argv[0]);
        return -1;
    }   

    unsigned iw = atoi (argv[2]);
    unsigned ih = atoi (argv[3]);
    unsigned ow = atoi (argv[5]);
    unsigned oh = atoi (argv[6]);
     
    unsigned in_size = iw * ih * 3 /2;
    unsigned out_size = ow * oh * 3 /2;

    unsigned char * in = (unsigned char *) malloc (in_size);
    unsigned char * out = (unsigned char *) malloc (out_size);

    FILE * ifp    = fopen (argv[1], "rb");
    char yuv_name [20];
    sprintf (yuv_name, "%dx%d.yuv", ow, oh);
    FILE * ofp = fopen (yuv_name, "wb");
 
    fread (in, 1, in_size, ifp);
    
    int ret;
    ret = resize_yuv (in, iw, ih, out, ow, oh);
    fwrite (out, 1, out_size, ofp);
    
    ret = yuv_to_jpg (out, ow, oh, argv[4]);

    fclose (ifp);
    fclose (ofp);
    free (in);
    free (out);

    return 0;
}
