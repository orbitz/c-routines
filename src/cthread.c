#include <stdlib.h>
#include <setjmp.h>

#include <cthread.h>
#include <cthread_error.h>
#include <cthread_collection.h>


struct _cthread_t {
  jmp_buf env;
  cthread_state_t state;
};

typedef struct _cthread_t* cthread_t;

static cthread_t _self = NULL;
static cthread_collection_t _thread_collection;
static int _curr_thread_idx = 0;

static cthread_t _choose_next_thread() {
  cthread_t ret_thread;
  _curr_thread_idx = (_curr_thread_idx + 1) % _thread_collection.count;
  cthread_error_t ret = cthread_collection_get(&_thread_collection,
                                               _curr_thread_idx,
                                               &ret_thread);
  if(CTHREAD_SUCCESS != ret) {
    return NULL;
  }
  else {
    return ret_thread;
  }
}

static void _run_next_thread(cthread_t cth) {
  longjmp(cth->env, 1);
}

static cthread_t _cthread_create() {
  return malloc(sizeof (struct _cthread_t));
}

int cthread_compare(cthread_t *cth1, cthread_t *cth2) {
  return cth1 == cth2;
}

cthread_state_t cthread_state(cthread_t *cth) {
  return cth->state;
}

cthread_error_t cthread_create(cthread_t *cth, void (*cthread_func)(void*), void *cthread_data) {
  if(cthread_collection_is_empty(&_thread_collection)) {
    /*
     * If the collection is empty it means we are in the first call
     * to create and we need to create a thread for the main thread calling it.
     * We could have just required a `cthread_init` be called but decided this
     * felt better.
     */
    cthread_error_t ret = cthread_collection_init(&_thread_collection);
    if(CTHREAD_SUCCESS != ret) {
      return ret;
    }

    /*
     * Setup self
     */
    cthread_t new_self = _cthread_create();
    if(NULL == new_self) {
      return CTHREAD_ALLOC_ERR;
    }

    _self = new_self;
    ret = cthread_collection_append(&_thread_collection, &new_self);
    if(CTHREAD_SUCCESS != ret) {
      return ret;
    }
    
  }

  if(!setjmp(_self->env)) {
    cthread_t new_thread = _cthread_create();
    if(NULL == new_thread) {
      /*
       * Not sure this is the right thing to do
       */
      return CTHREAD_ALLOC_ERR;
    }

    ret = cthread_colection_append(&_thread_collection, &new_thread);
    _self = new_thread;
    cthread_func(cthread_data);
    fprintf(stderr, "Should never reach here, exiting\n");
    exit(35);

  }
  
  return CTHREAD_SUCCESS;
}

cthread_error_t cthread_join(cthread_t*, void**) {
}

cthread_t *cthread_self() {
  return _self;
}

cthread_state_t cthread_yield() {
}

void cthread_exit(void *value_ptr) {
  (void)value_ptr;

  cthread_collection_remove(&_thread_collection, cthread_self());
  cthread_t next_thread = _choose_next_thread();
  if(NULL == next_thread) {
    /*
     * Not really much of an option here.  If we can't choose another
     * thread we are royaly f'd
     */
    exit(34);
  }
  _run_next_thread(next_thread);
}
