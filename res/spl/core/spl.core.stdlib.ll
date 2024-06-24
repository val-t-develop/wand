;  SPL - Simple Programming Language compiler
;  Copyright (C) 2022  Valentyn Tymchyshyn
;
;  This program is free software: you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation, either version 3 of the License, or any
;  later version.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with this program.  If not, see <https://www.gnu.org/licenses/>.

;  Valentyn Tymchyshyn (val.t.develop) (val.t.develo@gmail.com)
;
;  Generated lib.c llvm ir representation.

; ModuleID = 'lib.c'
source_filename = "lib.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.__spl__gcmap = type { i32, i32, ptr }
%struct.__spl__gcmap__entry = type { ptr, i32 }
%struct.String_t = type { ptr, i64 }

@__spl__m = dso_local global %struct.__spl__gcmap zeroinitializer, align 8
@.str = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@.str.2 = private unnamed_addr constant [5 x i8] c"true\00", align 1
@.str.3 = private unnamed_addr constant [6 x i8] c"false\00", align 1
@.str.4 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@.str.5 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@.str.6 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.7 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.8 = private unnamed_addr constant [3 x i8] c"%f\00", align 1
@.str.9 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str.10 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@.str.11 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@llvm.compiler.used = appending global [25 x ptr] [ptr @__spl__init__gcmap, ptr @__spl__destroy__gcmap, ptr @__spl__add__to__gc, ptr @__spl__get__refs, ptr @__spl__set__refs, ptr @__spl__dec__refs, ptr @__spl__inc__refs, ptr @__spl__alloc, ptr @__spl__write, ptr @__spl__destroyvar, ptr @__spl__constructor__String, ptr @__spl__constructor__String__String, ptr @__spl__constructor__String____StringLiteral, ptr @System.out.print__spl__void__char, ptr @System.out.println__spl__void__char, ptr @System.out.print__spl__void__bool, ptr @System.out.println__spl__void__bool, ptr @System.out.print__spl__void__int, ptr @System.out.println__spl__void__int, ptr @System.out.print__spl__void__float, ptr @System.out.println__spl__void__float, ptr @System.out.print__spl__void__double, ptr @System.out.println__spl__void__double, ptr @System.out.print__spl__void__String, ptr @System.out.println__spl__void__String], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__init__gcmap() #0 {
  store i32 0, ptr @__spl__m, align 8
  store i32 10, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 1), align 4
  %1 = load i32, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 1), align 4
  %2 = sext i32 %1 to i64
  %3 = mul i64 %2, 16
  %4 = call noalias ptr @malloc(i64 noundef %3) #7
  store ptr %4, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  ret void
}

; Function Attrs: nounwind allocsize(0)
declare dso_local noalias ptr @malloc(i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__destroy__gcmap() #0 {
  %1 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  call void @free(ptr noundef %1) #8
  ret void
}

; Function Attrs: nounwind
declare dso_local void @free(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__add__to__gc(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %5 = load i32, ptr @__spl__m, align 8
  %6 = add nsw i32 %5, 1
  %7 = load i32, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 1), align 4
  %8 = icmp slt i32 %6, %7
  br i1 %8, label %9, label %24

9:                                                ; preds = %2
  %10 = load ptr, ptr %3, align 8
  %11 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %12 = load i32, ptr @__spl__m, align 8
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %11, i64 %13
  %15 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %14, i32 0, i32 0
  store ptr %10, ptr %15, align 8
  %16 = load i32, ptr %4, align 4
  %17 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %18 = load i32, ptr @__spl__m, align 8
  %19 = sext i32 %18 to i64
  %20 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %17, i64 %19
  %21 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %20, i32 0, i32 1
  store i32 %16, ptr %21, align 8
  %22 = load i32, ptr @__spl__m, align 8
  %23 = add nsw i32 %22, 1
  store i32 %23, ptr @__spl__m, align 8
  br label %46

24:                                               ; preds = %2
  %25 = load i32, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 1), align 4
  %26 = mul nsw i32 %25, 2
  store i32 %26, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 1), align 4
  %27 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %28 = load i32, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 1), align 4
  %29 = sext i32 %28 to i64
  %30 = mul i64 %29, 16
  %31 = call ptr @realloc(ptr noundef %27, i64 noundef %30) #9
  store ptr %31, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %32 = load ptr, ptr %3, align 8
  %33 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %34 = load i32, ptr @__spl__m, align 8
  %35 = sext i32 %34 to i64
  %36 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %33, i64 %35
  %37 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %36, i32 0, i32 0
  store ptr %32, ptr %37, align 8
  %38 = load i32, ptr %4, align 4
  %39 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %40 = load i32, ptr @__spl__m, align 8
  %41 = sext i32 %40 to i64
  %42 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %39, i64 %41
  %43 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %42, i32 0, i32 1
  store i32 %38, ptr %43, align 8
  %44 = load i32, ptr @__spl__m, align 8
  %45 = add nsw i32 %44, 1
  store i32 %45, ptr @__spl__m, align 8
  br label %46

