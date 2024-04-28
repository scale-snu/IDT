#ifndef SMART_PTR_HPP
#define SMART_PTR_HPP
#include <assert.h>
#include <set>
#include <iostream>

#ifndef NULL
#define NULL ((void*)0)
#endif

namespace cctki_piraha {

extern std::set<void*> *ptrs;

// TODO: This code is disabled because it leads to segfaults during
// startup. Most likely, this code is used during initialisation of
// global variables, but also implicitly assumes that all global
// variables have already been initialised.
#if 0
//#ifndef NDEBUG
inline void add(std::set<void*>& v,void *t) {
    if(t == NULL)
        return;
    // TODO: Don't separate finding and inserting; do it in one go to
    // save a lookup.
    assert(v.find(t) == v.end());
    v.insert(t);
}

inline void remove(std::set<void*>& v,void* t) {
        // TODO: Don't separate finding and erasing; do it in one go
        // to save a lookup.
	std::set<void*>::iterator it = v.find(t);
	assert(it != v.end());
	v.erase(it);
}
#else
inline void add(std::set<void*>& v,void* t) {
}
inline void remove(std::set<void*>& v,void* t) {
}
#endif

template<typename T>
class smart_ptr;

template<typename T>
class smart_ptr_guts {
    int ref_count;
    public:
    T * const ptr;
    bool array;
    smart_ptr_guts(int rc,T *p,bool array_) : ref_count(rc), ptr(p), array(array_) {
        if(ptr != NULL) {
            add(*ptrs,(void*)ptr);
        }
    }
    ~smart_ptr_guts() {
        if(ptr != NULL) {
            remove(*ptrs,(void*)ptr);
            if(array)
                delete[] ptr;
            else
                delete ptr;
        }
    }
    void inc() {
        ref_count++;
    }
    bool dec() {
        int r = ref_count;
        if(ref_count>0)
            ref_count--;
        return r==1;
    }
    int ref_count_() {
        return ref_count;
    }
};

// Count references to an object in a thread-safe
// way using pthreads and do automatic clean up.
template<typename T>
class smart_ptr {
    smart_ptr_guts<T> *guts;
    void clean() {
        assert(this != NULL);
        if(guts != NULL && guts->dec()) {
            delete guts;
            guts = NULL;
        }
    }
    public:
    void inc() {
        if(valid())
            guts->inc();
    }
    smart_ptr(T *ptr,bool array_=false) {
        if(ptr == NULL) {
            guts = NULL;
        } else {
            guts = new smart_ptr_guts<T>(1,ptr,array_);
        }
    }
    smart_ptr(const smart_ptr<T> &sm) : guts(sm.guts) {
        if(sm.guts != NULL)
            guts->inc();
    }
    smart_ptr() : guts(NULL) {}
    ~smart_ptr() {
        clean();
    }
    void operator=(T *t) {
        clean();
        if(t == NULL) {
            guts = NULL;
        } else {
            guts = new smart_ptr_guts<T>(1,t,false);
        }
    }
    void set(T *t,bool array_) {
        clean();
        if(t == NULL) {
            guts = NULL;
        } else {
            guts = new smart_ptr_guts<T>(1,t,array_);
        }
    }
    void operator=(const smart_ptr<T>& s) {
        assert(this != NULL);
        clean();
        guts = s.guts;
        if(guts != NULL)
            guts->inc();
    }
    T& operator*() {
        assert(guts != NULL);
        return *guts->ptr;
    }
    T& operator[](unsigned int n) {
        assert(guts != NULL);
        assert(guts->array);
        return guts->ptr[n];
    }
    T *operator->() const {
        if(guts == NULL)
            return NULL;
        else
            return guts->ptr;
    }
    T *ptr() {
        assert(guts != NULL);
        return guts->ptr;
    }
    bool valid() const {
        return guts != NULL && guts->ptr != NULL;
    }
    int ref_count() {
        assert(guts != NULL);
        return guts->ref_count_();
    }
    template<typename C>
    smart_ptr<C> dup() {
        smart_ptr<C> c;
        c.guts = (smart_ptr_guts<C> *)guts;
        guts->inc();
        return c;
    }
    template<typename C>
    friend class smart_ptr;
};

}
#endif
