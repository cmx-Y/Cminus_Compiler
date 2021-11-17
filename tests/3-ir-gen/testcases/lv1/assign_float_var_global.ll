; ModuleID = 'cminus'
source_filename = "./testcases/lv1/assign_float_var_global.cminus"

@b = global float zeroinitializer
declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  store float 0x4093480000000000, float* @b
  %op0 = load float, float* @b
  call void @outputFloat(float %op0)
  ret void
}
