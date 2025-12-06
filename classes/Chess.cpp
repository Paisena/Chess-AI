#include "Chess.h"
#include <limits>
#include <cmath>
#include "MagicBitboards.h"



#define WHITE 0
#define BLACK -1

Chess::Chess()
{
    _grid = new Grid(8, 8);
    
    initMagicBitboards();

    for(int i = 0; i < 128; i++) {
        _bitboardLookup[i] = 0;
    }

    _bitboardLookup['P'] = WHITE_PAWNS;
    _bitboardLookup['N'] = WHITE_KNIGHTS;
    _bitboardLookup['B'] = WHITE_BISHOPS;
    _bitboardLookup['R'] = WHITE_ROOKS;
    _bitboardLookup['Q'] = WHITE_QUEENS;
    _bitboardLookup['K'] = WHITE_KING;
    _bitboardLookup['p'] = BLACK_PAWNS;
    _bitboardLookup['n'] = BLACK_KNIGHTS;
    _bitboardLookup['b'] = BLACK_BISHOPS;
    _bitboardLookup['r'] = BLACK_ROOKS;
    _bitboardLookup['q'] = BLACK_QUEENS;
    _bitboardLookup['k'] = BLACK_KING;
    _bitboardLookup['0'] = EMPTY_SQUARES;

    for (int i = 0; i < 64; i++) {
        _knightBitboards[i] = generateKnightMoveBitboard(i);
    }


}

Chess::~Chess()
{
    delete _grid;
}

char Chess::pieceNotation(int x, int y) const
{
    const char *wpieces = { "0PNBRQK" };
    const char *bpieces = { "0pnbrqk" };
    Bit *bit = _grid->getSquare(x, y)->bit();
    char notation = '0';
    if (bit) {
        notation = bit->gameTag() < 128 ? wpieces[bit->gameTag()] : bpieces[bit->gameTag()-128];
    }
    return notation;
}

Bit* Chess::PieceForPlayer(const int playerNumber, ChessPiece piece)
{
    const char* pieces[] = { "pawn.png", "knight.png", "bishop.png", "rook.png", "queen.png", "king.png" };

    Bit* bit = new Bit();
    // should possibly be cached from player class?
    const char* pieceName = pieces[piece - 1];
    std::string spritePath = std::string("") + (playerNumber == 0 ? "w_" : "b_") + pieceName;
    bit->LoadTextureFromFile(spritePath.c_str());
    bit->setOwner(getPlayerAt(playerNumber));
    bit->setSize(pieceSize, pieceSize);

    return bit;
}

void Chess::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 8;
    _gameOptions.rowY = 8;

    _grid->initializeChessSquares(pieceSize, "boardsquare.png");
    FENtoBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    _currentPlayer = 0;
    _moves = generateAllMoves(stateString(), _currentPlayer);

    if (gameHasAI()) {
        setAIPlayer(AI_PLAYER);
    }
    startGame();
}



