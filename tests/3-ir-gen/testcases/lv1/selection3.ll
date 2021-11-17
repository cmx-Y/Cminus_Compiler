; ModuleID = 'cminus'
source_filename = "./testcases/lv1/selection3.cminus"

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
  br label %label5
label4:                                                ; preds = %label_entry
  call void @output(i32 1234)
  br label %label5
label5:                                                ; preds = %label3, %label4
  call void @output(i32 24)
  %op6 = icmp slt i32 2, 1
  %op7 = zext i1 %op6 to i32
  %op8 = icmp ne i32 %op7, 0
  br i1 %op8, label %label9, label %label10
label9:                                                ; preds = %label5
  call void @output(i32 42)
  br label %label11
label10:                                                ; preds = %label5
  call void @output(i32 1234)
  br label %label11
label11:                                                ; preds = %label9, %label10
  ret void
}
