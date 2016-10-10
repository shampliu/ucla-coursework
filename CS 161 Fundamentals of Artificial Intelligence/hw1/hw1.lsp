; TREE-CONTAINS takes a target number N and an ordered tree TREE
; it executes a binary search on the ordered tree to look for N
; returns T if N is in the TREE, otherwise NIL

(defun TREE-CONTAINS (N TREE) 
	(cond 
		((null TREE) NIL)
		((numberp TREE) (equal N TREE))
		((< N (second TREE)) (TREE-CONTAINS N (first TREE)))
		((> N (second TREE)) (TREE-CONTAINS N (third TREE)))
		(t (equal N (second TREE)))
	)
)

; TREE-MAX takes an ordered tree TREE
; it recursively finds the rightmost node in the tree 
; returns the highest number in TREE
(defun TREE-MAX (TREE)
	(cond
		((null TREE) NIL)
		((atom TREE) TREE)
		(t (TREE-MAX (third TREE)))
	)
)

; TREE-ORDER takes an ordered tree TREE
; it performs an inorder traversal of TREE appending only the leaves to the result
; returns a single list of the numbers appearing in TREE in order
(defun TREE-ORDER (TREE)
	(cond
		((null TREE) NIL)
		((atom TREE) (list TREE))
		(t 
			(append
				(TREE-ORDER (first TREE)) (cons (second TREE) (TREE-ORDER (third TREE)))
			)
		)
	)
)

; SUB-LIST takes a list L, a start index START and a length LEN
; it traverses L until the starting index and then begins constructing the sub-list
; returns the sub-list of L, starting at START and having length LEN
(defun SUB-LIST (L START LEN) 
	(cond 
		((equal 0 LEN) NIL)
		((null L) NIL)
		((> START 0) (SUB-LIST (cdr L) (- START 1) LEN))
		(t (cons (car L) (SUB-LIST (cdr L) START (- LEN 1))))
	)
)

; SPLIT-LIST takes a list L
; it finds the midpoint of L and calls SUB-LIST on each half of L
; returns a list of two lists, L1 and L2 where L1 appended to L2 equals L and L2 is greater than L1 by 1 or equal to L1
(defun SPLIT-LIST (L) 
	(cond
		((evenp (length L))
			(let 
				((mid (/ (length L) 2)))
				(list 
					(SUB-LIST L 0 mid)
					(SUB-LIST L mid mid)
				)
			)
		) 
		(t 
			(let
				((mid (/ (- (length L) 1) 2)))
				(list
					(SUB-LIST L 0 mid)
					(SUB-LIST L mid (+ mid 1))
				)
			)
		)
	)
)

; BTREE-HEIGHT takes a binary tree TREE
; it does DFS and compares the left side to the right side at each depth
; returns the height of the TREE, i.e. the longest path from the root node to the farthest leaf node
(defun BTREE-HEIGHT (TREE)
	(cond 
		((atom TREE) 0)
		(t 
			(let 
				((left (BTREE-HEIGHT (first TREE))) (right (BTREE-HEIGHT (second TREE))))
				(cond 
					((> left right) (+ left 1))
					(t (+ right 1))
				)
			)
		)
	)
	
)

; LIST2BTREE takes a non-empty list of atoms LEAVES
; it recursively calls SPLIT-LIST on the LEAVES to split them in half, like the first part of merge sort
; returns binary tree constructed from LEAVES
(defun LIST2BTREE (LEAVES)
	(cond
		((equal (length LEAVES) 1) (first LEAVES))
		((equal (length LEAVES) 2) LEAVES)
    (t 
    	(list 
    		(LIST2BTREE (first (SPLIT-LIST LEAVES))) 
    		(LIST2BTREE (second (SPLIT-LIST LEAVES)))
    	)
    )
	)
)

; BTREE2LIST takes a binary tree TREE
; it does a DFS to append every element of TREE to the resulting list, starting from the left
; returns a list of atoms, consisting of every element in TREE
(defun BTREE2LIST (TREE)
	(cond
		((null TREE) NIL)
		((atom TREE) (list TREE))
		(t 
			(append (BTREE2LIST (first TREE)) (BTREE2LIST (second TREE)))
		)

	)
)

; IS-SAME takes two LISP expressions, E1 and E2 whose atoms are all numbers
; it checks if E1 == E2 by recursively comparing every atom / list of the expression
; returns T if E1 is equal to E2, otherwise NIL
(defun IS-SAME (E1 E2)
	(cond
		((null E1) (null E2))
		((atom E1) (and (atom E2) (= E1 E2)))
		(t
			(and (IS-SAME (car E1) (car E2)) (IS-SAME (cdr E1) (cdr E2))) 
		)
	)
)