void Chess::FENtoBoard(const std::string& fen) {
    // convert a FEN string to a board
    // FEN is a space delimited string with 6 fields
    // 1: piece placement (from white's perspective)
    // NOT PART OF THIS ASSIGNMENT BUT OTHER THINGS THAT CAN BE IN A FEN STRING
    // ARE BELOW
    // 2: active color (W or B)
    // 3: castling availability (KQkq or -)
    // 4: en passant target square (in algebraic notation, or -)
    // 5: halfmove clock (number of halfmoves since the last capture or pawn advance)
    
    int x = 0;
    int y = 7;
    bool boardReading = true;

    std::string digits = "12345678";

    for (int i = 0; i < fen.length(); i++)
    {
        if(fen[i] == ' ')
        {
            boardReading = false;
        }
        if(fen[i] == '/')
        {
            y--;
            x = -1;
        }

        if(!boardReading)
        {
            continue;
        }
        if(fen[i] == '1' || fen[i] == '2' ||fen[i] == '3' ||fen[i] == '4' || fen[i] == '5' || fen[i] == '6' || fen[i] == '7' || fen[i] == '8')
        {
            int num = (int)fen[i];
            x += num;
        }

        if(fen[i] == 'r')
        {
            Bit* piece = PieceForPlayer(1, Rook);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(Rook+128);
        }
        if(fen[i] == 'R')
        {
            Bit* piece = PieceForPlayer(0, Rook);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(Rook);
        }
        if(fen[i] == 'n')
        {
            Bit* piece = PieceForPlayer(1, Knight);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(Knight+128);
        }
        if(fen[i] == 'N')
        {
            Bit* piece = PieceForPlayer(0, Knight);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(Knight);
        }

        if(fen[i] == 'b')
        {
            Bit* piece = PieceForPlayer(1, Bishop);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(Bishop+128);
        }
        if(fen[i] == 'B')
        {
            Bit* piece = PieceForPlayer(0, Bishop);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(Bishop);
        }
        if(fen[i] == 'q')
        {
            Bit* piece = PieceForPlayer(1, Queen);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(Queen+128);
        }
        if(fen[i] == 'Q')
        {
            Bit* piece = PieceForPlayer(0, Queen);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(Queen);
        }
        if(fen[i] == 'k')
        {
            Bit* piece = PieceForPlayer(1, King);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(King+128);
        }
        if(fen[i] == 'K')
        {
            Bit* piece = PieceForPlayer(0, King);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(King);
        }
        if(fen[i] == 'p')
        {
            Bit* piece = PieceForPlayer(1, Pawn);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(Pawn+128);
        }
        if(fen[i] == 'P')
        {
            Bit* piece = PieceForPlayer(0, Pawn);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
            piece->setGameTag(Pawn);
        }
        x++;
    }
}

bool Chess::actionForEmptyHolder(BitHolder &holder)
{
    return false;
}

    

bool Chess::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    // need to implement friendly/unfriendly in bit so for now this hack
    int currentPlayer = getCurrentPlayer()->playerNumber() * 128;
    int pieceColor = bit.gameTag() & 128;
    if (pieceColor != currentPlayer) return false;

    bool ret = false;
    ChessSquare* square = (ChessSquare *) &src;
    if (square) {
        int squareIndex = square->getSquareIndex();
        for (auto move : _moves) {
            if (move.from == squareIndex) {
                ret = true;
                auto dest = _grid->getSquareByIndex(move.to);
                dest->setHighlighted(true);
            }
        } 
    }
    return ret;
}

bool Chess::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    ChessSquare* square = (ChessSquare *) &dst;
    ChessSquare* squareSrc = (ChessSquare *) &src;

    if (square) {
        int squareIndex = square->getSquareIndex();
        for (auto move : _moves) {
            if (move.to == squareIndex && move.from == squareSrc->getSquareIndex()) {
                return true;
            }
        }
    }
    return false;
}

void Chess::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    endTurn();
    _moves = generateAllMoves(stateString(), _currentPlayer);
}

void Chess::stopGame()
{
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

Player* Chess::ownerAt(int x, int y) const
{
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return nullptr;
    }

    auto square = _grid->getSquare(x, y);
    if (!square || !square->bit()) {
        return nullptr;
    }
    return square->bit()->getOwner();
}

Player* Chess::checkForWinner()
{
    return nullptr;
}

bool Chess::checkForDraw()
{
    return false;
}

std::string Chess::initialStateString()
{
    return stateString();
}

std::string Chess::stateString()
{
    std::string s;
    s.reserve(64);
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
            s += pieceNotation( x, y );
        }
    );
    return s;}

void Chess::setStateString(const std::string &s)
{
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        int index = y * 8 + x;
        char playerNumber = s[index] - '0';
        if (playerNumber) {
            square->setBit(PieceForPlayer(playerNumber - 1, Pawn));
        } else {
            square->setBit(nullptr);
        }
    });
}

