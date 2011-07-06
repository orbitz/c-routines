#ifndef _CTHREAD_COLLECTION_
#define _CTHREAD_COLLECTION_

#include <cthread.h>

struct _cthread_collection_t {
  int count;
  int reserved;
  cthread_t *threads;
};

typedef _cthread_collection_t cthread_collection_t;

cthread_error_t cthread_collection_init(cthread_collection_t*);
cthread_error_t cthread_collection_append(cthread_collection_t*, cthread_t*);
cthread_error_t cthread_collection_remove(cthread_collection_t*, cthread_t*);
cthread_error_t cthread_collection_get(cthread_collection_t*, unsigned int, cthread_t**);
void cthread_collection_destroy(cthread_collection_t*);

#endif
