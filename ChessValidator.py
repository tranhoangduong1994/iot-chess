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
        return self.board.is_stalemate() or self.board.is_insufficient_material()

    def getBitboard(self):
        str(self.board).replace(" ", "").replace("\n", "").replace("k", "1").replace("q", "1").replace("b", "1").replace("n", "1").replace("r", "1").replace("p", "1").replace("K", "1").replace("Q", "1").replace("B", "1").replace("N", "1").replace("R", "1").replace("P", "1").replace(".", "0")[::-1]

    def getAttackedSquares(self, attackerSquareIdx):
        print("getAttackedSquares" + str(attackerSquareIdx));
        attackedSquares = list(self.board.attacks(attackerSquareIdx))
        attackedSquareSet = chess.SquareSet()
        for square in attackedSquares:
            attackedSquareSet.add(square)
        print("result: " + str(bin(attackedSquareSet)).replace("0b", "")[::-1]);
        return str(bin(attackedSquareSet)).replace("0b", "")[::-1]
