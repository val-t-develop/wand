/*  SPL - Simple Programming Language compiler
*  Copyright (C) 2022-2024  Valentyn Tymchyshyn
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develop@gmail.com)
 *
 *  Realization of gc part of standard library of spl.
 */

#ifndef GC_H
#define GC_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Object {
    void *obj_ptr;
    int32_t refs_count;
} Object_t;

typedef struct Objects {
    Object_t *arr;
    int32_t size;
    int32_t capacity;
} Objects_t;

typedef struct Ref {
    void *ref;
    int32_t ObjID;
} Ref_t;

typedef struct Refs {
    Ref_t *arr;
    int32_t size;
    int32_t capacity;
} Refs_t;

Objects_t objects;
Refs_t refs;

__attribute__((used))
void __spl__init__gc() {
    objects.capacity=10;
    objects.size=0;
    objects.arr=(Object_t*)malloc(10*sizeof(Object_t));
    memset(objects.arr, 0, 10*sizeof(Object_t));
    refs.capacity=10;
    refs.size=0;
    refs.arr=(Ref_t*)malloc(10*sizeof(Ref_t));
    memset(refs.arr, 0, 10*sizeof(Ref_t));
}

__attribute__((used))
void __spl__destroy__gc() {
    if (objects.arr!=NULL) {
        free(objects.arr);
    }
    if (refs.arr!=NULL) {
        free(refs.arr);
    }
}

__attribute__((used))
void *__spl__alloc(int64_t size) {
    if (objects.size==objects.capacity) {
        objects.capacity*=2;
        objects.arr=(Object_t*)realloc(objects.arr, objects.capacity*sizeof(Object_t));
        memset(objects.arr+objects.size, 0, objects.size*sizeof(Object_t));
    }
    objects.arr[objects.size].refs_count=0;
    void *ptr=malloc(size);
    objects.arr[objects.size].obj_ptr=ptr;
    objects.size++;
    //printf("alloc: %p - 0\n", ptr);
    return ptr;
}

__attribute__((used))
void __spl__destroyobj(void *ptr, void (*destructor)(void*)) {;
    //printf("destroyobj: %p - ", ptr);
    for (int32_t i = 0; i < objects.size; ++i) {
        if (objects.arr[i].obj_ptr==ptr) {
            //printf("%d\n", objects.arr[i].refs_count);
            if (objects.arr[i].refs_count==0) {
                destructor(ptr);
                free(ptr);
                objects.arr[i].obj_ptr=NULL;
            }
        }
    }
}

__attribute__((used))
void __spl__destroyref(void *ptr, void (*destructor)(void*)) {
    //printf("destroyref: %p - ", ptr);
    for (int32_t i = 0; i < refs.size; ++i) {
        if (refs.arr[i].ref==ptr) {
            objects.arr[refs.arr[i].ObjID].refs_count--;
            //printf("%d\n", objects.arr[refs.arr[i].ObjID].refs_count);
            if (objects.arr[refs.arr[i].ObjID].refs_count==0) {
                destructor(objects.arr[refs.arr[i].ObjID].obj_ptr);
                free(objects.arr[refs.arr[i].ObjID].obj_ptr);
                objects.arr[refs.arr[i].ObjID].obj_ptr=NULL;
            }
            refs.arr[i].ref=NULL;
            refs.arr[i].ObjID=0;
        }
    }
}

__attribute__((used))
void __spl__destroyref_not_delete(void *ptr, void (*destructor)(void*)) {
    //printf("destroyref_not_delete: %p - ", ptr);
    for (int32_t i = 0; i < refs.size; ++i) {
        if (refs.arr[i].ref==ptr) {
            objects.arr[refs.arr[i].ObjID].refs_count--;
            //printf("%d\n", objects.arr[refs.arr[i].ObjID].refs_count);
            refs.arr[i].ref=NULL;
            refs.arr[i].ObjID=0;
        }
    }
}

__attribute__((used))
void __spl__addref(void* ref, void* obj, void (*destructor)(void*)) {
    __spl__destroyref(ref, destructor);
    //printf("addref: %p - %p\n", ref, obj);
    for (int32_t i = 0; i < objects.size; ++i) {
        if (objects.arr[i].obj_ptr==obj) {
            objects.arr[i].refs_count++;
            if (refs.size==refs.capacity) {
                refs.capacity*=2;
                refs.arr=(Ref_t*)realloc(refs.arr, refs.capacity*sizeof(Ref_t));
                memset(refs.arr+refs.size, 0, refs.size*sizeof(Ref_t));
            }
            refs.arr[refs.size].ref=ref;
            refs.arr[refs.size].ObjID=i;
            refs.size++;
            return;
        }
    }
    printf("Can not create ref to obj\n");
}

__attribute__((used))
void __spl__write(void *dest, void *source, void (*destructor)(void*)) {
    __spl__destroyref(dest, destructor);
    //printf("write: %p - %p\n", dest, source);
    int32_t objID = -1;
    for (int32_t i = 0; i < refs.size; ++i) {
        if (refs.arr[i].ref==source) {
            objID=refs.arr[i].ObjID;
        }
    }
    if (objID==-1) {
        return;
    }
    objects.arr[objID].refs_count++;
    if (refs.size==refs.capacity) {
        refs.capacity*=2;
        refs.arr=(Ref_t*)realloc(refs.arr, refs.capacity*sizeof(Ref_t));
        memset(refs.arr+refs.size, 0, refs.size*sizeof(Ref_t));
    }
    refs.arr[refs.size].ref=dest;
    refs.arr[refs.size].ObjID=objID;
    refs.size++;
}

#endif //GC_H
