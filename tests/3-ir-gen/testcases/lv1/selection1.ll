; ModuleID = 'cminus'
source_filename = "./testcases/lv1/selection1.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca i32
  store i32 2, i32* %op0
  call void @output(i32 24)
  ret void
}
