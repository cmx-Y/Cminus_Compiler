; ModuleID = 'cminus'
source_filename = "./testcases/lv1/iteration2.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca i32
  store i32 10, i32* %op0
  br label %label1
label1:                                                ; preds = %label_entry, %label6
  %op2 = load i32, i32* %op0
  %op3 = icmp sgt i32 %op2, 0
  %op4 = zext i1 %op3 to i32
  %op5 = icmp ne i32 %op4, 0
  br i1 %op5, label %label6, label %label10
label6:                                                ; preds = %label1
  %op7 = load i32, i32* %op0
  call void @output(i32 %op7)
  %op8 = load i32, i32* %op0
  %op9 = sub i32 %op8, 1
  store i32 %op9, i32* %op0
  br label %label1
label10:                                                ; preds = %label1
  ret void
}
