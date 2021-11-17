; ModuleID = 'cminus'
source_filename = "./testcases/lv0_2/num_eq_float.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = fcmp ueq float 0x3ff0000000000000,0x4000000000000000
  %op1 = zext i1 %op0 to i32
  call void @output(i32 %op1)
  %op2 = fcmp ueq float 0x4000000000000000,0x4000000000000000
  %op3 = zext i1 %op2 to i32
  call void @output(i32 %op3)
  %op4 = fcmp ueq float 0x4008000000000000,0x4000000000000000
  %op5 = zext i1 %op4 to i32
  call void @output(i32 %op5)
  ret void
}
