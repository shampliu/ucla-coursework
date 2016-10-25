;
; CS161 HW3: Sokoban
;
; *********************
;    READ THIS FIRST
; *********************
;
; All functions that you need to modify are marked with 'EXERCISE' in their
; header comments. This file also contains many helper functions. You may call
; any of them in your functions.
;
; Do not modify a-star.lsp.
;
; *Warning*: The provided A* code only supports the maximum cost of 4999 for any
; node. That is f(n)=g(n)+h(n) < 5000. So, be careful when you write your
; heuristic functions.  Do not make them return anything too large.
;
; For Allegro Common Lisp users: The free version of Allegro puts a limit on
; memory. So, it may crash on some hard sokoban problems and there is no easy
; fix (unless you buy Allegro). Of course, other versions of Lisp may also crash
; if the problem is too hard, but the amount of memory available will be
; relatively more relaxed. Improving the quality of the heuristic will mitigate
; this problem, as it will allow A* to solve hard problems with fewer node
; expansions. In either case, this limitation should not significantly affect
; your grade.
; 
; Remember that most functions are not graded on efficiency (only correctness).
; Efficiency can only influence your heuristic performance in the competition
; (which will affect your score).
;  


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; General utility functions
; They are not necessary for this homework.
; Use/modify them for your own convenience.
;

;
; For reloading modified code.
; I found this easier than typing (load "filename") every time.
;
(defun reload ()
  (load "hw3.lsp")
  )

;
; For loading a-star.lsp.
;
(defun load-a-star ()
  (load "a-star.lsp"))

;
; Reloads hw3.lsp and a-star.lsp
;
(defun reload-all ()
  (reload)
  (load-a-star)
  )

