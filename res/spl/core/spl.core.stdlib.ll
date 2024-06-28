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
target triple = "x86_64-redhat-linux-gnu"

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
@.str.4 = private unnamed_addr constant [3 x i8] c"%f\00", align 1
@.str.5 = private unnamed_addr constant [5 x i8] c"true\00", align 1
@.str.6 = private unnamed_addr constant [6 x i8] c"false\00", align 1
@.str.7 = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@.str.8 = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@.str.9 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@.str.10 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@.str.11 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.12 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str.13 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@.str.14 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@llvm.compiler.used = appending global [34 x ptr] [ptr @__spl__init__gc, ptr @__spl__destroy__gc, ptr @__spl__alloc, ptr @__spl__destroyobj, ptr @__spl__destroyref, ptr @__spl__addref, ptr @__spl__write, ptr @__spl__constructor__String, ptr @__spl__constructor__String__String, ptr @__spl__constructor__String__char, ptr @__spl__constructor__String__bool, ptr @__spl__constructor__String__int, ptr @__spl__constructor__String__float, ptr @__spl__constructor__String__double, ptr @__spl__constructor__String____StringLiteral, ptr @__spl__destructor__String, ptr @__String.concat__spl__void__String__String__String, ptr @__String.concat__spl__void__String__String__char, ptr @__String.concat__spl__void__String__String__bool, ptr @__String.concat__spl__void__String__String__int, ptr @__String.concat__spl__void__String__String__float, ptr @__String.concat__spl__void__String__String__double, ptr @System.out.print__spl__void__char, ptr @System.out.println__spl__void__char, ptr @System.out.print__spl__void__bool, ptr @System.out.println__spl__void__bool, ptr @System.out.print__spl__void__int, ptr @System.out.println__spl__void__int, ptr @System.out.print__spl__void__float, ptr @System.out.println__spl__void__float, ptr @System.out.print__spl__void__double, ptr @System.out.println__spl__void__double, ptr @System.out.print__spl__void__String, ptr @System.out.println__spl__void__String], section "llvm.metadata"

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
declare dso_local noalias ptr @malloc(i64 noundef) #1

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
declare dso_local void @free(ptr noundef) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__alloc(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  store i32 %0, ptr %2, align 4
  %4 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %5 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 2), align 4
  %6 = icmp eq i32 %4, %5
  br i1 %6, label %7, label %25

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
  %16 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 2), align 4
  %17 = sdiv i32 %16, 2
  %18 = sext i32 %17 to i64
  %19 = mul i64 %18, 16
  %20 = getelementptr inbounds %struct.Object, ptr %15, i64 %19
  %21 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 2), align 4
  %22 = sdiv i32 %21, 2
  %23 = sext i32 %22 to i64
  %24 = mul i64 %23, 16
  call void @llvm.memset.p0.i64(ptr align 8 %20, i8 0, i64 %24, i1 false)
  br label %25

25:                                               ; preds = %7, %1
  %26 = load ptr, ptr @objects, align 8
  %27 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %28 = sext i32 %27 to i64
  %29 = getelementptr inbounds %struct.Object, ptr %26, i64 %28
  %30 = getelementptr inbounds %struct.Object, ptr %29, i32 0, i32 1
  store i32 0, ptr %30, align 8
  %31 = load i32, ptr %2, align 4
  %32 = sext i32 %31 to i64
  %33 = call noalias ptr @malloc(i64 noundef %32) #8
  store ptr %33, ptr %3, align 8
  %34 = load ptr, ptr %3, align 8
  %35 = load ptr, ptr @objects, align 8
  %36 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %37 = sext i32 %36 to i64
  %38 = getelementptr inbounds %struct.Object, ptr %35, i64 %37
  %39 = getelementptr inbounds %struct.Object, ptr %38, i32 0, i32 0
  store ptr %34, ptr %39, align 8
  %40 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %41 = add nsw i32 %40, 1
  store i32 %41, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %42 = load ptr, ptr %3, align 8
  ret ptr %42
}

