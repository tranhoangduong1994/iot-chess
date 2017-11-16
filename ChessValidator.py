import chess
class Validator():
    def init(self):
        self.board = chess.Board()

    def move(self, move):
        self.board.push(chess.Move.from_uci(move))

    def isMoveValid(self, move):
        return chess.Move.from_uci(move) in self.board.legal_moves
    
    def isGameOver(self):
        return self.board.is_game_over();

    def isDrawGame(self):
        return self.board.is_stalemate() or board.is_insufficient_material()

#    def legalMoves(self):
#        print "Legal moves are {}".format(self.board.legal_moves)
#        return self.board.legal_moves