46:                                               ; preds = %24, %9
  ret void
}

; Function Attrs: nounwind allocsize(1)
declare dso_local ptr @realloc(ptr noundef, i64 noundef) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @__spl__get__refs(ptr noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 0, ptr %4, align 4
  br label %5

5:                                                ; preds = %26, %1
  %6 = load i32, ptr %4, align 4
  %7 = load i32, ptr @__spl__m, align 8
  %8 = icmp slt i32 %6, %7
  br i1 %8, label %9, label %29

9:                                                ; preds = %5
  %10 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %11 = load i32, ptr %4, align 4
  %12 = sext i32 %11 to i64
  %13 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %10, i64 %12
  %14 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %13, i32 0, i32 0
  %15 = load ptr, ptr %14, align 8
  %16 = load ptr, ptr %3, align 8
  %17 = icmp eq ptr %15, %16
  br i1 %17, label %18, label %25

18:                                               ; preds = %9
  %19 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %20 = load i32, ptr %4, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %19, i64 %21
  %23 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %22, i32 0, i32 1
  %24 = load i32, ptr %23, align 8
  store i32 %24, ptr %2, align 4
  br label %30

25:                                               ; preds = %9
  br label %26

26:                                               ; preds = %25
  %27 = load i32, ptr %4, align 4
  %28 = add nsw i32 %27, 1
  store i32 %28, ptr %4, align 4
  br label %5, !llvm.loop !4

29:                                               ; preds = %5
  store i32 -1, ptr %2, align 4
  br label %30

30:                                               ; preds = %29, %18
  %31 = load i32, ptr %2, align 4
  ret i32 %31
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__set__refs(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  store i32 0, ptr %5, align 4
  br label %6

6:                                                ; preds = %27, %2
  %7 = load i32, ptr %5, align 4
  %8 = load i32, ptr @__spl__m, align 8
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %30

10:                                               ; preds = %6
  %11 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %12 = load i32, ptr %5, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %11, i64 %13
  %15 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %14, i32 0, i32 0
  %16 = load ptr, ptr %15, align 8
  %17 = load ptr, ptr %3, align 8
  %18 = icmp eq ptr %16, %17
  br i1 %18, label %19, label %26

19:                                               ; preds = %10
  %20 = load i32, ptr %4, align 4
  %21 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %22 = load i32, ptr %5, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %21, i64 %23
  %25 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %24, i32 0, i32 1
  store i32 %20, ptr %25, align 8
  br label %26

26:                                               ; preds = %19, %10
  br label %27

27:                                               ; preds = %26
  %28 = load i32, ptr %5, align 4
  %29 = add nsw i32 %28, 1
  store i32 %29, ptr %5, align 4
  br label %6, !llvm.loop !6

30:                                               ; preds = %6
  %31 = load ptr, ptr %3, align 8
  %32 = load i32, ptr %4, align 4
  call void @__spl__add__to__gc(ptr noundef %31, i32 noundef %32)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__dec__refs(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
  store i32 0, ptr %3, align 4
  br label %4

4:                                                ; preds = %26, %1
  %5 = load i32, ptr %3, align 4
  %6 = load i32, ptr @__spl__m, align 8
  %7 = icmp slt i32 %5, %6
  br i1 %7, label %8, label %29

8:                                                ; preds = %4
  %9 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %10 = load i32, ptr %3, align 4
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %9, i64 %11
  %13 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %12, i32 0, i32 0
  %14 = load ptr, ptr %13, align 8
  %15 = load ptr, ptr %2, align 8
  %16 = icmp eq ptr %14, %15
  br i1 %16, label %17, label %25

17:                                               ; preds = %8
  %18 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %19 = load i32, ptr %3, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %18, i64 %20
  %22 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %21, i32 0, i32 1
  %23 = load i32, ptr %22, align 8
  %24 = add nsw i32 %23, -1
  store i32 %24, ptr %22, align 8
  br label %25

25:                                               ; preds = %17, %8
  br label %26

26:                                               ; preds = %25
  %27 = load i32, ptr %3, align 4
  %28 = add nsw i32 %27, 1
  store i32 %28, ptr %3, align 4
  br label %4, !llvm.loop !7

29:                                               ; preds = %4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__inc__refs(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
  store i32 0, ptr %3, align 4
  br label %4

4:                                                ; preds = %26, %1
  %5 = load i32, ptr %3, align 4
  %6 = load i32, ptr @__spl__m, align 8
  %7 = icmp slt i32 %5, %6
  br i1 %7, label %8, label %29

8:                                                ; preds = %4
  %9 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %10 = load i32, ptr %3, align 4
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %9, i64 %11
  %13 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %12, i32 0, i32 0
  %14 = load ptr, ptr %13, align 8
  %15 = load ptr, ptr %2, align 8
  %16 = icmp eq ptr %14, %15
  br i1 %16, label %17, label %25

17:                                               ; preds = %8
  %18 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  %19 = load i32, ptr %3, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %18, i64 %20
  %22 = getelementptr inbounds %struct.__spl__gcmap__entry, ptr %21, i32 0, i32 1
  %23 = load i32, ptr %22, align 8
  %24 = add nsw i32 %23, 1
  store i32 %24, ptr %22, align 8
  br label %25

25:                                               ; preds = %17, %8
  br label %26

26:                                               ; preds = %25
  %27 = load i32, ptr %3, align 4
  %28 = add nsw i32 %27, 1
  store i32 %28, ptr %3, align 4
  br label %4, !llvm.loop !8

29:                                               ; preds = %4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__alloc(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  store i32 %0, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  %5 = sext i32 %4 to i64
  %6 = call noalias ptr @malloc(i64 noundef %5) #7
  store ptr %6, ptr %3, align 8
  %7 = load ptr, ptr %3, align 8
  call void @__spl__add__to__gc(ptr noundef %7, i32 noundef 1)
  %8 = load ptr, ptr %3, align 8
  ret ptr %8
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__write(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = icmp ne ptr %5, null
  br i1 %6, label %7, label %9

7:                                                ; preds = %2
  %8 = load ptr, ptr %3, align 8
  call void @__spl__dec__refs(ptr noundef %8)
  br label %9

9:                                                ; preds = %7, %2
  %10 = load ptr, ptr %4, align 8
  %11 = icmp ne ptr %10, null
  br i1 %11, label %12, label %14

12:                                               ; preds = %9
  %13 = load ptr, ptr %4, align 8
  call void @__spl__inc__refs(ptr noundef %13)
  br label %14

14:                                               ; preds = %12, %9
  %15 = load ptr, ptr %3, align 8
  %16 = icmp ne ptr %15, null
  br i1 %16, label %17, label %25

17:                                               ; preds = %14
  %18 = load ptr, ptr %3, align 8
  %19 = call i32 @__spl__get__refs(ptr noundef %18)
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %21, label %24

21:                                               ; preds = %17
  %22 = load ptr, ptr %3, align 8
  call void @__spl__set__refs(ptr noundef %22, i32 noundef -1)
  %23 = load ptr, ptr %3, align 8
  call void @free(ptr noundef %23) #8
  br label %24

24:                                               ; preds = %21, %17
  br label %25

25:                                               ; preds = %24, %14
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__destroyvar(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = icmp ne ptr %5, null
  br i1 %6, label %7, label %22

7:                                                ; preds = %2
  %8 = load ptr, ptr %3, align 8
  call void @__spl__dec__refs(ptr noundef %8)
  %9 = load ptr, ptr %3, align 8
  %10 = call i32 @__spl__get__refs(ptr noundef %9)
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %12, label %21

12:                                               ; preds = %7
  %13 = load ptr, ptr %3, align 8
  call void @__spl__set__refs(ptr noundef %13, i32 noundef -1)
  %14 = load ptr, ptr %4, align 8
  %15 = icmp ne ptr %14, null
  br i1 %15, label %16, label %19

16:                                               ; preds = %12
  %17 = load ptr, ptr %4, align 8
  %18 = load ptr, ptr %3, align 8
  call void %17(ptr noundef %18)
  br label %19

19:                                               ; preds = %16, %12
  %20 = load ptr, ptr %3, align 8
  call void @free(ptr noundef %20) #8
  br label %21

21:                                               ; preds = %19, %7
  br label %22

22:                                               ; preds = %21, %2
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String() #0 {
  %1 = alloca ptr, align 8
  %2 = call ptr @__spl__alloc(i32 noundef 16)
  store ptr %2, ptr %1, align 8
  %3 = load ptr, ptr %1, align 8
  %4 = getelementptr inbounds %struct.String_t, ptr %3, i32 0, i32 0
  store ptr null, ptr %4, align 8
  %5 = load ptr, ptr %1, align 8
  %6 = getelementptr inbounds %struct.String_t, ptr %5, i32 0, i32 1
  store i64 0, ptr %6, align 8
  %7 = load ptr, ptr %1, align 8
  ret ptr %7
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__String(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %4 = call ptr @__spl__alloc(i32 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load ptr, ptr %2, align 8
  %6 = getelementptr inbounds %struct.String_t, ptr %5, i32 0, i32 1
  %7 = load i64, ptr %6, align 8
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds %struct.String_t, ptr %8, i32 0, i32 1
  store i64 %7, ptr %9, align 8
  %10 = load ptr, ptr %3, align 8
  %11 = getelementptr inbounds %struct.String_t, ptr %10, i32 0, i32 1
  %12 = load i64, ptr %11, align 8
  %13 = icmp ne i64 %12, 0
  br i1 %13, label %14, label %41

14:                                               ; preds = %1
  %15 = load ptr, ptr %3, align 8
  %16 = getelementptr inbounds %struct.String_t, ptr %15, i32 0, i32 1
  %17 = load i64, ptr %16, align 8
  %18 = add i64 %17, 1
  %19 = mul i64 1, %18
  %20 = call noalias ptr @malloc(i64 noundef %19) #7
  %21 = load ptr, ptr %3, align 8
  %22 = getelementptr inbounds %struct.String_t, ptr %21, i32 0, i32 0
  store ptr %20, ptr %22, align 8
  %23 = load ptr, ptr %3, align 8
  %24 = getelementptr inbounds %struct.String_t, ptr %23, i32 0, i32 0
  %25 = load ptr, ptr %24, align 8
  %26 = load ptr, ptr %2, align 8
  %27 = getelementptr inbounds %struct.String_t, ptr %26, i32 0, i32 0
  %28 = load ptr, ptr %27, align 8
  %29 = load ptr, ptr %3, align 8
  %30 = getelementptr inbounds %struct.String_t, ptr %29, i32 0, i32 1
  %31 = load i64, ptr %30, align 8
  %32 = mul i64 %31, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %25, ptr align 1 %28, i64 %32, i1 false)
  %33 = load ptr, ptr %3, align 8
  %34 = getelementptr inbounds %struct.String_t, ptr %33, i32 0, i32 0
  %35 = load ptr, ptr %34, align 8
  %36 = load ptr, ptr %3, align 8
  %37 = getelementptr inbounds %struct.String_t, ptr %36, i32 0, i32 1
  %38 = load i64, ptr %37, align 8
  %39 = mul i64 %38, 1
  %40 = getelementptr inbounds i8, ptr %35, i64 %39
  store i8 0, ptr %40, align 1
  br label %44

41:                                               ; preds = %1
  %42 = load ptr, ptr %3, align 8
  %43 = getelementptr inbounds %struct.String_t, ptr %42, i32 0, i32 0
  store ptr null, ptr %43, align 8
  br label %44

44:                                               ; preds = %41, %14
  %45 = load ptr, ptr %3, align 8
  ret ptr %45
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String____StringLiteral(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %4 = call ptr @__spl__alloc(i32 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load ptr, ptr %2, align 8
  %6 = call i64 @strlen(ptr noundef %5) #10
  %7 = load ptr, ptr %3, align 8
  %8 = getelementptr inbounds %struct.String_t, ptr %7, i32 0, i32 1
  store i64 %6, ptr %8, align 8
  %9 = load ptr, ptr %3, align 8
  %10 = getelementptr inbounds %struct.String_t, ptr %9, i32 0, i32 1
  %11 = load i64, ptr %10, align 8
  %12 = icmp ne i64 %11, 0
  br i1 %12, label %13, label %38

13:                                               ; preds = %1
  %14 = load ptr, ptr %3, align 8
  %15 = getelementptr inbounds %struct.String_t, ptr %14, i32 0, i32 1
  %16 = load i64, ptr %15, align 8
  %17 = add i64 %16, 1
  %18 = mul i64 1, %17
  %19 = call noalias ptr @malloc(i64 noundef %18) #7
  %20 = load ptr, ptr %3, align 8
  %21 = getelementptr inbounds %struct.String_t, ptr %20, i32 0, i32 0
  store ptr %19, ptr %21, align 8
  %22 = load ptr, ptr %3, align 8
  %23 = getelementptr inbounds %struct.String_t, ptr %22, i32 0, i32 0
  %24 = load ptr, ptr %23, align 8
  %25 = load ptr, ptr %2, align 8
  %26 = load ptr, ptr %3, align 8
  %27 = getelementptr inbounds %struct.String_t, ptr %26, i32 0, i32 1
  %28 = load i64, ptr %27, align 8
  %29 = mul i64 %28, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %24, ptr align 1 %25, i64 %29, i1 false)
  %30 = load ptr, ptr %3, align 8
  %31 = getelementptr inbounds %struct.String_t, ptr %30, i32 0, i32 0
  %32 = load ptr, ptr %31, align 8
  %33 = load ptr, ptr %3, align 8
  %34 = getelementptr inbounds %struct.String_t, ptr %33, i32 0, i32 1
  %35 = load i64, ptr %34, align 8
  %36 = mul i64 %35, 1
  %37 = getelementptr inbounds i8, ptr %32, i64 %36
  store i8 0, ptr %37, align 1
  br label %41

38:                                               ; preds = %1
  %39 = load ptr, ptr %3, align 8
  %40 = getelementptr inbounds %struct.String_t, ptr %39, i32 0, i32 0
  store ptr null, ptr %40, align 8
  br label %41

41:                                               ; preds = %38, %13
  %42 = load ptr, ptr %3, align 8
  ret ptr %42
}

; Function Attrs: nounwind willreturn memory(read)
declare dso_local i64 @strlen(ptr noundef) #5

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__destructor__String(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds %struct.String_t, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = icmp ne ptr %5, null
  br i1 %6, label %7, label %11

7:                                                ; preds = %1
  %8 = load ptr, ptr %2, align 8
  %9 = getelementptr inbounds %struct.String_t, ptr %8, i32 0, i32 0
  %10 = load ptr, ptr %9, align 8
  call void @free(ptr noundef %10) #8
  br label %11

11:                                               ; preds = %7, %1
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__char(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  %3 = load i8, ptr %2, align 1
  %4 = sext i8 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %4)
  ret void
}

declare dso_local i32 @printf(ptr noundef, ...) #6

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__char(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  %3 = load i8, ptr %2, align 1
  %4 = sext i8 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__bool(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  %3 = load i8, ptr %2, align 1
  %4 = icmp ne i8 %3, 0
  br i1 %4, label %5, label %7

5:                                                ; preds = %1
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.2)
  br label %9

7:                                                ; preds = %1
  %8 = call i32 (ptr, ...) @printf(ptr noundef @.str.3)
  br label %9

9:                                                ; preds = %7, %5
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__bool(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  %3 = load i8, ptr %2, align 1
  %4 = icmp ne i8 %3, 0
  br i1 %4, label %5, label %7

5:                                                ; preds = %1
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.4)
  br label %9

7:                                                ; preds = %1
  %8 = call i32 (ptr, ...) @printf(ptr noundef @.str.5)
  br label %9

9:                                                ; preds = %7, %5
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__int(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.6, i32 noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__int(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.7, i32 noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__float(float noundef %0) #0 {
  %2 = alloca float, align 4
  store float %0, ptr %2, align 4
  %3 = load float, ptr %2, align 4
  %4 = fpext float %3 to double
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.8, double noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__float(float noundef %0) #0 {
  %2 = alloca float, align 4
  store float %0, ptr %2, align 4
  %3 = load float, ptr %2, align 4
  %4 = fpext float %3 to double
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.9, double noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__double(double noundef %0) #0 {
  %2 = alloca double, align 8
  store double %0, ptr %2, align 8
  %3 = load double, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.8, double noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__double(double noundef %0) #0 {
  %2 = alloca double, align 8
  store double %0, ptr %2, align 8
  %3 = load double, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.9, double noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__String(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds %struct.String_t, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.10, ptr noundef %5)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__String(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds %struct.String_t, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.11, ptr noundef %5)
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind allocsize(1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #5 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { nounwind allocsize(0) }
attributes #8 = { nounwind }
attributes #9 = { nounwind allocsize(1) }
attributes #10 = { nounwind willreturn memory(read) }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 18.1.6 (Fedora 18.1.6-3.fc40)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = distinct !{!6, !5}
!7 = distinct !{!7, !5}
!8 = distinct !{!8, !5}
