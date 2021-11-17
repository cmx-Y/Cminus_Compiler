; ModuleID = 'cminus'
source_filename = "./testcases/lv1/negidx_float.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca [10 x i32]
  %op1 = sitofp i32 3 to float
  %op2 = fsub float 0x4000000000000000, %op1
  %op3 = fptosi float %op2 to i32
  %op4 = icmp slt i32 %op3, 0
  br i1 %op4, label %label5, label %label6
label5:                                                ; preds = %label_entry
  call void @neg_idx_except()
  br label %label6
label6:                                                ; preds = %label_entry, %label5
  %op7 = getelementptr [10 x i32], [10 x i32]* %op0, i32 0, i32 %op3
  %op8 = load i32, i32* %op7
  ret void
}
