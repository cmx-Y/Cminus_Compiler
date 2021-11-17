; ModuleID = 'cminus'
source_filename = "./testcases/lv2/assign_chain.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca i32
  %op1 = alloca i32
  %op2 = alloca i32
  store i32 3, i32* %op2
  %op3 = load i32, i32* %op1
  store i32 3, i32 %op3
  %op4 = load i32, i32* %op0
  store i32 3, i32 %op4
  %op5 = load i32, i32* %op0
  call void @output(i32 %op5)
  %op6 = load i32, i32* %op1
  call void @output(i32 %op6)
  %op7 = load i32, i32* %op2
  call void @output(i32 %op7)
  ret void
}