;
; A shortcut function.
; goal-test and next-states stay the same throughout the assignment.
; So, you can just call (sokoban <init-state> #'<heuristic-name>).
; 
(defun sokoban (s h)
  (a* s #'goal-test #'next-states h)
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; end general utility functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; We now begin actual Sokoban code
;

; Define some global variables
(setq blank 0)
(setq wall 1)
(setq box 2)
(setq keeper 3)
(setq star 4)
(setq boxstar 5)
(setq keeperstar 6)

; Some helper functions for checking the content of a square
(defun isBlank (v)
  (= v blank)
  )

(defun isWall (v)
  (= v wall)
  )

(defun isBox (v)
  (= v box)
  )

(defun isKeeper (v)
  (= v keeper)
  )

(defun isStar (v)
  (= v star)
  )

(defun isBoxStar (v)
  (= v boxstar)
  )

(defun isKeeperStar (v)
  (= v keeperstar)
  )

;
; Helper function of getKeeperPosition
;
(defun getKeeperColumn (r col)
  (cond ((null r) nil)
	(t (if (or (isKeeper (car r)) (isKeeperStar (car r)))
	       col
	     (getKeeperColumn (cdr r) (+ col 1))
	     );end if
	   );end t
	);end cond
  )

;
; getKeeperPosition (s firstRow)
; Returns a list indicating the position of the keeper (c r).
; 
; Assumes that the keeper is in row >= firstRow.
; The top row is the zeroth row.
; The first (right) column is the zeroth column.
;
(defun getKeeperPosition (s row)
  (cond ((null s) nil)
	(t (let ((x (getKeeperColumn (car s) 0)))
	     (if x
		 ;keeper is in this row
		 (list x row)
		 ;otherwise move on
		 (getKeeperPosition (cdr s) (+ row 1))
		 );end if
	       );end let
	 );end t
	);end cond
  );end defun

;
; cleanUpList (l)
; Returns l with any NIL element removed.
; For example, if l is '(1 2 NIL 3 NIL), returns '(1 2 3).
;
(defun cleanUpList (L)
  (cond ((null L) nil)
	(t (let ((cur (car L))
		 (res (cleanUpList (cdr L)))
		 )
	     (if cur
		 (cons cur res)
		  res
		 )
	     );end let
	   );end t
	);end cond
  );end

;
; contains (item row)
; Returns T if item is found in the row, else NIL
; 
(defun contains (item row)
	(cond
		((null row) NIL)
		((equal item (car row)) t)
		(t (contains item (cdr row)))
	)
)

; 
; goal-test (s)
; Returns NIL if there is at least one box that's not on a star, 
; otherwise T (we have reached our goal). 
;
(defun goal-test (s)
  (cond
  	((null s) t)
  	((contains box (car s)) NIL)
  	(t (goal-test (cdr s)))
  )
 );end defun

; 
; isOutOfBounds (s pos)
; Checks the bounds of pos, which is a list pair consisting of
; (col, row) and makes sure that col and row are not less than
; 0 or greater than the columns and rows of the board respectively
;
(defun isOutOfBounds (s pos)
  (or 
  	(< (second pos) 0) 
  	(< (first pos) 0) 
  	(> (second pos) (- (length s) 1)) 
  	(> (first pos) (- (length (first s)) 1))
  )
)

; 
; get-square-helper (row col)
; Helper function for get-square that checks the row, passed in as a 
; list of squares and returns the item at column col
;
; * Note that row in here is not a number but a list
;
(defun get-square-helper (row col)
	(cond
		((null row) NIL)
		((= 0 col) (car row))
		(t (get-square-helper (cdr row) (- col 1)))
	)
)

; 
; get-square (s pos)
; Returns the item in s at the square specified by pos (col, row)
; 
(defun get-square (s pos)
	(let* (
		(col (first pos))
		(row (second pos)))
		(cond
			((isOutOfBounds s pos) wall) ; return a number, can't return NIL
			((= 0 row) (get-square-helper (car s) col))
			(t (get-square (cdr s) (list col (- row 1))))
		)
	)
)

;
; isEmpty (square)
; Returns true only if a square is a star or blank i.e. if it can be 
; occupied by the keeper or a box.  
;
(defun isEmpty (square) 
	(or (isStar square) (isBlank square))
)

; 
; isMovable (src dest)
; Returns true if there exists a box at position src and it can be 
; moved to position dest 
;
(defun isMovable (src dest)
	(and (or (isBox src) (isBoxStar src)) (isEmpty dest))
)

;
; set-square-helper (row col new)
; Helper function for set-square that goes through the row argument
; passed in as a list and returns the same list with the element at 
; column col replaced with new
; 
; * Note that row in here is not a number but a list
;
(defun set-square-helper (row col new)
	(cond
		((null row) NIL)
		((= 0 col) (cons new (cdr row)))
		(t (cons (car row) (set-square-helper (cdr row) (- col 1) new)))
	)
)

; 
; set-square (s col row new)
; Replaces the element in s at (col, row) with the new variable 
; passed in and returns the modified state.
; 
(defun set-square (s col row new)
	(cond
		((null s) NIL)
		((= 0 row) (cons (set-square-helper (car s) col new) (cdr s)))
		(t (cons (car s) (set-square (cdr s) col (- row 1) new)))
	)

)

;
; move (s src dest withstar withoutstar)
; Template function used to move either a box or the keeper from src 
; to dest in s. withstar is the variable of the target object if it's
; on a star (keeperstar or boxstar) and withoutstar is the opposite. 
; This function checks the current square of the object; if it's on a 
; star, it will leave behind a star when it moves. If the dest has a star
; then the object will be placed on dest with a star as withstar and vice 
; versa. Returns the new state after the move.  
;
(defun move (s src dest withstar withoutstar)
	(let* (
		(old (get-square s src))
		(new (get-square s dest)))
		(cond
			((= withstar old) 
				(cond
					((isStar new)	(set-square (set-square s (first src) (second src) star) (first dest) (second dest) withstar))
					(t (set-square (set-square s (first src) (second src) star) (first dest) (second dest) withoutstar))
				)
			)
			(t ; if object is not currently on a star
				(cond
					((isStar new) (set-square (set-square s (first src) (second src) blank) (first dest) (second dest) withstar))
					(t (set-square (set-square s (first src) (second src) blank) (first dest) (second dest) withoutstar))
				)
			)
		)
	)
)

;
; move-keeper (s src dest)
; Wrapper for the move function to move the keeper from src to dest
;
(defun move-keeper (s src dest) ; always call this function after move-box so keeper never goes into a boxStar
	(move s src dest keeperstar keeper)
)

;
; move-box (s src dest)
; Wrapper for the move function to move a box from src to dest
;
(defun move-box (s src dest)
	(move s src dest boxstar box)
)

;
; try-move (s delta)
; Attempts to move the keeper in s. Argument delta is the desired
; direction represented as a tuple (c, r) where c is the column 
; movement (left / right) and r is the row movement (up / down).
; Begins by calculating 3 necessary coordinates
;  1. keeper position
;  2. position of keeper after delta
;  3. if there is a box where the keeper moves, the position of the box
;     after the keeper pushes it
; If there's no box, moves the keeper to the next square. If there is 
; a box, move the box first and then the keeper.
; Returns NIL if move is invalid, otherwise the new state after moving.
;
(defun try-move (s delta)
	(let* (
		(pos (getKeeperPosition s 0))																													; keeper pos
		(next (list (+ (first delta) (first pos)) (+ (second delta) (second pos))))						; dest of keeper
		(nextsquare (get-square s next))
		(nextnext (list (+ (first delta) (first next)) (+ (second delta) (second next)))) 		; dest of box
		(nextnextsquare (get-square s nextnext)))

		(cond
			((isEmpty nextsquare) (move-keeper s pos next))
			((isMovable nextsquare nextnextsquare) 
				(move-keeper (move-box s next nextnext) pos next)
			)
			(t NIL)
		)
	)
)

; 
; next-states (s)
; Returns list of successor states of s by calling try-move in all four
; directions. Returns only the non-NIL states i.e. the valid moves.
; 
(defun next-states (s)
  (cleanUpList 
  	(list 
	  	(try-move s '(0 -1)) 	; UP
	  	(try-move s '(0 1)) 	; DOWN
	  	(try-move s '(1 0)) 	; RIGHT
	  	(try-move s '(-1 0))	; LEFT
  	)
  );end
);

;
; h0 (s)
; Returns the trivial admissible heuristic.
;
(defun h0 (s)
	0
 )

;
; count-boxes-row (row)
; Counts the number of boxes not on a star in the given list, row
;
(defun count-boxes-row (row)
	(cond
		((null row) 0)
		((isBox (car row)) (+ 1 (count-boxes-row (cdr row))))
		(t (count-boxes-row (cdr row)))
	)
)

; 
; h1 (s)
; Returns the number of misplaced boxes in s
;
(defun h1 (s)
	(cond
		((null s) 0)
		(t (+ (count-boxes-row (car s)) (h1 (cdr s))))
	)
)

; 
; abs (num)
; Returns absolute value of num 
;
(defun abs (num)
	(cond 
		((< num 0) (- 0 num))
		(t num)
	)
)

;
; manhattan (src dest)
; Returns the Manhattan Distance from src to dest
; 
(defun manhattan (src dest)
	(+ (abs (- (first src) (first dest))) (abs (- (second src) (second dest))))
)

; 
; min-dist (src dests curr)
; Returns the smallest Manhattan Distance from src coordinate (col, row)
; to any item in the list dests. 
;
(defun min-dist (src dests curr)
	(cond 
		((null dests) curr)
		(t
			(let* (
				(dist (manhattan src (car dests))))
				(cond
					((null curr) (min-dist src (cdr dests) dist))
					(t (cond
						((< dist curr) (min-dist src (cdr dests) dist))
						(t (min-dist src (cdr dests) curr))
					))
				)
			)
		)
	)
)

; 
; sum-min-dist (srcs dests)
; Returns the sum of the minimum distances from every item in srcs to a destination
; in dests.
;
(defun sum-min-dist (srcs dests)
	(cond 
		((null dests) 0)
		(t (+ (min-dist (car srcs) dests NIL) (sum-min-dist (cdr srcs) (cdr dests))))
	)
)

;
; sum-keeper-dist (s keeper dests)
; Returns the sum of the distance of keeper to every destination in dests
;
(defun sum-keeper-dist (s keeper dests)
	(cond
		((null dests) 0)
		(t (+ (manhattan keeper (car dests)) (sum-keeper-dist s keeper (cdr dests))))
	)
)

; 
; get-items-helper (row r c item)
; Helper function that gets the argument item in a specified row and returns a 
; list of the coordinates of each item found in that row. 
;
(defun get-items-helper (row r c item)
	(cond 
		((null row) NIL)
		((= item (car row))
			(cons (list c r) (get-items-helper (cdr row) r (+ 1 c) item))
		)
		(t (get-items-helper (cdr row) r (+ 1 c) item))
	)
)

; 
; get-items (s r item)
; Returns a list of the coordinates of every item found in s
;
(defun get-items (s r item)
	(cond 
		((null s) NIL)
		(t (append (get-items-helper (car s) r 0 item) (get-items (cdr s) (+ r 1) item)))
	)
)

;
; h704291153 (s)
; Sums up the min distances from boxes to stars and the distance of the keeper
; to each box. 
;
(defun h704291153 (s)
	(let* (
		(boxes (get-items s 0 box))
		(stars (get-items s 0 star))
		(pos (getKeeperPosition s 0)))
		(+ (sum-min-dist boxes stars) (sum-keeper-dist s pos boxes)) 
	)
  
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#|
 | Some predefined problems.  Each problem can be visualized by calling
 | (printstate <problem>).  Problems are ordered roughly by their difficulties.
 | For most problems, we also privide 2 additional number per problem: 1) # of
 | nodes expanded by A* using our next-states and h0 heuristic.  2) the depth of
 | the optimal solution.  These numbers are located at the comments of the
 | problems. For example, the first problem below was solved by 80 nodes
 | expansion of A* and its optimal solution depth is 7.
 |
 | Your implementation may not result in the same number of nodes expanded, but
 | it should probably give something in the same ballpark. As for the solution
 | depth, any admissible heuristic must make A* return an optimal solution. So,
 | the depths of the optimal solutions provided could be used for checking
 | whether your heuristic is admissible.
 |
 | Warning: some problems toward the end are quite hard and could be impossible
 | to solve without a good heuristic!
 |
 |#

;(80,7)
(setq p1 '((1 1 1 1 1 1)
	   (1 0 3 0 0 1)
	   (1 0 2 0 0 1)
	   (1 1 0 1 1 1)
	   (1 0 0 0 0 1)
	   (1 0 0 0 4 1)
	   (1 1 1 1 1 1)))

;(110,10)
(setq p2 '((1 1 1 1 1 1 1)
	   (1 0 0 0 0 0 1) 
	   (1 0 0 0 0 0 1) 
	   (1 0 0 2 1 4 1) 
	   (1 3 0 0 1 0 1)
	   (1 1 1 1 1 1 1)))

;(211,12)
(setq p3 '((1 1 1 1 1 1 1 1 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 0 0 0 2 0 3 4 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 1 1 1 1 1 1 1 1)))

;(300,13)
(setq p4 '((1 1 1 1 1 1 1)
	   (0 0 0 0 0 1 4)
	   (0 0 0 0 0 0 0)
	   (0 0 1 1 1 0 0)
	   (0 0 1 0 0 0 0)
	   (0 2 1 0 0 0 0)
	   (0 3 1 0 0 0 0)))

;(551,10)
(setq p5 '((1 1 1 1 1 1)
	   (1 1 0 0 1 1)
	   (1 0 0 0 0 1)
	   (1 4 2 2 4 1)
	   (1 0 0 0 0 1)
	   (1 1 3 1 1 1)
	   (1 1 1 1 1 1)))

;(722,12)
(setq p6 '((1 1 1 1 1 1 1 1)
	   (1 0 0 0 0 0 4 1)
	   (1 0 0 0 2 2 3 1)
	   (1 0 0 1 0 0 4 1)
	   (1 1 1 1 1 1 1 1)))

;(1738,50)
(setq p7 '((1 1 1 1 1 1 1 1 1 1)
	   (0 0 1 1 1 1 0 0 0 3)
	   (0 0 0 0 0 1 0 0 0 0)
	   (0 0 0 0 0 1 0 0 1 0)
	   (0 0 1 0 0 1 0 0 1 0)
	   (0 2 1 0 0 0 0 0 1 0)
	   (0 0 1 0 0 0 0 0 1 4)))

;(1763,22)
(setq p8 '((1 1 1 1 1 1)
	   (1 4 0 0 4 1)
	   (1 0 2 2 0 1)
	   (1 2 0 1 0 1)
	   (1 3 0 0 4 1)
	   (1 1 1 1 1 1)))

