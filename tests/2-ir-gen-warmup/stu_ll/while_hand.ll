define dso_local i32 @main() #0 {
    ;main return 0
    %1 = alloca i32
    store i32 0, i32* %1

    ;create a,i and assign
    %2 = alloca i32
    %3 = alloca i32
    store i32 10, i32* %2
    store i32 0, i32* %3
    
    ;load a,i for use
    %4 = load i32, i32* %3
    %5 = load i32, i32* %2
    %6 = icmp slt i32 %4, 10

    ;create a,i in loop
    %7 = alloca i32
    store i32 %4, i32* %7
    %8 = alloca i32
    store i32 %5, i32* %8

    ;first loop judge
    br i1 %6, label %9, label %16

9:
    ;%10 = i, %11 = i + 1
    %10 = load i32, i32* %7
    %11 = add i32 %10, 1
    store i32 %11, i32* %7

    ;%12 = i + 1 for judge
    %12 = load i32, i32* %7

    ;%13 = a, %14 = a + i
    %13 = load i32, i32* %8 
    %14 = add i32 %13, %12
    store i32 %14, i32* %8

    %15 = icmp slt i32 %12, 10
    br i1 %15, label %9, label %16

16:
    %17 = load i32, i32* %8
    ret i32 %17
}