; Function Attrs: nounwind allocsize(1)
declare dso_local ptr @realloc(ptr noundef, i64 noundef) #4

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
  br label %8, !llvm.loop !4

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
  br label %6, !llvm.loop !6

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

10:                                               ; preds = %68, %3
  %11 = load i32, ptr %7, align 4
  %12 = load i32, ptr getelementptr inbounds (%struct.Objects, ptr @objects, i32 0, i32 1), align 8
  %13 = icmp slt i32 %11, %12
  br i1 %13, label %14, label %71

14:                                               ; preds = %10
  %15 = load ptr, ptr @objects, align 8
  %16 = load i32, ptr %7, align 4
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds %struct.Object, ptr %15, i64 %17
  %19 = getelementptr inbounds %struct.Object, ptr %18, i32 0, i32 0
  %20 = load ptr, ptr %19, align 8
  %21 = load ptr, ptr %5, align 8
  %22 = icmp eq ptr %20, %21
  br i1 %22, label %23, label %67

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
  br i1 %33, label %34, label %52

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
  %43 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %44 = sdiv i32 %43, 2
  %45 = sext i32 %44 to i64
  %46 = mul i64 %45, 16
  %47 = getelementptr inbounds %struct.Ref, ptr %42, i64 %46
  %48 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %49 = sdiv i32 %48, 2
  %50 = sext i32 %49 to i64
  %51 = mul i64 %50, 16
  call void @llvm.memset.p0.i64(ptr align 8 %47, i8 0, i64 %51, i1 false)
  br label %52

52:                                               ; preds = %34, %23
  %53 = load ptr, ptr %4, align 8
  %54 = load ptr, ptr @refs, align 8
  %55 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %56 = sext i32 %55 to i64
  %57 = getelementptr inbounds %struct.Ref, ptr %54, i64 %56
  %58 = getelementptr inbounds %struct.Ref, ptr %57, i32 0, i32 0
  store ptr %53, ptr %58, align 8
  %59 = load i32, ptr %7, align 4
  %60 = load ptr, ptr @refs, align 8
  %61 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %62 = sext i32 %61 to i64
  %63 = getelementptr inbounds %struct.Ref, ptr %60, i64 %62
  %64 = getelementptr inbounds %struct.Ref, ptr %63, i32 0, i32 1
  store i32 %59, ptr %64, align 8
  %65 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %66 = add nsw i32 %65, 1
  store i32 %66, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  br label %73

67:                                               ; preds = %14
  br label %68

68:                                               ; preds = %67
  %69 = load i32, ptr %7, align 4
  %70 = add nsw i32 %69, 1
  store i32 %70, ptr %7, align 4
  br label %10, !llvm.loop !7

71:                                               ; preds = %10
  %72 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  br label %73

73:                                               ; preds = %71, %52
  ret void
}

declare dso_local i32 @printf(ptr noundef, ...) #5

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
  br label %11, !llvm.loop !8

35:                                               ; preds = %11
  %36 = load i32, ptr %7, align 4
  %37 = icmp eq i32 %36, -1
  br i1 %37, label %38, label %39

38:                                               ; preds = %35
  br label %83

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
  br i1 %49, label %50, label %68

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
  %59 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %60 = sdiv i32 %59, 2
  %61 = sext i32 %60 to i64
  %62 = mul i64 %61, 16
  %63 = getelementptr inbounds %struct.Ref, ptr %58, i64 %62
  %64 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 2), align 4
  %65 = sdiv i32 %64, 2
  %66 = sext i32 %65 to i64
  %67 = mul i64 %66, 16
  call void @llvm.memset.p0.i64(ptr align 8 %63, i8 0, i64 %67, i1 false)
  br label %68

