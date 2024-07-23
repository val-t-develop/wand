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

; ModuleID = 'clib/lib.c'
source_filename = "clib/lib.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-suse-linux"

%struct.Objects = type { ptr, i32, i32 }
%struct.Refs = type { ptr, i32, i32 }
%struct.Object = type { ptr, i32 }
%struct.Ref = type { ptr, i32 }
%struct.String_t = type { ptr, i64 }

@objects = dso_local global %struct.Objects zeroinitializer, align 8
@refs = dso_local global %struct.Refs zeroinitializer, align 8
@.str = private unnamed_addr constant [27 x i8] c"Can not create ref to obj\0A\00", align 1
@.str.1 = private unnamed_addr constant [6 x i8] c"true\00\00", align 1
@.str.2 = private unnamed_addr constant [7 x i8] c"false\00\00", align 1
@.str.3 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.4 = private unnamed_addr constant [4 x i8] c"%ld\00", align 1
@.str.5 = private unnamed_addr constant [3 x i8] c"%f\00", align 1
@.str.6 = private unnamed_addr constant [5 x i8] c"true\00", align 1
@.str.7 = private unnamed_addr constant [6 x i8] c"false\00", align 1
@.str.8 = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@.str.9 = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@.str.10 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@.str.11 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@.str.12 = private unnamed_addr constant [5 x i8] c"%hhd\00", align 1
@.str.13 = private unnamed_addr constant [6 x i8] c"%hhd\0A\00", align 1
@.str.14 = private unnamed_addr constant [4 x i8] c"%hd\00", align 1
@.str.15 = private unnamed_addr constant [5 x i8] c"%hd\0A\00", align 1
@.str.16 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.17 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@.str.18 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str.19 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@.str.20 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@llvm.compiler.used = appending global [46 x ptr] [ptr @__spl__init__gc, ptr @__spl__destroy__gc, ptr @__spl__alloc, ptr @__spl__destroyobj, ptr @__spl__destroyref, ptr @__spl__addref, ptr @__spl__write, ptr @__spl__constructor__String, ptr @__spl__constructor__String__String, ptr @__spl__constructor__String__char, ptr @__spl__constructor__String__bool, ptr @__spl__constructor__String__byte, ptr @__spl__constructor__String__short, ptr @__spl__constructor__String__int, ptr @__spl__constructor__String__long, ptr @__spl__constructor__String__float, ptr @__spl__constructor__String__double, ptr @__spl__constructor__String____StringLiteral, ptr @__spl__destructor__String, ptr @__String.concat__spl__void__String__String__String, ptr @__String.concat__spl__void__String__String__char, ptr @__String.concat__spl__void__String__String__bool, ptr @__String.concat__spl__void__String__String__byte, ptr @__String.concat__spl__void__String__String__short, ptr @__String.concat__spl__void__String__String__int, ptr @__String.concat__spl__void__String__String__long, ptr @__String.concat__spl__void__String__String__float, ptr @__String.concat__spl__void__String__String__double, ptr @System.out.print__spl__void__char, ptr @System.out.println__spl__void__char, ptr @System.out.print__spl__void__bool, ptr @System.out.println__spl__void__bool, ptr @System.out.print__spl__void__byte, ptr @System.out.println__spl__void__byte, ptr @System.out.print__spl__void__short, ptr @System.out.println__spl__void__short, ptr @System.out.print__spl__void__int, ptr @System.out.println__spl__void__int, ptr @System.out.print__spl__void__long, ptr @System.out.println__spl__void__long, ptr @System.out.print__spl__void__float, ptr @System.out.println__spl__void__float, ptr @System.out.print__spl__void__double, ptr @System.out.println__spl__void__double, ptr @System.out.print__spl__void__String, ptr @System.out.println__spl__void__String], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__init__gc() #0 {
  store i32 10, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 2), align 4
  store i32 0, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %1 = call noalias ptr @malloc(i64 noundef 160) #8
  store ptr %1, ptr @objects, align 8
  %2 = load ptr, ptr @objects, align 8
  call void @llvm.memset.p0.i64(ptr align 8 %2, i8 0, i64 160, i1 false)
  store i32 10, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  store i32 0, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %3 = call noalias ptr @malloc(i64 noundef 160) #8
  store ptr %3, ptr @refs, align 8
  %4 = load ptr, ptr @refs, align 8
  call void @llvm.memset.p0.i64(ptr align 8 %4, i8 0, i64 160, i1 false)
  ret void
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #1

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__destroy__gc() #0 {
  %1 = load ptr, ptr @objects, align 8
  %2 = icmp ne ptr %1, null
  br i1 %2, label %3, label %5

3:                                                ; preds = %0
  %4 = load ptr, ptr @objects, align 8
  call void @free(ptr noundef %4) #9
  br label %5

5:                                                ; preds = %3, %0
  %6 = load ptr, ptr @refs, align 8
  %7 = icmp ne ptr %6, null
  br i1 %7, label %8, label %10

8:                                                ; preds = %5
  %9 = load ptr, ptr @refs, align 8
  call void @free(ptr noundef %9) #9
  br label %10

10:                                               ; preds = %8, %5
  ret void
}

; Function Attrs: nounwind
declare void @free(ptr noundef) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__alloc(i64 noundef %0) #0 {
  %2 = alloca i64, align 8
  %3 = alloca ptr, align 8
  store i64 %0, ptr %2, align 8
  %4 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %5 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 2), align 4
  %6 = icmp eq i32 %4, %5
  br i1 %6, label %7, label %22

7:                                                ; preds = %1
  %8 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 2), align 4
  %9 = mul nsw i32 %8, 2
  store i32 %9, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 2), align 4
  %10 = load ptr, ptr @objects, align 8
  %11 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 2), align 4
  %12 = sext i32 %11 to i64
  %13 = mul i64 %12, 16
  %14 = call ptr @realloc(ptr noundef %10, i64 noundef %13) #10
  store ptr %14, ptr @objects, align 8
  %15 = load ptr, ptr @objects, align 8
  %16 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds %struct.Object, ptr %15, i64 %17
  %19 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %20 = sext i32 %19 to i64
  %21 = mul i64 %20, 16
  call void @llvm.memset.p0.i64(ptr align 8 %18, i8 0, i64 %21, i1 false)
  br label %22

22:                                               ; preds = %7, %1
  %23 = load ptr, ptr @objects, align 8
  %24 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %25 = sext i32 %24 to i64
  %26 = getelementptr inbounds %struct.Object, ptr %23, i64 %25
  %27 = getelementptr inbounds %struct.Object, ptr %26, i32 0, i32 1
  store i32 0, ptr %27, align 8
  %28 = load i64, ptr %2, align 8
  %29 = call noalias ptr @malloc(i64 noundef %28) #8
  store ptr %29, ptr %3, align 8
  %30 = load ptr, ptr %3, align 8
  %31 = load ptr, ptr @objects, align 8
  %32 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds %struct.Object, ptr %31, i64 %33
  %35 = getelementptr inbounds %struct.Object, ptr %34, i32 0, i32 0
  store ptr %30, ptr %35, align 8
  %36 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %37 = add nsw i32 %36, 1
  store i32 %37, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %38 = load ptr, ptr %3, align 8
  ret ptr %38
}

; Function Attrs: nounwind allocsize(1)
declare ptr @realloc(ptr noundef, i64 noundef) #4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__destroyobj(ptr noundef %0, ptr noundef %1, i8 noundef signext %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i8, align 1
  %7 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i8 %2, ptr %6, align 1
  store i32 0, ptr %7, align 4
  br label %8

8:                                                ; preds = %51, %3
  %9 = load i32, ptr %7, align 4
  %10 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %54

12:                                               ; preds = %8
  %13 = load ptr, ptr @objects, align 8
  %14 = load i32, ptr %7, align 4
  %15 = sext i32 %14 to i64
  %16 = getelementptr inbounds %struct.Object, ptr %13, i64 %15
  %17 = getelementptr inbounds %struct.Object, ptr %16, i32 0, i32 0
  %18 = load ptr, ptr %17, align 8
  %19 = load ptr, ptr %4, align 8
  %20 = icmp eq ptr %18, %19
  br i1 %20, label %21, label %50

21:                                               ; preds = %12
  %22 = load i8, ptr %6, align 1
  %23 = icmp ne i8 %22, 0
  br i1 %23, label %24, label %32

24:                                               ; preds = %21
  %25 = load ptr, ptr @objects, align 8
  %26 = load i32, ptr %7, align 4
  %27 = sext i32 %26 to i64
  %28 = getelementptr inbounds %struct.Object, ptr %25, i64 %27
  %29 = getelementptr inbounds %struct.Object, ptr %28, i32 0, i32 1
  %30 = load i32, ptr %29, align 8
  %31 = add nsw i32 %30, -1
  store i32 %31, ptr %29, align 8
  br label %32

32:                                               ; preds = %24, %21
  %33 = load ptr, ptr @objects, align 8
  %34 = load i32, ptr %7, align 4
  %35 = sext i32 %34 to i64
  %36 = getelementptr inbounds %struct.Object, ptr %33, i64 %35
  %37 = getelementptr inbounds %struct.Object, ptr %36, i32 0, i32 1
  %38 = load i32, ptr %37, align 8
  %39 = icmp eq i32 %38, 0
  br i1 %39, label %40, label %49

40:                                               ; preds = %32
  %41 = load ptr, ptr %5, align 8
  %42 = load ptr, ptr %4, align 8
  call void %41(ptr noundef %42)
  %43 = load ptr, ptr %4, align 8
  call void @free(ptr noundef %43) #9
  %44 = load ptr, ptr @objects, align 8
  %45 = load i32, ptr %7, align 4
  %46 = sext i32 %45 to i64
  %47 = getelementptr inbounds %struct.Object, ptr %44, i64 %46
  %48 = getelementptr inbounds %struct.Object, ptr %47, i32 0, i32 0
  store ptr null, ptr %48, align 8
  br label %49

49:                                               ; preds = %40, %32
  br label %50

50:                                               ; preds = %49, %12
  br label %51

