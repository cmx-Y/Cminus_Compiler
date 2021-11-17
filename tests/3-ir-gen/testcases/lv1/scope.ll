; ModuleID = 'cminus'
source_filename = "./testcases/lv1/scope.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca i32
  store i32 3, i32* %op0
  %op1 = load i32, i32* %op0
  call void @output(i32 %op1)
  %op2 = alloca i32
  store i32 11, i32* %op0
  %op3 = load i32, i32* %op0
  call void @output(i32 %op3)
  %op4 = load i32, i32* %op0
  call void @output(i32 %op4)
  ret void
}
