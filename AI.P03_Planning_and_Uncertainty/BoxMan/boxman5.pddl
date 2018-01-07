(define (problem boxman5)
 	(:domain boxman)
 	(:objects BOX1 BOX2 BOX3 -box P14 P15 P16 P21 P22 P23 P24 P25 P26 P31 P33 P34 P41 P42 P43 P44 P53 P54 P63 P64 -pos)
 
 	(:init 
		(Atman P41)
		(Atbox BOX1 P23)
		(Atbox BOX2 P34)
		(Atbox BOX3 P42)
		(Null P14)
		(Null P15)
		(Null P16)
		(Null P21)
		(Null P22)
		(Null P24)
		(Null P25)
		(Null P26)
		(Null P31)
		(Null P33)
		(Null P43)
		(Null P44)
		(Null P53)
		(Null P54)
		(Null P63)
		(Null P64)
		
		(Left P21 P31) (Right P31 P21)
		(Left P31 P41) (Right P41 P31)
		(Left P23 P33) (Right P33 P23)
		(Left P33 P43) (Right P43 P33)
		(Left P43 P53) (Right P53 P43)
		(Left P53 P63) (Right P63 P53)
		(Left P14 P24) (Right P24 P14)
		(Left P24 P34) (Right P34 P24)
		(Left P34 P44) (Right P44 P34)
		(Left P44 P54) (Right P54 P44)
		(Left P54 P64) (Right P64 P54)
		(Left P15 P25) (Right P25 P15)
		(Left P16 P26) (Right P26 P16)
		
		(Down P14 P15) (Up P15 P14)
		(Down P15 P16) (Up P16 P15)
		(Down P21 P22) (Up P22 P21)
		(Down P22 P23) (Up P23 P22)
		(Down P23 P24) (Up P24 P23)
		(Down P24 P25) (Up P25 P24)
		(Down P25 P26) (Up P26 P25)
		(Down P33 P34) (Up P34 P33)
		(Down P41 P42) (Up P42 P41)
		(Down P42 P43) (Up P43 P42)
		(Down P43 P44) (Up P44 P43)
		(Down P53 P54) (Up P54 P53)
		(Down P63 P64) (Up P64 P63)

		(Adj P21 P31) (Adj P31 P21)
		(Adj P31 P41) (Adj P41 P31)
		(Adj P23 P33) (Adj P33 P23)
		(Adj P33 P43) (Adj P43 P33)
		(Adj P43 P53) (Adj P53 P43)
		(Adj P53 P63) (Adj P63 P53)
		(Adj P14 P24) (Adj P24 P14)
		(Adj P24 P34) (Adj P34 P24)
		(Adj P34 P44) (Adj P44 P34)
		(Adj P44 P54) (Adj P54 P44)
		(Adj P54 P64) (Adj P64 P54)
		(Adj P15 P25) (Adj P25 P15)
		(Adj P16 P26) (Adj P26 P16)
		
		(Adj P14 P15) (Adj P15 P14)
		(Adj P15 P16) (Adj P16 P15)
		(Adj P21 P22) (Adj P22 P21)
		(Adj P22 P23) (Adj P23 P22)
		(Adj P23 P24) (Adj P24 P23)
		(Adj P24 P25) (Adj P25 P24)
		(Adj P25 P26) (Adj P26 P25)
		(Adj P33 P34) (Adj P34 P33)
		(Adj P41 P42) (Adj P42 P41)
		(Adj P42 P43) (Adj P43 P42)
		(Adj P43 P44) (Adj P44 P43)
		(Adj P53 P54) (Adj P54 P53)
		(Adj P63 P64) (Adj P64 P63)
	)
 	(:goal 
 		(and 
		(Atbox BOX1 P24)
		(Atbox BOX2 P34)
		(Atbox BOX3 P21)
		)
 	)
)