

"""
RULES OF THE GAME :                                                     |\n'
	      '| At the beginning, every playes has 9 pieces. The goal of the game 
	      '| is to leave your opponent with only two pieces or leave him       
	      '| without any possible moves. Table has 24 fields and it is possible|  
	      '| to move only to adjacent fields, if they are free. If one of the  |  
	      '| players make mill (three pieces in a row) then he can remove one  |  
	      '| opponent\'s piece by choosing one of the fields where opponent     |  
	      '| placed his piece. NOTE: it\'s not allowed to remove opponent piece |  
	      '| if it\'s already in a MILL, unless his all pieces are in MILLS,    |  
	      '| then you can remove any piece you want. You can remove piece in   |    
	      '| any phase of the game.                                            |   
	      '|                                                                   |   
	      '| PHASES OF THE GAME:                                               |    
	      '| 1. PHASE (INIT) -> in this phase both playes are intermittently   |   
	      '| placing pieces on free fields ( 0 - 23 on the table). When player +----
	      '| run out of pieces to place, then we are moving to the next stage  |\n'
	      '| of the game.                                                      |\n'
	      '| 2. PHASE (MOVE) -> in this phase players can move their pieces to |\n'
	      '| adjacent free fields. First, they input the piece they want to    |\n'
	      '| move, and then the field where they want to move that piece.      |\n'
	      '| 3. PHASE (FLY) -> if the player remained with only 3 pieces,      |\n'
	      '| then he has additional opportunity to move his pieces to ANY free |\n'
	      '| field on the table, filed doesn\'t has to be adjecent and jumping  |\n'
	      '| over pieces is allowed.                                           |\n'
	      '|                                                                   |\n'
	      '| When one of the players remained with only 2 pieces, or the player|\n'
	      '| can\'t perform any move because every adjacent field is taken, the |\n'
	      '| game is over. Right now, your enemy is computer, can you beat him?|\n'
	      '| You can choose if you want to play first. Yours pieces are marked |\n'
	      '| as W (white) and computers pieces are marked as B (black).        |\n'
	      '|                         GOOD LUCK!                                |\n'
	      '+---------------------------------------------------------------
"""


