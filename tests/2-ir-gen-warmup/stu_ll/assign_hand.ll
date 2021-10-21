; ModuleID = 'assign.c'

define dso_local i32 @main() #0 {
    ;main return 0
    %1 = alloca i32
    store i32 0, i32* %1

    ;allocate for int a[10]
    %aptr = alloca [10 x i32]

    ;get a[0] a[1]address
    %2 = getelementptr [10 x i32], [10 x i32]* %aptr , i64 0, i64 0
    %3 = getelementptr [10 x i32], [10 x i32]* %aptr , i64 0, i64 1

    ;store 10 to a[0]
    store i32 10, i32* %2
    ;load a[0] for use
    %4 = load i32, i32* %2
    %5 = mul i32 %4, 2
    ;store mul_result to a[1]
    store i32 %5, i32* %3
    ;load a[1] for return
    %6 = load i32, i32* %3
    ret i32 %6
}
