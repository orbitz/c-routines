#ifndef _CTHREAD_ERROR_
#define _CTHREAD_ERROR_

enum _cthread_error_t { CTHREAD_SUCCESS,
                        CTHREAD_ALLOC_ERR,
                        CTHREAD_INVALID_IDX_ERR };

typedef enum _cthread_error_t cthread_error_t;

#endif
