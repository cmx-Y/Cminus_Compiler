; ModuleID = 'cminus'
source_filename = "./testcases/lv3/complex3.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define i32 @gcd(i32 %arg0, i32 %arg1) {
label_entry:
  %op2 = alloca i32
  store i32 %arg0, i32* %op2
  %op3 = alloca i32
  store i32 %arg1, i32* %op3
  %op4 = load i32, i32* %op3
  %op5 = icmp eq i32 %op4, 0
  %op6 = zext i1 %op5 to i32
  %op7 = icmp ne i32 %op6, 0
  br i1 %op7, label %label8, label %label10
label8:                                                ; preds = %label_entry
  %op9 = load i32, i32* %op2
  ret i32 %op9
  br label %label20
label10:                                                ; preds = %label_entry
  %op11 = load i32, i32* %op3
  %op12 = load i32, i32* %op2
  %op13 = load i32, i32* %op2
  %op14 = load i32, i32* %op3
  %op15 = sdiv i32 %op13, %op14
  %op16 = load i32, i32* %op3
  %op17 = mul i32 %op15, %op16
  %op18 = sub i32 %op12, %op17
  %op19 = call i32 @gcd(i32 %op11, i32 %op18)
  ret i32 %op19
  br label %label20
label20:                                                ; preds = %label8, %label10
}
define void @main() {
label_entry:
  %op0 = alloca i32
  %op1 = alloca i32
  %op2 = alloca i32
  %op3 = call i32 @input()
  store i32 %op3, i32* %op0
  %op4 = call i32 @input()
  store i32 %op4, i32* %op1
  %op5 = load i32, i32* %op0
  %op6 = load i32, i32* %op1
  %op7 = icmp slt i32 %op5, %op6
  %op8 = zext i1 %op7 to i32
  %op9 = icmp ne i32 %op8, 0
  br i1 %op9, label %label10, label %label14
label10:                                                ; preds = %label_entry
  %op11 = load i32, i32* %op0
  store i32 %op11, i32* %op2
  %op12 = load i32, i32* %op1
  store i32 %op12, i32* %op0
  %op13 = load i32, i32* %op2
  store i32 %op13, i32* %op1
  br label %label14
label14:                                                ; preds = %label_entry, %label10
  %op15 = load i32, i32* %op0
  %op16 = load i32, i32* %op1
  %op17 = call i32 @gcd(i32 %op15, i32 %op16)
  store i32 %op17, i32* %op2
  %op18 = load i32, i32* %op2
  call void @output(i32 %op18)
  ret void
}
