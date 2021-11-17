; ModuleID = 'cminus'
source_filename = "./testcases/lv3/complex1.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define i32 @mod(i32 %arg0, i32 %arg1) {
label_entry:
  %op2 = alloca i32
  store i32 %arg0, i32* %op2
  %op3 = alloca i32
  store i32 %arg1, i32* %op3
}
define void @printfour(i32 %arg0) {
label_entry:
  %op1 = alloca i32
  store i32 %arg0, i32* %op1
  %op2 = alloca i32
  %op3 = alloca i32
  %op4 = alloca i32
  %op5 = alloca i32
  %op6 = call i32 @mod(i32* %op1, i32 10000)
  %op7 = load i32, i32* %op1
  store i32 10000, i32 %op7
  %op8 = call i32 @mod(i32* %op1, i32 10)
  %op9 = load i32, i32* %op5
  store i32 10, i32 %op9
  %op10 = sdiv i32* %op1, i32 10
  %op11 = load i32, i32* %op1
  store i32 %op10, i32 %op11
  %op12 = call i32 @mod(i32* %op1, i32 10)
  %op13 = load i32, i32* %op4
  store i32 10, i32 %op13
  %op14 = sdiv i32* %op1, i32 10
  %op15 = load i32, i32* %op1
  store i32 %op14, i32 %op15
  %op16 = call i32 @mod(i32* %op1, i32 10)
  %op17 = load i32, i32* %op3
  store i32 10, i32 %op17
  %op18 = sdiv i32* %op1, i32 10
  %op19 = load i32, i32* %op1
  store i32 %op18, i32 %op19
  %op20 = load i32, i32* %op2
  store i32* %op1, i32 %op20
  %op21 = load i32, i32* %op2
  call void @output(i32 %op21)
  %op22 = load i32, i32* %op3
  call void @output(i32 %op22)
  %op23 = load i32, i32* %op4
  call void @output(i32 %op23)
  %op24 = load i32, i32* %op5
  call void @output(i32 %op24)
  ret void
}
define void @main() {
label_entry:
  %op0 = alloca [2801 x i32]
  %op1 = alloca i32
  %op2 = alloca i32
  %op3 = alloca i32
  %op4 = alloca i32
  %op5 = alloca i32
  store i32 0, i32* %op5
  store i32 1234, i32* %op4
  %op6 = alloca i32
  store i32 0, i32* %op6
  store i32 2800, i32* %op2
  ret void
}
