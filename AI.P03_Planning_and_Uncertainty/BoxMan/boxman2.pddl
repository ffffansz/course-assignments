(define (problem boxman2)
 	(:domain boxman)
 	(:objects BOX1 BOX2 BOX3 BOX4 BOX5 -box P13 P14 P23 P24 P33 P34 P35 P36 P42 P43 P44 P45 P46 P52 P53 P54 P55 P56 P64 P65 P66 -pos)
 
 	(:init 
		(Atman P14)
		(Atbox BOX1 P55)
		(Atbox BOX2 P34)
		(Atbox BOX3 P35)
		(Atbox BOX4 P45)
		(Atbox BOX5 P23)
		(Null P13)
		(Null P24)
		(Null P33)
		(Null P36)
		(Null P42)
		(Null P43)
		(Null P44)
		(Null P46)
		(Null P52)
		(Null P53)
		(Null P54)
		(Null P56)
		(Null P64)
		(Null P65)
		(Null P66)
		(Left P42 P52) (Right P52 P42)
		(Left P13 P23) (Right P23 P13)
		(Left P23 P33) (Right P33 P23)
		(Left P33 P43) (Right P43 P33)
		(Left P43 P53) (Right P53 P43)
		(Left P14 P24) (Right P24 P14)
		(Left P24 P34) (Right P34 P24)
		(Left P34 P44) (Right P44 P34)
		(Left P44 P54) (Right P54 P44)
		(Left P54 P64) (Right P64 P54)
		(Left P35 P45) (Right P45 P35)
		(Left P45 P55) (Right P55 P45)
		(Left P55 P65) (Right P65 P55)
		(Left P36 P46) (Right P46 P36)
		(Left P46 P56) (Right P56 P46)
		(Left P56 P66) (Right P66 P56)
		(Down P13 P14) (Up P14 P13)
		(Down P23 P24) (Up P24 P23)
		(Down P33 P34) (Up P34 P33)
		(Down P34 P35) (Up P35 P34)
		(Down P35 P36) (Up P36 P35)
		(Down P42 P43) (Up P43 P42)
		(Down P43 P44) (Up P44 P43)
		(Down P44 P45) (Up P45 P44)
		(Down P45 P46) (Up P46 P45)
		(Down P52 P53) (Up P53 P52)
		(Down P53 P54) (Up P54 P53)
		(Down P54 P55) (Up P55 P54)
		(Down P55 P56) (Up P56 P55)
		(Down P64 P65) (Up P65 P64)
		(Down P65 P66) (Up P66 P65)
		
		(Adj P42 P52) (Adj P52 P42)
		(Adj P13 P23) (Adj P23 P13)
		(Adj P23 P33) (Adj P33 P23)
		(Adj P33 P43) (Adj P43 P33)
		(Adj P43 P53) (Adj P53 P43)
		(Adj P14 P24) (Adj P24 P14)
		(Adj P24 P34) (Adj P34 P24)
		(Adj P34 P44) (Adj P44 P34)
		(Adj P44 P54) (Adj P54 P44)
		(Adj P54 P64) (Adj P64 P54)
		(Adj P35 P45) (Adj P45 P35)
		(Adj P45 P55) (Adj P55 P45)
		(Adj P55 P65) (Adj P65 P55)
		(Adj P36 P46) (Adj P46 P36)
		(Adj P46 P56) (Adj P56 P46)
		(Adj P56 P66) (Adj P66 P56)
		(Adj P13 P14) (Adj P14 P13)
		(Adj P23 P24) (Adj P24 P23)
		(Adj P33 P34) (Adj P34 P33)
		(Adj P34 P35) (Adj P35 P34)
		(Adj P35 P36) (Adj P36 P35)
		(Adj P42 P43) (Adj P43 P42)
		(Adj P43 P44) (Adj P44 P43)
		(Adj P44 P45) (Adj P45 P44)
		(Adj P45 P46) (Adj P46 P45)
		(Adj P52 P53) (Adj P53 P52)
		(Adj P53 P54) (Adj P54 P53)
		(Adj P54 P55) (Adj P55 P54)
		(Adj P55 P56) (Adj P56 P55)
		(Adj P64 P65) (Adj P65 P64)
		(Adj P65 P66) (Adj P66 P65)
		
	)
 	(:goal 
 		(and 
		(Atbox BOX1 P44)
		(Atbox BOX2 P54)
		(Atbox BOX3 P33)
		(Atbox BOX4 P43)
		(Atbox BOX5 P53)
		)
 	)
)