;(1806,41)
(setq p9 '((1 1 1 1 1 1 1 1 1) 
	   (1 1 1 0 0 1 1 1 1) 
	   (1 0 0 0 0 0 2 0 1) 
	   (1 0 1 0 0 1 2 0 1) 
	   (1 0 4 0 4 1 3 0 1) 
	   (1 1 1 1 1 1 1 1 1)))

;(10082,51)
(setq p10 '((1 1 1 1 1 0 0)
	    (1 0 0 0 1 1 0)
	    (1 3 2 0 0 1 1)
	    (1 1 0 2 0 0 1)
	    (0 1 1 0 2 0 1)
	    (0 0 1 1 0 0 1)
	    (0 0 0 1 1 4 1)
	    (0 0 0 0 1 4 1)
	    (0 0 0 0 1 4 1)
	    (0 0 0 0 1 1 1)))

;(16517,48)
(setq p11 '((1 1 1 1 1 1 1)
	    (1 4 0 0 0 4 1)
	    (1 0 2 2 1 0 1)
	    (1 0 2 0 1 3 1)
	    (1 1 2 0 1 0 1)
	    (1 4 0 0 4 0 1)
	    (1 1 1 1 1 1 1)))

;(22035,38)
(setq p12 '((0 0 0 0 1 1 1 1 1 0 0 0)
	    (1 1 1 1 1 0 0 0 1 1 1 1)
	    (1 0 0 0 2 0 0 0 0 0 0 1)
	    (1 3 0 0 0 0 0 0 0 0 0 1)
	    (1 0 0 0 2 1 1 1 0 0 0 1)
	    (1 0 0 0 0 1 0 1 4 0 4 1)
	    (1 1 1 1 1 1 0 1 1 1 1 1)))

