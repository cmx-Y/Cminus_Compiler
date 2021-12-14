dbg_instr % = phi i32 [ %, %label8 ], [ %op55, %label16 ]
instr_op_num: 4
lval_name:
% = phi i32 [ %, %label8 ], [ %op55, %label16 ]
operand0:% = phi i32 [ %, %label58 ], [ undef, %label_entry ]..
operand1:label8:                                                ; preds = %label3
  br label %label11
..
operand2:%op55 = sdiv i32 %op53, %..
operand3:label16:                                                ; preds = %label11
  %op19 = mul i32 %, %
  %op21 = mul i32 %op19, %
  %op23 = mul i32 %op21, %
  %op25 = mul i32 %op23, %
  %op27 = mul i32 %op25, %
  %op29 = mul i32 %op27, %
  %op31 = mul i32 %op29, %
  %op33 = mul i32 %op31, %
  %op35 = mul i32 %op33, %
  %op37 = sdiv i32 %op35, %
  %op39 = sdiv i32 %op37, %
  %op41 = sdiv i32 %op39, %
  %op43 = sdiv i32 %op41, %
  %op45 = sdiv i32 %op43, %
  %op47 = sdiv i32 %op45, %
  %op49 = sdiv i32 %op47, %
  %op51 = sdiv i32 %op49, %
  %op53 = sdiv i32 %op51, %
  %op55 = sdiv i32 %op53, %
  %op57 = add i32 %, 1
  br label %label11
..
dbg_instr % = phi i32 [ 0, %label8 ], [ %op57, %label16 ]
instr_op_num: 4
lval_name:
% = phi i32 [ 0, %label8 ], [ %op57, %label16 ]
operand0:0..
operand1:label8:                                                ; preds = %label3
  br label %label11
..
operand2:%op57 = add i32 %, 1..
operand3:label16:                                                ; preds = %label11
  %op19 = mul i32 %, %
  %op21 = mul i32 %op19, %
  %op23 = mul i32 %op21, %
  %op25 = mul i32 %op23, %
  %op27 = mul i32 %op25, %
  %op29 = mul i32 %op27, %
  %op31 = mul i32 %op29, %
  %op33 = mul i32 %op31, %
  %op35 = mul i32 %op33, %
  %op37 = sdiv i32 %op35, %
  %op39 = sdiv i32 %op37, %
  %op41 = sdiv i32 %op39, %
  %op43 = sdiv i32 %op41, %
  %op45 = sdiv i32 %op43, %
  %op47 = sdiv i32 %op45, %
  %op49 = sdiv i32 %op47, %
  %op51 = sdiv i32 %op49, %
  %op53 = sdiv i32 %op51, %
  %op55 = sdiv i32 %op53, %
  %op57 = add i32 %, 1
  br label %label11
..
dbg_instr %op13 = icmp slt i32 %, 10000
instr_op_num: 2
lval_name:op13
dbg_instr %op14 = zext i1 %op13 to i32
instr_op_num: 1
lval_name:op14
dbg_instr %op15 = icmp ne i32 %op14, 0
instr_op_num: 2
lval_name:op15
dbg_instr br i1 %op15, label %label16, label %label58
instr_op_num: 3
lval_name:
dbg_instr %op19 = mul i32 %, %
instr_op_num: 2
lval_name:op19
 operand_0_name: 
