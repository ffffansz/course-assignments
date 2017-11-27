:- use_module(library(clpfd)).
% case 1
solve(case1, Ret):-
set_prolog_flag(answer_write_options, [max_depth(0)]),
Board = [E11, E12, 3, E14,
         E21, E22, E23, E24,
         E31, E32, E33, E34,
         E41, E42, E43, E44],
R1 = [E11, E12, 3, E14],
R2 = [E21, E22, E23, E24],
R3 = [E31, E32, E33, E34],
R4 = [E41, E42, E43, E44],
C1 = [E11, E21, E31, E41],
C2 = [E12, E22, E32, E42],
C3 = [3, E23, E33, E43],
C4 = [E14, E24, E34, E44],
Board ins 1..4,
E22 #> E12, 3 #> E14, E33 #> E23, E41 #> E42, E42 #> E43,
maplist(all_different, [R1, R2, R3, R4, C1, C2, C3, C4]),
findall(Board, label(Board), Ret).

% case 2
solve(case2, Ret):-
set_prolog_flag(answer_write_options, [max_depth(0)]),
Board = [E11, E12, E13, E14, E15,
         E21, E22, E23, E24, E25,
         E31, E32, E33, E34, E35,
         E41, E42, E43, E44, E45,
         E51, E52, E53, E54, 4],