std::vector<BitMove> Chess::generateAllMoves(const std::string& state, int player) 
{
    std::vector<BitMove> moves;
    uint64_t BitZero = 1ULL;

    moves.reserve(32);
    //std::string state = stateString();

    for (int i = 0; i < e_numBitboards; i++) {
        _bitboards[i] = 0;
    }

    for (int i = 0; i < 64; i++) {
        int bitIndex = _bitboardLookup[state[i]];
        _bitboards[bitIndex] |= 1ULL << i;
        
    }

    _bitboards[WHITE_ALL_PIECES] = _bitboards[WHITE_PAWNS] | 
                                        _bitboards[WHITE_KNIGHTS] | 
                                        _bitboards[WHITE_BISHOPS] | 
                                        _bitboards[WHITE_ROOKS] | 
                                        _bitboards[WHITE_QUEENS] | 
                                        _bitboards[WHITE_KING];

    _bitboards[BLACK_ALL_PIECES] = _bitboards[BLACK_PAWNS] |
                                        _bitboards[BLACK_KNIGHTS] |
                                        _bitboards[BLACK_BISHOPS] |
                                        _bitboards[BLACK_ROOKS] |
                                        _bitboards[BLACK_QUEENS] |
                                        _bitboards[BLACK_KING];

    _bitboards[OCCUPANCY] = _bitboards[WHITE_ALL_PIECES] | _bitboards[BLACK_ALL_PIECES];

    int oppBitIndex = getCurrentPlayer()->playerNumber() == 1 ? WHITE_PAWNS : BLACK_PAWNS;
    int bitIndex = getCurrentPlayer()->playerNumber() == 1 ? BLACK_PAWNS : WHITE_PAWNS;

    
    generateKingMoves(moves, _bitboards[WHITE_KING + bitIndex], getCurrentPlayer()->playerNumber(), _bitboards[WHITE_ALL_PIECES + bitIndex]);
    generateKnightMoves(moves, _bitboards[WHITE_KNIGHTS + bitIndex], _bitboards[WHITE_ALL_PIECES + bitIndex]);
    generatePawnMoveList(moves, _bitboards[WHITE_PAWNS + bitIndex], _bitboards[EMPTY_SQUARES], _bitboards[WHITE_ALL_PIECES + oppBitIndex], getCurrentPlayer()->playerNumber());
    generateBishopMoves(moves, _bitboards[WHITE_BISHOPS + bitIndex], _bitboards[OCCUPANCY], _bitboards[WHITE_ALL_PIECES + bitIndex]);
    generateRookMoves(moves, _bitboards[WHITE_ROOKS + bitIndex], _bitboards[OCCUPANCY], _bitboards[WHITE_ALL_PIECES + bitIndex]);
    generateQueenMoves(moves, _bitboards[WHITE_QUEENS + bitIndex], _bitboards[OCCUPANCY], _bitboards[WHITE_ALL_PIECES + bitIndex]);

    return moves;
}

void Chess::generateKingMoves(std::vector<BitMove>& moves, BitBoard kingBitboard, int colorAsInt, BitBoard occupancy) {
    constexpr uint64_t NotAFile(0xFEFEFEFEFEFEFEFEULL);
    constexpr uint64_t NotHFile(0x7F7F7F7F7F7F7F7FULL);
    constexpr uint64_t Rank1(0x00000000000000FFULL);
    constexpr uint64_t Rank8(0xFF00000000000000ULL);

    int fromSquare = bitScanForward(kingBitboard.getData());

    BitBoard kingMoves[8];

    kingMoves[0] = ((kingBitboard & NotAFile) << 1) & ~occupancy;
    kingMoves[1] = ((kingBitboard & NotHFile) >> 1)  & ~occupancy;
    kingMoves[2] = (((kingBitboard & (~Rank1))) >> 8)  & ~occupancy;
    kingMoves[3] = (((kingBitboard & (~Rank8))) << 8)  & ~occupancy;
    
    kingMoves[4] = ((kingBitboard & (NotAFile|(~Rank1))) >> 7) & ~occupancy;
    kingMoves[5] = ((kingBitboard & (NotAFile|(~Rank8))) << 9) & ~occupancy;
    kingMoves[6] = ((kingBitboard & (NotHFile|(~Rank1))) >> 9) & ~occupancy;
    kingMoves[7] = ((kingBitboard & (NotHFile|(~Rank8))) << 7) & ~occupancy;

    for (int i = 0; i < 8; i++)
    {
        int toSquare;
        if (bitScanForward(kingMoves[i].getData())==0 && (fromSquare == 1 || fromSquare == 8 || fromSquare == 9)) {
            toSquare = bitScanForward(kingMoves[i].getData());
        }
        else if(bitScanForward(kingMoves[i].getData()) != 0)
        {
            toSquare = bitScanForward(kingMoves[i].getData());
        }
        else {
            continue;
        }
        moves.emplace_back(fromSquare, toSquare, King);
    }
    return; 
}

