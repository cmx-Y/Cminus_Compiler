; ModuleID = 'cminus'
source_filename = "./testcases/lv2/funcall_type_mismatch1.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @f(i32 %arg0) {
label_entry:
  %op1 = alloca i32
  store i32 %arg0, i32* %op1
  %op2 = load i32, i32* %op1
  call void @output(i32 %op2)
  ret void
}
define void @main() {
label_entry:
  %op0 = alloca float
  %op1 = sitofp i32 10 to i32
  store i32 %op1, float* %op0
  %op2 = load float, float* %op0
  call void @f(float %op2)
  ret void
}
