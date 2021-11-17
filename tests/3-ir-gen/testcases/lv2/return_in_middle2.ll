; ModuleID = 'cminus'
source_filename = "./testcases/lv2/return_in_middle2.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define i32 @result() {
label_entry:
  %op0 = alloca i32
  store i32 10, i32* %op0
  call void @output(i32 4)
}
define void @main() {
label_entry:
  %op0 = call i32 @result()
  call void @output(i32 4)
  ret void
}
