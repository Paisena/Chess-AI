#pragma once

#include "Game.h"
#include "Grid.h"
#include "Bitboard.h"

constexpr int pieceSize = 80;
//typedef uint64_t BitBoard;

// enum ChessPiece
// {
//     NoPiece,
//     Pawn,
//     Knight,
//     Bishop,
//     Rook,
//     Queen,
//     King
// };

enum AllBitBoards
{
    WHITE_PAWNS,
    WHITE_KNIGHTS,
    WHITE_BISHOPS,
    WHITE_ROOKS,
    WHITE_QUEENS,
    WHITE_KING,
    WHITE_ALL_PIECES,
    BLACK_PAWNS,
    BLACK_KNIGHTS,
    BLACK_BISHOPS,
    BLACK_ROOKS,
    BLACK_QUEENS,
    BLACK_KING,
    BLACK_ALL_PIECES,
    OCCUPANCY,
    EMPTY_SQUARES,
    e_numBitboards
};

class Chess : public Game
{
public:
    Chess();
    ~Chess();

    int negInfinite = -1000000;
    int posInfinite = 1000000;

    void setUpBoard() override;

    bool canBitMoveFrom(Bit &bit, BitHolder &src) override;
    bool canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;
    bool actionForEmptyHolder(BitHolder &holder) override;
    void bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;

    void stopGame() override;

    Player *checkForWinner() override;
    bool checkForDraw() override;

    std::string initialStateString() override;
    std::string stateString() override;
    void setStateString(const std::string &s) override;

    Grid* getGrid() override { return _grid; }

    void printBitboard(BitBoard bitboard) {
        std::cout << "\n  a b c d e f g h\n";
        for (int rank = 7; rank >= 0; rank--) {
            std::cout << (rank + 1) << " ";
            for (int file = 0; file < 8; file++) {
                int square = rank * 8 + file;
                if (bitboard.getData() & (1ULL << square)) {
                    std::cout << "X ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << (rank + 1) << "\n";
            std::cout << std::flush;
        }
        std::cout << "  a b c d e f g h\n";
        std::cout << std::flush;
    }

private:
    Bit* PieceForPlayer(const int playerNumber, ChessPiece piece);
    Player* ownerAt(int x, int y) const;
    void FENtoBoard(const std::string& fen);
    char pieceNotation(int x, int y) const;
    std::vector<BitMove> generateAllMoves(const std::string& state, int player);
    void generateKingMoves(std::vector<BitMove>& moves, BitBoard kingBitboard, int colorAsInt, BitBoard occupancy);
    void addPawnBitboardMovesToList(std::vector<BitMove>& moves, const BitBoard bitboard, const int shift);
    void generatePawnMoveList(std::vector<BitMove>& moves, const BitBoard pawns, const BitBoard occupancy, const BitBoard enemyPieces, char color);
    BitBoard generateKnightMoveBitboard(int square);
    void generateKnightMoves(std::vector<BitMove>& moves, BitBoard knightBoard, BitBoard occupancy);

    int evaluateBoard(std::string state);
    void updateAI();
    int negamax(std::string& state, int depth, int alpha, int beta, int playerColor);

    void generateBishopMoves(std::vector<BitMove>& moves, BitBoard bishopBoard, BitBoard occupancy, BitBoard friendlies);
    void generateRookMoves(std::vector<BitMove>& moves, BitBoard rookBoard, BitBoard occupancy, BitBoard friendlies);
    void generateQueenMoves(std::vector<BitMove>& moves, BitBoard queenBoard, BitBoard occupancy, BitBoard friendlies);
    inline int bitScanForward(uint64_t bb) const {
    #if defined(_MSC_VER) && !defined(__clang__)
            unsigned long index;
            _BitScanForward64(&index, bb);
            return index;
    #else
            return __builtin_ffsll(bb) - 1;
    #endif
    };


    int _currentPlayer;
    Grid* _grid;
    BitBoard _knightBitboards[64];
    std::vector<BitMove> _moves;
    BitBoard _bitboards[e_numBitboards];
    int _bitboardLookup[128];
    int _countMoves = 0;
};