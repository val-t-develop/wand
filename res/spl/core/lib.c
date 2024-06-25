/*  SPL - Simple Programming Language compiler
 *  Copyright (C) 2022  Valentyn Tymchyshyn
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
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develo@gmail.com)
 *
 *  Realization of main parts of standard library of spl.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct __spl__gcmap__entry {
    void* ptr;
    int32_t refs;
} __spl__gcmap__entry_t;

typedef struct __spl__gcmap {
    int32_t size;
    int32_t capacity;
    __spl__gcmap__entry_t *entries;
} __spl__gcmap__t;

__spl__gcmap__t __spl__m;

__attribute__((used))
void __spl__init__gcmap() {
    __spl__m.size = 0;
    __spl__m.capacity = 10;
    __spl__m.entries = malloc(__spl__m.capacity*sizeof(__spl__gcmap__entry_t));
}

__attribute__((used))
void __spl__destroy__gcmap() {
    free(__spl__m.entries);
}

__attribute__((used))
void __spl__add__to__gc(void *ptr, int32_t refs) {
    if (__spl__m.size+1 < __spl__m.capacity) {
        __spl__m.entries[__spl__m.size].ptr = ptr;
        __spl__m.entries[__spl__m.size].refs = refs;
        __spl__m.size++;
    } else {
        __spl__m.capacity *= 2;
        __spl__m.entries = realloc(__spl__m.entries, __spl__m.capacity*sizeof(__spl__gcmap__entry_t));
        __spl__m.entries[__spl__m.size].ptr = ptr;
        __spl__m.entries[__spl__m.size].refs = refs;
        __spl__m.size++;
    }
}

__attribute__((used))
int32_t __spl__get__refs(void *ptr) {
    for(int i = 0; i < __spl__m.size; ++i) {
        if (__spl__m.entries[i].ptr == ptr) {
            return __spl__m.entries[i].refs;
        }
    }
    return -1;
}

__attribute__((used))
void __spl__set__refs(void *ptr, int32_t refs) {
    for(int i = 0; i < __spl__m.size; ++i) {
        if (__spl__m.entries[i].ptr == ptr) {
            __spl__m.entries[i].refs = refs;
        }
    }
    __spl__add__to__gc(ptr, refs);
}

__attribute__((used))
void __spl__dec__refs(void *ptr) {
    for(int i = 0; i < __spl__m.size; ++i) {
        if (__spl__m.entries[i].ptr == ptr) {
            __spl__m.entries[i].refs--;
        }
    }
}

__attribute__((used))
void __spl__inc__refs(void *ptr) {
    for(int i = 0; i < __spl__m.size; ++i) {
        if (__spl__m.entries[i].ptr == ptr) {
            __spl__m.entries[i].refs++;
        }
    }
}

__attribute__((used))
void *__spl__alloc(int32_t size) {
    void *ptr = malloc(size);
    __spl__add__to__gc(ptr, 1);
    return ptr;
}

__attribute__((used))
void __spl__write(void *dest, void *data) {

    if (dest != 0)
        __spl__dec__refs(dest);

    if (data != 0)
        __spl__inc__refs(data);

    if (dest != 0) {
        if (__spl__get__refs(dest) == 0) {
            __spl__set__refs(dest, -1);
            free(dest);
        }
    }
}

__attribute__((used))
void __spl__destroyvar(void *ptr, void (*destructor)(void*)) {

    if (ptr != 0) {
        __spl__dec__refs(ptr);
        if (__spl__get__refs(ptr) == 0) {
            __spl__set__refs(ptr, -1);
            if (destructor != 0) {
                destructor(ptr);
            }
            free(ptr);
        }
    }
}

typedef struct String_t {
    char* str;
    uint64_t size;
} String;

__attribute__((used))
String* __spl__constructor__String() {
    String* obj = __spl__alloc(sizeof(String));
    obj->str = NULL;
    obj->size=0;
    return obj;
}

__attribute__((used))
String* __spl__constructor__String__String(String* _str) {
    String* obj = __spl__alloc(sizeof(String));
    obj->size = _str->size;
    if (obj->size != 0) {
        obj->str = malloc(sizeof(char)*(obj->size+1));
        memcpy(obj->str, _str->str, obj->size*sizeof(char));
        obj->str[obj->size] = '\0';
    } else {
        obj->str = NULL;
    }
    return obj;
}

__attribute__((used))
String* __spl__constructor__String__char(char _a) {
    String* obj = __spl__alloc(sizeof(String));
    obj->size = 1;
    obj->str = malloc(sizeof(char)*2);
    obj->str[0] = _a;
    obj->str[1] = '\0';
    return obj;
}

__attribute__((used))
String* __spl__constructor__String__bool(int8_t _a) {
    String* obj = __spl__alloc(sizeof(String));
    if (_a) {
        obj->size = 4;
        obj->str = malloc(sizeof(char)*4);
        obj->str = "true\0";
    } else {
        obj->size = 5;
        obj->str = malloc(sizeof(char)*5);
        obj->str = "false\0";
    }
    return obj;
}

__attribute__((used))
String* __spl__constructor__String__int(int32_t _a) {
    String* obj = __spl__alloc(sizeof(String));
    obj->size = snprintf(NULL, 0,"%d", _a);
    obj->str = malloc(sizeof(char)*(obj->size+1));
    sprintf(obj->str, "%d", _a);
    return obj;
}

__attribute__((used))
String* __spl__constructor__String__float(float _a) {
    String* obj = __spl__alloc(sizeof(String));
    obj->size = snprintf(NULL, 0,"%f", _a);
    obj->str = malloc(sizeof(char)*(obj->size+1));
    sprintf(obj->str, "%f", _a);
    return obj;
}

__attribute__((used))
String* __spl__constructor__String__double(double _a) {
    String* obj = __spl__alloc(sizeof(String));
    obj->size = snprintf(NULL, 0,"%f", _a);
    obj->str = malloc(sizeof(char)*(obj->size+1));
    sprintf(obj->str, "%f", _a);
    return obj;
}

__attribute__((used))
String* __spl__constructor__String____StringLiteral(char* _str) {
    String* obj = __spl__alloc(sizeof(String));
    obj->size = strlen(_str);
    if (obj->size != 0) {
        obj->str = malloc(sizeof(char)*(obj->size+1));
        memcpy(obj->str, _str, obj->size*sizeof(char));
        obj->str[obj->size] = '\0';
    } else {
        obj->str = NULL;
    }
    return obj;
}

__attribute__((used))
void __spl__destructor__String(String* obj) {
    if (obj->str != NULL) {
        free(obj->str);
    }
}

__attribute__((used))
void __String___concat__spl__void__String__String__String(String* _this, String *_a, String *_b) {
    _this->size = _a->size+_b->size;
    if (_this->size != 0) {
        _this->str = malloc(sizeof(char)*(_this->size+1));
        memcpy(_this->str, _a->str, _a->size*sizeof(char));
        memcpy(_this->str+_a->size*sizeof(char), _b->str, _b->size*sizeof(char));
        _this->str[_this->size] = '\0';
    } else {
        _this->str = NULL;
    }
}

__attribute__((used))
void __String___concat__spl__void__String__String__char(String* _this, String *_a, char _b) {
    _this->size = _a->size+1;
    if (_this->size != 0) {
        _this->str = malloc(sizeof(char)*(_this->size+1));
        memcpy(_this->str, _a->str, _a->size*sizeof(char));
        _this->str[_a->size] = _b;
        _this->str[_this->size] = '\0';
    } else {
        _this->str = NULL;
    }
}

__attribute__((used))
void __String___concat__spl__void__String__String__bool(String* _this, String *_a, int8_t _b) {
    if (_b) {
        _this->size = _a->size+4;
        if (_this->size != 0) {
            _this->str = malloc(sizeof(char)*(_this->size+1));
            memcpy(_this->str, _a->str, _a->size*sizeof(char));
            memcpy(_this->str+_a->size*sizeof(char), "true", 4*sizeof(char));
            _this->str[_this->size] = '\0';
        } else {
            _this->str = NULL;
        }
    } else {
        _this->size = _a->size+5;
        if (_this->size != 0) {
            _this->str = malloc(sizeof(char)*(_this->size+1));
            memcpy(_this->str, _a->str, _a->size*sizeof(char));
            memcpy(_this->str+_a->size*sizeof(char), "false", 5*sizeof(char));
            _this->str[_this->size] = '\0';
        } else {
            _this->str = NULL;
        }
    }

}

__attribute__((used))
void __String___concat__spl__void__String__String__int(String* _this, String *_a, int32_t _b) {
    int32_t l = snprintf(NULL, 0,"%d", _b);
    char* str = malloc(l+1);
    sprintf(str, "%d", _b);
    _this->size = _a->size+l;
    if (_this->size != 0) {
        _this->str = malloc(sizeof(char)*(_this->size+1));
        memcpy(_this->str, _a->str, _a->size*sizeof(char));
        memcpy(_this->str+_a->size*sizeof(char), str, l*sizeof(char));
        _this->str[_this->size] = '\0';
    } else {
        _this->str = NULL;
    }
}

__attribute__((used))
void __String___concat__spl__void__String__String__float(String* _this, String *_a, float _b) {
    int32_t l = snprintf(NULL, 0,"%f", _b);
    char* str = malloc(l+1);
    sprintf(str, "%f", _b);
    _this->size = _a->size+l;
    if (_this->size != 0) {
        _this->str = malloc(sizeof(char)*(_this->size+1));
        memcpy(_this->str, _a->str, _a->size*sizeof(char));
        memcpy(_this->str+_a->size*sizeof(char), str, l*sizeof(char));
        _this->str[_this->size] = '\0';
    } else {
        _this->str = NULL;
    }
}

__attribute__((used))
void __String___concat__spl__void__String__String__double(String* _this, String *_a, double _b) {
    int32_t l = snprintf(NULL, 0,"%f", _b);
    char* str = malloc(l+1);
    sprintf(str, "%f", _b);
    _this->size = _a->size+l;
    if (_this->size != 0) {
        _this->str = malloc(sizeof(char)*(_this->size+1));
        memcpy(_this->str, _a->str, _a->size*sizeof(char));
        memcpy(_this->str+_a->size*sizeof(char), str, l*sizeof(char));
        _this->str[_this->size] = '\0';
    } else {
        _this->str = NULL;
    }
}

__attribute__((used))
void System___out___print__spl__void__char(char a) {
    printf("%c", a);
}

__attribute__((used))
void System___out___println__spl__void__char(char a) {
    printf("%c\n", a);
}

__attribute__((used))
void System___out___print__spl__void__bool(int8_t a) {
    if (a) {
        printf("true");
    } else {
        printf("false");
    }
}

__attribute__((used))
void System___out___println__spl__void__bool(int8_t a) {
    if (a) {
        printf("true\n");
    } else {
        printf("false\n");
    }
}

__attribute__((used))
void System___out___print__spl__void__int(int a) {
    printf("%d", a);
}

__attribute__((used))
void System___out___println__spl__void__int(int a) {
    printf("%d\n", a);
}

__attribute__((used))
void System___out___print__spl__void__float(float a) {
    printf("%f", a);
}

__attribute__((used))
void System___out___println__spl__void__float(float a) {
    printf("%f\n", a);
}

__attribute__((used))
void System___out___print__spl__void__double(double a) {
    printf("%f", a);
}

__attribute__((used))
void System___out___println__spl__void__double(double a) {
    printf("%f\n", a);
}

__attribute__((used))
void System___out___print__spl__void__String(String *str) {
    printf("%s", str->str);
}

__attribute__((used))
void System___out___println__spl__void__String(String *str) {
    printf("%s\n", str->str);
}