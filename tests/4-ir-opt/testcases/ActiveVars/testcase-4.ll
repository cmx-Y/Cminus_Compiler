; ModuleID = 'cminus'
source_filename = "/home/cxy/group/2021fall-compiler_cminus/tests/4-ir-opt/./testcases/ActiveVars/testcase-4.cminus"

@x = global [10 x i32] zeroinitializer
declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define i32 @minloc(i32* %arg0, i32 %arg1, i32 %arg2) {
label_entry:
  %op3 = alloca i32*
  %op11 = icmp slt i32 %arg1, 0
  br i1 %op11, label %label12, label %label13
label12:                                                ; preds = %label_entry
  call void @neg_idx_except()
  ret i32 0
label13:                                                ; preds = %label_entry
  %op15 = getelementptr i32, i32* %arg0, i32 %arg1
  %op16 = load i32, i32* %op15
  %op18 = add i32 %arg1, 1
  br label %label19
label19:                                                ; preds = %label13, %label42
  %op51 = phi i32 [ %arg1, %label13 ], [ %op54, %label42 ]
  %op52 = phi i32 [ %op16, %label13 ], [ %op55, %label42 ]
  %op53 = phi i32 [ %op18, %label13 ], [ %op44, %label42 ]
  %op22 = icmp slt i32 %op53, %arg2
  %op23 = zext i1 %op22 to i32
  %op24 = icmp ne i32 %op23, 0
  br i1 %op24, label %label25, label %label28
label25:                                                ; preds = %label19
  %op27 = icmp slt i32 %op53, 0
  br i1 %op27, label %label30, label %label31
label28:                                                ; preds = %label19
  ret i32 %op51
label30:                                                ; preds = %label25
  call void @neg_idx_except()
  ret i32 0
label31:                                                ; preds = %label25
  %op33 = getelementptr i32, i32* %arg0, i32 %op53
  %op34 = load i32, i32* %op33
  %op36 = icmp slt i32 %op34, %op52
  %op37 = zext i1 %op36 to i32
  %op38 = icmp ne i32 %op37, 0
  br i1 %op38, label %label39, label %label42
label39:                                                ; preds = %label31
  %op41 = icmp slt i32 %op53, 0
  br i1 %op41, label %label45, label %label46
label42:                                                ; preds = %label31, %label46
  %op54 = phi i32 [ %op51, %label31 ], [ %op53, %label46 ]
  %op55 = phi i32 [ %op52, %label31 ], [ %op49, %label46 ]
  %op44 = add i32 %op53, 1
  br label %label19
label45:                                                ; preds = %label39
  call void @neg_idx_except()
  ret i32 0
label46:                                                ; preds = %label39
  %op48 = getelementptr i32, i32* %arg0, i32 %op53
  %op49 = load i32, i32* %op48
  br label %label42
}
define void @sort(i32* %arg0, i32 %arg1, i32 %arg2) {
label_entry:
  %op3 = alloca i32*
  br label %label9
label9:                                                ; preds = %label_entry, %label47
  %op52 = phi i32 [ %op29, %label47 ], [ undef, %label_entry ]
  %op53 = phi i32 [ %op21, %label47 ], [ undef, %label_entry ]
  %op54 = phi i32 [ %arg1, %label_entry ], [ %op51, %label47 ]
  %op12 = sub i32 %arg2, 1
  %op13 = icmp slt i32 %op54, %op12
  %op14 = zext i1 %op13 to i32
  %op15 = icmp ne i32 %op14, 0
  br i1 %op15, label %label16, label %label24
label16:                                                ; preds = %label9
  %op21 = call i32 @minloc(i32* %arg0, i32 %op54, i32 %arg2)
  %op23 = icmp slt i32 %op21, 0
  br i1 %op23, label %label25, label %label26
label24:                                                ; preds = %label9
  ret void
label25:                                                ; preds = %label16
  call void @neg_idx_except()
  ret void
label26:                                                ; preds = %label16
  %op28 = getelementptr i32, i32* %arg0, i32 %op21
  %op29 = load i32, i32* %op28
  %op31 = icmp slt i32 %op54, 0
  br i1 %op31, label %label32, label %label33
label32:                                                ; preds = %label26
  call void @neg_idx_except()
  ret void
label33:                                                ; preds = %label26
  %op35 = getelementptr i32, i32* %arg0, i32 %op54
  %op36 = load i32, i32* %op35
  %op38 = icmp slt i32 %op21, 0
  br i1 %op38, label %label39, label %label40
label39:                                                ; preds = %label33
  call void @neg_idx_except()
  ret void
label40:                                                ; preds = %label33
  %op42 = getelementptr i32, i32* %arg0, i32 %op21
  store i32 %op36, i32* %op42
  %op45 = icmp slt i32 %op54, 0
  br i1 %op45, label %label46, label %label47
label46:                                                ; preds = %label40
  call void @neg_idx_except()
  ret void
label47:                                                ; preds = %label40
  %op49 = getelementptr i32, i32* %arg0, i32 %op54
  store i32 %op29, i32* %op49
  %op51 = add i32 %op54, 1
  br label %label9
}
define void @main() {
label_entry:
  %op1 = icmp slt i32 0, 0
  br i1 %op1, label %label2, label %label3
label2:                                                ; preds = %label_entry
  call void @neg_idx_except()
  ret void
label3:                                                ; preds = %label_entry
  %op4 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 0
  store i32 4, i32* %op4
  %op5 = icmp slt i32 1, 0
  br i1 %op5, label %label6, label %label7
label6:                                                ; preds = %label3
  call void @neg_idx_except()
  ret void
label7:                                                ; preds = %label3
  %op8 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 1
  store i32 7, i32* %op8
  %op9 = icmp slt i32 2, 0
  br i1 %op9, label %label10, label %label11
label10:                                                ; preds = %label7
  call void @neg_idx_except()
  ret void
label11:                                                ; preds = %label7
  %op12 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 2
  store i32 9, i32* %op12
  %op13 = icmp slt i32 3, 0
  br i1 %op13, label %label14, label %label15
label14:                                                ; preds = %label11
  call void @neg_idx_except()
  ret void
label15:                                                ; preds = %label11
  %op16 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 3
  store i32 2, i32* %op16
  %op17 = icmp slt i32 4, 0
  br i1 %op17, label %label18, label %label19
label18:                                                ; preds = %label15
  call void @neg_idx_except()
  ret void
label19:                                                ; preds = %label15
  %op20 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 4
  store i32 0, i32* %op20
  %op21 = icmp slt i32 5, 0
  br i1 %op21, label %label22, label %label23
label22:                                                ; preds = %label19
  call void @neg_idx_except()
  ret void
label23:                                                ; preds = %label19
  %op24 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 5
  store i32 6, i32* %op24
  %op25 = icmp slt i32 6, 0
  br i1 %op25, label %label26, label %label27
label26:                                                ; preds = %label23
  call void @neg_idx_except()
  ret void
label27:                                                ; preds = %label23
  %op28 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 6
  store i32 1, i32* %op28
  %op29 = icmp slt i32 7, 0
  br i1 %op29, label %label30, label %label31
label30:                                                ; preds = %label27
  call void @neg_idx_except()
  ret void
label31:                                                ; preds = %label27
  %op32 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 7
  store i32 3, i32* %op32
  %op33 = icmp slt i32 8, 0
  br i1 %op33, label %label34, label %label35
label34:                                                ; preds = %label31
  call void @neg_idx_except()
  ret void
label35:                                                ; preds = %label31
  %op36 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 8
  store i32 5, i32* %op36
  %op37 = icmp slt i32 9, 0
  br i1 %op37, label %label38, label %label39
label38:                                                ; preds = %label35
  call void @neg_idx_except()
  ret void
label39:                                                ; preds = %label35
  %op40 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 9
  store i32 8, i32* %op40
  %op41 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 0
  call void @sort(i32* %op41, i32 0, i32 10)
  br label %label42
label42:                                                ; preds = %label39, %label52
  %op57 = phi i32 [ 0, %label39 ], [ %op56, %label52 ]
  %op44 = icmp slt i32 %op57, 10
  %op45 = zext i1 %op44 to i32
  %op46 = icmp ne i32 %op45, 0
  br i1 %op46, label %label47, label %label50
label47:                                                ; preds = %label42
  %op49 = icmp slt i32 %op57, 0
  br i1 %op49, label %label51, label %label52
label50:                                                ; preds = %label42
  ret void
label51:                                                ; preds = %label47
  call void @neg_idx_except()
  ret void
label52:                                                ; preds = %label47
  %op53 = getelementptr [10 x i32], [10 x i32]* @x, i32 0, i32 %op57
  %op54 = load i32, i32* %op53
  call void @output(i32 %op54)
  %op56 = add i32 %op57, 1
  br label %label42
}