51:                                               ; preds = %50
  %52 = load i32, ptr %7, align 4
  %53 = add nsw i32 %52, 1
  store i32 %53, ptr %7, align 4
  br label %8, !llvm.loop !6

54:                                               ; preds = %8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__destroyref(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  store i32 0, ptr %5, align 4
  br label %6

6:                                                ; preds = %90, %2
  %7 = load i32, ptr %5, align 4
  %8 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %93

10:                                               ; preds = %6
  %11 = load ptr, ptr @refs, align 8
  %12 = load i32, ptr %5, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.Ref, ptr %11, i64 %13
  %15 = getelementptr inbounds %struct.Ref, ptr %14, i32 0, i32 0
  %16 = load ptr, ptr %15, align 8
  %17 = load ptr, ptr %3, align 8
  %18 = icmp eq ptr %16, %17
  br i1 %18, label %19, label %89

19:                                               ; preds = %10
  %20 = load ptr, ptr @objects, align 8
  %21 = load ptr, ptr @refs, align 8
  %22 = load i32, ptr %5, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds %struct.Ref, ptr %21, i64 %23
  %25 = getelementptr inbounds %struct.Ref, ptr %24, i32 0, i32 1
  %26 = load i32, ptr %25, align 8
  %27 = sext i32 %26 to i64
  %28 = getelementptr inbounds %struct.Object, ptr %20, i64 %27
  %29 = getelementptr inbounds %struct.Object, ptr %28, i32 0, i32 1
  %30 = load i32, ptr %29, align 8
  %31 = add nsw i32 %30, -1
  store i32 %31, ptr %29, align 8
  %32 = load ptr, ptr @objects, align 8
  %33 = load ptr, ptr @refs, align 8
  %34 = load i32, ptr %5, align 4
  %35 = sext i32 %34 to i64
  %36 = getelementptr inbounds %struct.Ref, ptr %33, i64 %35
  %37 = getelementptr inbounds %struct.Ref, ptr %36, i32 0, i32 1
  %38 = load i32, ptr %37, align 8
  %39 = sext i32 %38 to i64
  %40 = getelementptr inbounds %struct.Object, ptr %32, i64 %39
  %41 = getelementptr inbounds %struct.Object, ptr %40, i32 0, i32 1
  %42 = load i32, ptr %41, align 8
  %43 = icmp eq i32 %42, 0
  br i1 %43, label %44, label %78

44:                                               ; preds = %19
  %45 = load ptr, ptr %4, align 8
  %46 = load ptr, ptr @objects, align 8
  %47 = load ptr, ptr @refs, align 8
  %48 = load i32, ptr %5, align 4
  %49 = sext i32 %48 to i64
  %50 = getelementptr inbounds %struct.Ref, ptr %47, i64 %49
  %51 = getelementptr inbounds %struct.Ref, ptr %50, i32 0, i32 1
  %52 = load i32, ptr %51, align 8
  %53 = sext i32 %52 to i64
  %54 = getelementptr inbounds %struct.Object, ptr %46, i64 %53
  %55 = getelementptr inbounds %struct.Object, ptr %54, i32 0, i32 0
  %56 = load ptr, ptr %55, align 8
  call void %45(ptr noundef %56)
  %57 = load ptr, ptr @objects, align 8
  %58 = load ptr, ptr @refs, align 8
  %59 = load i32, ptr %5, align 4
  %60 = sext i32 %59 to i64
  %61 = getelementptr inbounds %struct.Ref, ptr %58, i64 %60
  %62 = getelementptr inbounds %struct.Ref, ptr %61, i32 0, i32 1
  %63 = load i32, ptr %62, align 8
  %64 = sext i32 %63 to i64
  %65 = getelementptr inbounds %struct.Object, ptr %57, i64 %64
  %66 = getelementptr inbounds %struct.Object, ptr %65, i32 0, i32 0
  %67 = load ptr, ptr %66, align 8
  call void @free(ptr noundef %67) #9
  %68 = load ptr, ptr @objects, align 8
  %69 = load ptr, ptr @refs, align 8
  %70 = load i32, ptr %5, align 4
  %71 = sext i32 %70 to i64
  %72 = getelementptr inbounds %struct.Ref, ptr %69, i64 %71
  %73 = getelementptr inbounds %struct.Ref, ptr %72, i32 0, i32 1
  %74 = load i32, ptr %73, align 8
  %75 = sext i32 %74 to i64
  %76 = getelementptr inbounds %struct.Object, ptr %68, i64 %75
  %77 = getelementptr inbounds %struct.Object, ptr %76, i32 0, i32 0
  store ptr null, ptr %77, align 8
  br label %78

78:                                               ; preds = %44, %19
  %79 = load ptr, ptr @refs, align 8
  %80 = load i32, ptr %5, align 4
  %81 = sext i32 %80 to i64
  %82 = getelementptr inbounds %struct.Ref, ptr %79, i64 %81
  %83 = getelementptr inbounds %struct.Ref, ptr %82, i32 0, i32 0
  store ptr null, ptr %83, align 8
  %84 = load ptr, ptr @refs, align 8
  %85 = load i32, ptr %5, align 4
  %86 = sext i32 %85 to i64
  %87 = getelementptr inbounds %struct.Ref, ptr %84, i64 %86
  %88 = getelementptr inbounds %struct.Ref, ptr %87, i32 0, i32 1
  store i32 0, ptr %88, align 8
  br label %89

89:                                               ; preds = %78, %10
  br label %90

90:                                               ; preds = %89
  %91 = load i32, ptr %5, align 4
  %92 = add nsw i32 %91, 1
  store i32 %92, ptr %5, align 4
  br label %6, !llvm.loop !8

93:                                               ; preds = %6
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__addref(ptr noundef %0, ptr noundef %1, ptr noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store ptr %2, ptr %6, align 8
  %8 = load ptr, ptr %4, align 8
  %9 = load ptr, ptr %6, align 8
  call void @__spl__destroyref(ptr noundef %8, ptr noundef %9)
  store i32 0, ptr %7, align 4
  br label %10

10:                                               ; preds = %65, %3
  %11 = load i32, ptr %7, align 4
  %12 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %13 = icmp slt i32 %11, %12
  br i1 %13, label %14, label %68

14:                                               ; preds = %10
  %15 = load ptr, ptr @objects, align 8
  %16 = load i32, ptr %7, align 4
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds %struct.Object, ptr %15, i64 %17
  %19 = getelementptr inbounds %struct.Object, ptr %18, i32 0, i32 0
  %20 = load ptr, ptr %19, align 8
  %21 = load ptr, ptr %5, align 8
  %22 = icmp eq ptr %20, %21
  br i1 %22, label %23, label %64

23:                                               ; preds = %14
  %24 = load ptr, ptr @objects, align 8
  %25 = load i32, ptr %7, align 4
  %26 = sext i32 %25 to i64
  %27 = getelementptr inbounds %struct.Object, ptr %24, i64 %26
  %28 = getelementptr inbounds %struct.Object, ptr %27, i32 0, i32 1
  %29 = load i32, ptr %28, align 8
  %30 = add nsw i32 %29, 1
  store i32 %30, ptr %28, align 8
  %31 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %32 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %33 = icmp eq i32 %31, %32
  br i1 %33, label %34, label %49

34:                                               ; preds = %23
  %35 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %36 = mul nsw i32 %35, 2
  store i32 %36, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %37 = load ptr, ptr @refs, align 8
  %38 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %39 = sext i32 %38 to i64
  %40 = mul i64 %39, 16
  %41 = call ptr @realloc(ptr noundef %37, i64 noundef %40) #10
  store ptr %41, ptr @refs, align 8
  %42 = load ptr, ptr @refs, align 8
  %43 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %44 = sext i32 %43 to i64
  %45 = getelementptr inbounds %struct.Ref, ptr %42, i64 %44
  %46 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %47 = sext i32 %46 to i64
  %48 = mul i64 %47, 16
  call void @llvm.memset.p0.i64(ptr align 8 %45, i8 0, i64 %48, i1 false)
  br label %49

49:                                               ; preds = %34, %23
  %50 = load ptr, ptr %4, align 8
  %51 = load ptr, ptr @refs, align 8
  %52 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %53 = sext i32 %52 to i64
  %54 = getelementptr inbounds %struct.Ref, ptr %51, i64 %53
  %55 = getelementptr inbounds %struct.Ref, ptr %54, i32 0, i32 0
  store ptr %50, ptr %55, align 8
  %56 = load i32, ptr %7, align 4
  %57 = load ptr, ptr @refs, align 8
  %58 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %59 = sext i32 %58 to i64
  %60 = getelementptr inbounds %struct.Ref, ptr %57, i64 %59
  %61 = getelementptr inbounds %struct.Ref, ptr %60, i32 0, i32 1
  store i32 %56, ptr %61, align 8
  %62 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %63 = add nsw i32 %62, 1
  store i32 %63, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  br label %70

64:                                               ; preds = %14
  br label %65

65:                                               ; preds = %64
  %66 = load i32, ptr %7, align 4
  %67 = add nsw i32 %66, 1
  store i32 %67, ptr %7, align 4
  br label %10, !llvm.loop !9

68:                                               ; preds = %10
  %69 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  br label %70

70:                                               ; preds = %68, %49
  ret void
}

declare i32 @printf(ptr noundef, ...) #5

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__write(ptr noundef %0, ptr noundef %1, ptr noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store ptr %2, ptr %6, align 8
  %9 = load ptr, ptr %4, align 8
  %10 = load ptr, ptr %6, align 8
  call void @__spl__destroyref(ptr noundef %9, ptr noundef %10)
  store i32 -1, ptr %7, align 4
  store i32 0, ptr %8, align 4
  br label %11

11:                                               ; preds = %32, %3
  %12 = load i32, ptr %8, align 4
  %13 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %14 = icmp slt i32 %12, %13
  br i1 %14, label %15, label %35

15:                                               ; preds = %11
  %16 = load ptr, ptr @refs, align 8
  %17 = load i32, ptr %8, align 4
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds %struct.Ref, ptr %16, i64 %18
  %20 = getelementptr inbounds %struct.Ref, ptr %19, i32 0, i32 0
  %21 = load ptr, ptr %20, align 8
  %22 = load ptr, ptr %5, align 8
  %23 = icmp eq ptr %21, %22
  br i1 %23, label %24, label %31

24:                                               ; preds = %15
  %25 = load ptr, ptr @refs, align 8
  %26 = load i32, ptr %8, align 4
  %27 = sext i32 %26 to i64
  %28 = getelementptr inbounds %struct.Ref, ptr %25, i64 %27
  %29 = getelementptr inbounds %struct.Ref, ptr %28, i32 0, i32 1
  %30 = load i32, ptr %29, align 8
  store i32 %30, ptr %7, align 4
  br label %31

31:                                               ; preds = %24, %15
  br label %32

32:                                               ; preds = %31
  %33 = load i32, ptr %8, align 4
  %34 = add nsw i32 %33, 1
  store i32 %34, ptr %8, align 4
  br label %11, !llvm.loop !10

35:                                               ; preds = %11
  %36 = load i32, ptr %7, align 4
  %37 = icmp eq i32 %36, -1
  br i1 %37, label %38, label %39

38:                                               ; preds = %35
  br label %80

39:                                               ; preds = %35
  %40 = load ptr, ptr @objects, align 8
  %41 = load i32, ptr %7, align 4
  %42 = sext i32 %41 to i64
  %43 = getelementptr inbounds %struct.Object, ptr %40, i64 %42
  %44 = getelementptr inbounds %struct.Object, ptr %43, i32 0, i32 1
  %45 = load i32, ptr %44, align 8
  %46 = add nsw i32 %45, 1
  store i32 %46, ptr %44, align 8
  %47 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %48 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %49 = icmp eq i32 %47, %48
  br i1 %49, label %50, label %65

50:                                               ; preds = %39
  %51 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %52 = mul nsw i32 %51, 2
  store i32 %52, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %53 = load ptr, ptr @refs, align 8
  %54 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %55 = sext i32 %54 to i64
  %56 = mul i64 %55, 16
  %57 = call ptr @realloc(ptr noundef %53, i64 noundef %56) #10
  store ptr %57, ptr @refs, align 8
  %58 = load ptr, ptr @refs, align 8
  %59 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %60 = sext i32 %59 to i64
  %61 = getelementptr inbounds %struct.Ref, ptr %58, i64 %60
  %62 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %63 = sext i32 %62 to i64
  %64 = mul i64 %63, 16
  call void @llvm.memset.p0.i64(ptr align 8 %61, i8 0, i64 %64, i1 false)
  br label %65

65:                                               ; preds = %50, %39
  %66 = load ptr, ptr %4, align 8
  %67 = load ptr, ptr @refs, align 8
  %68 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %69 = sext i32 %68 to i64
  %70 = getelementptr inbounds %struct.Ref, ptr %67, i64 %69
  %71 = getelementptr inbounds %struct.Ref, ptr %70, i32 0, i32 0
  store ptr %66, ptr %71, align 8
  %72 = load i32, ptr %7, align 4
  %73 = load ptr, ptr @refs, align 8
  %74 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %75 = sext i32 %74 to i64
  %76 = getelementptr inbounds %struct.Ref, ptr %73, i64 %75
  %77 = getelementptr inbounds %struct.Ref, ptr %76, i32 0, i32 1
  store i32 %72, ptr %77, align 8
  %78 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %79 = add nsw i32 %78, 1
  store i32 %79, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  br label %80

80:                                               ; preds = %65, %38
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String() #0 {
  %1 = alloca ptr, align 8
  %2 = call ptr @__spl__alloc(i64 noundef 16)
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
  %4 = call ptr @__spl__alloc(i64 noundef 16)
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
  br i1 %13, label %14, label %40

14:                                               ; preds = %1
  %15 = load ptr, ptr %3, align 8
  %16 = getelementptr inbounds %struct.String_t, ptr %15, i32 0, i32 1
  %17 = load i64, ptr %16, align 8
  %18 = add i64 %17, 1
  %19 = mul i64 1, %18
  %20 = call noalias ptr @malloc(i64 noundef %19) #8
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
  %39 = getelementptr inbounds i8, ptr %35, i64 %38
  store i8 0, ptr %39, align 1
  br label %43

40:                                               ; preds = %1
  %41 = load ptr, ptr %3, align 8
  %42 = getelementptr inbounds %struct.String_t, ptr %41, i32 0, i32 0
  store ptr null, ptr %42, align 8
  br label %43

43:                                               ; preds = %40, %14
  %44 = load ptr, ptr %3, align 8
  ret ptr %44
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #6

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__char(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  %3 = alloca ptr, align 8
  store i8 %0, ptr %2, align 1
  %4 = call ptr @__spl__alloc(i64 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = getelementptr inbounds %struct.String_t, ptr %5, i32 0, i32 1
  store i64 1, ptr %6, align 8
  %7 = call noalias ptr @malloc(i64 noundef 2) #8
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds %struct.String_t, ptr %8, i32 0, i32 0
  store ptr %7, ptr %9, align 8
  %10 = load i8, ptr %2, align 1
  %11 = load ptr, ptr %3, align 8
  %12 = getelementptr inbounds %struct.String_t, ptr %11, i32 0, i32 0
  %13 = load ptr, ptr %12, align 8
  %14 = getelementptr inbounds i8, ptr %13, i64 0
  store i8 %10, ptr %14, align 1
  %15 = load ptr, ptr %3, align 8
  %16 = getelementptr inbounds %struct.String_t, ptr %15, i32 0, i32 0
  %17 = load ptr, ptr %16, align 8
  %18 = getelementptr inbounds i8, ptr %17, i64 1
  store i8 0, ptr %18, align 1
  %19 = load ptr, ptr %3, align 8
  ret ptr %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__bool(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  %3 = alloca ptr, align 8
  store i8 %0, ptr %2, align 1
  %4 = call ptr @__spl__alloc(i64 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load i8, ptr %2, align 1
  %6 = icmp ne i8 %5, 0
  br i1 %6, label %7, label %16

7:                                                ; preds = %1
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds %struct.String_t, ptr %8, i32 0, i32 1
  store i64 4, ptr %9, align 8
  %10 = call noalias ptr @malloc(i64 noundef 5) #8
  %11 = load ptr, ptr %3, align 8
  %12 = getelementptr inbounds %struct.String_t, ptr %11, i32 0, i32 0
  store ptr %10, ptr %12, align 8
  %13 = load ptr, ptr %3, align 8
  %14 = getelementptr inbounds %struct.String_t, ptr %13, i32 0, i32 0
  %15 = load ptr, ptr %14, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %15, ptr align 1 @.str.1, i64 5, i1 false)
  br label %25

16:                                               ; preds = %1
  %17 = load ptr, ptr %3, align 8
  %18 = getelementptr inbounds %struct.String_t, ptr %17, i32 0, i32 1
  store i64 5, ptr %18, align 8
  %19 = call noalias ptr @malloc(i64 noundef 6) #8
  %20 = load ptr, ptr %3, align 8
  %21 = getelementptr inbounds %struct.String_t, ptr %20, i32 0, i32 0
  store ptr %19, ptr %21, align 8
  %22 = load ptr, ptr %3, align 8
  %23 = getelementptr inbounds %struct.String_t, ptr %22, i32 0, i32 0
  %24 = load ptr, ptr %23, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %24, ptr align 1 @.str.2, i64 6, i1 false)
  br label %25

25:                                               ; preds = %16, %7
  %26 = load ptr, ptr %3, align 8
  ret ptr %26
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__byte(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  %3 = alloca ptr, align 8
  store i8 %0, ptr %2, align 1
  %4 = call ptr @__spl__alloc(i64 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load i8, ptr %2, align 1
  %6 = sext i8 %5 to i32
  %7 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.3, i32 noundef %6) #9
  %8 = sext i32 %7 to i64
  %9 = load ptr, ptr %3, align 8
  %10 = getelementptr inbounds %struct.String_t, ptr %9, i32 0, i32 1
  store i64 %8, ptr %10, align 8
  %11 = load ptr, ptr %3, align 8
  %12 = getelementptr inbounds %struct.String_t, ptr %11, i32 0, i32 1
  %13 = load i64, ptr %12, align 8
  %14 = add i64 %13, 1
  %15 = mul i64 1, %14
  %16 = call noalias ptr @malloc(i64 noundef %15) #8
  %17 = load ptr, ptr %3, align 8
  %18 = getelementptr inbounds %struct.String_t, ptr %17, i32 0, i32 0
  store ptr %16, ptr %18, align 8
  %19 = load ptr, ptr %3, align 8
  %20 = getelementptr inbounds %struct.String_t, ptr %19, i32 0, i32 0
  %21 = load ptr, ptr %20, align 8
  %22 = load i8, ptr %2, align 1
  %23 = sext i8 %22 to i32
  %24 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %21, ptr noundef @.str.3, i32 noundef %23) #9
  %25 = load ptr, ptr %3, align 8
  ret ptr %25
}

; Function Attrs: nounwind
declare i32 @snprintf(ptr noundef, i64 noundef, ptr noundef, ...) #3

; Function Attrs: nounwind
declare i32 @sprintf(ptr noundef, ptr noundef, ...) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__short(i16 noundef signext %0) #0 {
  %2 = alloca i16, align 2
  %3 = alloca ptr, align 8
  store i16 %0, ptr %2, align 2
  %4 = call ptr @__spl__alloc(i64 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load i16, ptr %2, align 2
  %6 = sext i16 %5 to i32
  %7 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.3, i32 noundef %6) #9
  %8 = sext i32 %7 to i64
  %9 = load ptr, ptr %3, align 8
  %10 = getelementptr inbounds %struct.String_t, ptr %9, i32 0, i32 1
  store i64 %8, ptr %10, align 8
  %11 = load ptr, ptr %3, align 8
  %12 = getelementptr inbounds %struct.String_t, ptr %11, i32 0, i32 1
  %13 = load i64, ptr %12, align 8
  %14 = add i64 %13, 1
  %15 = mul i64 1, %14
  %16 = call noalias ptr @malloc(i64 noundef %15) #8
  %17 = load ptr, ptr %3, align 8
  %18 = getelementptr inbounds %struct.String_t, ptr %17, i32 0, i32 0
  store ptr %16, ptr %18, align 8
  %19 = load ptr, ptr %3, align 8
  %20 = getelementptr inbounds %struct.String_t, ptr %19, i32 0, i32 0
  %21 = load ptr, ptr %20, align 8
  %22 = load i16, ptr %2, align 2
  %23 = sext i16 %22 to i32
  %24 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %21, ptr noundef @.str.3, i32 noundef %23) #9
  %25 = load ptr, ptr %3, align 8
  ret ptr %25
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__int(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  store i32 %0, ptr %2, align 4
  %4 = call ptr @__spl__alloc(i64 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load i32, ptr %2, align 4
  %6 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.3, i32 noundef %5) #9
  %7 = sext i32 %6 to i64
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds %struct.String_t, ptr %8, i32 0, i32 1
  store i64 %7, ptr %9, align 8
  %10 = load ptr, ptr %3, align 8
  %11 = getelementptr inbounds %struct.String_t, ptr %10, i32 0, i32 1
  %12 = load i64, ptr %11, align 8
  %13 = add i64 %12, 1
  %14 = mul i64 1, %13
  %15 = call noalias ptr @malloc(i64 noundef %14) #8
  %16 = load ptr, ptr %3, align 8
  %17 = getelementptr inbounds %struct.String_t, ptr %16, i32 0, i32 0
  store ptr %15, ptr %17, align 8
  %18 = load ptr, ptr %3, align 8
  %19 = getelementptr inbounds %struct.String_t, ptr %18, i32 0, i32 0
  %20 = load ptr, ptr %19, align 8
  %21 = load i32, ptr %2, align 4
  %22 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %20, ptr noundef @.str.3, i32 noundef %21) #9
  %23 = load ptr, ptr %3, align 8
  ret ptr %23
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__long(i64 noundef %0) #0 {
  %2 = alloca i64, align 8
  %3 = alloca ptr, align 8
  store i64 %0, ptr %2, align 8
  %4 = call ptr @__spl__alloc(i64 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load i64, ptr %2, align 8
  %6 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.4, i64 noundef %5) #9
  %7 = sext i32 %6 to i64
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds %struct.String_t, ptr %8, i32 0, i32 1
  store i64 %7, ptr %9, align 8
  %10 = load ptr, ptr %3, align 8
  %11 = getelementptr inbounds %struct.String_t, ptr %10, i32 0, i32 1
  %12 = load i64, ptr %11, align 8
  %13 = add i64 %12, 1
  %14 = mul i64 1, %13
  %15 = call noalias ptr @malloc(i64 noundef %14) #8
  %16 = load ptr, ptr %3, align 8
  %17 = getelementptr inbounds %struct.String_t, ptr %16, i32 0, i32 0
  store ptr %15, ptr %17, align 8
  %18 = load ptr, ptr %3, align 8
  %19 = getelementptr inbounds %struct.String_t, ptr %18, i32 0, i32 0
  %20 = load ptr, ptr %19, align 8
  %21 = load i64, ptr %2, align 8
  %22 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %20, ptr noundef @.str.4, i64 noundef %21) #9
  %23 = load ptr, ptr %3, align 8
  ret ptr %23
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__float(float noundef %0) #0 {
  %2 = alloca float, align 4
  %3 = alloca ptr, align 8
  store float %0, ptr %2, align 4
  %4 = call ptr @__spl__alloc(i64 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load float, ptr %2, align 4
  %6 = fpext float %5 to double
  %7 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.5, double noundef %6) #9
  %8 = sext i32 %7 to i64
  %9 = load ptr, ptr %3, align 8
  %10 = getelementptr inbounds %struct.String_t, ptr %9, i32 0, i32 1
  store i64 %8, ptr %10, align 8
  %11 = load ptr, ptr %3, align 8
  %12 = getelementptr inbounds %struct.String_t, ptr %11, i32 0, i32 1
  %13 = load i64, ptr %12, align 8
  %14 = add i64 %13, 1
  %15 = mul i64 1, %14
  %16 = call noalias ptr @malloc(i64 noundef %15) #8
  %17 = load ptr, ptr %3, align 8
  %18 = getelementptr inbounds %struct.String_t, ptr %17, i32 0, i32 0
  store ptr %16, ptr %18, align 8
  %19 = load ptr, ptr %3, align 8
  %20 = getelementptr inbounds %struct.String_t, ptr %19, i32 0, i32 0
  %21 = load ptr, ptr %20, align 8
  %22 = load float, ptr %2, align 4
  %23 = fpext float %22 to double
  %24 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %21, ptr noundef @.str.5, double noundef %23) #9
  %25 = load ptr, ptr %3, align 8
  ret ptr %25
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__double(double noundef %0) #0 {
  %2 = alloca double, align 8
  %3 = alloca ptr, align 8
  store double %0, ptr %2, align 8
  %4 = call ptr @__spl__alloc(i64 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load double, ptr %2, align 8
  %6 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.5, double noundef %5) #9
  %7 = sext i32 %6 to i64
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds %struct.String_t, ptr %8, i32 0, i32 1
  store i64 %7, ptr %9, align 8
  %10 = load ptr, ptr %3, align 8
  %11 = getelementptr inbounds %struct.String_t, ptr %10, i32 0, i32 1
  %12 = load i64, ptr %11, align 8
  %13 = add i64 %12, 1
  %14 = mul i64 1, %13
  %15 = call noalias ptr @malloc(i64 noundef %14) #8
  %16 = load ptr, ptr %3, align 8
  %17 = getelementptr inbounds %struct.String_t, ptr %16, i32 0, i32 0
  store ptr %15, ptr %17, align 8
  %18 = load ptr, ptr %3, align 8
  %19 = getelementptr inbounds %struct.String_t, ptr %18, i32 0, i32 0
  %20 = load ptr, ptr %19, align 8
  %21 = load double, ptr %2, align 8
  %22 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %20, ptr noundef @.str.5, double noundef %21) #9
  %23 = load ptr, ptr %3, align 8
  ret ptr %23
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String____StringLiteral(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %4 = call ptr @__spl__alloc(i64 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load ptr, ptr %2, align 8
  %6 = call i64 @strlen(ptr noundef %5) #11
  %7 = load ptr, ptr %3, align 8
  %8 = getelementptr inbounds %struct.String_t, ptr %7, i32 0, i32 1
  store i64 %6, ptr %8, align 8
  %9 = load ptr, ptr %3, align 8
  %10 = getelementptr inbounds %struct.String_t, ptr %9, i32 0, i32 1
  %11 = load i64, ptr %10, align 8
  %12 = icmp ne i64 %11, 0
  br i1 %12, label %13, label %37

13:                                               ; preds = %1
  %14 = load ptr, ptr %3, align 8
  %15 = getelementptr inbounds %struct.String_t, ptr %14, i32 0, i32 1
  %16 = load i64, ptr %15, align 8
  %17 = add i64 %16, 1
  %18 = mul i64 1, %17
  %19 = call noalias ptr @malloc(i64 noundef %18) #8
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
  %36 = getelementptr inbounds i8, ptr %32, i64 %35
  store i8 0, ptr %36, align 1
  br label %40

37:                                               ; preds = %1
  %38 = load ptr, ptr %3, align 8
  %39 = getelementptr inbounds %struct.String_t, ptr %38, i32 0, i32 0
  store ptr null, ptr %39, align 8
  br label %40

40:                                               ; preds = %37, %13
  %41 = load ptr, ptr %3, align 8
  ret ptr %41
}

; Function Attrs: nounwind willreturn memory(read)
declare i64 @strlen(ptr noundef) #7

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
  call void @free(ptr noundef %10) #9
  br label %11

11:                                               ; preds = %7, %1
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__String.concat__spl__void__String__String__String(ptr noundef %0, ptr noundef %1, ptr noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store ptr %2, ptr %6, align 8
  %7 = load ptr, ptr %5, align 8
  %8 = getelementptr inbounds %struct.String_t, ptr %7, i32 0, i32 1
  %9 = load i64, ptr %8, align 8
  %10 = load ptr, ptr %6, align 8
  %11 = getelementptr inbounds %struct.String_t, ptr %10, i32 0, i32 1
  %12 = load i64, ptr %11, align 8
  %13 = add i64 %9, %12
  %14 = load ptr, ptr %4, align 8
  %15 = getelementptr inbounds %struct.String_t, ptr %14, i32 0, i32 1
  store i64 %13, ptr %15, align 8
  %16 = load ptr, ptr %4, align 8
  %17 = getelementptr inbounds %struct.String_t, ptr %16, i32 0, i32 1
  %18 = load i64, ptr %17, align 8
  %19 = icmp ne i64 %18, 0
  br i1 %19, label %20, label %60

20:                                               ; preds = %3
  %21 = load ptr, ptr %4, align 8
  %22 = getelementptr inbounds %struct.String_t, ptr %21, i32 0, i32 1
  %23 = load i64, ptr %22, align 8
  %24 = add i64 %23, 1
  %25 = mul i64 1, %24
  %26 = call noalias ptr @malloc(i64 noundef %25) #8
  %27 = load ptr, ptr %4, align 8
  %28 = getelementptr inbounds %struct.String_t, ptr %27, i32 0, i32 0
  store ptr %26, ptr %28, align 8
  %29 = load ptr, ptr %4, align 8
  %30 = getelementptr inbounds %struct.String_t, ptr %29, i32 0, i32 0
  %31 = load ptr, ptr %30, align 8
  %32 = load ptr, ptr %5, align 8
  %33 = getelementptr inbounds %struct.String_t, ptr %32, i32 0, i32 0
  %34 = load ptr, ptr %33, align 8
  %35 = load ptr, ptr %5, align 8
  %36 = getelementptr inbounds %struct.String_t, ptr %35, i32 0, i32 1
  %37 = load i64, ptr %36, align 8
  %38 = mul i64 %37, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %31, ptr align 1 %34, i64 %38, i1 false)
  %39 = load ptr, ptr %4, align 8
  %40 = getelementptr inbounds %struct.String_t, ptr %39, i32 0, i32 0
  %41 = load ptr, ptr %40, align 8
  %42 = load ptr, ptr %5, align 8
  %43 = getelementptr inbounds %struct.String_t, ptr %42, i32 0, i32 1
  %44 = load i64, ptr %43, align 8
  %45 = getelementptr inbounds i8, ptr %41, i64 %44
  %46 = load ptr, ptr %6, align 8
  %47 = getelementptr inbounds %struct.String_t, ptr %46, i32 0, i32 0
  %48 = load ptr, ptr %47, align 8
  %49 = load ptr, ptr %6, align 8
  %50 = getelementptr inbounds %struct.String_t, ptr %49, i32 0, i32 1
  %51 = load i64, ptr %50, align 8
  %52 = mul i64 %51, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %45, ptr align 1 %48, i64 %52, i1 false)
  %53 = load ptr, ptr %4, align 8
  %54 = getelementptr inbounds %struct.String_t, ptr %53, i32 0, i32 0
  %55 = load ptr, ptr %54, align 8
  %56 = load ptr, ptr %4, align 8
  %57 = getelementptr inbounds %struct.String_t, ptr %56, i32 0, i32 1
  %58 = load i64, ptr %57, align 8
  %59 = getelementptr inbounds i8, ptr %55, i64 %58
  store i8 0, ptr %59, align 1
  br label %63

