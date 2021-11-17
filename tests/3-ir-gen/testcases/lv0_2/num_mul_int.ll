; ModuleID = 'cminus'
source_filename = "./testcases/lv0_2/num_mul_int.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = mul i32 2, 617
  call void @output(i32 %op0)
  ret void
}