68:                                               ; preds = %50, %39
  %69 = load ptr, ptr %4, align 8
  %70 = load ptr, ptr @refs, align 8
  %71 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %72 = sext i32 %71 to i64
  %73 = getelementptr inbounds %struct.Ref, ptr %70, i64 %72
  %74 = getelementptr inbounds %struct.Ref, ptr %73, i32 0, i32 0
  store ptr %69, ptr %74, align 8
  %75 = load i32, ptr %7, align 4
  %76 = load ptr, ptr @refs, align 8
  %77 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %78 = sext i32 %77 to i64
  %79 = getelementptr inbounds %struct.Ref, ptr %76, i64 %78
  %80 = getelementptr inbounds %struct.Ref, ptr %79, i32 0, i32 1
  store i32 %75, ptr %80, align 8
  %81 = load i32, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  %82 = add nsw i32 %81, 1
  store i32 %82, ptr getelementptr inbounds (%struct.Refs, ptr @refs, i32 0, i32 1), align 8
  br label %83

83:                                               ; preds = %68, %38
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
  %4 = call ptr @__spl__alloc(i32 noundef 16)
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
  %4 = call ptr @__spl__alloc(i32 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load i8, ptr %2, align 1
  %6 = icmp ne i8 %5, 0
  br i1 %6, label %7, label %15

7:                                                ; preds = %1
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds %struct.String_t, ptr %8, i32 0, i32 1
  store i64 4, ptr %9, align 8
  %10 = call noalias ptr @malloc(i64 noundef 4) #8
  %11 = load ptr, ptr %3, align 8
  %12 = getelementptr inbounds %struct.String_t, ptr %11, i32 0, i32 0
  store ptr %10, ptr %12, align 8
  %13 = load ptr, ptr %3, align 8
  %14 = getelementptr inbounds %struct.String_t, ptr %13, i32 0, i32 0
  store ptr @.str.1, ptr %14, align 8
  br label %23

15:                                               ; preds = %1
  %16 = load ptr, ptr %3, align 8
  %17 = getelementptr inbounds %struct.String_t, ptr %16, i32 0, i32 1
  store i64 5, ptr %17, align 8
  %18 = call noalias ptr @malloc(i64 noundef 5) #8
  %19 = load ptr, ptr %3, align 8
  %20 = getelementptr inbounds %struct.String_t, ptr %19, i32 0, i32 0
  store ptr %18, ptr %20, align 8
  %21 = load ptr, ptr %3, align 8
  %22 = getelementptr inbounds %struct.String_t, ptr %21, i32 0, i32 0
  store ptr @.str.2, ptr %22, align 8
  br label %23

23:                                               ; preds = %15, %7
  %24 = load ptr, ptr %3, align 8
  ret ptr %24
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__int(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  store i32 %0, ptr %2, align 4
  %4 = call ptr @__spl__alloc(i32 noundef 16)
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

; Function Attrs: nounwind
declare dso_local i32 @snprintf(ptr noundef, i64 noundef, ptr noundef, ...) #3

; Function Attrs: nounwind
declare dso_local i32 @sprintf(ptr noundef, ptr noundef, ...) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__float(float noundef %0) #0 {
  %2 = alloca float, align 4
  %3 = alloca ptr, align 8
  store float %0, ptr %2, align 4
  %4 = call ptr @__spl__alloc(i32 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load float, ptr %2, align 4
  %6 = fpext float %5 to double
  %7 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.4, double noundef %6) #9
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
  %24 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %21, ptr noundef @.str.4, double noundef %23) #9
  %25 = load ptr, ptr %3, align 8
  ret ptr %25
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String__double(double noundef %0) #0 {
  %2 = alloca double, align 8
  %3 = alloca ptr, align 8
  store double %0, ptr %2, align 8
  %4 = call ptr @__spl__alloc(i32 noundef 16)
  store ptr %4, ptr %3, align 8
  %5 = load double, ptr %2, align 8
  %6 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.4, double noundef %5) #9
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
  %22 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %20, ptr noundef @.str.4, double noundef %21) #9
  %23 = load ptr, ptr %3, align 8
  ret ptr %23
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @__spl__constructor__String____StringLiteral(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %4 = call ptr @__spl__alloc(i32 noundef 16)
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
declare dso_local i64 @strlen(ptr noundef) #7

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
  br i1 %19, label %20, label %61

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
  %45 = mul i64 %44, 1
  %46 = getelementptr inbounds i8, ptr %41, i64 %45
  %47 = load ptr, ptr %6, align 8
  %48 = getelementptr inbounds %struct.String_t, ptr %47, i32 0, i32 0
  %49 = load ptr, ptr %48, align 8
  %50 = load ptr, ptr %6, align 8
  %51 = getelementptr inbounds %struct.String_t, ptr %50, i32 0, i32 1
  %52 = load i64, ptr %51, align 8
  %53 = mul i64 %52, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %46, ptr align 1 %49, i64 %53, i1 false)
  %54 = load ptr, ptr %4, align 8
  %55 = getelementptr inbounds %struct.String_t, ptr %54, i32 0, i32 0
  %56 = load ptr, ptr %55, align 8
  %57 = load ptr, ptr %4, align 8
  %58 = getelementptr inbounds %struct.String_t, ptr %57, i32 0, i32 1
  %59 = load i64, ptr %58, align 8
  %60 = getelementptr inbounds i8, ptr %56, i64 %59
  store i8 0, ptr %60, align 1
  br label %64

