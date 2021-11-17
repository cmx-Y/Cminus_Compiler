; ModuleID = 'cminus'
source_filename = "./testcases/lv0_2/num_mul_mixed.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = sitofp i32 2 to float
  %op1 = fmul float %op0, 0x404ed999a0000000
  %op2 = fptosi float %op1 to i32
  call void @output(i32 %op2)
  ret void
}
