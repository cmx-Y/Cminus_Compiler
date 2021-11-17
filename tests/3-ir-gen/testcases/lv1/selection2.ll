; ModuleID = 'cminus'
source_filename = "./testcases/lv1/selection2.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = icmp sgt i32 2, 1
  %op1 = zext i1 %op0 to i32
  %op2 = icmp ne i32 %op1, 0
  br i1 %op2, label %label3, label %label4
label3:                                                ; preds = %label_entry
  call void @output(i32 42)
  br label %label4
label4:                                                ; preds = %label_entry, %label3
  call void @output(i32 24)
  %op5 = icmp sgt i32 1, 2
  %op6 = zext i1 %op5 to i32
  %op7 = icmp ne i32 %op6, 0
  br i1 %op7, label %label8, label %label9
label8:                                                ; preds = %label4
  call void @output(i32 1234)
  br label %label9
label9:                                                ; preds = %label4, %label8
  ret void
}