;(26905,28)
(setq p13 '((1 1 1 1 1 1 1 1 1 1)
	    (1 4 0 0 0 0 0 2 0 1)
	    (1 0 2 0 0 0 0 0 4 1)
	    (1 0 3 0 0 0 0 0 2 1)
	    (1 0 0 0 0 0 0 0 0 1)
	    (1 0 0 0 0 0 0 0 4 1)
	    (1 1 1 1 1 1 1 1 1 1)))

;(41715,53)
(setq p14 '((0 0 1 0 0 0 0)
	    (0 2 1 4 0 0 0)
	    (0 2 0 4 0 0 0)
	    (3 2 1 1 1 0 0)
	    (0 0 1 4 0 0 0)))

;(48695,44)
(setq p15 '((1 1 1 1 1 1 1)
	    (1 0 0 0 0 0 1)
	    (1 0 0 2 2 0 1)
	    (1 0 2 0 2 3 1)
	    (1 4 4 1 1 1 1)
	    (1 4 4 1 0 0 0)
	    (1 1 1 1 0 0 0)
	    ))

;(91344,111)
(setq p16 '((1 1 1 1 1 0 0 0)
	    (1 0 0 0 1 0 0 0)
	    (1 2 1 0 1 1 1 1)
	    (1 4 0 0 0 0 0 1)
	    (1 0 0 5 0 5 0 1)
	    (1 0 5 0 1 0 1 1)
	    (1 1 1 0 3 0 1 0)
	    (0 0 1 1 1 1 1 0)))