60:                                               ; preds = %3
  %61 = load ptr, ptr %4, align 8
  %62 = getelementptr inbounds %struct.String_t, ptr %61, i32 0, i32 0
  store ptr null, ptr %62, align 8
  br label %63

63:                                               ; preds = %60, %20
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__String.concat__spl__void__String__String__char(ptr noundef %0, ptr noundef %1, i8 noundef signext %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i8, align 1
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i8 %2, ptr %6, align 1
  %7 = load ptr, ptr %5, align 8
  %8 = getelementptr inbounds %struct.String_t, ptr %7, i32 0, i32 1
  %9 = load i64, ptr %8, align 8
  %10 = add i64 %9, 1
  %11 = load ptr, ptr %4, align 8
  %12 = getelementptr inbounds %struct.String_t, ptr %11, i32 0, i32 1
  store i64 %10, ptr %12, align 8
  %13 = load ptr, ptr %4, align 8
  %14 = getelementptr inbounds %struct.String_t, ptr %13, i32 0, i32 1
  %15 = load i64, ptr %14, align 8
  %16 = icmp ne i64 %15, 0
  br i1 %16, label %17, label %51

17:                                               ; preds = %3
  %18 = load ptr, ptr %4, align 8
  %19 = getelementptr inbounds %struct.String_t, ptr %18, i32 0, i32 1
  %20 = load i64, ptr %19, align 8
  %21 = add i64 %20, 1
  %22 = mul i64 1, %21
  %23 = call noalias ptr @malloc(i64 noundef %22) #8
  %24 = load ptr, ptr %4, align 8
  %25 = getelementptr inbounds %struct.String_t, ptr %24, i32 0, i32 0
  store ptr %23, ptr %25, align 8
  %26 = load ptr, ptr %4, align 8
  %27 = getelementptr inbounds %struct.String_t, ptr %26, i32 0, i32 0
  %28 = load ptr, ptr %27, align 8
  %29 = load ptr, ptr %5, align 8
  %30 = getelementptr inbounds %struct.String_t, ptr %29, i32 0, i32 0
  %31 = load ptr, ptr %30, align 8
  %32 = load ptr, ptr %5, align 8
  %33 = getelementptr inbounds %struct.String_t, ptr %32, i32 0, i32 1
  %34 = load i64, ptr %33, align 8
  %35 = mul i64 %34, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %28, ptr align 1 %31, i64 %35, i1 false)
  %36 = load i8, ptr %6, align 1
  %37 = load ptr, ptr %4, align 8
  %38 = getelementptr inbounds %struct.String_t, ptr %37, i32 0, i32 0
  %39 = load ptr, ptr %38, align 8
  %40 = load ptr, ptr %5, align 8
  %41 = getelementptr inbounds %struct.String_t, ptr %40, i32 0, i32 1
  %42 = load i64, ptr %41, align 8
  %43 = getelementptr inbounds i8, ptr %39, i64 %42
  store i8 %36, ptr %43, align 1
  %44 = load ptr, ptr %4, align 8
  %45 = getelementptr inbounds %struct.String_t, ptr %44, i32 0, i32 0
  %46 = load ptr, ptr %45, align 8
  %47 = load ptr, ptr %4, align 8
  %48 = getelementptr inbounds %struct.String_t, ptr %47, i32 0, i32 1
  %49 = load i64, ptr %48, align 8
  %50 = getelementptr inbounds i8, ptr %46, i64 %49
  store i8 0, ptr %50, align 1
  br label %54

