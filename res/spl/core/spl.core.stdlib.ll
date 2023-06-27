; ModuleID = 'lib.c'
source_filename = "lib.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.__spl__gcmap = type { i32, i32, %struct.__spl__gcmap__entry* }
%struct.__spl__gcmap__entry = type { i8*, i32 }

@__spl__m = dso_local global %struct.__spl__gcmap zeroinitializer, align 8
@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@llvm.compiler.used = appending global [13 x i8*] [i8* bitcast (void ()* @__spl__init__gcmap to i8*), i8* bitcast (void ()* @__spl__destroy__gcmap to i8*), i8* bitcast (void (i8*, i32)* @__spl__add__to__gc to i8*), i8* bitcast (i32 (i8*)* @__spl__get__refs to i8*), i8* bitcast (void (i8*, i32)* @__spl__set__refs to i8*), i8* bitcast (void (i8*)* @__spl__dec__refs to i8*), i8* bitcast (void (i8*)* @__spl__inc__refs to i8*), i8* bitcast (i8* (i32)* @__spl__alloc to i8*), i8* bitcast (void (i8*, i8*)* @__spl__write to i8*), i8* bitcast (void (i8*)* @__spl__destroyvar to i8*), i8* bitcast (void (i32)* @System.out.println__spl__void__int to i8*), i8* bitcast (void (float)* @System.out.println__spl__void__float to i8*), i8* bitcast (void (double)* @System.out.println__spl__void__double to i8*)], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__init__gcmap() #0 {
  store i32 0, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  store i32 10, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 1), align 4
  %1 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 1), align 4
  %2 = sext i32 %1 to i64
  %3 = mul i64 %2, 16
  %4 = call noalias i8* @malloc(i64 noundef %3) #3
  %5 = bitcast i8* %4 to %struct.__spl__gcmap__entry*
  store %struct.__spl__gcmap__entry* %5, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  ret void
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__destroy__gcmap() #0 {
  %1 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %2 = bitcast %struct.__spl__gcmap__entry* %1 to i8*
  call void @free(i8* noundef %2) #3
  ret void
}

; Function Attrs: nounwind
declare void @free(i8* noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__add__to__gc(i8* noundef %0, i32 noundef %1) #0 {
  %3 = alloca i8*, align 8
  %4 = alloca i32, align 4
  store i8* %0, i8** %3, align 8
  store i32 %1, i32* %4, align 4
  %5 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  %6 = add nsw i32 %5, 1
  %7 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 1), align 4
  %8 = icmp slt i32 %6, %7
  br i1 %8, label %9, label %24

9:                                                ; preds = %2
  %10 = load i8*, i8** %3, align 8
  %11 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %12 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %11, i64 %13
  %15 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %14, i32 0, i32 0
  store i8* %10, i8** %15, align 8
  %16 = load i32, i32* %4, align 4
  %17 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %18 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  %19 = sext i32 %18 to i64
  %20 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %17, i64 %19
  %21 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %20, i32 0, i32 1
  store i32 %16, i32* %21, align 8
  %22 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  %23 = add nsw i32 %22, 1
  store i32 %23, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  br label %48

24:                                               ; preds = %2
  %25 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 1), align 4
  %26 = mul nsw i32 %25, 2
  store i32 %26, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 1), align 4
  %27 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %28 = bitcast %struct.__spl__gcmap__entry* %27 to i8*
  %29 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 1), align 4
  %30 = sext i32 %29 to i64
  %31 = mul i64 %30, 16
  %32 = call i8* @realloc(i8* noundef %28, i64 noundef %31) #3
  %33 = bitcast i8* %32 to %struct.__spl__gcmap__entry*
  store %struct.__spl__gcmap__entry* %33, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %34 = load i8*, i8** %3, align 8
  %35 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %36 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  %37 = sext i32 %36 to i64
  %38 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %35, i64 %37
  %39 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %38, i32 0, i32 0
  store i8* %34, i8** %39, align 8
  %40 = load i32, i32* %4, align 4
  %41 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %42 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  %43 = sext i32 %42 to i64
  %44 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %41, i64 %43
  %45 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %44, i32 0, i32 1
  store i32 %40, i32* %45, align 8
  %46 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  %47 = add nsw i32 %46, 1
  store i32 %47, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  br label %48

48:                                               ; preds = %24, %9
  ret void
}

