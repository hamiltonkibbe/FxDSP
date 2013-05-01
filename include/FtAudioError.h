/** 
 * @file FtAudioError.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */

#ifndef FTAUDIOERROR_H_
#define FTAUDIOERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

/** Error codes */
typedef enum FtAudioError
{
	/** No Error (0) */
	FT_NOERR,

	/** Generic Error (1) */
	FT_ERR,

	/** Number of defined error codes */
	FT_N_ERRORS
}FtAudioError_t;

#ifdef __cplusplus
}
#endif

#endif /* FTAUDIOERROR_H_ */