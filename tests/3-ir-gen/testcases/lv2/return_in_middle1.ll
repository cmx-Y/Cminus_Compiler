; ModuleID = 'cminus'
source_filename = "./testcases/lv2/return_in_middle1.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define i32 @result() {
label_entry:
  %op0 = alloca i32
  %op1 = icmp ne i32 1, 0
  br i1 %op1, label %label2, label %label3
label2:                                                ; preds = %label_entry
  store i32 1, i32* %op0
  ret i32 0
  br label %label4
label3:                                                ; preds = %label_entry
  store i32 2, i32* %op0
  br label %label4
label4:                                                ; preds = %label2, %label3
  call void @output(i32 3)
  ret i32 3
}
define void @main() {
label_entry:
  %op0 = call i32 @result()
  call void @output(i32 %op0)
  ret void
}