; Function Attrs: nounwind
declare i8* @realloc(i8* noundef, i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @__spl__get__refs(i8* noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i8*, align 8
  %4 = alloca i32, align 4
  store i8* %0, i8** %3, align 8
  store i32 0, i32* %4, align 4
  br label %5

5:                                                ; preds = %26, %1
  %6 = load i32, i32* %4, align 4
  %7 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  %8 = icmp slt i32 %6, %7
  br i1 %8, label %9, label %29

9:                                                ; preds = %5
  %10 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %11 = load i32, i32* %4, align 4
  %12 = sext i32 %11 to i64
  %13 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %10, i64 %12
  %14 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %13, i32 0, i32 0
  %15 = load i8*, i8** %14, align 8
  %16 = load i8*, i8** %3, align 8
  %17 = icmp eq i8* %15, %16
  br i1 %17, label %18, label %25

18:                                               ; preds = %9
  %19 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %20 = load i32, i32* %4, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %19, i64 %21
  %23 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %22, i32 0, i32 1
  %24 = load i32, i32* %23, align 8
  store i32 %24, i32* %2, align 4
  br label %30

25:                                               ; preds = %9
  br label %26

26:                                               ; preds = %25
  %27 = load i32, i32* %4, align 4
  %28 = add nsw i32 %27, 1
  store i32 %28, i32* %4, align 4
  br label %5, !llvm.loop !6

29:                                               ; preds = %5
  store i32 -1, i32* %2, align 4
  br label %30

30:                                               ; preds = %29, %18
  %31 = load i32, i32* %2, align 4
  ret i32 %31
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__set__refs(i8* noundef %0, i32 noundef %1) #0 {
  %3 = alloca i8*, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i8* %0, i8** %3, align 8
  store i32 %1, i32* %4, align 4
  store i32 0, i32* %5, align 4
  br label %6

6:                                                ; preds = %27, %2
  %7 = load i32, i32* %5, align 4
  %8 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %30

10:                                               ; preds = %6
  %11 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %12 = load i32, i32* %5, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %11, i64 %13
  %15 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %14, i32 0, i32 0
  %16 = load i8*, i8** %15, align 8
  %17 = load i8*, i8** %3, align 8
  %18 = icmp eq i8* %16, %17
  br i1 %18, label %19, label %26

19:                                               ; preds = %10
  %20 = load i32, i32* %4, align 4
  %21 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %22 = load i32, i32* %5, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %21, i64 %23
  %25 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %24, i32 0, i32 1
  store i32 %20, i32* %25, align 8
  br label %26

26:                                               ; preds = %19, %10
  br label %27

27:                                               ; preds = %26
  %28 = load i32, i32* %5, align 4
  %29 = add nsw i32 %28, 1
  store i32 %29, i32* %5, align 4
  br label %6, !llvm.loop !8

30:                                               ; preds = %6
  %31 = load i8*, i8** %3, align 8
  %32 = load i32, i32* %4, align 4
  call void @__spl__add__to__gc(i8* noundef %31, i32 noundef %32)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__dec__refs(i8* noundef %0) #0 {
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  store i8* %0, i8** %2, align 8
  store i32 0, i32* %3, align 4
  br label %4

4:                                                ; preds = %26, %1
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  %7 = icmp slt i32 %5, %6
  br i1 %7, label %8, label %29

8:                                                ; preds = %4
  %9 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %10 = load i32, i32* %3, align 4
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %9, i64 %11
  %13 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %12, i32 0, i32 0
  %14 = load i8*, i8** %13, align 8
  %15 = load i8*, i8** %2, align 8
  %16 = icmp eq i8* %14, %15
  br i1 %16, label %17, label %25

17:                                               ; preds = %8
  %18 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %19 = load i32, i32* %3, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %18, i64 %20
  %22 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %21, i32 0, i32 1
  %23 = load i32, i32* %22, align 8
  %24 = add nsw i32 %23, -1
  store i32 %24, i32* %22, align 8
  br label %25

25:                                               ; preds = %17, %8
  br label %26

26:                                               ; preds = %25
  %27 = load i32, i32* %3, align 4
  %28 = add nsw i32 %27, 1
  store i32 %28, i32* %3, align 4
  br label %4, !llvm.loop !9

29:                                               ; preds = %4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__inc__refs(i8* noundef %0) #0 {
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  store i8* %0, i8** %2, align 8
  store i32 0, i32* %3, align 4
  br label %4

4:                                                ; preds = %26, %1
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 0), align 8
  %7 = icmp slt i32 %5, %6
  br i1 %7, label %8, label %29

8:                                                ; preds = %4
  %9 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %10 = load i32, i32* %3, align 4
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %9, i64 %11
  %13 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %12, i32 0, i32 0
  %14 = load i8*, i8** %13, align 8
  %15 = load i8*, i8** %2, align 8
  %16 = icmp eq i8* %14, %15
  br i1 %16, label %17, label %25

17:                                               ; preds = %8
  %18 = load %struct.__spl__gcmap__entry*, %struct.__spl__gcmap__entry** getelementptr inbounds (%struct.__spl__gcmap, %struct.__spl__gcmap* @__spl__m, i32 0, i32 2), align 8
  %19 = load i32, i32* %3, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %18, i64 %20
  %22 = getelementptr inbounds %struct.__spl__gcmap__entry, %struct.__spl__gcmap__entry* %21, i32 0, i32 1
  %23 = load i32, i32* %22, align 8
  %24 = add nsw i32 %23, 1
  store i32 %24, i32* %22, align 8
  br label %25

25:                                               ; preds = %17, %8
  br label %26

26:                                               ; preds = %25
  %27 = load i32, i32* %3, align 4
  %28 = add nsw i32 %27, 1
  store i32 %28, i32* %3, align 4
  br label %4, !llvm.loop !10

29:                                               ; preds = %4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @__spl__alloc(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i8*, align 8
  store i32 %0, i32* %2, align 4
  %4 = load i32, i32* %2, align 4
  %5 = sext i32 %4 to i64
  %6 = call noalias i8* @malloc(i64 noundef %5) #3
  store i8* %6, i8** %3, align 8
  %7 = load i8*, i8** %3, align 8
  call void @__spl__add__to__gc(i8* noundef %7, i32 noundef 1)
  %8 = load i8*, i8** %3, align 8
  ret i8* %8
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__write(i8* noundef %0, i8* noundef %1) #0 {
  %3 = alloca i8*, align 8
  %4 = alloca i8*, align 8
  store i8* %0, i8** %3, align 8
  store i8* %1, i8** %4, align 8
  %5 = load i8*, i8** %3, align 8
  %6 = icmp ne i8* %5, null
  br i1 %6, label %7, label %9

7:                                                ; preds = %2
  %8 = load i8*, i8** %3, align 8
  call void @__spl__dec__refs(i8* noundef %8)
  br label %9

9:                                                ; preds = %7, %2
  %10 = load i8*, i8** %4, align 8
  %11 = icmp ne i8* %10, null
  br i1 %11, label %12, label %14

12:                                               ; preds = %9
  %13 = load i8*, i8** %4, align 8
  call void @__spl__inc__refs(i8* noundef %13)
  br label %14

14:                                               ; preds = %12, %9
  %15 = load i8*, i8** %3, align 8
  %16 = icmp ne i8* %15, null
  br i1 %16, label %17, label %25

17:                                               ; preds = %14
  %18 = load i8*, i8** %3, align 8
  %19 = call i32 @__spl__get__refs(i8* noundef %18)
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %21, label %24

21:                                               ; preds = %17
  %22 = load i8*, i8** %3, align 8
  call void @__spl__set__refs(i8* noundef %22, i32 noundef -1)
  %23 = load i8*, i8** %3, align 8
  call void @free(i8* noundef %23) #3
  br label %24

24:                                               ; preds = %21, %17
  br label %25

25:                                               ; preds = %24, %14
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__destroyvar(i8* noundef %0) #0 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i8*, i8** %2, align 8
  %4 = icmp ne i8* %3, null
  br i1 %4, label %5, label %14

5:                                                ; preds = %1
  %6 = load i8*, i8** %2, align 8
  call void @__spl__dec__refs(i8* noundef %6)
  %7 = load i8*, i8** %2, align 8
  %8 = call i32 @__spl__get__refs(i8* noundef %7)
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %10, label %13

10:                                               ; preds = %5
  %11 = load i8*, i8** %2, align 8
  call void @__spl__set__refs(i8* noundef %11, i32 noundef -1)
  %12 = load i8*, i8** %2, align 8
  call void @free(i8* noundef %12) #3
  br label %13

13:                                               ; preds = %10, %5
  br label %14

14:                                               ; preds = %13, %1
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__int(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 noundef %3)
  ret void
}

declare i32 @printf(i8* noundef, ...) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__float(float noundef %0) #0 {
  %2 = alloca float, align 4
  store float %0, float* %2, align 4
  %3 = load float, float* %2, align 4
  %4 = fpext float %3 to double
  %5 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), double noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__double(double noundef %0) #0 {
  %2 = alloca double, align 8
  store double %0, double* %2, align 8
  %3 = load double, double* %2, align 8
  %4 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), double noundef %3)
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Debian clang version 14.0.6"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