61:                                               ; preds = %3
  %62 = load ptr, ptr %4, align 8
  %63 = getelementptr inbounds %struct.String_t, ptr %62, i32 0, i32 0
  store ptr null, ptr %63, align 8
  br label %64

64:                                               ; preds = %61, %20
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
  br i1 %8, label %9, label %58

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
  br i1 %19, label %20, label %54

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
  %45 = mul i64 %44, 1
  %46 = getelementptr inbounds i8, ptr %41, i64 %45
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %46, ptr align 1 @.str.5, i64 4, i1 false)
  %47 = load ptr, ptr %4, align 8
  %48 = getelementptr inbounds %struct.String_t, ptr %47, i32 0, i32 0
  %49 = load ptr, ptr %48, align 8
  %50 = load ptr, ptr %4, align 8
  %51 = getelementptr inbounds %struct.String_t, ptr %50, i32 0, i32 1
  %52 = load i64, ptr %51, align 8
  %53 = getelementptr inbounds i8, ptr %49, i64 %52
  store i8 0, ptr %53, align 1
  br label %57

54:                                               ; preds = %9
  %55 = load ptr, ptr %4, align 8
  %56 = getelementptr inbounds %struct.String_t, ptr %55, i32 0, i32 0
  store ptr null, ptr %56, align 8
  br label %57

57:                                               ; preds = %54, %20
  br label %107

58:                                               ; preds = %3
  %59 = load ptr, ptr %5, align 8
  %60 = getelementptr inbounds %struct.String_t, ptr %59, i32 0, i32 1
  %61 = load i64, ptr %60, align 8
  %62 = add i64 %61, 5
  %63 = load ptr, ptr %4, align 8
  %64 = getelementptr inbounds %struct.String_t, ptr %63, i32 0, i32 1
  store i64 %62, ptr %64, align 8
  %65 = load ptr, ptr %4, align 8
  %66 = getelementptr inbounds %struct.String_t, ptr %65, i32 0, i32 1
  %67 = load i64, ptr %66, align 8
  %68 = icmp ne i64 %67, 0
  br i1 %68, label %69, label %103

