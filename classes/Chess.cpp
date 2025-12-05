#include "Chess.h"
#include <limits>
#include <cmath>



Chess::Chess()
{
    _grid = new Grid(8, 8);
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
    startGame();
    _moves = generateAllMoves();
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
    _moves = generateAllMoves();
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

std::vector<BitMove> Chess::generateAllMoves() 
{
    std::vector<BitMove> moves;
    uint64_t BitZero = 1ULL;

    moves.reserve(32);
    std::string state = stateString();

    uint64_t whiteKnights = 0LL;
    uint64_t whitePawns = 0LL;
    uint64_t whiteKing = 0LL;
    uint64_t whiteBishops = 0LL;
    uint64_t whiteQueen = 0LL;
    uint64_t blackKnights = 0LL;

    for (int i = 0; i < 64; i++)
    {
        if (state[i] == 'N') {
            whiteKnights |= BitZero << i;
        } else if (state[i] == 'P') {
            whitePawns |= BitZero << i;
        } else if (state[i] == 'K') {
            whiteKing |= BitZero << i;
        } else if (state[i] == 'n') {
            blackKnights |= BitZero << i;
        } else if (state[i] == 'B') {
            whiteBishops |= BitZero << i;
        } else if (state[i] == 'Q') {
            whiteQueen |= BitZero << i;
        }
    }

    uint64_t whiteOccupancy = whiteKnights | whitePawns | whiteKing | whiteBishops | whiteQueen;
    uint64_t blackOccupancy = blackKnights;
    std::cout <<"printing" << std::endl;
    printBitboard(~whiteOccupancy);
    generateKingMoves(moves, whiteKing, 1, whiteOccupancy);
    generateKnightMoves(moves, whiteKnights, whiteOccupancy);
    generatePawnMoveList(moves, whitePawns, ~whiteOccupancy, 0, 1);

    //generateKnightMoves(moves, blackKnights, blackOccupancy);

    return moves;
}

void Chess::generateKingMoves(std::vector<BitMove>& moves, uint64_t kingBitboard, int colorAsInt, uint64_t occupancy) {
    constexpr uint64_t NotAFile(0xFEFEFEFEFEFEFEFEULL);
    constexpr uint64_t NotHFile(0x7F7F7F7F7F7F7F7FULL);
    constexpr uint64_t Rank1(0x00000000000000FFULL);
    constexpr uint64_t Rank8(0xFF00000000000000ULL);

    int fromSquare = bitScanForward(kingBitboard);
    std::cout << "kingbitboard" << std::endl;
    printBitboard(kingBitboard);

    uint64_t kingMoves[8];

    kingMoves[0] = ((kingBitboard & NotAFile) << 1) & ~occupancy;
    printBitboard(kingMoves[0]);
    kingMoves[1] = ((kingBitboard & NotHFile) >> 1)  & ~occupancy;
    printBitboard(kingMoves[1]);
    kingMoves[2] = (((kingBitboard & (~Rank1))) >> 8)  & ~occupancy;
    printBitboard(kingMoves[2]);
    kingMoves[3] = (((kingBitboard & (~Rank8))) << 8)  & ~occupancy;
    printBitboard(kingMoves[3]);
    
    kingMoves[4] = ((kingBitboard & (NotAFile|(~Rank1))) >> 7) & ~occupancy;
    printBitboard(kingMoves[4]);
    kingMoves[5] = ((kingBitboard & (NotAFile|(~Rank8))) << 9) & ~occupancy;
    printBitboard(kingMoves[5]);
    kingMoves[6] = ((kingBitboard & (NotHFile|(~Rank1))) >> 9) & ~occupancy;
    printBitboard(kingMoves[6]);
    kingMoves[7] = ((kingBitboard & (NotHFile|(~Rank8))) << 7) & ~occupancy;
    printBitboard(kingMoves[7]);

    for (int i = 0; i < 8; i++)
    {
        int toSquare;
        if (bitScanForward(kingMoves[i])==0 && (fromSquare == 1 || fromSquare == 8 || fromSquare == 9)) {
            toSquare = bitScanForward(kingMoves[i]);
        }
        else if(bitScanForward(kingMoves[i]) != 0)
        {
            toSquare = bitScanForward(kingMoves[i]);
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
    if (bitboard == 0)
        return;
    printBitboard(bitboard);
    while (bitboard)
    {
        int toSquare = bitScanForward(bitboard);
        int fromSquare = toSquare - shift;
        moves.emplace_back(fromSquare, toSquare, Pawn);

        bitboard &= bitboard - 1;
    }
    
}

#define WHITE 1
#define BLACK -1

void Chess::generatePawnMoveList(std::vector<BitMove>& moves, const BitBoard pawns, const BitBoard emptySquares, const BitBoard enemyPieces, char color) {
    if (pawns == 0) {
        return;
    }

    constexpr uint64_t NotAFile(0xFEFEFEFEFEFEFEFEULL);
    constexpr uint64_t NotHFile(0x7F7F7F7F7F7F7F7FULL);
    constexpr uint64_t Rank3(0x0000000000FF0000ULL);
    constexpr uint64_t Rank6(0x0000FF0000000000ULL);

    BitBoard singleMoves = (color == WHITE) ? (pawns << 8) & emptySquares : (pawns >> 8) & emptySquares;

    BitBoard doubleMoves = (color == WHITE) ? ((singleMoves & Rank3) << 8) & emptySquares : ((singleMoves & Rank6 >> 8) & emptySquares);

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



void Chess::generateKnightMoves(std::vector<BitMove>& moves, uint64_t knightBoard, uint64_t occupancy) {
    while (knightBoard) {
        int fromSquare = bitScanForward(knightBoard);
        uint64_t moveBitboard = _knightBitboards[fromSquare];
        //printBitboard(moveBitboard);
        
        while (moveBitboard) {
            int toSquare = bitScanForward(moveBitboard);
            if (!(occupancy & (1ULL<<toSquare))) {
                moves.push_back(BitMove(fromSquare,toSquare, Knight));
            }
            moveBitboard &= moveBitboard -1;
        }

        knightBoard &= knightBoard - 1;
    }
}