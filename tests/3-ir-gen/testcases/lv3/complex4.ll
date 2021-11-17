; ModuleID = 'cminus'
source_filename = "./testcases/lv3/complex4.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define float @get(float* %arg0, i32 %arg1, i32 %arg2, i32 %arg3) {
label_entry:
  %op4 = alloca float*
  store float* %arg0, float** %op4
  %op5 = alloca i32
  store i32 %arg1, i32* %op5
  %op6 = alloca i32
  store i32 %arg2, i32* %op6
  %op7 = alloca i32
  store i32 %arg3, i32* %op7
}
define float @abs(float %arg0) {
label_entry:
  %op1 = alloca float
  store float %arg0, float* %op1
}
define float @isZero(float %arg0) {
label_entry:
  %op1 = alloca float
  store float %arg0, float* %op1
}
define i32 @gauss(float* %arg0, float* %arg1, i32 %arg2) {
label_entry:
  %op3 = alloca float*
  store float* %arg0, float** %op3
  %op4 = alloca float*
  store float* %arg1, float** %op4
  %op5 = alloca i32
  store i32 %arg2, i32* %op5
  %op6 = alloca i32
  %op7 = alloca i32
  %op8 = alloca i32
  %op9 = alloca i32
  %op10 = alloca i32
  %op11 = alloca i32
  %op12 = alloca float
  %op13 = add i32* %op5, i32 1
  %op14 = load i32, i32* %op9
  store i32 %op13, i32 %op14
  store i32 0, i32* %op6
  store i32 0, i32* %op11
  store i32 0, i32* %op8
  %op15 = sub i32* %op5, i32 1
  %op16 = load i32, i32* %op6
  store i32 %op15, i32 %op16
}
define void @main() {
label_entry:
  %op0 = alloca i32
  %op1 = alloca [3 x float]
  %op2 = alloca [12 x float]
  %op3 = icmp slt i32 0, 0
  br i1 %op3, label %label4, label %label5
label4:                                                ; preds = %label_entry
  call void @neg_idx_except()
  br label %label5
label5:                                                ; preds = %label_entry, %label4
  %op6 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 0
  %op7 = sitofp i32 1 to i32
  store i32 %op7, float* %op6
  %op8 = icmp slt i32 1, 0
  br i1 %op8, label %label9, label %label10
label9:                                                ; preds = %label5
  call void @neg_idx_except()
  br label %label10
label10:                                                ; preds = %label5, %label9
  %op11 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 1
  %op12 = sitofp i32 2 to i32
  store i32 %op12, float* %op11
  %op13 = icmp slt i32 2, 0
  br i1 %op13, label %label14, label %label15
label14:                                                ; preds = %label10
  call void @neg_idx_except()
  br label %label15
label15:                                                ; preds = %label10, %label14
  %op16 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 2
  %op17 = sitofp i32 1 to i32
  store i32 %op17, float* %op16
  %op18 = icmp slt i32 3, 0
  br i1 %op18, label %label19, label %label20
label19:                                                ; preds = %label15
  call void @neg_idx_except()
  br label %label20
label20:                                                ; preds = %label15, %label19
  %op21 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 3
  %op22 = sitofp i32 1 to i32
  store i32 %op22, float* %op21
  %op23 = mul i32 1, 4
  %op24 = add i32 %op23, 0
  %op25 = icmp slt i32 %op24, 0
  br i1 %op25, label %label26, label %label27
label26:                                                ; preds = %label20
  call void @neg_idx_except()
  br label %label27
label27:                                                ; preds = %label20, %label26
  %op28 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 %op24
  %op29 = sitofp i32 2 to i32
  store i32 %op29, float* %op28
  %op30 = mul i32 1, 4
  %op31 = add i32 %op30, 1
  %op32 = icmp slt i32 %op31, 0
  br i1 %op32, label %label33, label %label34
label33:                                                ; preds = %label27
  call void @neg_idx_except()
  br label %label34
label34:                                                ; preds = %label27, %label33
  %op35 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 %op31
  %op36 = sitofp i32 3 to i32
  store i32 %op36, float* %op35
  %op37 = mul i32 1, 4
  %op38 = add i32 %op37, 2
  %op39 = icmp slt i32 %op38, 0
  br i1 %op39, label %label40, label %label41
label40:                                                ; preds = %label34
  call void @neg_idx_except()
  br label %label41
label41:                                                ; preds = %label34, %label40
  %op42 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 %op38
  %op43 = sitofp i32 4 to i32
  store i32 %op43, float* %op42
  %op44 = mul i32 1, 4
  %op45 = add i32 %op44, 3
  %op46 = icmp slt i32 %op45, 0
  br i1 %op46, label %label47, label %label48
label47:                                                ; preds = %label41
  call void @neg_idx_except()
  br label %label48
label48:                                                ; preds = %label41, %label47
  %op49 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 %op45
  %op50 = sitofp i32 3 to i32
  store i32 %op50, float* %op49
  %op51 = mul i32 2, 4
  %op52 = add i32 %op51, 0
  %op53 = icmp slt i32 %op52, 0
  br i1 %op53, label %label54, label %label55
label54:                                                ; preds = %label48
  call void @neg_idx_except()
  br label %label55
label55:                                                ; preds = %label48, %label54
  %op56 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 %op52
  %op57 = sitofp i32 1 to i32
  store i32 %op57, float* %op56
  %op58 = mul i32 2, 4
  %op59 = add i32 %op58, 1
  %op60 = icmp slt i32 %op59, 0
  br i1 %op60, label %label61, label %label62
label61:                                                ; preds = %label55
  call void @neg_idx_except()
  br label %label62
label62:                                                ; preds = %label55, %label61
  %op63 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 %op59
  %op64 = sitofp i32 1 to i32
  store i32 %op64, float* %op63
  %op65 = sub i32 0, 2
  %op66 = mul i32 2, 4
  %op67 = add i32 %op66, 2
  %op68 = icmp slt i32 %op67, 0
  br i1 %op68, label %label69, label %label70
label69:                                                ; preds = %label62
  call void @neg_idx_except()
  br label %label70
label70:                                                ; preds = %label62, %label69
  %op71 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 %op67
  %op72 = sitofp i32 %op65 to i32
  store i32 %op72, float* %op71
  %op73 = mul i32 2, 4
  %op74 = add i32 %op73, 3
  %op75 = icmp slt i32 %op74, 0
  br i1 %op75, label %label76, label %label77
label76:                                                ; preds = %label70
  call void @neg_idx_except()
  br label %label77
label77:                                                ; preds = %label70, %label76
  %op78 = getelementptr [12 x float], [12 x float]* %op2, i32 0, i32 %op74
  %op79 = sitofp i32 0 to i32
  store i32 %op79, float* %op78
  %op80 = load [3 x float], [3 x float]* %op1
  %op81 = load [12 x float], [12 x float]* %op2
  %op82 = call i32 @gauss([3 x float] %op80, [12 x float] %op81, i32 3)
  store i32 0, i32* %op0
}