69:                                               ; preds = %58
  %70 = load ptr, ptr %4, align 8
  %71 = getelementptr inbounds %struct.String_t, ptr %70, i32 0, i32 1
  %72 = load i64, ptr %71, align 8
  %73 = add i64 %72, 1
  %74 = mul i64 1, %73
  %75 = call noalias ptr @malloc(i64 noundef %74) #8
  %76 = load ptr, ptr %4, align 8
  %77 = getelementptr inbounds %struct.String_t, ptr %76, i32 0, i32 0
  store ptr %75, ptr %77, align 8
  %78 = load ptr, ptr %4, align 8
  %79 = getelementptr inbounds %struct.String_t, ptr %78, i32 0, i32 0
  %80 = load ptr, ptr %79, align 8
  %81 = load ptr, ptr %5, align 8
  %82 = getelementptr inbounds %struct.String_t, ptr %81, i32 0, i32 0
  %83 = load ptr, ptr %82, align 8
  %84 = load ptr, ptr %5, align 8
  %85 = getelementptr inbounds %struct.String_t, ptr %84, i32 0, i32 1
  %86 = load i64, ptr %85, align 8
  %87 = mul i64 %86, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %80, ptr align 1 %83, i64 %87, i1 false)
  %88 = load ptr, ptr %4, align 8
  %89 = getelementptr inbounds %struct.String_t, ptr %88, i32 0, i32 0
  %90 = load ptr, ptr %89, align 8
  %91 = load ptr, ptr %5, align 8
  %92 = getelementptr inbounds %struct.String_t, ptr %91, i32 0, i32 1
  %93 = load i64, ptr %92, align 8
  %94 = mul i64 %93, 1
  %95 = getelementptr inbounds i8, ptr %90, i64 %94
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %95, ptr align 1 @.str.6, i64 5, i1 false)
  %96 = load ptr, ptr %4, align 8
  %97 = getelementptr inbounds %struct.String_t, ptr %96, i32 0, i32 0
  %98 = load ptr, ptr %97, align 8
  %99 = load ptr, ptr %4, align 8
  %100 = getelementptr inbounds %struct.String_t, ptr %99, i32 0, i32 1
  %101 = load i64, ptr %100, align 8
  %102 = getelementptr inbounds i8, ptr %98, i64 %101
  store i8 0, ptr %102, align 1
  br label %106

103:                                              ; preds = %58
  %104 = load ptr, ptr %4, align 8
  %105 = getelementptr inbounds %struct.String_t, ptr %104, i32 0, i32 0
  store ptr null, ptr %105, align 8
  br label %106

106:                                              ; preds = %103, %69
  br label %107

107:                                              ; preds = %106, %57
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
  br i1 %29, label %30, label %68

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
  %55 = mul i64 %54, 1
  %56 = getelementptr inbounds i8, ptr %51, i64 %55
  %57 = load ptr, ptr %8, align 8
  %58 = load i32, ptr %7, align 4
  %59 = sext i32 %58 to i64
  %60 = mul i64 %59, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %56, ptr align 1 %57, i64 %60, i1 false)
  %61 = load ptr, ptr %4, align 8
  %62 = getelementptr inbounds %struct.String_t, ptr %61, i32 0, i32 0
  %63 = load ptr, ptr %62, align 8
  %64 = load ptr, ptr %4, align 8
  %65 = getelementptr inbounds %struct.String_t, ptr %64, i32 0, i32 1
  %66 = load i64, ptr %65, align 8
  %67 = getelementptr inbounds i8, ptr %63, i64 %66
  store i8 0, ptr %67, align 1
  br label %71

68:                                               ; preds = %3
  %69 = load ptr, ptr %4, align 8
  %70 = getelementptr inbounds %struct.String_t, ptr %69, i32 0, i32 0
  store ptr null, ptr %70, align 8
  br label %71

71:                                               ; preds = %68, %30
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
  %11 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.4, double noundef %10) #9
  store i32 %11, ptr %7, align 4
  %12 = load i32, ptr %7, align 4
  %13 = add nsw i32 %12, 1
  %14 = sext i32 %13 to i64
  %15 = call noalias ptr @malloc(i64 noundef %14) #8
  store ptr %15, ptr %8, align 8
  %16 = load ptr, ptr %8, align 8
  %17 = load float, ptr %6, align 4
  %18 = fpext float %17 to double
  %19 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %16, ptr noundef @.str.4, double noundef %18) #9
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
  br i1 %31, label %32, label %70

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
  %57 = mul i64 %56, 1
  %58 = getelementptr inbounds i8, ptr %53, i64 %57
  %59 = load ptr, ptr %8, align 8
  %60 = load i32, ptr %7, align 4
  %61 = sext i32 %60 to i64
  %62 = mul i64 %61, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %58, ptr align 1 %59, i64 %62, i1 false)
  %63 = load ptr, ptr %4, align 8
  %64 = getelementptr inbounds %struct.String_t, ptr %63, i32 0, i32 0
  %65 = load ptr, ptr %64, align 8
  %66 = load ptr, ptr %4, align 8
  %67 = getelementptr inbounds %struct.String_t, ptr %66, i32 0, i32 1
  %68 = load i64, ptr %67, align 8
  %69 = getelementptr inbounds i8, ptr %65, i64 %68
  store i8 0, ptr %69, align 1
  br label %73

