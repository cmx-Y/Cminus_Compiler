; ModuleID = 'cminus'
source_filename = "./testcases/lv0_2/num_lt_int.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = icmp slt i32 1, 2
  %op1 = zext i1 %op0 to i32
  call void @output(i32 %op1)
  %op2 = icmp slt i32 2, 2
  %op3 = zext i1 %op2 to i32
  call void @output(i32 %op3)
  %op4 = icmp slt i32 3, 2
  %op5 = zext i1 %op4 to i32
  call void @output(i32 %op5)
  ret void
}
