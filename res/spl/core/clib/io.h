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
 *  Realization of io part of standard library of spl.
 */

#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdbool.h>

__attribute__((used))
void System___out___print__spl__void__char(char a) {
    printf("%c", a);
}

__attribute__((used))
void System___out___println__spl__void__char(char a) {
    printf("%c\n", a);
}

__attribute__((used))
void System___out___print__spl__void__bool(bool a) {
    if (a) {
        printf("true");
    } else {
        printf("false");
    }
}

__attribute__((used))
void System___out___println__spl__void__bool(bool a) {
    if (a) {
        printf("true\n");
    } else {
        printf("false\n");
    }
}

__attribute__((used))
void System___out___print__spl__void__byte(int8_t a) {
    printf("%hhd", a);
}

__attribute__((used))
void System___out___println__spl__void__byte(int8_t a) {
    printf("%hhd\n", a);
}

__attribute__((used))
void System___out___print__spl__void__short(int16_t a) {
    printf("%hd", a);
}

__attribute__((used))
void System___out___println__spl__void__short(int16_t a) {
    printf("%hd\n", a);
}

__attribute__((used))
void System___out___print__spl__void__int(int32_t a) {
    printf("%d", a);
}

__attribute__((used))
void System___out___println__spl__void__int(int32_t a) {
    printf("%d\n", a);
}

__attribute__((used))
void System___out___print__spl__void__long(int64_t a) {
    printf("%ld", a);
}

__attribute__((used))
void System___out___println__spl__void__long(int64_t a) {
    printf("%ld\n", a);
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

#endif //IO_H
