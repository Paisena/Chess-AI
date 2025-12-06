Fork or clone your this chess project into a new GitHub repository.

Add support for FEN stringsLinks to an external site. to your game setup so that instead of the current way you are setting up your game board you are setting it up with a call similar to the following call.

FENtoBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

Your routine should be able to take just the board position portion of a FEN string, or the entire FEN string like so:

FENtoBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

(you can ignore the end for now)

This will allow you to quickly check that your castling, promotion and en passant code is working.

Chess AI Submission:
For writing the chess AI one major issue I ran into was setting up the _bitboard array for all the different moves for each piece. I had difficult time understanding how the bitboards work and when I first ran the code after setting it up, the game would not allow me tomove any pieces. Evenutally I figured out the that bitIndex and oppBitIndex had to be flipped due to the getCurrentPlayerNumber function returning a 0 for white instead of it being 1. I also ran into a lot of work when I tried to convert all my uint64_t functions into the Bitboard class. I had to add all the operators for all the situations that were in my code.

I was able to achive a depth of 4 at a realtivly fast speed but my AI is not the smartest one.
Video of AI in the Github repo