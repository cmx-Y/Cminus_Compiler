define dso_local i32 @main() #0 {
   ;main return 0
   %1 = alloca i32
   store i32 0, i32* %1

   ;assign 5.555 to a
   %2 = alloca float
   store float 0x40163851E0000000, float* %2
   %3 = load float, float* %2

   ;assign 1
   %4 = alloca float
   store float 1.000000e+00, float *%4
   %5 = load float, float* %4

   ;compare a with 1
   %6 = fcmp ugt float %3, %5
   br i1 %6, label %7, label %10

7:
   ;if ture
   %8 = alloca i32
   store i32 233, i32* %8
   %9 = load i32, i32* %8
   ret i32 %9
10:
   ;if false
   %11 = load i32, i32* %1
   ret i32 %11
}
