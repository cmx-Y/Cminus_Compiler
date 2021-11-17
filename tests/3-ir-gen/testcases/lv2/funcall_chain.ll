; ModuleID = 'cminus'
source_filename = "./testcases/lv2/funcall_chain.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define i32 @addone(i32 %arg0) {
label_entry:
  %op1 = alloca i32
  store i32 %arg0, i32* %op1
}
define void @main() {
label_entry:
  %op0 = alloca i32
  %op1 = call i32 @addone(i32 1230)
  %op2 = call i32 @addone(i32 1230)
  %op3 = call i32 @addone(i32 1230)
  %op4 = call i32 @addone(i32 1230)
  store i32 1230, i32* %op0
  %op5 = load i32, i32* %op0
  call void @output(i32 %op5)
  ret void
}