51:                                               ; preds = %3
  %52 = load ptr, ptr %4, align 8
  %53 = getelementptr inbounds %struct.String_t, ptr %52, i32 0, i32 0
  store ptr null, ptr %53, align 8
  br label %54

54:                                               ; preds = %51, %17
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__String.concat__spl__void__String__String__bool(ptr noundef %0, ptr noundef %1, i8 noundef signext %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i8, align 1
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i8 %2, ptr %6, align 1
  %7 = load i8, ptr %6, align 1
  %8 = icmp ne i8 %7, 0
  br i1 %8, label %9, label %57

9:                                                ; preds = %3
  %10 = load ptr, ptr %5, align 8
  %11 = getelementptr inbounds %struct.String_t, ptr %10, i32 0, i32 1
  %12 = load i64, ptr %11, align 8
  %13 = add i64 %12, 4
  %14 = load ptr, ptr %4, align 8
  %15 = getelementptr inbounds %struct.String_t, ptr %14, i32 0, i32 1
  store i64 %13, ptr %15, align 8
  %16 = load ptr, ptr %4, align 8
  %17 = getelementptr inbounds %struct.String_t, ptr %16, i32 0, i32 1
  %18 = load i64, ptr %17, align 8
  %19 = icmp ne i64 %18, 0
  br i1 %19, label %20, label %53

20:                                               ; preds = %9
  %21 = load ptr, ptr %4, align 8
  %22 = getelementptr inbounds %struct.String_t, ptr %21, i32 0, i32 1
  %23 = load i64, ptr %22, align 8
  %24 = add i64 %23, 1
  %25 = mul i64 1, %24
  %26 = call noalias ptr @malloc(i64 noundef %25) #8
  %27 = load ptr, ptr %4, align 8
  %28 = getelementptr inbounds %struct.String_t, ptr %27, i32 0, i32 0
  store ptr %26, ptr %28, align 8
  %29 = load ptr, ptr %4, align 8
  %30 = getelementptr inbounds %struct.String_t, ptr %29, i32 0, i32 0
  %31 = load ptr, ptr %30, align 8
  %32 = load ptr, ptr %5, align 8
  %33 = getelementptr inbounds %struct.String_t, ptr %32, i32 0, i32 0
  %34 = load ptr, ptr %33, align 8
  %35 = load ptr, ptr %5, align 8
  %36 = getelementptr inbounds %struct.String_t, ptr %35, i32 0, i32 1
  %37 = load i64, ptr %36, align 8
  %38 = mul i64 %37, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %31, ptr align 1 %34, i64 %38, i1 false)
  %39 = load ptr, ptr %4, align 8
  %40 = getelementptr inbounds %struct.String_t, ptr %39, i32 0, i32 0
  %41 = load ptr, ptr %40, align 8
  %42 = load ptr, ptr %5, align 8
  %43 = getelementptr inbounds %struct.String_t, ptr %42, i32 0, i32 1
  %44 = load i64, ptr %43, align 8
  %45 = getelementptr inbounds i8, ptr %41, i64 %44
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %45, ptr align 1 @.str.6, i64 4, i1 false)
  %46 = load ptr, ptr %4, align 8
  %47 = getelementptr inbounds %struct.String_t, ptr %46, i32 0, i32 0
  %48 = load ptr, ptr %47, align 8
  %49 = load ptr, ptr %4, align 8
  %50 = getelementptr inbounds %struct.String_t, ptr %49, i32 0, i32 1
  %51 = load i64, ptr %50, align 8
  %52 = getelementptr inbounds i8, ptr %48, i64 %51
  store i8 0, ptr %52, align 1
  br label %56

