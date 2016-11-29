;
; check-assign (assign literal)
; function that checks if a variable in the current assignment list is equal to 
; the literal. returns true if found, nil if the variable is the negation of the 
; literal. if the literal is not in the assignment list, returns true (because it 
; can be satisfied by simply assigning the variable)
;
(defun check-assign (assign literal)
  (let* ( 
    (val (car assign)))

    (cond
      ((null assign) t) ; returns true if we ran out of assignments 
      ((= literal val) t)
      ((= (+ literal val) 0) nil)
      (t (check-assign (cdr assign) literal))
    )
  )
)

; 
; check-clause (assign clause)
; checks a clause to see if it is satisfied by the currently assigned variables
; or can be satisfied by assigning values to the unassigned variables
;
(defun check-clause (assign clause) 
  (if (null clause) 
    nil
    (cond 
      ((check-assign assign (car clause)) t)
      (t (check-clause assign (cdr clause)))
    )
  )
)

;
; check-clauses (assign clauses)
; checks the list of clauses to see if it is satisfied by the currently assigned variables
; or can be satisfied by assigning values to the unassigned variables
;
(defun check-clauses (assign clauses)
  (if (null clauses) 
    t
    (and (check-clause assign (car clauses)) (check-clauses assign (cdr clauses)))
  )
)

;
; assign-new (assign sign)
; assigns the next variable in the assignment list a value of positive or negative depending 
; on the value of sign passed in (either 1 or -1). since the assignment list is populated in
; chronological order, the next variable is just the length + 1
; 
(defun assign-new (assign sign)
  (let* (
    (var (+ (length assign) 1)))
    (if (> sign 0) 
      (append assign (list var))
      (append assign (list (- 0 var)))
    )
  ) 
)

; BELOW:
; Attempted to optimize the algorithm by pruning the clauses at each depth. 
; For example if the assignment list was (1 2) and the clauses were ((1 4 5) (2 4) (-2 5 7) (-1 3) (5)), 
; the pruned clauses list would be ((5 7) (3))
;  
; ; 
; ; find-var (var clause)
; ; helper function that returns true if var is found in clause
; ;
; (defun find-var (var clause)
;   (cond 
;     ((null var) nil)
;     ((null clause) nil)
;     ((= var (car clause)) t)
;     (t (find-var var (cdr clause)))
;   )
; )


; (defun prune-clause-h (var clause) ; takes out the negative
;   (if (null clause)
;     nil
;     (cond
;       ((= (- 0 var) (car clause)) (prune-clause-h var (cdr clause)))
;       (t (append (list (car clause)) (prune-clause-h var (cdr clause))))
;     )
;   )
; )

; (defun prune-clause (assign clause)
;   (if (or (null clause) (find-var (car assign) clause))
;     nil
;     (if (null assign)
;       clause
;       (prune-clause (cdr assign) (prune-clause-h (car assign) clause))
;     )
;   )
; )

; (defun prune-clauses (assign clauses)
;   (if (null assign) 
;     clauses
;     (cond 
;       ((null clauses) nil)
;       (t (cons (prune-clause assign (car clauses)) (prune-clauses assign (cdr clauses))))
;     )
;   )
; )

;
; recursive-backtracking (n assign clauses)
; recursive backtracking search algorithm. performs a check on the constraints (clauses) first to see if 
; the assignment list is valid. if it is and the assignment list is all fully assigned, return the list.
; else, do a dfs and try assigning True or False to the next unassigned variable in the list 
;
(defun recursive-backtracking (n assign clauses)
  (if (check-clauses assign clauses)
    (if (= (length assign) n) 
      assign
      (or (recursive-backtracking n (assign-new assign 1) clauses) (recursive-backtracking n (assign-new assign -1) clauses))
    )
    nil
  )
)

;
; sat? (n delta)
; wrapper function that calls the recursive-backtracking search function to solve the SAT 
; problem with n variables and delta as the CNFs
;
(defun sat? (n delta) 
  (recursive-backtracking n '() delta)
)
