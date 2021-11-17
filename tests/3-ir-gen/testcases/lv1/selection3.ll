; ModuleID = 'cminus'
source_filename = "./testcases/lv1/selection3.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  call void @output(i32 24)
  ret void
}
