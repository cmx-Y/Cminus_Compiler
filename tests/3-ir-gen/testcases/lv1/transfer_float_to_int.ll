; ModuleID = 'cminus'
source_filename = "./testcases/lv1/transfer_float_to_int.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op0 = alloca i32
  %op1 = fptosi float 0x3ff0000000000000 to i32
  store i32 %op1, i32* %op0
  %op2 = load i32, i32* %op0
  call void @output(i32 %op2)
  ret void
}