53:                                               ; preds = %9
  %54 = load ptr, ptr %4, align 8
  %55 = getelementptr inbounds %struct.String_t, ptr %54, i32 0, i32 0
  store ptr null, ptr %55, align 8
  br label %56

56:                                               ; preds = %53, %20
  br label %105

57:                                               ; preds = %3
  %58 = load ptr, ptr %5, align 8
  %59 = getelementptr inbounds %struct.String_t, ptr %58, i32 0, i32 1
  %60 = load i64, ptr %59, align 8
  %61 = add i64 %60, 5
  %62 = load ptr, ptr %4, align 8
  %63 = getelementptr inbounds %struct.String_t, ptr %62, i32 0, i32 1
  store i64 %61, ptr %63, align 8
  %64 = load ptr, ptr %4, align 8
  %65 = getelementptr inbounds %struct.String_t, ptr %64, i32 0, i32 1
  %66 = load i64, ptr %65, align 8
  %67 = icmp ne i64 %66, 0
  br i1 %67, label %68, label %101

68:                                               ; preds = %57
  %69 = load ptr, ptr %4, align 8
  %70 = getelementptr inbounds %struct.String_t, ptr %69, i32 0, i32 1
  %71 = load i64, ptr %70, align 8
  %72 = add i64 %71, 1
  %73 = mul i64 1, %72
  %74 = call noalias ptr @malloc(i64 noundef %73) #8
  %75 = load ptr, ptr %4, align 8
  %76 = getelementptr inbounds %struct.String_t, ptr %75, i32 0, i32 0
  store ptr %74, ptr %76, align 8
  %77 = load ptr, ptr %4, align 8
  %78 = getelementptr inbounds %struct.String_t, ptr %77, i32 0, i32 0
  %79 = load ptr, ptr %78, align 8
  %80 = load ptr, ptr %5, align 8
  %81 = getelementptr inbounds %struct.String_t, ptr %80, i32 0, i32 0
  %82 = load ptr, ptr %81, align 8
  %83 = load ptr, ptr %5, align 8
  %84 = getelementptr inbounds %struct.String_t, ptr %83, i32 0, i32 1
  %85 = load i64, ptr %84, align 8
  %86 = mul i64 %85, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %79, ptr align 1 %82, i64 %86, i1 false)
  %87 = load ptr, ptr %4, align 8
  %88 = getelementptr inbounds %struct.String_t, ptr %87, i32 0, i32 0
  %89 = load ptr, ptr %88, align 8
  %90 = load ptr, ptr %5, align 8
  %91 = getelementptr inbounds %struct.String_t, ptr %90, i32 0, i32 1
  %92 = load i64, ptr %91, align 8
  %93 = getelementptr inbounds i8, ptr %89, i64 %92
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %93, ptr align 1 @.str.7, i64 5, i1 false)
  %94 = load ptr, ptr %4, align 8
  %95 = getelementptr inbounds %struct.String_t, ptr %94, i32 0, i32 0
  %96 = load ptr, ptr %95, align 8
  %97 = load ptr, ptr %4, align 8
  %98 = getelementptr inbounds %struct.String_t, ptr %97, i32 0, i32 1
  %99 = load i64, ptr %98, align 8
  %100 = getelementptr inbounds i8, ptr %96, i64 %99
  store i8 0, ptr %100, align 1
  br label %104

