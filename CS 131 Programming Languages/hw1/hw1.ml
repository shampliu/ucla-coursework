let rec check a b = match b with
| h::t -> if a = h then true else check a t
| [] -> false;;
(* 'a -> 'a list -> bool = <fun> *)

let rec subset a b = match a with
| h::t -> if check h b then subset t b else false
| [] -> true;;
(* 'a list -> 'a list -> bool = <fun> *)

let equal_sets a b = subset a b && subset b a;;
(* 'a list -> 'a list -> bool = <fun> *)

let rec set_union a b = match a with 
| h::t -> if subset [h] b then set_union t b else (set_union t b) @ [h]
| [] -> b;;
(* 'a list -> 'a list -> 'a list = <fun> *)

let rec set_intersection a b = match a with
| h::t -> if subset [h] b then (set_intersection t b) @ [h] else set_intersection t b
| [] -> [];; 
(* 'a list -> 'a list -> 'a list = <fun> *)

let rec set_diff a b = match a with
| h::t -> if subset [h] b then set_diff t b else (set_diff t b) @ [h]
| [] -> [];; 
(* 'a list -> 'a list -> 'a list = <fun> *)

let rec computed_fixed_point eq f x = if eq (f x) x then x else computed_fixed_point eq f (f x);;
(* ('a -> 'a -> bool) -> ('a -> 'a) -> 'a -> 'a = <fun> *)

let rec loop f p x = if p <= 0 then x else loop f (p-1) (f x);;
(* ('a -> 'a) -> int -> 'a -> 'a = <fun> *)

let rec computed_periodic_point eq f p x = if eq (loop f p x) x then x else computed_periodic_point eq f p (f x);;
(* ('a -> 'a -> bool) -> ('a -> 'a) -> int -> 'a -> 'a = <fun> *)

type ('nonterminal, 'terminal) symbol =
| N of 'nonterminal
| T of 'terminal;;

let check_symbol s l = match s with 
| T s -> true
| N s -> subset [s] l;;
(* ('a, 'b) symbol -> 'a list -> bool = <fun> *)

let rec check_rhs r l = match r with
| h::t -> if check_symbol h l then check_rhs t l else false
| [] -> true;;
(* ('a, 'b) symbol list -> 'a list -> bool = <fun> *)

let rec build_list_once r l = match r with 
| [] -> l
| (a, b)::t -> if subset [a] l then build_list_once t l else (if check_rhs b l then build_list_once t (l @ [a]) else build_list_once t l);;
(* 'a * ('a, 'b) symbol list) list -> 'a list -> 'a list = <fun> *)

let rec build_full_list r l = if equal_sets (build_list_once r l) l then l else build_full_list r (build_list_once r l);;
(* ('a * ('a, 'b) symbol list) list -> 'a list -> 'a list = <fun> *)

let rec filter_rules r l = match r with
| (a, b)::t -> if check_rhs b l then (a, b)::(filter_rules t l) else filter_rules t l
| [] -> [];;
(* ('a * ('b, 'c) symbol list) list -> 'b list -> ('a * ('b, 'c) symbol list) list = <fun> *)

let filter_blind_alleys g = match g with
| (start, rules) -> (start, filter_rules rules (build_full_list rules []));;
(* 'a * ('b * ('b, 'c) symbol list) list -> 'a * ('b * ('b, 'c) symbol list) list = <fun> *)
