import chess
class Validator():
    def init(self):
        self.board = chess.Board()
#    def legalMoves(self):
#        print "Legal moves are {}".format(self.board.legal_moves)
#        return self.board.legal_moves

    def isMoveValid(self, move):
        return chess.Move.from_uci(move) in self.board.legal_moves

    def move(self, move):
        self.board.push(chess.Move.from_uci(move))