what 
  use_list_size: 22
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op21 = mul i32 %op19, %
instr_op_num: 2
lval_name:op21
 operand_0_name: op19
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op23 = mul i32 %op21, %
instr_op_num: 2
lval_name:op23
 operand_0_name: op21
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op25 = mul i32 %op23, %
instr_op_num: 2
lval_name:op25
 operand_0_name: op23
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op27 = mul i32 %op25, %
instr_op_num: 2
lval_name:op27
 operand_0_name: op25
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op29 = mul i32 %op27, %
instr_op_num: 2
lval_name:op29
 operand_0_name: op27
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op31 = mul i32 %op29, %
instr_op_num: 2
lval_name:op31
 operand_0_name: op29
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op33 = mul i32 %op31, %
instr_op_num: 2
lval_name:op33
 operand_0_name: op31
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op35 = mul i32 %op33, %
instr_op_num: 2
lval_name:op35
 operand_0_name: op33
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op37 = sdiv i32 %op35, %
instr_op_num: 2
lval_name:op37
 operand_0_name: op35
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op39 = sdiv i32 %op37, %
instr_op_num: 2
lval_name:op39
 operand_0_name: op37
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op41 = sdiv i32 %op39, %
instr_op_num: 2
lval_name:op41
 operand_0_name: op39
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op43 = sdiv i32 %op41, %
instr_op_num: 2
lval_name:op43
 operand_0_name: op41
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op45 = sdiv i32 %op43, %
instr_op_num: 2
lval_name:op45
 operand_0_name: op43
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op47 = sdiv i32 %op45, %
instr_op_num: 2
lval_name:op47
 operand_0_name: op45
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op49 = sdiv i32 %op47, %
instr_op_num: 2
lval_name:op49
 operand_0_name: op47
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op51 = sdiv i32 %op49, %
instr_op_num: 2
lval_name:op51
 operand_0_name: op49
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op53 = sdiv i32 %op51, %
instr_op_num: 2
lval_name:op53
 operand_0_name: op51
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op55 = sdiv i32 %op53, %
instr_op_num: 2
lval_name:op55
 operand_0_name: op53
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op57 = add i32 %, 1
instr_op_num: 2
lval_name:op57
 operand_0_name: 
what 
  use_list_size: 3
 operand_1_name: 
what 
  use_list_size: 1
dbg_instr br label %label11
instr_op_num: 1
lval_name:
dbg_instr % = phi i32 [ %, %label8 ], [ %op55, %label16 ]
instr_op_num: 4
lval_name:
% = phi i32 [ %, %label8 ], [ %op55, %label16 ]
operand0:% = phi i32 [ %, %label58 ], [ undef, %label_entry ]..
operand1:label8:                                                ; preds = %label3
  br label %label11
..
operand2:%op55 = sdiv i32 %op53, %..
operand3:label16:                                                ; preds = %label11
  %op19 = mul i32 %, %
  %op21 = mul i32 %op19, %
  %op23 = mul i32 %op21, %
  %op25 = mul i32 %op23, %
  %op27 = mul i32 %op25, %
  %op29 = mul i32 %op27, %
  %op31 = mul i32 %op29, %
  %op33 = mul i32 %op31, %
  %op35 = mul i32 %op33, %
  %op37 = sdiv i32 %op35, %
  %op39 = sdiv i32 %op37, %
  %op41 = sdiv i32 %op39, %
  %op43 = sdiv i32 %op41, %
  %op45 = sdiv i32 %op43, %
  %op47 = sdiv i32 %op45, %
  %op49 = sdiv i32 %op47, %
  %op51 = sdiv i32 %op49, %
  %op53 = sdiv i32 %op51, %
  %op55 = sdiv i32 %op53, %
  %op57 = add i32 %, 1
  br label %label11
..
dbg_instr % = phi i32 [ 0, %label8 ], [ %op57, %label16 ]
instr_op_num: 4
lval_name:
% = phi i32 [ 0, %label8 ], [ %op57, %label16 ]
operand0:0..
operand1:label8:                                                ; preds = %label3
  br label %label11