BitBoard Chess::generateKnightMoveBitboard(int square) {
    BitBoard bitboard = 0ULL;
    int rank = square / 8;
    int file = square % 8;

    std::pair<int, int> knightOffsets[] = {
        {2,1}, {2, -1}, {-2,1} ,{-2,-1},
        {1,2}, { 1,-2}, {-1,2}, {-1,-2},
    };

    constexpr uint64_t onebit = 1;
    for (auto [dr, df] : knightOffsets) {
        int r = rank + dr, f = file + df;
        if (r >= 0 && r < 8 && f >= 0 && f < 8) {
            bitboard |= onebit << (r * 8 + f);
        }
    }

    return bitboard;
}

void Chess::addPawnBitboardMovesToList(std::vector<BitMove>& moves, BitBoard bitboard, const int shift) {
    if (bitboard.getData() == 0)
        return;
    while (bitboard.getData())
    {
        int toSquare = bitScanForward(bitboard.getData());
        int fromSquare = toSquare - shift;
        moves.emplace_back(fromSquare, toSquare, Pawn);

        bitboard &= bitboard - 1;
    }
    
}


void Chess::generatePawnMoveList(std::vector<BitMove>& moves, const BitBoard pawns, const BitBoard emptySquares, const BitBoard enemyPieces, char color) {
    if (pawns.getData() == 0) {
        return;
    }

    constexpr uint64_t NotAFile(0xFEFEFEFEFEFEFEFEULL);
    constexpr uint64_t NotHFile(0x7F7F7F7F7F7F7F7FULL);
    constexpr uint64_t Rank3(0x0000000000FF0000ULL);
    constexpr uint64_t Rank6(0x0000FF0000000000ULL);

    BitBoard singleMoves = (color == WHITE) ? (pawns << 8) & emptySquares : (pawns >> 8) & emptySquares;

    BitBoard doubleMoves = (color == WHITE) ? ((singleMoves & Rank3) << 8) & emptySquares : ((singleMoves & Rank6) >> 8) & emptySquares;
    
    BitBoard capturesLeft = (color == WHITE) ? ((pawns & NotAFile) << 7) & enemyPieces : ((pawns & NotAFile) >> 9) & enemyPieces;
    BitBoard capturesRight = (color == WHITE) ? ((pawns & NotHFile) << 9) & enemyPieces : ((pawns & NotHFile) >> 7) & enemyPieces;

    int shiftForward = (color == WHITE) ? 8 : -8;
    int doubleShift = (color == WHITE) ? 16 : -16;
    int captureLeftShift = (color == WHITE) ? 7 : -9;
    int captureRightShift = (color == WHITE) ? 9 : -7;

    addPawnBitboardMovesToList(moves, singleMoves, shiftForward);

    addPawnBitboardMovesToList(moves, doubleMoves, doubleShift);

    addPawnBitboardMovesToList(moves, capturesLeft, captureLeftShift);
    addPawnBitboardMovesToList(moves, capturesRight, captureRightShift);
}



void Chess::generateKnightMoves(std::vector<BitMove>& moves, BitBoard knightBoard, BitBoard occupancy) {
    while (knightBoard.getData()) {
        int fromSquare = bitScanForward(knightBoard.getData());
        BitBoard moveBitboard = _knightBitboards[fromSquare];

        while (moveBitboard.getData()) {
            int toSquare = bitScanForward(moveBitboard.getData());
            if (!(occupancy & (1ULL<<toSquare))) {
                moves.push_back(BitMove(fromSquare,toSquare, Knight));
            }
            moveBitboard &= moveBitboard -1;
        }

        knightBoard &= knightBoard - 1;
    }
}

void Chess::generateBishopMoves(std::vector<BitMove>& moves, BitBoard bishopBoard, BitBoard occupancy, BitBoard friendlies) {
    while (bishopBoard.getData()) {
        int fromSquare = bitScanForward(bishopBoard.getData());
        BitBoard moveBitboard = BitBoard(getBishopAttacks(fromSquare, occupancy.getData()) & ~friendlies.getData());
        
        while (moveBitboard.getData()) {
            int toSquare = bitScanForward(moveBitboard.getData());
            moves.push_back(BitMove(fromSquare,toSquare, Bishop));
            moveBitboard &= moveBitboard -1;
        }

        bishopBoard &= bishopBoard - 1;
    }
}