101:                                              ; preds = %57
  %102 = load ptr, ptr %4, align 8
  %103 = getelementptr inbounds %struct.String_t, ptr %102, i32 0, i32 0
  store ptr null, ptr %103, align 8
  br label %104

104:                                              ; preds = %101, %68
  br label %105

105:                                              ; preds = %104, %56
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__String.concat__spl__void__String__String__byte(ptr noundef %0, ptr noundef %1, i8 noundef signext %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i8, align 1
  %7 = alloca i32, align 4
  %8 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i8 %2, ptr %6, align 1
  %9 = load i8, ptr %6, align 1
  %10 = sext i8 %9 to i32
  %11 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.3, i32 noundef %10) #9
  store i32 %11, ptr %7, align 4
  %12 = load i32, ptr %7, align 4
  %13 = add nsw i32 %12, 1
  %14 = sext i32 %13 to i64
  %15 = call noalias ptr @malloc(i64 noundef %14) #8
  store ptr %15, ptr %8, align 8
  %16 = load ptr, ptr %8, align 8
  %17 = load i8, ptr %6, align 1
  %18 = sext i8 %17 to i32
  %19 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %16, ptr noundef @.str.3, i32 noundef %18) #9
  %20 = load ptr, ptr %5, align 8
  %21 = getelementptr inbounds %struct.String_t, ptr %20, i32 0, i32 1
  %22 = load i64, ptr %21, align 8
  %23 = load i32, ptr %7, align 4
  %24 = sext i32 %23 to i64
  %25 = add i64 %22, %24
  %26 = load ptr, ptr %4, align 8
  %27 = getelementptr inbounds %struct.String_t, ptr %26, i32 0, i32 1
  store i64 %25, ptr %27, align 8
  %28 = load ptr, ptr %4, align 8
  %29 = getelementptr inbounds %struct.String_t, ptr %28, i32 0, i32 1
  %30 = load i64, ptr %29, align 8
  %31 = icmp ne i64 %30, 0
  br i1 %31, label %32, label %69

32:                                               ; preds = %3
  %33 = load ptr, ptr %4, align 8
  %34 = getelementptr inbounds %struct.String_t, ptr %33, i32 0, i32 1
  %35 = load i64, ptr %34, align 8
  %36 = add i64 %35, 1
  %37 = mul i64 1, %36
  %38 = call noalias ptr @malloc(i64 noundef %37) #8
  %39 = load ptr, ptr %4, align 8
  %40 = getelementptr inbounds %struct.String_t, ptr %39, i32 0, i32 0
  store ptr %38, ptr %40, align 8
  %41 = load ptr, ptr %4, align 8
  %42 = getelementptr inbounds %struct.String_t, ptr %41, i32 0, i32 0
  %43 = load ptr, ptr %42, align 8
  %44 = load ptr, ptr %5, align 8
  %45 = getelementptr inbounds %struct.String_t, ptr %44, i32 0, i32 0
  %46 = load ptr, ptr %45, align 8
  %47 = load ptr, ptr %5, align 8
  %48 = getelementptr inbounds %struct.String_t, ptr %47, i32 0, i32 1
  %49 = load i64, ptr %48, align 8
  %50 = mul i64 %49, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %43, ptr align 1 %46, i64 %50, i1 false)
  %51 = load ptr, ptr %4, align 8
  %52 = getelementptr inbounds %struct.String_t, ptr %51, i32 0, i32 0
  %53 = load ptr, ptr %52, align 8
  %54 = load ptr, ptr %5, align 8
  %55 = getelementptr inbounds %struct.String_t, ptr %54, i32 0, i32 1
  %56 = load i64, ptr %55, align 8
  %57 = getelementptr inbounds i8, ptr %53, i64 %56
  %58 = load ptr, ptr %8, align 8
  %59 = load i32, ptr %7, align 4
  %60 = sext i32 %59 to i64
  %61 = mul i64 %60, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %57, ptr align 1 %58, i64 %61, i1 false)
  %62 = load ptr, ptr %4, align 8
  %63 = getelementptr inbounds %struct.String_t, ptr %62, i32 0, i32 0
  %64 = load ptr, ptr %63, align 8
  %65 = load ptr, ptr %4, align 8
  %66 = getelementptr inbounds %struct.String_t, ptr %65, i32 0, i32 1
  %67 = load i64, ptr %66, align 8
  %68 = getelementptr inbounds i8, ptr %64, i64 %67
  store i8 0, ptr %68, align 1
  br label %72

69:                                               ; preds = %3
  %70 = load ptr, ptr %4, align 8
  %71 = getelementptr inbounds %struct.String_t, ptr %70, i32 0, i32 0
  store ptr null, ptr %71, align 8
  br label %72

72:                                               ; preds = %69, %32
  %73 = load ptr, ptr %8, align 8
  call void @free(ptr noundef %73) #9
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__String.concat__spl__void__String__String__short(ptr noundef %0, ptr noundef %1, i16 noundef signext %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i16, align 2
  %7 = alloca i32, align 4
  %8 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i16 %2, ptr %6, align 2
  %9 = load i16, ptr %6, align 2
  %10 = sext i16 %9 to i32
  %11 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.3, i32 noundef %10) #9
  store i32 %11, ptr %7, align 4
  %12 = load i32, ptr %7, align 4
  %13 = add nsw i32 %12, 1
  %14 = sext i32 %13 to i64
  %15 = call noalias ptr @malloc(i64 noundef %14) #8
  store ptr %15, ptr %8, align 8
  %16 = load ptr, ptr %8, align 8
  %17 = load i16, ptr %6, align 2
  %18 = sext i16 %17 to i32
  %19 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %16, ptr noundef @.str.3, i32 noundef %18) #9
  %20 = load ptr, ptr %5, align 8
  %21 = getelementptr inbounds %struct.String_t, ptr %20, i32 0, i32 1
  %22 = load i64, ptr %21, align 8
  %23 = load i32, ptr %7, align 4
  %24 = sext i32 %23 to i64
  %25 = add i64 %22, %24
  %26 = load ptr, ptr %4, align 8
  %27 = getelementptr inbounds %struct.String_t, ptr %26, i32 0, i32 1
  store i64 %25, ptr %27, align 8
  %28 = load ptr, ptr %4, align 8
  %29 = getelementptr inbounds %struct.String_t, ptr %28, i32 0, i32 1
  %30 = load i64, ptr %29, align 8
  %31 = icmp ne i64 %30, 0
  br i1 %31, label %32, label %69

32:                                               ; preds = %3
  %33 = load ptr, ptr %4, align 8
  %34 = getelementptr inbounds %struct.String_t, ptr %33, i32 0, i32 1
  %35 = load i64, ptr %34, align 8
  %36 = add i64 %35, 1
  %37 = mul i64 1, %36
  %38 = call noalias ptr @malloc(i64 noundef %37) #8
  %39 = load ptr, ptr %4, align 8
  %40 = getelementptr inbounds %struct.String_t, ptr %39, i32 0, i32 0
  store ptr %38, ptr %40, align 8
  %41 = load ptr, ptr %4, align 8
  %42 = getelementptr inbounds %struct.String_t, ptr %41, i32 0, i32 0
  %43 = load ptr, ptr %42, align 8
  %44 = load ptr, ptr %5, align 8
  %45 = getelementptr inbounds %struct.String_t, ptr %44, i32 0, i32 0
  %46 = load ptr, ptr %45, align 8
  %47 = load ptr, ptr %5, align 8
  %48 = getelementptr inbounds %struct.String_t, ptr %47, i32 0, i32 1
  %49 = load i64, ptr %48, align 8
  %50 = mul i64 %49, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %43, ptr align 1 %46, i64 %50, i1 false)
  %51 = load ptr, ptr %4, align 8
  %52 = getelementptr inbounds %struct.String_t, ptr %51, i32 0, i32 0
  %53 = load ptr, ptr %52, align 8
  %54 = load ptr, ptr %5, align 8
  %55 = getelementptr inbounds %struct.String_t, ptr %54, i32 0, i32 1
  %56 = load i64, ptr %55, align 8
  %57 = getelementptr inbounds i8, ptr %53, i64 %56
  %58 = load ptr, ptr %8, align 8
  %59 = load i32, ptr %7, align 4
  %60 = sext i32 %59 to i64
  %61 = mul i64 %60, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %57, ptr align 1 %58, i64 %61, i1 false)
  %62 = load ptr, ptr %4, align 8
  %63 = getelementptr inbounds %struct.String_t, ptr %62, i32 0, i32 0
  %64 = load ptr, ptr %63, align 8
  %65 = load ptr, ptr %4, align 8
  %66 = getelementptr inbounds %struct.String_t, ptr %65, i32 0, i32 1
  %67 = load i64, ptr %66, align 8
  %68 = getelementptr inbounds i8, ptr %64, i64 %67
  store i8 0, ptr %68, align 1
  br label %72

