; ModuleID = 'cminus'
source_filename = "./testcases/lv0_2/num_add_mixed.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = sitofp i32 1000 to float
  %op1 = fadd float %op0, 0x4037666660000000
  %op2 = fptosi float %op1 to i32
  call void @output(i32 %op2)
  ret void
}
