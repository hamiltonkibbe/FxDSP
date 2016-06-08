/**
 * @file Error.h
 * @author Hamilton Kibbe
 * @copyright 2012 Hamilton Kibbe
 */

#ifndef ERROR_H_
#define ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

/** Error codes */
typedef enum Error
{
    /** No Error (0) */
    NOERR,

    /** Generic Error (1) */
    ERROR,

    /** Malloc failure... */
    NULL_PTR_ERROR,

    /** invalid value... */
    VALUE_ERROR,

    /** Number of defined error codes */
    N_ERRORS
}Error_t;


#ifdef __cplusplus
}
#endif

#endif /* ERROR_H_ */
