; ModuleID = 'cminus'
source_filename = "./testcases/lv0_2/num_comp2.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = sitofp i32 1 to float
  %op1 = fcmp ugt float %op0,0x4000000000000000
  %op2 = zext i1 %op1 to i32
  %op3 = icmp slt i32 %op2, 3
  %op4 = zext i1 %op3 to i32
  call void @output(i32 %op4)
  %op5 = icmp sgt i32 1, 2
  %op6 = zext i1 %op5 to i32
  %op7 = icmp eq i32 3, 4
  %op8 = zext i1 %op7 to i32
  %op9 = icmp sge i32 %op8, 0
  %op10 = zext i1 %op9 to i32
  %op11 = icmp slt i32 %op6, %op10
  %op12 = zext i1 %op11 to i32
  call void @output(i32 %op12)
  %op13 = sitofp i32 3 to float
  %op14 = fcmp ueq float %op13,0x4010000000000000
  %op15 = zext i1 %op14 to i32
  %op16 = icmp sge i32 %op15, 0
  %op17 = zext i1 %op16 to i32
  %op18 = icmp ne i32 4, 4
  %op19 = zext i1 %op18 to i32
  %op20 = icmp sle i32 %op17, %op19
  %op21 = zext i1 %op20 to i32
  call void @output(i32 %op21)
  %op22 = icmp sgt i32 1, 2
  %op23 = zext i1 %op22 to i32
  %op24 = icmp eq i32 3, 4
  %op25 = zext i1 %op24 to i32
  %op26 = icmp sge i32 %op25, 0
  %op27 = zext i1 %op26 to i32
  %op28 = icmp slt i32 %op23, %op27
  %op29 = zext i1 %op28 to i32
  %op30 = sitofp i32 4 to float
  %op31 = fcmp une float 0x4010000000000000,%op30
  %op32 = zext i1 %op31 to i32
  %op33 = icmp sle i32 %op29, %op32
  %op34 = zext i1 %op33 to i32
  call void @output(i32 %op34)
  ret void
}
