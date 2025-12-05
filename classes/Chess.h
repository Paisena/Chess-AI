#pragma once

#include "Game.h"
#include "Grid.h"
#include "Bitboard.h"

constexpr int pieceSize = 80;
typedef uint64_t BitBoard;

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

class Chess : public Game
{
public:
    Chess();
    ~Chess();

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

    void printBitboard(uint64_t bitboard) {
        std::cout << "\n  a b c d e f g h\n";
        for (int rank = 7; rank >= 0; rank--) {
            std::cout << (rank + 1) << " ";
            for (int file = 0; file < 8; file++) {
                int square = rank * 8 + file;
                if (bitboard & (1ULL << square)) {
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
    std::vector<BitMove> generateAllMoves();
    void generateKingMoves(std::vector<BitMove>& moves, uint64_t kingBitboard, int colorAsInt, uint64_t occupancy);
    void addPawnBitboardMovesToList(std::vector<BitMove>& moves, const BitBoard bitboard, const int shift);
    void generatePawnMoveList(std::vector<BitMove>& moves, const BitBoard pawns, const BitBoard occupancy, const BitBoard enemyPieces, char color);
    BitBoard generateKnightMoveBitboard(int square);
    void generateKnightMoves(std::vector<BitMove>& moves, uint64_t knightBoard, uint64_t occupancy);
    inline int bitScanForward(uint64_t bb) const {
    #if defined(_MSC_VER) && !defined(__clang__)
            unsigned long index;
            _BitScanForward64(&index, bb);
            return index;
    #else
            return __builtin_ffsll(bb) - 1;
    #endif
    };



    Grid* _grid;
    BitBoard _knightBitboards[64];
    std::vector<BitMove> _moves;
};