69:                                               ; preds = %3
  %70 = load ptr, ptr %4, align 8
  %71 = getelementptr inbounds %struct.String_t, ptr %70, i32 0, i32 0
  store ptr null, ptr %71, align 8
  br label %72

72:                                               ; preds = %69, %32
  %73 = load ptr, ptr %8, align 8
  call void @free(ptr noundef %73) #9
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__String.concat__spl__void__String__String__int(ptr noundef %0, ptr noundef %1, i32 noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i32 %2, ptr %6, align 4
  %9 = load i32, ptr %6, align 4
  %10 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.3, i32 noundef %9) #9
  store i32 %10, ptr %7, align 4
  %11 = load i32, ptr %7, align 4
  %12 = add nsw i32 %11, 1
  %13 = sext i32 %12 to i64
  %14 = call noalias ptr @malloc(i64 noundef %13) #8
  store ptr %14, ptr %8, align 8
  %15 = load ptr, ptr %8, align 8
  %16 = load i32, ptr %6, align 4
  %17 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %15, ptr noundef @.str.3, i32 noundef %16) #9
  %18 = load ptr, ptr %5, align 8
  %19 = getelementptr inbounds %struct.String_t, ptr %18, i32 0, i32 1
  %20 = load i64, ptr %19, align 8
  %21 = load i32, ptr %7, align 4
  %22 = sext i32 %21 to i64
  %23 = add i64 %20, %22
  %24 = load ptr, ptr %4, align 8
  %25 = getelementptr inbounds %struct.String_t, ptr %24, i32 0, i32 1
  store i64 %23, ptr %25, align 8
  %26 = load ptr, ptr %4, align 8
  %27 = getelementptr inbounds %struct.String_t, ptr %26, i32 0, i32 1
  %28 = load i64, ptr %27, align 8
  %29 = icmp ne i64 %28, 0
  br i1 %29, label %30, label %67

30:                                               ; preds = %3
  %31 = load ptr, ptr %4, align 8
  %32 = getelementptr inbounds %struct.String_t, ptr %31, i32 0, i32 1
  %33 = load i64, ptr %32, align 8
  %34 = add i64 %33, 1
  %35 = mul i64 1, %34
  %36 = call noalias ptr @malloc(i64 noundef %35) #8
  %37 = load ptr, ptr %4, align 8
  %38 = getelementptr inbounds %struct.String_t, ptr %37, i32 0, i32 0
  store ptr %36, ptr %38, align 8
  %39 = load ptr, ptr %4, align 8
  %40 = getelementptr inbounds %struct.String_t, ptr %39, i32 0, i32 0
  %41 = load ptr, ptr %40, align 8
  %42 = load ptr, ptr %5, align 8
  %43 = getelementptr inbounds %struct.String_t, ptr %42, i32 0, i32 0
  %44 = load ptr, ptr %43, align 8
  %45 = load ptr, ptr %5, align 8
  %46 = getelementptr inbounds %struct.String_t, ptr %45, i32 0, i32 1
  %47 = load i64, ptr %46, align 8
  %48 = mul i64 %47, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %41, ptr align 1 %44, i64 %48, i1 false)
  %49 = load ptr, ptr %4, align 8
  %50 = getelementptr inbounds %struct.String_t, ptr %49, i32 0, i32 0
  %51 = load ptr, ptr %50, align 8
  %52 = load ptr, ptr %5, align 8
  %53 = getelementptr inbounds %struct.String_t, ptr %52, i32 0, i32 1
  %54 = load i64, ptr %53, align 8
  %55 = getelementptr inbounds i8, ptr %51, i64 %54
  %56 = load ptr, ptr %8, align 8
  %57 = load i32, ptr %7, align 4
  %58 = sext i32 %57 to i64
  %59 = mul i64 %58, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %55, ptr align 1 %56, i64 %59, i1 false)
  %60 = load ptr, ptr %4, align 8
  %61 = getelementptr inbounds %struct.String_t, ptr %60, i32 0, i32 0
  %62 = load ptr, ptr %61, align 8
  %63 = load ptr, ptr %4, align 8
  %64 = getelementptr inbounds %struct.String_t, ptr %63, i32 0, i32 1
  %65 = load i64, ptr %64, align 8
  %66 = getelementptr inbounds i8, ptr %62, i64 %65
  store i8 0, ptr %66, align 1
  br label %70

67:                                               ; preds = %3
  %68 = load ptr, ptr %4, align 8
  %69 = getelementptr inbounds %struct.String_t, ptr %68, i32 0, i32 0
  store ptr null, ptr %69, align 8
  br label %70

70:                                               ; preds = %67, %30
  %71 = load ptr, ptr %8, align 8
  call void @free(ptr noundef %71) #9
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__String.concat__spl__void__String__String__long(ptr noundef %0, ptr noundef %1, i64 noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i64, align 8
  %7 = alloca i32, align 4
  %8 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i64 %2, ptr %6, align 8
  %9 = load i64, ptr %6, align 8
  %10 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.4, i64 noundef %9) #9
  store i32 %10, ptr %7, align 4
  %11 = load i32, ptr %7, align 4
  %12 = add nsw i32 %11, 1
  %13 = sext i32 %12 to i64
  %14 = call noalias ptr @malloc(i64 noundef %13) #8
  store ptr %14, ptr %8, align 8
  %15 = load ptr, ptr %8, align 8
  %16 = load i64, ptr %6, align 8
  %17 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %15, ptr noundef @.str.4, i64 noundef %16) #9
  %18 = load ptr, ptr %5, align 8
  %19 = getelementptr inbounds %struct.String_t, ptr %18, i32 0, i32 1
  %20 = load i64, ptr %19, align 8
  %21 = load i32, ptr %7, align 4
  %22 = sext i32 %21 to i64
  %23 = add i64 %20, %22
  %24 = load ptr, ptr %4, align 8
  %25 = getelementptr inbounds %struct.String_t, ptr %24, i32 0, i32 1
  store i64 %23, ptr %25, align 8
  %26 = load ptr, ptr %4, align 8
  %27 = getelementptr inbounds %struct.String_t, ptr %26, i32 0, i32 1
  %28 = load i64, ptr %27, align 8
  %29 = icmp ne i64 %28, 0
  br i1 %29, label %30, label %67

30:                                               ; preds = %3
  %31 = load ptr, ptr %4, align 8
  %32 = getelementptr inbounds %struct.String_t, ptr %31, i32 0, i32 1
  %33 = load i64, ptr %32, align 8
  %34 = add i64 %33, 1
  %35 = mul i64 1, %34
  %36 = call noalias ptr @malloc(i64 noundef %35) #8
  %37 = load ptr, ptr %4, align 8
  %38 = getelementptr inbounds %struct.String_t, ptr %37, i32 0, i32 0
  store ptr %36, ptr %38, align 8
  %39 = load ptr, ptr %4, align 8
  %40 = getelementptr inbounds %struct.String_t, ptr %39, i32 0, i32 0
  %41 = load ptr, ptr %40, align 8
  %42 = load ptr, ptr %5, align 8
  %43 = getelementptr inbounds %struct.String_t, ptr %42, i32 0, i32 0
  %44 = load ptr, ptr %43, align 8
  %45 = load ptr, ptr %5, align 8
  %46 = getelementptr inbounds %struct.String_t, ptr %45, i32 0, i32 1
  %47 = load i64, ptr %46, align 8
  %48 = mul i64 %47, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %41, ptr align 1 %44, i64 %48, i1 false)
  %49 = load ptr, ptr %4, align 8
  %50 = getelementptr inbounds %struct.String_t, ptr %49, i32 0, i32 0
  %51 = load ptr, ptr %50, align 8
  %52 = load ptr, ptr %5, align 8
  %53 = getelementptr inbounds %struct.String_t, ptr %52, i32 0, i32 1
  %54 = load i64, ptr %53, align 8
  %55 = getelementptr inbounds i8, ptr %51, i64 %54
  %56 = load ptr, ptr %8, align 8
  %57 = load i32, ptr %7, align 4
  %58 = sext i32 %57 to i64
  %59 = mul i64 %58, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %55, ptr align 1 %56, i64 %59, i1 false)
  %60 = load ptr, ptr %4, align 8
  %61 = getelementptr inbounds %struct.String_t, ptr %60, i32 0, i32 0
  %62 = load ptr, ptr %61, align 8
  %63 = load ptr, ptr %4, align 8
  %64 = getelementptr inbounds %struct.String_t, ptr %63, i32 0, i32 1
  %65 = load i64, ptr %64, align 8
  %66 = getelementptr inbounds i8, ptr %62, i64 %65
  store i8 0, ptr %66, align 1
  br label %70

67:                                               ; preds = %3
  %68 = load ptr, ptr %4, align 8
  %69 = getelementptr inbounds %struct.String_t, ptr %68, i32 0, i32 0
  store ptr null, ptr %69, align 8
  br label %70

70:                                               ; preds = %67, %30
  %71 = load ptr, ptr %8, align 8
  call void @free(ptr noundef %71) #9
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__String.concat__spl__void__String__String__float(ptr noundef %0, ptr noundef %1, float noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca float, align 4
  %7 = alloca i32, align 4
  %8 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store float %2, ptr %6, align 4
  %9 = load float, ptr %6, align 4
  %10 = fpext float %9 to double
  %11 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.5, double noundef %10) #9
  store i32 %11, ptr %7, align 4
  %12 = load i32, ptr %7, align 4
  %13 = add nsw i32 %12, 1
  %14 = sext i32 %13 to i64
  %15 = call noalias ptr @malloc(i64 noundef %14) #8
  store ptr %15, ptr %8, align 8
  %16 = load ptr, ptr %8, align 8
  %17 = load float, ptr %6, align 4
  %18 = fpext float %17 to double
  %19 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %16, ptr noundef @.str.5, double noundef %18) #9
  %20 = load ptr, ptr %5, align 8
  %21 = getelementptr inbounds %struct.String_t, ptr %20, i32 0, i32 1
  %22 = load i64, ptr %21, align 8
  %23 = load i32, ptr %7, align 4
  %24 = sext i32 %23 to i64
  %25 = add i64 %22, %24
  %26 = load ptr, ptr %4, align 8
  %27 = getelementptr inbounds %struct.String_t, ptr %26, i32 0, i32 1
  store i64 %25, ptr %27, align 8
  %28 = load ptr, ptr %4, align 8
  %29 = getelementptr inbounds %struct.String_t, ptr %28, i32 0, i32 1
  %30 = load i64, ptr %29, align 8
  %31 = icmp ne i64 %30, 0
  br i1 %31, label %32, label %69

