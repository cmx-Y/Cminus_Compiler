; ModuleID = 'cminus'
source_filename = "./testcases/lv1/assign_float_array_local.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca [10 x float]
  %op1 = icmp slt i32 3, 0
  br i1 %op1, label %label2, label %label3
label2:                                                ; preds = %label_entry
  call void @neg_idx_except()
  br label %label3
label3:                                                ; preds = %label_entry, %label2
  %op4 = getelementptr [10 x float], [10 x float]* %op0, i32 0, i32 3
  store float 0x4093480000000000, float* %op4
  %op5 = icmp slt i32 3, 0
  br i1 %op5, label %label6, label %label7
label6:                                                ; preds = %label3
  call void @neg_idx_except()
  br label %label7
label7:                                                ; preds = %label3, %label6
  %op8 = getelementptr [10 x float], [10 x float]* %op0, i32 0, i32 3
  %op9 = load float, float* %op8
  call void @outputFloat(float %op9)
  ret void
}
