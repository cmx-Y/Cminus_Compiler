define dso_local i32 @callee(i32 %0) #0 {
    ;store formal parameter
    %2 = alloca i32
    store i32 %0, i32* %2

    ;load a for mul operation
    %3 = load i32, i32* %2

    %4 = mul i32 2, %3
    ret i32 %4
}
define dso_local i32 @main() #0 {
    ;main return 0
    %1 = alloca i32
    store i32 0, i32* %1

    %2 = call i32 @callee(i32 110)
    ret i32 %2
}
