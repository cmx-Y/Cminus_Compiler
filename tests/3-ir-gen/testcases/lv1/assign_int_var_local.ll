; ModuleID = 'cminus'
source_filename = "./testcases/lv1/assign_int_var_local.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca i32
  store i32 1234, i32* %op0
  %op1 = load i32, i32* %op0
  call void @output(i32 %op1)
  ret void
}