;(3301278,76)
(setq p17 '((1 1 1 1 1 1 1 1 1 1)
	    (1 3 0 0 1 0 0 0 4 1)
	    (1 0 2 0 2 0 0 4 4 1)
	    (1 0 2 2 2 1 1 4 4 1)
	    (1 0 0 0 0 1 1 4 4 1)
	    (1 1 1 1 1 1 0 0 0 0)))

;(??,25)
(setq p18 '((0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
	    (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
	    (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
	    (0 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0)
	    (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
	    (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
	    (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 4 1 0 0 0 0)
	    (0 0 0 0 1 0 2 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 2 0 0 0 4 1 0 0 0 0)
	    ))
;(??,21)
(setq p19 '((0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (1 1 1 1 0 0 0 0 1 1 1 1)
	    (0 0 0 0 1 0 0 1 0 0 0 0)
	    (0 0 0 0 0 0 3 0 0 0 2 0)
	    (0 0 0 0 1 0 0 1 0 0 0 4)
	    (1 1 1 1 0 0 0 0 1 1 1 1)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 2 0 4 1 0 0 0)))

;(??,??)
(setq p20 '((0 0 0 1 1 1 1 0 0)
	    (1 1 1 1 0 0 1 1 0)
	    (1 0 0 0 2 0 0 1 0)
	    (1 0 0 5 5 5 0 1 0)
	    (1 0 0 4 0 4 0 1 1)
	    (1 1 0 5 0 5 0 0 1)
	    (0 1 1 5 5 5 0 0 1)
	    (0 0 1 0 2 0 1 1 1)
	    (0 0 1 0 3 0 1 0 0)
	    (0 0 1 1 1 1 1 0 0)))

