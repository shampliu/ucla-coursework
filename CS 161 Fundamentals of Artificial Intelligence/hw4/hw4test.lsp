
(setq L 
	'(
		"cnfs/sat/cnf_10.cnf"
		"cnfs/sat/cnf_20.cnf"
		"cnfs/sat/cnf_30.cnf"
		"cnfs/sat/cnf_50.cnf"
		"cnfs/unsat/cnf_12.cnf"
		"cnfs/unsat/cnf_20.cnf"
		"cnfs/unsat/cnf_30.cnf"
		"cnfs/unsat/cnf_42.cnf"))


(defun test-h (l)
	(cond ((null l) t)
		  (t (format t (car l)) (time (print (solve-cnf (car l)))) (format t "----------~%") (test-h (cdr l)))))

(defun test ()
	(load "parse_cnf.lsp")
	(load "hw4.lsp")
	(test-h L))