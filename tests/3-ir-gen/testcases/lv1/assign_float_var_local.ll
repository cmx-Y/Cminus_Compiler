; ModuleID = 'cminus'
source_filename = "./testcases/lv1/assign_float_var_local.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca float
  store float 0x4093480000000000, float* %op0
  %op1 = load float, float* %op0
  call void @outputFloat(float %op1)
  ret void
}
