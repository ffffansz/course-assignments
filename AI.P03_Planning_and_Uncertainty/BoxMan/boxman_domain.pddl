(define (domain boxman)
	(:requirements :strips :typing :equality :universal-preconditions :conditional-effects)
	(:types pos box)
	(:predicates   
  	    (Atman ?x - pos)
		(Atbox ?x - box ?y - pos)
		(Adj ?x ?y - pos)
		(Null ?x - pos)
		(Left ?x ?y - pos)
		(Right ?x ?y - pos)
		(Up ?x ?y - pos)
		(Down ?x ?y - pos)
		)
		
	(:action MOVE
             :parameters (?x ?y - pos)
             :precondition (and (Atman ?x) (Null ?y) (Adj ?x ?y) )
             :effect 	(and (Atman ?y) 
							(not (Atman ?x))
							(Null ?x)
							(not (Null ?y))	
							
						)
			)
			
	(:action PUSH
             :parameters (?x - box ?u ?v ?w - pos)
             :precondition (and (Atman ?u) (Atbox ?x ?v) (Adj ?u ?v) (Adj ?v ?w) (Null ?w))
             :effect 	(and 
							
								(when (and (Left ?u ?v) (Left ?v ?w) (Null ?w))
									(and (Atman ?v) 
										(Atbox ?x ?w) 
										(Null ?u) 
										(not (Atman ?u)) 
										(not (Atbox ?x ?v)) 
										(not (Null ?w))
									)
								)
							
							
								(when (and (Right ?u ?v) (Right ?v ?w) (Null ?w))
									(and (Atman ?v) 
										(Atbox ?x ?w) 
										(Null ?u) 
										(not (Atman ?u)) 
										(not (Atbox ?x ?v)) 
										(not (Null ?w))
									)
								)
							
						
								(when (and (Up ?u ?v) (Up ?v ?w) (Null ?w))
									(and (Atman ?v) 
										(Atbox ?x ?w) 
										(Null ?u) 
										(not (Atman ?u)) 
										(not (Atbox ?x ?v)) 
										(not (Null ?w))
									)
								)
							
							
								(when (and (Down ?u ?v) (Down ?v ?w) (Null ?w))
									(and (Atman ?v) 
										(Atbox ?x ?w) 
										(Null ?u) 
										(not (Atman ?u)) 
										(not (Atbox ?x ?v)) 
										(not (Null ?w))
									)
								)
							
						)
			)
)