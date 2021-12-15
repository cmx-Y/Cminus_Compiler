; ModuleID = 'cminus'
source_filename = "/home/cxy/group/2021fall-compiler_cminus/tests/4-ir-opt/./testcases/ActiveVars/testcase-2.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  %op9 = icmp sgt i32 0, 2
  %op10 = zext i1 %op9 to i32
  %op11 = icmp ne i32 %op10, 0
  br i1 %op11, label %label12, label %label15
label12:                                                ; preds = %label_entry
  %op14 = add i32 4, 1
  br label %label15
label15:                                                ; preds = %label_entry, %label12
  %op35 = phi i32 [ 4, %label_entry ], [ %op14, %label12 ]
  %op18 = icmp sgt i32 %op35, 6
  %op19 = zext i1 %op18 to i32
  %op20 = icmp ne i32 %op19, 0
  br i1 %op20, label %label21, label %label24
label21:                                                ; preds = %label15
  %op23 = add i32 8, 1
  br label %label24
label24:                                                ; preds = %label15, %label21
  %op36 = phi i32 [ 8, %label15 ], [ %op23, %label21 ]
  %op27 = icmp sgt i32 %op36, 10
  %op28 = zext i1 %op27 to i32
  %op29 = icmp ne i32 %op28, 0
  br i1 %op29, label %label30, label %label34
label30:                                                ; preds = %label24
  %op33 = add i32 12, 10
  br label %label34
label34:                                                ; preds = %label24, %label30
  %op37 = phi i32 [ 12, %label24 ], [ %op33, %label30 ]
  ret void
}
