type ('nonterminal, 'terminal) symbol =
  | N of 'nonterminal
  | T of 'terminal;;

let rec format_rules_helper r x = match r with 
| [] -> []
| (nt,rhs)::t -> if nt = x then rhs::(format_rules_helper t x) else format_rules_helper t x;; 
(* ('a * 'b) list -> 'a -> 'b list = <fun> *)

let format_rules r = function x -> format_rules_helper r x;;
(* ('a * 'b) list -> 'a -> 'b list = <fun> *)

let convert_grammar gram1 = match gram1 with
  | (start, rules) -> (start, format_rules rules);;
(* 'a * ('b * 'c) list -> 'a * ('b -> 'c list) = <fun> *)

let rec check_symbols rules_f rule accept deriv frag = match rule with
  | [] -> accept deriv frag
  | _ -> match frag with
    | [] -> None
    | first_term::other_terms -> match rule with
      | (T ts)::t -> if ts = first_term then check_symbols rules_f t accept deriv other_terms else None
      | (N nts)::t -> check_rhs nts rules_f (rules_f nts) (check_symbols rules_f t accept) deriv frag
(* 
  ('a -> ('a, 'b) symbol list list) ->
  ('a, 'b) symbol list ->
  (('a * ('a, 'b) symbol list) list -> 'b list -> 'c option) ->
  ('a * ('a, 'b) symbol list) list -> 'b list -> 'c option = <fun>
*)

and check_rhs start rules_f rhs accept deriv frag = match rhs with
  | [] -> None
  | rule::others -> match (check_symbols rules_f rule accept (deriv@[start, rule]) frag) with 
    | None -> check_rhs start rules_f others accept deriv frag
    | matched -> matched;;  
(*
  'a ->
  ('a -> ('a, 'b) symbol list list) ->
  ('a, 'b) symbol list list ->
  (('a * ('a, 'b) symbol list) list -> 'b list -> 'c option) ->
  ('a * ('a, 'b) symbol list) list -> 'b list -> 'c option = <fun>
*)

let parse_prefix gram accept frag = match gram with
  | (start, rules_f) -> check_rhs start rules_f (rules_f start) accept [] frag;;
(*
  a * ('a -> ('a, 'b) symbol list list) ->
  (('a * ('a, 'b) symbol list) list -> 'b list -> 'c option) ->
  'b list -> 'c option = <fun>
*)
