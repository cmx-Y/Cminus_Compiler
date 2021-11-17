; ModuleID = 'cminus'
source_filename = "./testcases/lv1/innout.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca i32
  %op1 = call i32 @input()
  store i32 %op1, i32* %op0
  %op2 = load i32, i32* %op0
  call void @output(i32 %op2)
  ret void
}