R1 = [E11, E12, E13, E14, E15],
R2 = [E21, E22, E23, E24, E25],
R3 = [E31, E32, E33, E34, E35],
R4 = [E41, E42, E43, E44, E45],
R5 = [E51, E52, E53, E54, 4],
C1 = [E11, E21, E31, E41, E51],
C2 = [E12, E22, E32, E42, E52],
C3 = [E13, E23, E33, E43, E53],
C4 = [E14, E24, E34, E44, E54],
C5 = [E15, E25, E35, E45, 4],
Board ins 1..5,
maplist(#>, [E21, E11, E23, E24, E25, E33, E51], [E11, E12, E22, E23, E24, E32, E52]),
maplist(all_different, [R1, R2, R3, R4, R5, C1, C2, C3, C4, C5]),
findall(Board, label(Board), Ret).

% case 3
solve(case3, Ret):-
set_prolog_flag(answer_write_options, [max_depth(0)]),
Board = [E11, E12, E13, E14, 2, 6,
         E21, E22, E23, E24, E25, 3,
         3, E32, E33, E34, E35, E36,
         E41, E42, 4, E44, E45, E46,
         E51, E52, E53, E54, E55, E56,
         E61, E62, E63, E64, E65, E66],
R1 = [E11, E12, E13, E14, 2, 6],
R2 = [E21, E22, E23, E24, E25, 3],
R3 = [3, E32, E33, E34, E35, E36],
R4 = [E41, E42, 4, E44, E45, E46],
R5 = [E51, E52, E53, E54, E55, E56],
R6 = [E61, E62, E63, E64, E65, E66],
C1 = [E11, E21, 3, E41, E51, E61],
C2 = [E12, E22, E32, E42, E52, E62],
C3 = [E13, E23, E33, 4, E53, E63],
C4 = [E14, E24, E34, E44, E54, E64],
C5 = [2, E25, E35, E45, E55, E65],
C6 = [6, 3, E36, E46, E56, E66],
Board ins 1..6,
maplist(#>, 
[E11, E22, E24, E32, E36, 4, E44, E65, E66], 
[E12, E32, E25, 3, 3, E44, E45, E64, E65]),
maplist(all_different, [R1, R2, R3, R4, R5, R6, C1, C2, C3, C4, C5, C6]),
findall(Board, label(Board), Ret).

% case 4
solve(case4, Ret):-
set_prolog_flag(answer_write_options, [max_depth(0)]),
Board = [E11, E12, E13, E14, E15, E16, 6,
         E21, E22, E23, E24, E25, E26, E27,
         E31, E32, E33, E34, E35, E36, E37,
         E41, E42, E43, E44, E45, E46, 2,
         E51, E52, E53, E54, E55, E56, E57,
         E61, 5, E63, E64, E65, E66, E67,
         E71, E72, E73, E74, E75, E76, E77],
R1 = [E11, E12, E13, E14, E15, E16, 6],
R2 = [E21, E22, E23, E24, E25, E26, E27],
R3 = [E31, E32, E33, E34, E35, E36, E37],
R4 = [E41, E42, E43, E44, E45, E46, 2],
R5 = [E51, E52, E53, E54, E55, E56, E57],
R6 = [E61, 5, E63, E64, E65, E66, E67],
R7 = [E71, E72, E73, E74, E75, E76, E77],
C1 = [E11, E21, E31, E41, E51, E61, E71],
C2 = [E12, E22, E32, E42, E52, 5, E72],
C3 = [E13, E23, E33, E34, E53, E63, E73],
C4 = [E14, E24, E34, E44, E54, E64, E74],
C5 = [E15, E25, E35, E45, E55, E65, E75],
C6 = [E16, E26, E36, E46, E56, E66, E76],
C7 = [6, E27, E37, 2, E57, E67, E77],
Board ins 1..7,
maplist(#>, 
[E12, E12, E16, E23, E25, E32, E33, E36, E42, E43, E45, E45, E52, E53, E54, 5, E66, E75, E76],
[E11, E13, 6, E33, E26, E31, E32, E37, E52, E42, E35, E46, E53, E63, E44, E61, E76, E65, E77]),
maplist(all_different, [R1, R2, R3, R4, R5, R6, R7, C1, C2, C3, C4, C5, C6, C7]),
findall(Board, label(Board), Ret).

% case 5
solve(case5, Ret):-
set_prolog_flag(answer_write_options, [max_depth(0)]),
Board = [E11, E12, E13, E14, E15, E16, E17, E18,
         E21, E22, E23, E24, 6, E26, 7, E28,
         E31, E32, E33, 4, E35, E36, E37, E38,
         E41, E42, E43, E44, E45, E46, E47, E48,
         E51, E52, E53, E54, E55, E56, E57, 6,
         E61, E62, E63, E64, E65, 4, E67, E68,
         E71, E72, E73, E74, E75, E76, E77, 3,
         E81, E82, E83, E84, E85, E86, E87, E88],
R1 = [E11, E12, E13, E14, E15, E16, E17, E18],
R2 = [E21, E22, E23, E24, 6, E26, 7, E28],
R3 = [E31, E32, E33, 4, E35, E36, E37, E38],
R4 = [E41, E42, E43, E44, E45, E46, E47, E48],
R5 = [E51, E52, E53, E54, E55, E56, E57, 6],
R6 = [E61, E62, E63, E64, E65, 4, E67, E68],
R7 = [E71, E72, E73, E74, E75, E76, E77, 3],
R8 = [E81, E82, E83, E84, E85, E86, E87, E88],
C1 = [E11, E21, E31, E41, E51, E61, E71, E81],
C2 = [E12, E22, E32, E42, E52, E62, E72, E82],
C3 = [E13, E23, E33, E43, E53, E63, E73, E83],
C4 = [E14, E24, 4, E44, E54, E64, E74, E84],
C5 = [E15, 6, E35, E45, E55, E65, E75, E85],
C6 = [E16, E26, E36, E46, E56, 4, E76, E86],
C7 = [E17, 7, E37, E47, E57, E67, E77, E87],
C8 = [E18, E28, E38, E48, 6, E68, 3, E88],
Board ins 1..8,
maplist(#>, 
[E12, E13, E16, E17, E18, E22, E26, 7, 7, E33, 4, 4, 4, E41, E42, E44, E48, E51, 6, E65, 4, 4, 4, E68, E73],
[E13, E14, E15, E16, E17, E21, E36, E26, E37, E23, E24, E44, E33, E42, E43, E54, 6, E52, E57, E75, E65, E67, E56, 6, E74]),
maplist(all_different, [R1, R2, R3, R4, R5, R6, R7, R8, C1, C2, C3, C4, C5, C6, C7, C8]),
findall(Board, label(Board), Ret).