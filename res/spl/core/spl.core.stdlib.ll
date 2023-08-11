; ModuleID = 'lib.c'
source_filename = "lib.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.__spl__gcmap = type { i32, i32, ptr }
%struct.__spl__gcmap__entry = type { ptr, i32 }

@__spl__m = dso_local global %struct.__spl__gcmap zeroinitializer, align 8
@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@llvm.compiler.used = appending global [13 x ptr] [ptr @__spl__init__gcmap, ptr @__spl__destroy__gcmap, ptr @__spl__add__to__gc, ptr @__spl__get__refs, ptr @__spl__set__refs, ptr @__spl__dec__refs, ptr @__spl__inc__refs, ptr @__spl__alloc, ptr @__spl__write, ptr @__spl__destroyvar, ptr @System.out.println__spl__void__int, ptr @System.out.println__spl__void__float, ptr @System.out.println__spl__void__double], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__init__gcmap() #0 {
  store i32 0, ptr @__spl__m, align 8
  store i32 10, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 1), align 4
  %1 = load i32, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 1), align 4
  %2 = sext i32 %1 to i64
  %3 = mul i64 %2, 16
  %4 = call noalias ptr @malloc(i64 noundef %3) #5
  store ptr %4, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  ret void
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__spl__destroy__gcmap() #0 {
  %1 = load ptr, ptr getelementptr inbounds (%struct.__spl__gcmap, ptr @__spl__m, i32 0, i32 2), align 8
  call void @free(ptr noundef %1) #6
  ret void
}

; Function Attrs: nounwind
declare void @free(ptr noundef) #2

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
  %31 = call ptr @realloc(ptr noundef %27, i64 noundef %30) #7
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
declare ptr @realloc(ptr noundef, i64 noundef) #3

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
  br label %5, !llvm.loop !6

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
  br label %6, !llvm.loop !8

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
  br label %4, !llvm.loop !9

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
  br label %4, !llvm.loop !10

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
  %6 = call noalias ptr @malloc(i64 noundef %5) #5
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
  call void @free(ptr noundef %23) #6
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
  call void @free(ptr noundef %20) #6
  br label %21

21:                                               ; preds = %19, %7
  br label %22

22:                                               ; preds = %21, %2
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__int(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %3)
  ret void
}

declare i32 @printf(ptr noundef, ...) #4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__float(float noundef %0) #0 {
  %2 = alloca float, align 4
  store float %0, ptr %2, align 4
  %3 = load float, ptr %2, align 4
  %4 = fpext float %3 to double
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, double noundef %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @System.out.println__spl__void__double(double noundef %0) #0 {
  %2 = alloca double, align 8
  store double %0, ptr %2, align 8
  %3 = load double, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, double noundef %3)
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind allocsize(1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind allocsize(0) }
attributes #6 = { nounwind }
attributes #7 = { nounwind allocsize(1) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Debian clang version 15.0.6"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
