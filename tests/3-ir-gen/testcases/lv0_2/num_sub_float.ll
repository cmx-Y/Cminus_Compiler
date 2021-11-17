; ModuleID = 'cminus'
source_filename = "./testcases/lv0_2/num_sub_float.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = fsub float 0x4069000000000000, 0x401ea3d700000000
  %op1 = fptosi float %op0 to i32
  call void @output(i32 %op1)
  ret void
}
