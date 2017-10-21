/*
g++ -o pixelator pixelator.cpp -lavutil -lavformat -lavcodec -lz -lavutil -lm -I /usr/include/ffmpeg-compat/ -L /usr/lib64/ffmpeg-compat/
*/

//#include <ffmpeg-compat/libavcodec/avcodec.h>
//#include <ffmpeg-compat/libavformat/avformat.h>

#ifdef __cplusplus
extern "C"
{
    #include <ffmpeg-compat/libavcodec/avcodec.h>
    #include <ffmpeg-compat/libavformat/avformat.h>
    #include <ffmpeg-compat/libswscale/swscale.h>
}
#endif

int main(int argc, char *argv[])
{
  av_register_all();

  //Pointer to header file informations
  AVFormatContext *pFormatCtx;

  // Open video file
  if(av_open_input_file(&pFormatCtx, argv[1], NULL, 0, NULL)!=0)
    return -1; // Couldn't open file

  // Retrieve stream information
  if(av_find_stream_info(pFormatCtx)<0)
  return -1; // Couldn't find stream information

  // Dump information about file onto standard error
  dump_format(pFormatCtx, 0, argv[1], 0);

  unsigned int i;
  AVCodecContext *pCodecCtxOrig = NULL;
  AVCodecContext *pCodecCtx = NULL;

  // Find the first video stream
  int videoStream=-1;
  for(i=0; i<pFormatCtx->nb_streams; i++)
    if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
      videoStream=i;
      break;
    }
  if(videoStream==-1)
    return -1; // Didn't find a video stream

  // Get a pointer to the codec context for the video stream
  pCodecCtx=pFormatCtx->streams[videoStream]->codec;

  AVCodec *pCodec = NULL;

  // Find the decoder for the video stream
  pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
  if(pCodec==NULL) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1; // Codec not found
  }
  // Copy context
  pCodecCtx = avcodec_alloc_context3(pCodec);
  if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
    fprintf(stderr, "Couldn't copy codec context");
    return -1; // Error copying codec context
  }
  // Open codec
  if(avcodec_open2(pCodecCtx, pCodec)<0)
    return -1; // Could not open codec

}