void Chess::generateRookMoves(std::vector<BitMove>& moves, BitBoard rookBoard, BitBoard occupancy, BitBoard friendlies) {
    while (rookBoard.getData()) {
        int fromSquare = bitScanForward(rookBoard.getData());
        BitBoard moveBitboard = BitBoard(getRookAttacks(fromSquare, occupancy.getData()) & ~friendlies.getData());
        
        while (moveBitboard.getData()) {
            int toSquare = bitScanForward(moveBitboard.getData());
            moves.push_back(BitMove(fromSquare,toSquare, Rook));
            moveBitboard &= moveBitboard -1;
        }

        rookBoard &= rookBoard - 1;
    }
}

void Chess::generateQueenMoves(std::vector<BitMove>& moves, BitBoard queenBoard, BitBoard occupancy, BitBoard friendlies) {
    while (queenBoard.getData()) {
        int fromSquare = bitScanForward(queenBoard.getData());
        BitBoard moveBitboard = BitBoard(getQueenAttacks(fromSquare, occupancy.getData()) & ~friendlies.getData());
        
        while (moveBitboard.getData()) {
            int toSquare = bitScanForward(moveBitboard.getData());
            moves.push_back(BitMove(fromSquare,toSquare, Queen));
            moveBitboard &= moveBitboard -1;
        }

        queenBoard &= queenBoard - 1;
    }
}

void Chess::updateAI() 
{
    int bestVal = negInfinite;
    BitMove bestMove;
    std::string state = stateString();
    _countMoves = 0;

    for(auto move : _moves) {
        char boardSave = state[move.to];
        char pieceMoving = state[move.from];

        state[move.to] = pieceMoving;
        state[move.from] = '0';

        int moveVal = -negamax(state, 5, negInfinite, posInfinite, 1);
        // Undo the move
        state[move.from] = pieceMoving;
        state[move.to] = boardSave;
        // If the value of the current move is more than the best value, update best
        if (moveVal > bestVal) {
            bestMove = move;
            bestVal = moveVal;
        }

    };
    if (bestVal != negInfinite) {
        std::cout << "Moves checked: " << _countMoves << std::endl;

        int srcSquare = bestMove.from;
        int dstSquare = bestMove.to;
        BitHolder& src = getHolderAt(srcSquare&7, srcSquare/8);
        BitHolder& dst = getHolderAt(dstSquare&7, dstSquare/8);
        Bit *bit = src.bit();
        dst.dropBitAtPoint(bit, ImVec2(0,0));
        src.setBit(nullptr);
        bitMovedFromTo(*bit, src, dst);
    }

}

static std::map<char, int> evaluateScores = {
    {'P', 100}, {'N', 300}, {'B', 400}, {'R', 500}, {'Q', 900}, {'K', 2000},
    {'p', -100}, {'n', -300}, {'b', -400}, {'r', -500}, {'q', -900}, {'k', -2000},
    {'0', 0}
};

int Chess::negamax(std::string& state, int depth, int alpha, int beta, int playerColor) 
{   
    _countMoves++;
    if (depth == 0) {
        return evaluateBoard(state) * playerColor;
    }
    
    auto newMoves = generateAllMoves(state, playerColor);
    
    int bestVal = negInfinite; // Min value

    for(auto move : newMoves) {
        int srcSquare = move.from;
        int dstSquare = move.to;

        char oldDst = state[dstSquare];
        char srcPce = state[srcSquare];
        state[dstSquare] = srcPce;
        state[srcSquare] = '0';

        bestVal = std::max(bestVal, -negamax(state, depth - 1, -beta, -alpha, -playerColor));
        // Undo the move
        state[dstSquare] = oldDst;
        state[srcSquare] = srcPce;

        alpha = std::max(alpha, bestVal);
        if (alpha >= beta) {
            break; // Beta cut-off
        }
    }
    return bestVal;
}

int Chess::evaluateBoard(const std::string state) {
    int score = 0;
    for (char ch: state)
    {
        score += evaluateScores[ch];
    }

    return score;
}