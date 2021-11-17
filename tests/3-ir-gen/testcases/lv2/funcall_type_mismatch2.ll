; ModuleID = 'cminus'
source_filename = "./testcases/lv2/funcall_type_mismatch2.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @f(float %arg0) {
label_entry:
  %op1 = alloca float
  store float %arg0, float* %op1
  %op2 = load float, float* %op1
  call void @outputFloat(float %op2)
  ret void
}
define void @main() {
label_entry:
  %op0 = alloca i32
  %op1 = fptosi float 0x4012000000000000 to i32
  store i32 %op1, i32* %op0
  %op2 = load i32, i32* %op0
  call void @f(i32 %op2)
  ret void
}
