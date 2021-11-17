; ModuleID = 'cminus'
source_filename = "./testcases/lv1/idx_float.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca [10 x i32]
  %op1 = icmp slt i32 0, 0
  br i1 %op1, label %label2, label %label3
label2:                                                ; preds = %label_entry
  call void @neg_idx_except()
  br label %label3
label3:                                                ; preds = %label_entry, %label2
  %op4 = getelementptr [10 x i32], [10 x i32]* %op0, i32 0, i32 0
  store i32 1024, i32* %op4
  %op5 = fptosi float 0x3fb99999a0000000 to i32
  %op6 = icmp slt i32 %op5, 0
  br i1 %op6, label %label7, label %label8
label7:                                                ; preds = %label3
  call void @neg_idx_except()
  br label %label8
label8:                                                ; preds = %label3, %label7
  %op9 = getelementptr [10 x i32], [10 x i32]* %op0, i32 0, i32 %op5
  %op10 = load i32, i32* %op9
  call void @output(i32 %op10)
  ret void
}
