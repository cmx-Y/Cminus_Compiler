; ModuleID = 'cminus'
source_filename = "./testcases/lv0_1/output_float.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  call void @outputFloat(float 0x405ed999a0000000)
  ret void
}
