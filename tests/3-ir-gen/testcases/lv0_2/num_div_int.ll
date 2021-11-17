; ModuleID = 'cminus'
source_filename = "./testcases/lv0_2/num_div_int.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = sdiv i32 2468, 2
  call void @output(i32 %op0)
  ret void
}
