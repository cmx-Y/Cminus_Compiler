; ModuleID = 'cminus'
source_filename = "./testcases/lv3/complex1.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define i32 @mod(i32 %arg0, i32 %arg1) {
label_entry:
  %op2 = alloca i32
  store i32 %arg0, i32* %op2
  %op3 = alloca i32
  store i32 %arg1, i32* %op3
  %op4 = load i32, i32* %op2
  %op5 = load i32, i32* %op2
  %op6 = load i32, i32* %op3
  %op7 = sdiv i32 %op5, %op6
  %op8 = load i32, i32* %op3
  %op9 = mul i32 %op7, %op8
  %op10 = sub i32 %op4, %op9
  ret i32 %op10
}
define void @printfour(i32 %arg0) {
label_entry:
  %op1 = alloca i32
  store i32 %arg0, i32* %op1
  %op2 = alloca i32
  %op3 = alloca i32
  %op4 = alloca i32
  %op5 = alloca i32
  %op6 = load i32, i32* %op1
  %op7 = call i32 @mod(i32 %op6, i32 10000)
  store i32 %op7, i32* %op1
  %op8 = load i32, i32* %op1
  %op9 = call i32 @mod(i32 %op8, i32 10)
  store i32 %op9, i32* %op5
  %op10 = load i32, i32* %op1
  %op11 = sdiv i32 %op10, 10
  store i32 %op11, i32* %op1
  %op12 = load i32, i32* %op1
  %op13 = call i32 @mod(i32 %op12, i32 10)
  store i32 %op13, i32* %op4
  %op14 = load i32, i32* %op1
  %op15 = sdiv i32 %op14, 10
  store i32 %op15, i32* %op1
  %op16 = load i32, i32* %op1
  %op17 = call i32 @mod(i32 %op16, i32 10)
  store i32 %op17, i32* %op3
  %op18 = load i32, i32* %op1
  %op19 = sdiv i32 %op18, 10
  store i32 %op19, i32* %op1
  %op20 = load i32, i32* %op1
  store i32 %op20, i32* %op2
  %op21 = load i32, i32* %op2
  call void @output(i32 %op21)
  %op22 = load i32, i32* %op3
  call void @output(i32 %op22)
  %op23 = load i32, i32* %op4
  call void @output(i32 %op23)
  %op24 = load i32, i32* %op5
  call void @output(i32 %op24)
  ret void
}
define void @main() {
label_entry:
  %op0 = alloca [2801 x i32]
  %op1 = alloca i32
  %op2 = alloca i32
  %op3 = alloca i32
  %op4 = alloca i32
  %op5 = alloca i32
  store i32 0, i32* %op5
  store i32 1234, i32* %op4
  %op6 = alloca i32
  store i32 0, i32* %op6
  br label %label7
label7:                                                ; preds = %label_entry, %label17
  %op8 = load i32, i32* %op6
  %op9 = icmp slt i32 %op8, 2800
  %op10 = zext i1 %op9 to i32
  %op11 = icmp ne i32 %op10, 0
  br i1 %op11, label %label12, label %label15
label12:                                                ; preds = %label7
  %op13 = load i32, i32* %op6
  %op14 = icmp slt i32 %op13, 0
  br i1 %op14, label %label16, label %label17
label15:                                                ; preds = %label7
  store i32 2800, i32* %op2
  br label %label21
label16:                                                ; preds = %label12
  call void @neg_idx_except()
  br label %label17
label17:                                                ; preds = %label12, %label16
  %op18 = getelementptr [2801 x i32], [2801 x i32]* %op0, i32 0, i32 %op13
  store i32 2000, i32* %op18
  %op19 = load i32, i32* %op6
  %op20 = add i32 %op19, 1
  store i32 %op20, i32* %op6
  br label %label7
label21:                                                ; preds = %label15, %label37
  %op22 = load i32, i32* %op2
  %op23 = icmp ne i32 %op22, 0
  br i1 %op23, label %label24, label %label27
label24:                                                ; preds = %label21
  %op25 = alloca i32
  store i32 0, i32* %op4
  %op26 = load i32, i32* %op2
  store i32 %op26, i32* %op1
  br label %label28
label27:                                                ; preds = %label21
  ret void
label28:                                                ; preds = %label24, %label76
  %op29 = load i32, i32* %op1
  %op30 = icmp ne i32 %op29, 0
  %op31 = zext i1 %op30 to i32
  %op32 = icmp ne i32 %op31, 0
  br i1 %op32, label %label33, label %label37
label33:                                                ; preds = %label28
  %op34 = load i32, i32* %op4
  %op35 = load i32, i32* %op1
  %op36 = icmp slt i32 %op35, 0
  br i1 %op36, label %label46, label %label47
label37:                                                ; preds = %label28
  %op38 = load i32, i32* %op5
  %op39 = load i32, i32* %op4
  %op40 = sdiv i32 %op39, 10000
  %op41 = add i32 %op38, %op40
  call void @printfour(i32 %op41)
  %op42 = load i32, i32* %op4
  %op43 = call i32 @mod(i32 %op42, i32 10000)
  store i32 %op43, i32* %op5
  %op44 = load i32, i32* %op2
  %op45 = sub i32 %op44, 14
  store i32 %op45, i32* %op2
  br label %label21
label46:                                                ; preds = %label33
  call void @neg_idx_except()
  br label %label47
label47:                                                ; preds = %label33, %label46
  %op48 = getelementptr [2801 x i32], [2801 x i32]* %op0, i32 0, i32 %op35
  %op49 = load i32, i32* %op48
  %op50 = mul i32 %op49, 10000
  %op51 = add i32 %op34, %op50
  store i32 %op51, i32* %op4
  %op52 = load i32, i32* %op1
  %op53 = mul i32 2, %op52
  %op54 = sub i32 %op53, 1
  store i32 %op54, i32* %op3
  %op55 = load i32, i32* %op4
  %op56 = load i32, i32* %op3
  %op57 = call i32 @mod(i32 %op55, i32 %op56)
  %op58 = load i32, i32* %op1
  %op59 = icmp slt i32 %op58, 0
  br i1 %op59, label %label60, label %label61
label60:                                                ; preds = %label47
  call void @neg_idx_except()
  br label %label61
label61:                                                ; preds = %label47, %label60
  %op62 = getelementptr [2801 x i32], [2801 x i32]* %op0, i32 0, i32 %op58
  store i32 %op57, i32* %op62
  %op63 = load i32, i32* %op4
  %op64 = load i32, i32* %op3
  %op65 = sdiv i32 %op63, %op64
  store i32 %op65, i32* %op4
  %op66 = load i32, i32* %op1
  %op67 = sub i32 %op66, 1
  store i32 %op67, i32* %op1
  %op68 = load i32, i32* %op1
  %op69 = icmp ne i32 %op68, 0
  %op70 = zext i1 %op69 to i32
  %op71 = icmp ne i32 %op70, 0
  br i1 %op71, label %label72, label %label76
label72:                                                ; preds = %label61
  %op73 = load i32, i32* %op4
  %op74 = load i32, i32* %op1
  %op75 = mul i32 %op73, %op74
  store i32 %op75, i32* %op4
  br label %label76
label76:                                                ; preds = %label61, %label72
  br label %label28
}
