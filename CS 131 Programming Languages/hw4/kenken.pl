% kenken(
%  4,
%  [
%   +(6, [1-1, 1-2, 2-1]),
%   *(96, [1-3, 1-4, 2-2, 2-3, 2-4]),
%   -(1, 3-1, 3-2),
%   -(1, 4-1, 4-2),
%   +(8, [3-3, 4-3, 4-4]),
%   *(2, [3-4])
%  ],
%  T
% ), write(T), nl, fail.
%
% I used the above 4 x 4 test case to benchmark both my regular and plain kenken functions. 
% By nesting the call between two calls to statistics, I was able to see how long they took.
% The regular kenken took 1ms while running plain_kenken on the same 4x4 took 4798 ms. 
% I tried running the 6x6 test case provided in the spec but plain_kenken's computation time grows exponentially so I didn't have time to wait for it to finish. Anything larger than a 4x4 kenken puzzle takes a significant amount of time with plain_kenken. Meanwhile, regular kenken has no problem solving the 6x6 puzzle. 

% Regular Kenken Statistics
%
% Times              since start      since last
%
%    user   time       0.004 sec       0.004 sec
%    system time       0.004 sec       0.004 sec
%    cpu    time       0.008 sec       0.008 sec
%    real   time      25.506 sec      25.506 sec

% Plain Kenken Statistics 
%
% Times              since start      since last
%
%    user   time       0.005 sec       0.001 sec
%    system time       0.006 sec       0.002 sec
%    cpu    time       0.011 sec       0.003 sec
%    real   time     111.819 sec      86.313 sec

% No-Op Kenken API
% In the case of a no-op puzzle, the constraints passed would be similar to the add or multiply function, but this time the product / sum would be the general target result we're looking for among the squares specified in the list.
%  
% To solve these, we would match the constraint by checking it against every possible operation in a long disjunction by calling add, subtract, multiply, or divide to try to reach the given result. Upon failure, we would backtrack and try the next operation and if all 4 fail, then we can conclude that it's not solveable. 
% The function call would be something like: match_constraint_noop (target, list)
%  
% pseudocode:
%   match list with empty list, return true, constraint is satisfied
%   match list with one item, if it equals the target, return true
%   match list with multiple items
%     call add on the items and check if it equals the target
%     OR call multiply
%     OR call subtract
%     OR call quotient
%
% Basically this approach is a brute force way to solve the no-op kenken puzzle. 
% % % REGULAR KENKEN % % %
kenken(N, C, T) :- 
    length(T,N), maplist(checkLength(N), T),
    maplist(setDomain(N), T),
    checkConstraints(T, C), 
    maplist(fd_all_different, T),
    transpose(T, TT),
    maplist(fd_all_different, TT),
    maplist(fd_labeling, T). 

% check if the Row is of size Length
checkLength(Length, Row) :- length(Row, Length).

% make sure each element of Row ranges from 1 to Max
setDomain(Max, Row) :- fd_domain(Row, 1, Max).

checkConstraints(_, []). 
checkConstraints(Matrix, [H|T]) :- matchConstraint(Matrix, H), checkConstraints(Matrix, T).

matchConstraint(Matrix, +(S, L)) :- add(Matrix, S, L, 0).
matchConstraint(Matrix, *(P, L)) :- mult(Matrix, P, L, 1).
matchConstraint(Matrix, -(D, J, K)) :- sub(Matrix, D, J, K). 
matchConstraint(Matrix, /(Q, J, K)) :- div(Matrix, Q, J, K).

add(_, Sum, [], Sum).
add(Matrix, Sum, [H|T], Accum) :- 
    getElement(H, Matrix, E),
    CurrentSum #= Accum + E,
    add(Matrix, Sum, T, CurrentSum).
     
mult(_, Prod, [], Prod).
mult(Matrix, Prod, [H|T], Accum) :-
    getElement(H, Matrix, E),
    CurrentProd #= Accum * E,
    mult(Matrix, Prod, T, CurrentProd).

sub(_, Diff, _, _, Diff).
sub(Matrix, Diff, J, K) :-
    getElement(J, Matrix, E1),
    getElement(K, Matrix, E2),
    Diff1 #= E1 - E2,
    sub(Matrix, Diff, J, K, Diff1).
