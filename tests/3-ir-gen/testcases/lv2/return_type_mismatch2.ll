; ModuleID = 'cminus'
source_filename = "./testcases/lv2/return_type_mismatch2.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define float @f() {
label_entry:
  ret i32 7
}
define void @main() {
label_entry:
  %op0 = call float @f()
  call void @outputFloat(float %op0)
  ret void
}
