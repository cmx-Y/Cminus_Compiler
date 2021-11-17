; ModuleID = 'cminus'
source_filename = "./testcases/lv0_1/return.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  ret void
}
