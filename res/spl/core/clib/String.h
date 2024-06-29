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
 *  Realization of String part of standard library of spl.
 */

#ifndef STRING_H
#define STRING_H

#include "gc.h"
#include <stdio.h>

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
        obj->str = malloc(sizeof(char)*5);
        memcpy(obj->str, "true\0", 5*sizeof(char));
    } else {
        obj->size = 5;
        obj->str = malloc(sizeof(char)*6);
        memcpy(obj->str, "false\0", 6*sizeof(char));
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
        memcpy(_this->str+_a->size, _b->str, _b->size*sizeof(char));
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
            memcpy(_this->str+_a->size, "true", 4*sizeof(char));
            _this->str[_this->size] = '\0';
        } else {
            _this->str = NULL;
        }
    } else {
        _this->size = _a->size+5;
        if (_this->size != 0) {
            _this->str = malloc(sizeof(char)*(_this->size+1));
            memcpy(_this->str, _a->str, _a->size*sizeof(char));
            memcpy(_this->str+_a->size, "false", 5*sizeof(char));
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
        memcpy(_this->str+_a->size, str, l*sizeof(char));
        _this->str[_this->size] = '\0';
    } else {
        _this->str = NULL;
    }
    free(str);
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
        memcpy(_this->str+_a->size, str, l*sizeof(char));
        _this->str[_this->size] = '\0';
    } else {
        _this->str = NULL;
    }
    free(str);
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
        memcpy(_this->str+_a->size, str, l*sizeof(char));
        _this->str[_this->size] = '\0';
    } else {
        _this->str = NULL;
    }
    free(str);
}

#endif //STRING_H
