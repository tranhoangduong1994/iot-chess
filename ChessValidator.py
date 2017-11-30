import chess
class Validator():
    def init(self):
        self.board = chess.Board()

    def move(self, move):
        self.board.push(chess.Move.from_uci(move))

    def isMoveValid(self, move):
        return chess.Move.from_uci(move) in self.board.legal_moves
    
    def isGameOver(self):
        return self.board.is_game_over()

    def isDrawGame(self):
        return self.board.is_stalemate() or board.is_insufficient_material()

    def getAttackedSquares(self, attackerSquareIdx):
        attackedSquares = list(board.attacks(attackerSquareIdx))
        attackedSquareSet = chess.SquareSet()
        for square in attackedSquares:
            attackedSquareSet.add(square)
        return str(bin(attackedSquareSet)).replace("0b", "")[::-1]
