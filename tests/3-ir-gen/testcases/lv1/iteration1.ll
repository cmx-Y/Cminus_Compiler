; ModuleID = 'cminus'
source_filename = "./testcases/lv1/iteration1.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca i32
  store i32 10, i32* %op0
  br label %label1
label1:                                                ; preds = %label_entry, %label4
  %op2 = load i32, i32* %op0
  %op3 = icmp ne i32 %op2, 0
  br i1 %op3, label %label4, label %label8
label4:                                                ; preds = %label1
  %op5 = load i32, i32* %op0
  call void @output(i32 %op5)
  %op6 = load i32, i32* %op0
  %op7 = sub i32 %op6, 1
  store i32 %op7, i32* %op0
  br label %label1
label8:                                                ; preds = %label1
  ret void
}
