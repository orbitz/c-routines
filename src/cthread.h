#ifndef _CTHREAD_H_
#define _CTHREAD_H_

#include <cthread_error.h>

struct _cthread_t;

typedef struct _cthread_t* cthread_t;

enum _cthread_state_t { CTHREAD_SLEEP,
                        CTHREAD_RUNNING,
                        CTHREAD_INTERRUPT };

typedef enum _cthread_state_t cthread_state_t;

int cthread_compare(cthread_t*, cthread_t*);
cthread_state_t cthread_state(cthread_t*);

cthread_error_t cthread_create(cthread_t*, void (*)(void*), void*);
cthread_error_t cthread_join(cthread_t*, void**);
cthread_t *cthread_self();

cthread_state_t cthread_yield();
void cthread_exit(void*);



#endif