32:                                               ; preds = %3
  %33 = load ptr, ptr %4, align 8
  %34 = getelementptr inbounds %struct.String_t, ptr %33, i32 0, i32 1
  %35 = load i64, ptr %34, align 8
  %36 = add i64 %35, 1
  %37 = mul i64 1, %36
  %38 = call noalias ptr @malloc(i64 noundef %37) #8
  %39 = load ptr, ptr %4, align 8
  %40 = getelementptr inbounds %struct.String_t, ptr %39, i32 0, i32 0
  store ptr %38, ptr %40, align 8
  %41 = load ptr, ptr %4, align 8
  %42 = getelementptr inbounds %struct.String_t, ptr %41, i32 0, i32 0
  %43 = load ptr, ptr %42, align 8
  %44 = load ptr, ptr %5, align 8
  %45 = getelementptr inbounds %struct.String_t, ptr %44, i32 0, i32 0
  %46 = load ptr, ptr %45, align 8
  %47 = load ptr, ptr %5, align 8
  %48 = getelementptr inbounds %struct.String_t, ptr %47, i32 0, i32 1
  %49 = load i64, ptr %48, align 8
  %50 = mul i64 %49, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %43, ptr align 1 %46, i64 %50, i1 false)
  %51 = load ptr, ptr %4, align 8
  %52 = getelementptr inbounds %struct.String_t, ptr %51, i32 0, i32 0
  %53 = load ptr, ptr %52, align 8
  %54 = load ptr, ptr %5, align 8
  %55 = getelementptr inbounds %struct.String_t, ptr %54, i32 0, i32 1
  %56 = load i64, ptr %55, align 8
  %57 = getelementptr inbounds i8, ptr %53, i64 %56
  %58 = load ptr, ptr %8, align 8
  %59 = load i32, ptr %7, align 4
  %60 = sext i32 %59 to i64
  %61 = mul i64 %60, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %57, ptr align 1 %58, i64 %61, i1 false)
  %62 = load ptr, ptr %4, align 8
  %63 = getelementptr inbounds %struct.String_t, ptr %62, i32 0, i32 0
  %64 = load ptr, ptr %63, align 8
  %65 = load ptr, ptr %4, align 8
  %66 = getelementptr inbounds %struct.String_t, ptr %65, i32 0, i32 1
  %67 = load i64, ptr %66, align 8
  %68 = getelementptr inbounds i8, ptr %64, i64 %67
  store i8 0, ptr %68, align 1
  br label %72

69:                                               ; preds = %3
  %70 = load ptr, ptr %4, align 8
  %71 = getelementptr inbounds %struct.String_t, ptr %70, i32 0, i32 0
  store ptr null, ptr %71, align 8
  br label %72

72:                                               ; preds = %69, %32
  %73 = load ptr, ptr %8, align 8
  call void @free(ptr noundef %73) #9
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__String.concat__spl__void__String__String__double(ptr noundef %0, ptr noundef %1, double noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca double, align 8
  %7 = alloca i32, align 4
  %8 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store double %2, ptr %6, align 8
  %9 = load double, ptr %6, align 8
  %10 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.5, double noundef %9) #9
  store i32 %10, ptr %7, align 4
  %11 = load i32, ptr %7, align 4
  %12 = add nsw i32 %11, 1
  %13 = sext i32 %12 to i64
  %14 = call noalias ptr @malloc(i64 noundef %13) #8
  store ptr %14, ptr %8, align 8
  %15 = load ptr, ptr %8, align 8
  %16 = load double, ptr %6, align 8
  %17 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %15, ptr noundef @.str.5, double noundef %16) #9
  %18 = load ptr, ptr %5, align 8
  %19 = getelementptr inbounds %struct.String_t, ptr %18, i32 0, i32 1
  %20 = load i64, ptr %19, align 8
  %21 = load i32, ptr %7, align 4
  %22 = sext i32 %21 to i64
  %23 = add i64 %20, %22
  %24 = load ptr, ptr %4, align 8
  %25 = getelementptr inbounds %struct.String_t, ptr %24, i32 0, i32 1
  store i64 %23, ptr %25, align 8
  %26 = load ptr, ptr %4, align 8
  %27 = getelementptr inbounds %struct.String_t, ptr %26, i32 0, i32 1
  %28 = load i64, ptr %27, align 8
  %29 = icmp ne i64 %28, 0
  br i1 %29, label %30, label %67

30:                                               ; preds = %3
  %31 = load ptr, ptr %4, align 8
  %32 = getelementptr inbounds %struct.String_t, ptr %31, i32 0, i32 1
  %33 = load i64, ptr %32, align 8
  %34 = add i64 %33, 1
  %35 = mul i64 1, %34
  %36 = call noalias ptr @malloc(i64 noundef %35) #8
  %37 = load ptr, ptr %4, align 8
  %38 = getelementptr inbounds %struct.String_t, ptr %37, i32 0, i32 0
  store ptr %36, ptr %38, align 8
  %39 = load ptr, ptr %4, align 8
  %40 = getelementptr inbounds %struct.String_t, ptr %39, i32 0, i32 0
  %41 = load ptr, ptr %40, align 8
  %42 = load ptr, ptr %5, align 8
  %43 = getelementptr inbounds %struct.String_t, ptr %42, i32 0, i32 0
  %44 = load ptr, ptr %43, align 8
  %45 = load ptr, ptr %5, align 8
  %46 = getelementptr inbounds %struct.String_t, ptr %45, i32 0, i32 1
  %47 = load i64, ptr %46, align 8
  %48 = mul i64 %47, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %41, ptr align 1 %44, i64 %48, i1 false)
  %49 = load ptr, ptr %4, align 8
  %50 = getelementptr inbounds %struct.String_t, ptr %49, i32 0, i32 0
  %51 = load ptr, ptr %50, align 8
  %52 = load ptr, ptr %5, align 8
  %53 = getelementptr inbounds %struct.String_t, ptr %52, i32 0, i32 1
  %54 = load i64, ptr %53, align 8
  %55 = getelementptr inbounds i8, ptr %51, i64 %54
  %56 = load ptr, ptr %8, align 8
  %57 = load i32, ptr %7, align 4
  %58 = sext i32 %57 to i64
  %59 = mul i64 %58, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %55, ptr align 1 %56, i64 %59, i1 false)
  %60 = load ptr, ptr %4, align 8
  %61 = getelementptr inbounds %struct.String_t, ptr %60, i32 0, i32 0
  %62 = load ptr, ptr %61, align 8
  %63 = load ptr, ptr %4, align 8
  %64 = getelementptr inbounds %struct.String_t, ptr %63, i32 0, i32 1
  %65 = load i64, ptr %64, align 8
  %66 = getelementptr inbounds i8, ptr %62, i64 %65
  store i8 0, ptr %66, align 1
  br label %70

67:                                               ; preds = %3
  %68 = load ptr, ptr %4, align 8
  %69 = getelementptr inbounds %struct.String_t, ptr %68, i32 0, i32 0
  store ptr null, ptr %69, align 8
  br label %70

70:                                               ; preds = %67, %30
  %71 = load ptr, ptr %8, align 8
  call void @free(ptr noundef %71) #9
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__char(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  %3 = load i8, ptr %2, align 1
  %4 = sext i8 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.8, i32 noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__char(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  %3 = load i8, ptr %2, align 1
  %4 = sext i8 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.9, i32 noundef %4)
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
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.6)
  br label %9

7:                                                ; preds = %1
  %8 = call i32 (ptr, ...) @printf(ptr noundef @.str.7)
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
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.10)
  br label %9

7:                                                ; preds = %1
  %8 = call i32 (ptr, ...) @printf(ptr noundef @.str.11)
  br label %9

9:                                                ; preds = %7, %5
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__byte(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  %3 = load i8, ptr %2, align 1
  %4 = sext i8 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.12, i32 noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__byte(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  %3 = load i8, ptr %2, align 1
  %4 = sext i8 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.13, i32 noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__short(i16 noundef signext %0) #0 {
  %2 = alloca i16, align 2
  store i16 %0, ptr %2, align 2
  %3 = load i16, ptr %2, align 2
  %4 = sext i16 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.14, i32 noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__short(i16 noundef signext %0) #0 {
  %2 = alloca i16, align 2
  store i16 %0, ptr %2, align 2
  %3 = load i16, ptr %2, align 2
  %4 = sext i16 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.15, i32 noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__int(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__int(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.16, i32 noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__long(i64 noundef %0) #0 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i64 noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__long(i64 noundef %0) #0 {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.17, i64 noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__float(float noundef %0) #0 {
  %2 = alloca float, align 4
  store float %0, ptr %2, align 4
  %3 = load float, ptr %2, align 4
  %4 = fpext float %3 to double
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.5, double noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__float(float noundef %0) #0 {
  %2 = alloca float, align 4
  store float %0, ptr %2, align 4
  %3 = load float, ptr %2, align 4
  %4 = fpext float %3 to double
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.18, double noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__double(double noundef %0) #0 {
  %2 = alloca double, align 8
  store double %0, ptr %2, align 8
  %3 = load double, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.5, double noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__double(double noundef %0) #0 {
  %2 = alloca double, align 8
  store double %0, ptr %2, align 8
  %3 = load double, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.18, double noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__String(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds %struct.String_t, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.19, ptr noundef %5)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__String(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds %struct.String_t, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.20, ptr noundef %5)
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind allocsize(1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #7 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #8 = { nounwind allocsize(0) }
attributes #9 = { nounwind }
attributes #10 = { nounwind allocsize(1) }
attributes #11 = { nounwind willreturn memory(read) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.1.8"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