;(??,??)
(setq p21 '((0 0 1 1 1 1 1 1 1 0)
	    (1 1 1 0 0 1 1 1 1 0)
	    (1 0 0 2 0 0 0 1 1 0)
	    (1 3 2 0 2 0 0 0 1 0)
	    (1 1 0 2 0 2 0 0 1 0)
	    (0 1 1 0 2 0 2 0 1 0)
	    (0 0 1 1 0 2 0 0 1 0)
	    (0 0 0 1 1 1 1 0 1 0)
	    (0 0 0 0 1 4 1 0 0 1)
	    (0 0 0 0 1 4 4 4 0 1)
	    (0 0 0 0 1 0 1 4 0 1)
	    (0 0 0 0 1 4 4 4 0 1)
	    (0 0 0 0 1 1 1 1 1 1)))

;(??,??)
(setq p22 '((0 0 0 0 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 0 0 1 2 0 0 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 1 1 1 0 0 2 1 1 0 0 0 0 0 0 0 0 0)
	    (0 0 1 0 0 2 0 2 0 1 0 0 0 0 0 0 0 0 0)
	    (1 1 1 0 1 0 1 1 0 1 0 0 0 1 1 1 1 1 1)
	    (1 0 0 0 1 0 1 1 0 1 1 1 1 1 0 0 4 4 1)
	    (1 0 2 0 0 2 0 0 0 0 0 0 0 0 0 0 4 4 1)
	    (1 1 1 1 1 0 1 1 1 0 1 3 1 1 0 0 4 4 1)
	    (0 0 0 0 1 0 0 0 0 0 1 1 1 1 1 1 1 1 1)
	    (0 0 0 0 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#|
 | Utility functions for printing states and moves.
 | You do not need to understand any of the functions below this point.
 |#

;
; Helper function of prettyMoves
; from s1 --> s2
;
(defun detectDiff (s1 s2)
  (let* ((k1 (getKeeperPosition s1 0))
	 (k2 (getKeeperPosition s2 0))
	 (deltaX (- (car k2) (car k1)))
	 (deltaY (- (cadr k2) (cadr k1)))
	 )
    (cond ((= deltaX 0) (if (> deltaY 0) 'DOWN 'UP))
	  (t (if (> deltaX 0) 'RIGHT 'LEFT))
	  );end cond
    );end let
  );end defun

;
; Translates a list of states into a list of moves.
; Usage: (prettyMoves (a* <problem> #'goal-test #'next-states #'heuristic))
;
(defun prettyMoves (m)
  (cond ((null m) nil)
	((= 1 (length m)) (list 'END))
	(t (cons (detectDiff (car m) (cadr m)) (prettyMoves (cdr m))))
	);end cond
  );

;
; Print the content of the square to stdout.
;
(defun printSquare (s)
  (cond ((= s blank) (format t " "))
	((= s wall) (format t "#"))
	((= s box) (format t "$"))
	((= s keeper) (format t "@"))
	((= s star) (format t "."))
	((= s boxstar) (format t "*"))
	((= s keeperstar) (format t "+"))
	(t (format t "|"))
	);end cond
  )

;
; Print a row
;
(defun printRow (r)
  (dolist (cur r)
    (printSquare cur)
    )
  );

;
; Print a state
;
(defun printState (s)
  (progn
    (dolist (cur s)
      (printRow cur)
      (format t "~%")
      )
    );end progn
  )

;
; Print a list of states with delay.
;
(defun printStates (sl delay)
  (dolist (cur sl)
    (printState cur)
    (sleep delay)
    );end dolist
  );end defun
