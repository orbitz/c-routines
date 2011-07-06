#include <stdlib.h>

#include <cthread.h>
#include <cthread_collection.h>
#include <cthread_error.h>

#define INITIAL_RESERVED 1000

static cthread_error_t _cthread_collection_resize(cthread_collection_t*);

cthread_error_t cthread_collection_init(cthread_collection_t *cth_c) {
  cth_c->count = 0;
  cth_c->reserved = INITIAL_RESERVED;
  cth_c->threads = malloc(sizeof *cth_c->threads * INITIAL_RESERVED);
  if(NULL == cth_c->threads) {
    return CTHREAD_ALLOC_ERR;
  }
  else {
    return CTHREAD_SUCCESS;
  }
}

cthread_error_t cthread_collection_append(cthread_collection_t *cth_c, cthread_t *cth) {
  if(cth_c->count == cth_c->reserved) {
    cthread_error_t resize_err = _cthread_collection_resize(cth_c);
    if(CTHREAD_SUCCESS != resize_err) {
      return resize_err;
    }
  }

  cth_c->threads[cth_c->count] = *cth;
  cth_c->count += 1;
  return CTHREAD_SUCCESS;
}

cthread_error_t cthread_collection_remove(cthread_collection_t *cth_c, cthread_t *cth) {
  int i = 0;
  for(i = 0; i < cth_c->count; i++) {
    if(0 == cthread_compare(cth_c->threads[i], *cth)) {
      for(; i < cth_c->count - 1; i++) {
        cth_c->threads[i] = cth_c->threads[i + 1];
      }
      return CTHREAD_SUCCESS;
    }
  }
  return CTHREAD_SUCCESS;
}

cthread_error_t cthread_collection_get(cthread_collection_t *cth_c, unsigned int idx, cthread_t *cth) {
  if(idx >= cth_c->count) {
    return CTHREAD_INVALID_IDX_ERR;
  }
  else {
    *cth = cth_c->threads[idx];
    return CTHREAD_SUCCESS;
  }
}

cthread_error_t cthread_collection_destroy(cthread_collection_t *cth_c) {
  free(cth_c->threads);
  return CTHREAD_SUCCESS;
}


static cthread_error_t _cthread_collection_resize(cthread_collection_t *cth_c) {
  cthread_t *threads = malloc(sizeof *threads * (cth_c->reserved * 2));
  if(NULL == threads) {
    return CTHREAD_ALLOC_ERR;
  }
  else {
    memcpy(threads, cth_c->threads, sizeof *threads * cth_c->count);
    cth_c->threads = threads;
    return CTHREAD_SUCCESS;
  }
}
