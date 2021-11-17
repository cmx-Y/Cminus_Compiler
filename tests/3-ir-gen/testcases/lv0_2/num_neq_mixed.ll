; ModuleID = 'cminus'
source_filename = "./testcases/lv0_2/num_neq_mixed.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = sitofp i32 2 to float
  %op1 = fcmp une float 0x3ff0000000000000,%op0
  %op2 = zext i1 %op1 to i32
  call void @output(i32 %op2)
  %op3 = sitofp i32 2 to float
  %op4 = fcmp une float %op3,0x4000000000000000
  %op5 = zext i1 %op4 to i32
  call void @output(i32 %op5)
  %op6 = sitofp i32 3 to float
  %op7 = fcmp une float %op6,0x4000000000000000
  %op8 = zext i1 %op7 to i32
  call void @output(i32 %op8)
  ret void
}