sub(Matrix, Diff, J, K) :-
    getElement(J, Matrix, E1),
    getElement(K, Matrix, E2),
    Diff2 #= E2 - E1,
    sub(Matrix, Diff, J, K, Diff2).

div(_, Quot, _, _, Quot).
div(Matrix, Quot, J, K) :-
    getElement(J, Matrix, E1),
    getElement(K, Matrix, E2),
    Quot1 #= E1 / E2,
    div(Matrix, Quot, J, K, Quot1).
div(Matrix, Quot, J, K) :-
    getElement(J, Matrix, E1),
    getElement(K, Matrix, E2),
    Quot2 #= E2 / E1,
    div(Matrix, Quot, J, K, Quot2).

% I is the row, J is the col, M is the matrix, E is the element returned
getElement(I-J, M, E) :- nth1(I, M, Row), nth1(J, Row, E). 

% SWI-PROLOG clpfd module's transpose function
transpose([], []).
transpose([F|Fs], Ts) :-
    transpose(F, [F|Fs], Ts).

transpose([], _, []).
transpose([_|Rs], Ms, [Ts|Tss]) :-
    lists_firsts_rests(Ms, Ts, Ms1),
    transpose(Rs, Ms1, Tss).

lists_firsts_rests([], [], []).
lists_firsts_rests([[F|Os]|Rest], [F|Fs], [Os|Oss]) :-
    lists_firsts_rests(Rest, Fs, Oss).

% % % PLAIN KENKEN % % %  
plain_kenken(N, C, T) :- 
    length(T, N), maplist(checkLength(N), T),
    plain_check_domain(N, T),
    maplist(plain_all_different, T),
    transpose(T, TT), maplist(plain_all_different, TT),
    plain_check_constraints(T, C).

plain_check_domain(_, []).
plain_check_domain(Max, [H|T]) :-
    findall(Num, between(1, Max, Num), L), 
    plain_check_domain_helper(H, L),
    plain_check_domain(Max, T).

plain_check_domain_helper([], []).
plain_check_domain_helper(L, [H|T]) :-
    select(H, L, NewList),
    plain_check_domain_helper(NewList, T).

plain_all_different([]).
plain_all_different([H|T]) :- \+ member(H, T), plain_all_different(T).
	
plain_check_constraints(_, []). 
plain_check_constraints(Matrix, [H|T]) :- plain_match_constraint(Matrix, H), plain_check_constraints(Matrix, T).

plain_match_constraint(T, +(S, L)) :- plain_add(T, S, L, 0).
plain_match_constraint(T, *(P, L)) :- plain_mult(T, P, L, 1).
plain_match_constraint(T, -(D, J, K)) :- plain_sub(T, D, J, K).
plain_match_constraint(T, /(Q, J, K)) :- plain_div(T, Q, J, K).

plain_add(_, Sum, [], Sum).
plain_add(Matrix, Sum, [H|T], Accum) :-
    getElement(H, Matrix, E),
    CurrentSum is E + Accum,
    plain_add(Matrix, Sum, T, CurrentSum).

plain_mult(_, Prod, [], Prod).
plain_mult(Matrix, Prod, [H|T], Accum) :-
    getElement(H, Matrix, E),
    CurrentProd is E * Accum,
    plain_mult(Matrix, Prod, T, CurrentProd).

plain_sub(_, Diff, _, _, Diff).
plain_sub(Matrix, Diff, J, K) :-
    getElement(J, Matrix, E1),
    getElement(K, Matrix, E2),
    Diff1 is E1 - E2, 
    plain_sub(Matrix, Diff, J, K, Diff1).
plain_sub(Matrix, Diff, J, K) :-
    getElement(J, Matrix, E1),
    getElement(K, Matrix, E2),
    Diff2 is E2 - E1,
    plain_sub(Matrix, Diff, J, K, Diff2).

plain_div(_, Quot, _, _, Quot).
plain_div(Matrix, Quot, J, K) :-
    getElement(J, Matrix, E1),
    getElement(K, Matrix, E2),
    Quot1 is E1 / E2,
    plain_div(Matrix, Quot, J, K, Quot1).
plain_div(Matrix, Quot, J, K) :-
    getElement(J, Matrix, E1),
    getElement(K, Matrix, E2),
    Quot2 is E2 / E1,
    plain_div(Matrix, Quot, J, K, Quot2).