..
operand2:%op57 = add i32 %, 1..
operand3:label16:                                                ; preds = %label11
  %op19 = mul i32 %, %
  %op21 = mul i32 %op19, %
  %op23 = mul i32 %op21, %
  %op25 = mul i32 %op23, %
  %op27 = mul i32 %op25, %
  %op29 = mul i32 %op27, %
  %op31 = mul i32 %op29, %
  %op33 = mul i32 %op31, %
  %op35 = mul i32 %op33, %
  %op37 = sdiv i32 %op35, %
  %op39 = sdiv i32 %op37, %
  %op41 = sdiv i32 %op39, %
  %op43 = sdiv i32 %op41, %
  %op45 = sdiv i32 %op43, %
  %op47 = sdiv i32 %op45, %
  %op49 = sdiv i32 %op47, %
  %op51 = sdiv i32 %op49, %
  %op53 = sdiv i32 %op51, %
  %op55 = sdiv i32 %op53, %
  %op57 = add i32 %, 1
  br label %label11
..
dbg_instr %op13 = icmp slt i32 %, 10000
instr_op_num: 2
lval_name:op13
dbg_instr %op14 = zext i1 %op13 to i32
instr_op_num: 1
lval_name:op14
dbg_instr %op15 = icmp ne i32 %op14, 0
instr_op_num: 2
lval_name:op15
dbg_instr br i1 %op15, label %label16, label %label58
instr_op_num: 3
lval_name:
dbg_instr br label %label11
instr_op_num: 1
lval_name:
dbg_instr %op19 = mul i32 %, %
instr_op_num: 2
lval_name:op19
 operand_0_name: 
what 
  use_list_size: 22
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op21 = mul i32 %op19, %
instr_op_num: 2
lval_name:op21
 operand_0_name: op19
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op23 = mul i32 %op21, %
instr_op_num: 2
lval_name:op23
 operand_0_name: op21
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op25 = mul i32 %op23, %
instr_op_num: 2
lval_name:op25
 operand_0_name: op23
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op27 = mul i32 %op25, %
instr_op_num: 2
lval_name:op27
 operand_0_name: op25
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op29 = mul i32 %op27, %
instr_op_num: 2
lval_name:op29
 operand_0_name: op27
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op31 = mul i32 %op29, %
instr_op_num: 2
lval_name:op31
 operand_0_name: op29
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op33 = mul i32 %op31, %
instr_op_num: 2
lval_name:op33
 operand_0_name: op31
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op35 = mul i32 %op33, %
instr_op_num: 2
lval_name:op35
 operand_0_name: op33
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op37 = sdiv i32 %op35, %
instr_op_num: 2
lval_name:op37
 operand_0_name: op35
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op39 = sdiv i32 %op37, %
instr_op_num: 2
lval_name:op39
 operand_0_name: op37
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op41 = sdiv i32 %op39, %
instr_op_num: 2
lval_name:op41
 operand_0_name: op39
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op43 = sdiv i32 %op41, %
instr_op_num: 2
lval_name:op43
 operand_0_name: op41
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op45 = sdiv i32 %op43, %
instr_op_num: 2
lval_name:op45
 operand_0_name: op43
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op47 = sdiv i32 %op45, %
instr_op_num: 2
lval_name:op47
 operand_0_name: op45
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op49 = sdiv i32 %op47, %
instr_op_num: 2
lval_name:op49
 operand_0_name: op47
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op51 = sdiv i32 %op49, %
instr_op_num: 2
lval_name:op51
 operand_0_name: op49
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op53 = sdiv i32 %op51, %
instr_op_num: 2
lval_name:op53
 operand_0_name: op51
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op55 = sdiv i32 %op53, %
instr_op_num: 2
lval_name:op55
 operand_0_name: op53
  use_list_size: 1
 operand_1_name: 
what 
  use_list_size: 22
dbg_instr %op57 = add i32 %, 1
instr_op_num: 2
lval_name:op57
 operand_0_name: 
what 
  use_list_size: 3
 operand_1_name: 
what 
  use_list_size: 1
dbg_instr br label %label11
instr_op_num: 1
lval_name:
dbg_instr % = phi i32 [ %, %label58 ], [ undef, %label_entry ]
instr_op_num: 2
lval_name:
% = phi i32 [ %, %label58 ], [ undef, %label_entry ]
operand0:% = phi i32 [ %, %label8 ], [ %op55, %label16 ]..
operand1:label58:                                                ; preds = %label11
  %op60 = add i32 %, 1
  br label %label3
