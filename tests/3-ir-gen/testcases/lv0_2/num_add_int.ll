; ModuleID = 'cminus'
source_filename = "./testcases/lv0_2/num_add_int.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = add i32 1000, 234
  call void @output(i32 %op0)
  ret void
}