70:                                               ; preds = %3
  %71 = load ptr, ptr %4, align 8
  %72 = getelementptr inbounds %struct.String_t, ptr %71, i32 0, i32 0
  store ptr null, ptr %72, align 8
  br label %73

73:                                               ; preds = %70, %32
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
  %10 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef null, i64 noundef 0, ptr noundef @.str.4, double noundef %9) #9
  store i32 %10, ptr %7, align 4
  %11 = load i32, ptr %7, align 4
  %12 = add nsw i32 %11, 1
  %13 = sext i32 %12 to i64
  %14 = call noalias ptr @malloc(i64 noundef %13) #8
  store ptr %14, ptr %8, align 8
  %15 = load ptr, ptr %8, align 8
  %16 = load double, ptr %6, align 8
  %17 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %15, ptr noundef @.str.4, double noundef %16) #9
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
  br i1 %29, label %30, label %68

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
  %55 = mul i64 %54, 1
  %56 = getelementptr inbounds i8, ptr %51, i64 %55
  %57 = load ptr, ptr %8, align 8
  %58 = load i32, ptr %7, align 4
  %59 = sext i32 %58 to i64
  %60 = mul i64 %59, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %56, ptr align 1 %57, i64 %60, i1 false)
  %61 = load ptr, ptr %4, align 8
  %62 = getelementptr inbounds %struct.String_t, ptr %61, i32 0, i32 0
  %63 = load ptr, ptr %62, align 8
  %64 = load ptr, ptr %4, align 8
  %65 = getelementptr inbounds %struct.String_t, ptr %64, i32 0, i32 1
  %66 = load i64, ptr %65, align 8
  %67 = getelementptr inbounds i8, ptr %63, i64 %66
  store i8 0, ptr %67, align 1
  br label %71

68:                                               ; preds = %3
  %69 = load ptr, ptr %4, align 8
  %70 = getelementptr inbounds %struct.String_t, ptr %69, i32 0, i32 0
  store ptr null, ptr %70, align 8
  br label %71

71:                                               ; preds = %68, %30
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__char(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  %3 = load i8, ptr %2, align 1
  %4 = sext i8 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.7, i32 noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__char(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  %3 = load i8, ptr %2, align 1
  %4 = sext i8 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.8, i32 noundef %4)
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
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.5)
  br label %9

7:                                                ; preds = %1
  %8 = call i32 (ptr, ...) @printf(ptr noundef @.str.6)
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
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.9)
  br label %9

7:                                                ; preds = %1
  %8 = call i32 (ptr, ...) @printf(ptr noundef @.str.10)
  br label %9

9:                                                ; preds = %7, %5
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
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.11, i32 noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__float(float noundef %0) #0 {
  %2 = alloca float, align 4
  store float %0, ptr %2, align 4
  %3 = load float, ptr %2, align 4
  %4 = fpext float %3 to double
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, double noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__float(float noundef %0) #0 {
  %2 = alloca float, align 4
  store float %0, ptr %2, align 4
  %3 = load float, ptr %2, align 4
  %4 = fpext float %3 to double
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.12, double noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__double(double noundef %0) #0 {
  %2 = alloca double, align 8
  store double %0, ptr %2, align 8
  %3 = load double, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, double noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__double(double noundef %0) #0 {
  %2 = alloca double, align 8
  store double %0, ptr %2, align 8
  %3 = load double, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.12, double noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.print__spl__void__String(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds %struct.String_t, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.13, ptr noundef %5)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__String(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds %struct.String_t, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.14, ptr noundef %5)
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
