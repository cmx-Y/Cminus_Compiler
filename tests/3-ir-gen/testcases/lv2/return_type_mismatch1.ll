; ModuleID = 'cminus'
source_filename = "./testcases/lv2/return_type_mismatch1.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define i32 @f() {
label_entry:
  ret float 0x406d2999a0000000
}
define void @main() {
label_entry:
  %op0 = call i32 @f()
  %op1 = fptosi i32 %op0 to i32
  call void @output(i32 %op1)
  ret void
}
