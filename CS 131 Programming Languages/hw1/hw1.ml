let rec check a b = match b with
| h::t -> if a = h then true else check a t
| [] -> false;;

let rec subset a b = match a with
| h::t -> if check h b then subset t b else false
| [] -> true;;

let equal_sets a b = subset a b && subset b a;;

let rec set_union a b = match a with 
| h::t -> if subset [h] b then set_union t b else (set_union t b) @ [h]
| [] -> b;;

let rec set_intersection a b = match a with
| h::t -> if subset [h] b then (set_intersection t b) @ [h] else set_intersection t b
| [] -> [];; 

let rec set_diff a b = match a with
| h::t -> if subset [h] b then set_diff t b else (set_diff t b) @ [h]
| [] -> [];; 

let rec computed_fixed_point eq f x = if eq (f x) x then x else computed_fixed_point eq f (f x);;

let rec loop f p x = if p <= 0 then x else loop f (p-1) (f x);;

let rec computed_periodic_point eq f p x = if eq (loop f p x) x then x else computed_periodic_point eq f p (f x);;

type ('nonterminal, 'terminal) symbol =
| N of 'nonterminal
| T of 'terminal;;

(* s is the symbol to check, l is the list of good symbols *)
let check_symbol s l = match s with 
| T s -> true
| N s -> subset [s] l;;

let rec check_rhs r l = match r with
| h::t -> if check_symbol h l then check_rhs t l else false
| [] -> true;;

let rec build_list_once r l = match r with 
| [] -> l
| (a, b)::t -> if subset [a] l then build_list_once t l else (if check_rhs b l then build_list_once t (l @ [a]) else build_list_once t l);;

let rec build_full_list r l = if equal_sets (build_list_once r l) l then l else build_full_list r (build_list_once r l);;

let rec filter_rules r l = match r with
| (a, b)::t -> if check_rhs b l then (a, b)::(filter_rules t l) else filter_rules t l
| [] -> [];;

let filter_blind_alleys g = match g with
| (start, rules) -> (start, filter_rules rules (build_full_list rules []));;
