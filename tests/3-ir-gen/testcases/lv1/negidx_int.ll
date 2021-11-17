; ModuleID = 'cminus'
source_filename = "./testcases/lv1/negidx_int.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca [10 x i32]
  %op1 = sub i32 2, 3
  %op2 = icmp slt i32 %op1, 0
  br i1 %op2, label %label3, label %label4
label3:                                                ; preds = %label_entry
  call void @neg_idx_except()
  br label %label4
label4:                                                ; preds = %label_entry, %label3
  %op5 = getelementptr [10 x i32], [10 x i32]* %op0, i32 0, i32 %op1
  %op6 = load i32, i32* %op5
  ret void
}
