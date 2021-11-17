; ModuleID = 'cminus'
source_filename = "./testcases/lv0_2/num_comp1.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = mul i32 2, 2
  %op1 = add i32 %op0, 5
  %op2 = sub i32 %op1, 6
  %op3 = sitofp i32 %op2 to float
  %op4 = fdiv float 0x3ff3333340000000, %op3
  %op5 = fmul float %op4, 0x40099999a0000000
  %op6 = sitofp i32 9 to float
  %op7 = fadd float %op6, %op5
  %op8 = sitofp i32 55 to float
  %op9 = fsub float %op7, %op8
  call void @outputFloat(float %op9)
  ret void
}
