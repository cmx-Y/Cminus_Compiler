; ModuleID = 'cminus'
source_filename = "./testcases/lv2/return_in_middle2.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define i32 @result() {
label_entry:
  %op0 = alloca i32
  store i32 10, i32* %op0
  br label %label1
label1:                                                ; preds = %label_entry, %label6
  %op2 = load i32, i32* %op0
  %op3 = icmp sgt i32 %op2, 0
  %op4 = zext i1 %op3 to i32
  %op5 = icmp ne i32 %op4, 0
  br i1 %op5, label %label6, label %label7
label6:                                                ; preds = %label1
  ret i32 0
  br label %label1
label7:                                                ; preds = %label1
  call void @output(i32 4)
  ret i32 1
}
define void @main() {
label_entry:
  %op0 = call i32 @result()
  call void @output(i32 %op0)
  ret void
}
