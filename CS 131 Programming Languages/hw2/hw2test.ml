(* Instructor Provided Test Cases *)
let accept_all derivation string = Some (derivation, string)
let accept_empty_suffix derivation = function
   | [] -> Some (derivation, [])
   | _ -> None;;

(* My Tests *)
type ingredient_nonterminals = 
  | Coffee | Espresso | Milk | Syrup | Powder | Sugar;;

let coffee_grammar = (Coffee, function
  | Coffee -> [[N Espresso; N Milk; N Syrup]; [N Espresso; N Milk]; [N Espresso]]
  | Espresso -> [[T "Single Shot"]; [T "Double Shot"]; [T "Single Shot"; N Sugar]; [T "Double Shot"; N Sugar]]
  | Sugar -> [[T "Brown"]; [T "Cane"]; [T "Brown"; N Sugar]]
  | Milk -> [[T "2%"]; [T "Whole"]; [T "Nonfat"]; [T "Soy"]]
  | Syrup -> [[T "Strawberry"]; [T "Caramel"]; [T "Vanilla"]]);;

let test_1 = (parse_prefix coffee_grammar accept_all ["Single Shot"; "Brown"; "2%"; "Strawberry"] = Some ([
  (Coffee, [N Espresso; N Milk; N Syrup]); 
  (Espresso, [T "Single Shot"; N Sugar]); 
  (Sugar, [T "Brown"]); 
  (Milk, [T "2%"]); 
  (Syrup, [T "Strawberry"])
], []));;

let test_2 = (parse_prefix coffee_grammar accept_empty_suffix ["Single Shot"; "Brown"; "Brown"; "Brown"; "Brown"; "Brown"; "Brown"; "Cane"] = Some ([
  (Coffee, [N Espresso]); 
  (Espresso, [T "Single Shot"; N Sugar]); 
  (Sugar, [T "Brown"; N Sugar]); 
  (Sugar, [T "Brown"; N Sugar]); 
  (Sugar, [T "Brown"; N Sugar]); 
  (Sugar, [T "Brown"; N Sugar]); 
  (Sugar, [T "Brown"; N Sugar]); 
  (Sugar, [T "Brown"; N Sugar]); 
  (Sugar, [T "Cane"])   
], []));;

