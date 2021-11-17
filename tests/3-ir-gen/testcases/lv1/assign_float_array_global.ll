; ModuleID = 'cminus'
source_filename = "./testcases/lv1/assign_float_array_global.cminus"

@b = global [10 x float] zeroinitializer
declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = icmp slt i32 3, 0
  br i1 %op0, label %label1, label %label2
label1:                                                ; preds = %label_entry
  call void @neg_idx_except()
  br label %label2
label2:                                                ; preds = %label_entry, %label1
  %op3 = getelementptr [10 x float], [10 x float]* @b, i32 0, i32 3
  store float 0x4093480000000000, float* %op3
  %op4 = icmp slt i32 3, 0
  br i1 %op4, label %label5, label %label6
label5:                                                ; preds = %label2
  call void @neg_idx_except()
  br label %label6
label6:                                                ; preds = %label2, %label5
  %op7 = getelementptr [10 x float], [10 x float]* @b, i32 0, i32 3
  %op8 = load float, float* %op7
  call void @outputFloat(float %op8)
  ret void
}
