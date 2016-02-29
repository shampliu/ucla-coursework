(define (diff-keywords x y)
  (or
    (xor (equal? x 'quote) (equal? y 'quote))
    (xor (equal? x 'let) (equal? y 'let))
    (xor (equal? x 'if) (equal? y 'if))
    (xor (equal? x 'lambda) (equal? y 'lambda))
  )
)

(define (check-vars x y)
  (cond
    ((and (equal? x '()) (equal? y '())) #t)
    ((equal? (length x) (length y))
      (cond
        ((equal? (car (car x)) (car (car y)))
          (check-vars (cdr x) (cdr y))
        )
        (else #f)
      )
    )
    (else #f)
  )
)

(define (cmp-quote x y)
  (cond
    ((equal? (cdr x) (cdr y)) (quote x))
    (else `(if TCP ,x ,y))
  )
)

(define (cmp-let x y)
  (cond
    ((check-vars (car (cdr x)) (car (cdr y)))
      (cons 'let 
        (compare-expr (cdr x) (cdr y))
      )
    )
    (else `(if TCP ,x ,y))
  )
)

(define (cmp-lambda x y)
  (cond
    ((equal? (car (cdr x)) (car (cdr y)))
      (cons (compare-expr (car x) (car y))
        (compare-expr (cdr x) (cdr y))
      )
    )
    (else `(if TCP ,x ,y))
  )
)

(define (compare-expr x y)
  (cond
    ((equal? x y) x)
    ((and (eq? x #t) (eq? y #f)) (quote TCP))
    ((and (eq? x #f) (eq? y #t)) (quote (not TCP)))
    ((and (list? x) (list? y))
      (cond
        ((equal? (length x) (length y))
          (cond
            ((diff-keywords (car x) (car y))
              `(if TCP ,x ,y)
            )
            ((and (equal? (car x) 'quote) (equal? (car y) 'quote))
              (cmp-quote x y)
            )

            ((and (equal? (car x) 'let) (equal? (car y) 'let))
              (cmp-let x y)
            )

            ((and (equal? (car x) 'lambda) (equal? (car y) 'lambda))
              (cmp-lambda x y)
            )
            (else
              (cons (compare-expr (car x) (car y))
                (compare-expr (cdr x) (cdr y))
              )
            )
          )
        )
        (else `(if TCP ,x ,y))
      )
    )
    (else `(if TCP ,x ,y))
  )
)

(define (test-compare-expr x y)
  (let ((expr (compare-expr x y)))
    (and
      (equal? 

        (eval x)
        (eval 
          (cons 'let 
            (cons '((TCP #t))
              (list expr)
            )
          )
        )
      )
      (equal? 
        (eval y)
        (eval 
          (cons 'let 
            (cons '((TCP #f))
              (list expr)
            )
          )
        )
      )
    )
  )
)


(define test-x 
 '(let ((a 4) (b 3))
    (list
      (- 5 2)
      '(#t #t #t)

      (cons (+ b a) (list 1 3 4))
      (if b a #f)

      (quote (#f a))
      (quote (a))

      (let ((x 10) (y 12)) (+ x y))
      (let ((r 1) (s 2)) (- r s))

      ((lambda (c d) (+ c d)) a 4)
      ((lambda (e f) (- e f)) b 4)
     

    )
  )
)

(define test-y
 '(let ((a 4) (b 2))
    (list
      (+ 5 2)
      '(#t #t #f)

      (list (- b a) (list 1 2 3))
      (if b a #f)

      (quote (#f a))
      (quote (a b))

      (let ((x 10) (y 12)) (- x y))
      (let ((r 1) (q 2)) (- r q))

      ((lambda (c d) (+ c d)) b 4)
      ((lambda (e g) (- e g)) a 4)
    )
  )
)


