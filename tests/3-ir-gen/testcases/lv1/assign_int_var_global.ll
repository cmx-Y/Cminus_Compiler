; ModuleID = 'cminus'
source_filename = "./testcases/lv1/assign_int_var_global.cminus"

@a = global i32 zeroinitializer
declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  store i32 1234, i32* @a
  %op0 = load i32, i32* @a
  call void @output(i32 %op0)
  ret void
}
