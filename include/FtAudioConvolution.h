/** 
 * @file FtAudioConvolution.h
 * @author Hamilton Kibbe
 * @copyright 2013 Hamilton Kibbe
 */

#ifndef FTAUDIOCONVOLUTION_H
#define FTAUDIOCONVOLUTION_H


#ifdef __cplusplus
extern "C" {
#endif
    
FtAudioError_t
FtAudioConvolve(float       *in1, 
                unsigned    in1_length, 
                float       *in2, 
                unsigned    in2_length, 
                float       *dest);


#ifdef __cplusplus
}
#endif

#endif /* FTAUDIOCONVOLUTION_H */