..
dbg_instr % = phi i32 [ 1, %label_entry ], [ %op60, %label58 ]
instr_op_num: 4
lval_name:
% = phi i32 [ 1, %label_entry ], [ %op60, %label58 ]
operand0:1..
operand1:label_entry:
  br label %label3
..
operand2:%op60 = add i32 %, 1..
operand3:label58:                                                ; preds = %label11
  %op60 = add i32 %, 1
  br label %label3
..
dbg_instr % = phi i32 [ %, %label58 ], [ undef, %label_entry ]
instr_op_num: 2
lval_name:
% = phi i32 [ %, %label58 ], [ undef, %label_entry ]
operand0:% = phi i32 [ 0, %label8 ], [ %op57, %label16 ]..
operand1:label58:                                                ; preds = %label11
  %op60 = add i32 %, 1
  br label %label3
..
dbg_instr %op5 = icmp slt i32 %, 10000
instr_op_num: 2
lval_name:op5
dbg_instr %op6 = zext i1 %op5 to i32
instr_op_num: 1
lval_name:op6
dbg_instr %op7 = icmp ne i32 %op6, 0
instr_op_num: 2
lval_name:op7
dbg_instr br i1 %op7, label %label8, label %label9
instr_op_num: 3
lval_name:
dbg_instr %op60 = add i32 %, 1
instr_op_num: 2
lval_name:op60
 operand_0_name: 
what 
  use_list_size: 22
 operand_1_name: 
what 
  use_list_size: 1
dbg_instr br label %label3
instr_op_num: 1
lval_name:
declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  br label %label3
label3:                                                ; preds = %label_entry, %label58
  %op61 = phi i32 [ %op64, %label58 ], [ undef, %label_entry ]
  %op62 = phi i32 [ 1, %label_entry ], [ %op60, %label58 ]
  %op63 = phi i32 [ %op65, %label58 ], [ undef, %label_entry ]
  %op5 = icmp slt i32 %op62, 10000
  %op6 = zext i1 %op5 to i32
  %op7 = icmp ne i32 %op6, 0
  br i1 %op7, label %label8, label %label9
label8:                                                ; preds = %label3
  br label %label11
label9:                                                ; preds = %label3
  call void @output(i32 %op61)
  ret void
label11:                                                ; preds = %label8, %label16
  %op64 = phi i32 [ %op61, %label8 ], [ %op55, %label16 ]
  %op65 = phi i32 [ 0, %label8 ], [ %op57, %label16 ]
  %op13 = icmp slt i32 %op65, 10000
  %op14 = zext i1 %op13 to i32
  %op15 = icmp ne i32 %op14, 0
  br i1 %op15, label %label16, label %label58
label16:                                                ; preds = %label11
  %op19 = mul i32 %op62, %op62
  %op21 = mul i32 %op19, %op62
  %op23 = mul i32 %op21, %op62
  %op25 = mul i32 %op23, %op62
  %op27 = mul i32 %op25, %op62
  %op29 = mul i32 %op27, %op62
  %op31 = mul i32 %op29, %op62
  %op33 = mul i32 %op31, %op62
  %op35 = mul i32 %op33, %op62
  %op37 = sdiv i32 %op35, %op62
  %op39 = sdiv i32 %op37, %op62
  %op41 = sdiv i32 %op39, %op62
  %op43 = sdiv i32 %op41, %op62
  %op45 = sdiv i32 %op43, %op62
  %op47 = sdiv i32 %op45, %op62
  %op49 = sdiv i32 %op47, %op62
  %op51 = sdiv i32 %op49, %op62
  %op53 = sdiv i32 %op51, %op62
  %op55 = sdiv i32 %op53, %op62
  %op57 = add i32 %op65, 1
  br label %label11
label58:                                                ; preds = %label11
  %op60 = add i32 %op62, 1
  br label %label3
}
