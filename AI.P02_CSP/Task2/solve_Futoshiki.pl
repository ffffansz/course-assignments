:- use_module(library(clpfd)).
% example 1
solve(ex1, Ret):-
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
findall(Board, label(Board), Ret)