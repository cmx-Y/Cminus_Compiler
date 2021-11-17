; ModuleID = 'cminus'
source_filename = "./testcases/lv2/return_in_middle1.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define i32 @result() {
label_entry:
  %op0 = alloca i32
  call void @output(i32 3)
}
define void @main() {
label_entry:
  %op0 = call i32 @result()
  call void @output(i32 3)
  ret void
}
