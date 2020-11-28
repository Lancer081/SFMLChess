#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <string.h>

using namespace sf;

enum Pieces { P, N, B, R, Q, K, p, n, b, r, q, k };

enum Side { white, black, both };

enum Square {
	a8, b8, c8, d8, e8, f8, g8, h8,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a1, b1, c1, d1, e1, f1, g1, h1, noSqr
};

enum CastlingRights { wk = 1, wq = 2, bk = 4, bq = 8 };

// bishop relevant occupancy bit count for every square on board
const int bishopRelevantBits[64] = {
	6, 5, 5, 5, 5, 5, 5, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 5, 5, 5, 5, 5, 5, 6
};

// rook relevant occupancy bit count for every square on board
const int rookRelevantBits[64] = {
	12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12
};

// rook magic numbers
uint64_t rookMagicNumbers[64] = {
	0x8a80104000800020ULL,
	0x140002000100040ULL,
	0x2801880a0017001ULL,
	0x100081001000420ULL,
	0x200020010080420ULL,
	0x3001c0002010008ULL,
	0x8480008002000100ULL,
	0x2080088004402900ULL,
	0x800098204000ULL,
	0x2024401000200040ULL,
	0x100802000801000ULL,
	0x120800800801000ULL,
	0x208808088000400ULL,
	0x2802200800400ULL,
	0x2200800100020080ULL,
	0x801000060821100ULL,
	0x80044006422000ULL,
	0x100808020004000ULL,
	0x12108a0010204200ULL,
	0x140848010000802ULL,
	0x481828014002800ULL,
	0x8094004002004100ULL,
	0x4010040010010802ULL,
	0x20008806104ULL,
	0x100400080208000ULL,
	0x2040002120081000ULL,
	0x21200680100081ULL,
	0x20100080080080ULL,
	0x2000a00200410ULL,
	0x20080800400ULL,
	0x80088400100102ULL,
	0x80004600042881ULL,
	0x4040008040800020ULL,
	0x440003000200801ULL,
	0x4200011004500ULL,
	0x188020010100100ULL,
	0x14800401802800ULL,
	0x2080040080800200ULL,
	0x124080204001001ULL,
	0x200046502000484ULL,
	0x480400080088020ULL,
	0x1000422010034000ULL,
	0x30200100110040ULL,
	0x100021010009ULL,
	0x2002080100110004ULL,
	0x202008004008002ULL,
	0x20020004010100ULL,
	0x2048440040820001ULL,
	0x101002200408200ULL,
	0x40802000401080ULL,
	0x4008142004410100ULL,
	0x2060820c0120200ULL,
	0x1001004080100ULL,
	0x20c020080040080ULL,
	0x2935610830022400ULL,
	0x44440041009200ULL,
	0x280001040802101ULL,
	0x2100190040002085ULL,
	0x80c0084100102001ULL,
	0x4024081001000421ULL,
	0x20030a0244872ULL,
	0x12001008414402ULL,
	0x2006104900a0804ULL,
	0x1004081002402ULL
};

// bishop magic numbers
uint64_t bishopMagicNumbers[64] = {
	0x40040844404084ULL,
	0x2004208a004208ULL,
	0x10190041080202ULL,
	0x108060845042010ULL,
	0x581104180800210ULL,
	0x2112080446200010ULL,
	0x1080820820060210ULL,
	0x3c0808410220200ULL,
	0x4050404440404ULL,
	0x21001420088ULL,
	0x24d0080801082102ULL,
	0x1020a0a020400ULL,
	0x40308200402ULL,
	0x4011002100800ULL,
	0x401484104104005ULL,
	0x801010402020200ULL,
	0x400210c3880100ULL,
	0x404022024108200ULL,
	0x810018200204102ULL,
	0x4002801a02003ULL,
	0x85040820080400ULL,
	0x810102c808880400ULL,
	0xe900410884800ULL,
	0x8002020480840102ULL,
	0x220200865090201ULL,
	0x2010100a02021202ULL,
	0x152048408022401ULL,
	0x20080002081110ULL,
	0x4001001021004000ULL,
	0x800040400a011002ULL,
	0xe4004081011002ULL,
	0x1c004001012080ULL,
	0x8004200962a00220ULL,
	0x8422100208500202ULL,
	0x2000402200300c08ULL,
	0x8646020080080080ULL,
	0x80020a0200100808ULL,
	0x2010004880111000ULL,
	0x623000a080011400ULL,
	0x42008c0340209202ULL,
	0x209188240001000ULL,
	0x400408a884001800ULL,
	0x110400a6080400ULL,
	0x1840060a44020800ULL,
	0x90080104000041ULL,
	0x201011000808101ULL,
	0x1a2208080504f080ULL,
	0x8012020600211212ULL,
	0x500861011240000ULL,
	0x180806108200800ULL,
	0x4000020e01040044ULL,
	0x300000261044000aULL,
	0x802241102020002ULL,
	0x20906061210001ULL,
	0x5a84841004010310ULL,
	0x4010801011c04ULL,
	0xa010109502200ULL,
	0x4a02012000ULL,
	0x500201010098b028ULL,
	0x8040002811040900ULL,
	0x28000010020204ULL,
	0x6000020202d0240ULL,
	0x8918844842082200ULL,
	0x4010011029020020ULL
};

// castling rights update constants
const int castlingRights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

const uint64_t notAFile = 0xfefefefefefefefe;
const uint64_t notHFile = 0x7f7f7f7f7f7f7f7f;
const uint64_t notGHFile = 0x3f3f3f3f3f3f3f3f;
const uint64_t notABFile = 0xfcfcfcfcfcfcfcfc;

const int SQUARE_SIZE = 50.0f;
const int SCREEN_SIZE = SQUARE_SIZE * 8;

class Move {
public:
	int from;
	int to;
	int piece;
	int promotedPiece;

	bool isEnpassant;
	bool isCastling;
	bool isCapture;
	bool isDoublePawnPush;

	Move()
	{
		from = 0;
		to = 0;
		piece = 0;
		isCapture = false;
		isEnpassant = false;
		isDoublePawnPush = false;
		isCastling = false;
	}

	Move(int from, int to, int piece, int promotedPiece = 0, bool isEnpassant = false,
		 bool isCapture = false, bool isDoublePawnPush = false, bool isCastling = false)
	{
		this->from = from;
		this->to = to;
		this->piece = piece;
		this->isCapture = isCapture;
		this->promotedPiece = promotedPiece;
		this->isEnpassant = isEnpassant;
		this->isDoublePawnPush = isDoublePawnPush;
		this->isCastling = isCastling;
	}
};

RectangleShape squares[64];

Sprite pieces[64];

uint64_t boards[12];
uint64_t occupancy[3];

bool whiteToMove = true;

// the enpassant square
int enpassant = noSqr;

// represents our castling rights using the bits of an integer
int castling = 0;

const char* squareToCoords[] = {
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

Texture wPawnTex, wKnightTex, wBishopTex, wRookTex, wQueenTex, wKingTex;
Texture bPawnTex, bKnightTex, bBishopTex, bRookTex, bQueenTex, bKingTex;

static inline bool getBit(uint64_t bitboard, int bit) {	return bitboard & (1ULL << bit); }
static inline void setBit(uint64_t* bitboard, int bit) { *bitboard |= (1ULL << bit); }
static inline void popBit(uint64_t* bitboard, int bit) { *bitboard &= ~(1ULL << bit); }

int countSetBits(uint64_t bitboard)
{
	int count = 0;
	while (bitboard)
	{
		bitboard &= bitboard - 1ULL;
		count++;
	}
	return count;
}

// gets the least significant first bit index
int getLSBIndex(uint64_t bitboard)
{
	int count = 0;
	while ((bitboard & 1ULL) == 0)
	{
		count++;
		bitboard >>= 1ULL;
	}
	return count;
}

void initBitboards()
{
	boards[P] = 0b0000000011111111000000000000000000000000000000000000000000000000;
	boards[p] = 0b0000000000000000000000000000000000000000000000001111111100000000;
	boards[R] = 0b1000000100000000000000000000000000000000000000000000000000000000;
	boards[r] = 0b0000000000000000000000000000000000000000000000000000000010000001;
	boards[N] = 0b0100001000000000000000000000000000000000000000000000000000000000;
	boards[n] = 0b0000000000000000000000000000000000000000000000000000000001000010;
	boards[B] = 0b0010010000000000000000000000000000000000000000000000000000000000;
	boards[b] = 0b0000000000000000000000000000000000000000000000000000000000100100;
	boards[Q] = 0b0000100000000000000000000000000000000000000000000000000000000000;
	boards[q] = 0b0000000000000000000000000000000000000000000000000000000000001000;
	boards[K] = 0b0001000000000000000000000000000000000000000000000000000000000000;
	boards[k] = 0b0000000000000000000000000000000000000000000000000000000000010000;
	occupancy[white] = 0b000000000000000000000000000000000000000001111111111111111;
	occupancy[black] = 0b111111111111111100000000000000000000000000000000000000000;
	occupancy[both] = occupancy[white] | occupancy[black];
}

void printBitboard(uint64_t bitboard)
{
	printf("\n");

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;

			if (!file)
				printf(" %d", 8 - rank);

			printf(" %d", getBit(bitboard, square) ? 1 : 0);
		}

		printf("\n");
	}

	printf("\n   a b c d e f g h \n\n");

	printf("   Bitboard: %llu\n\n", bitboard);
}

void loadTextures()
{
	wPawnTex.loadFromFile("images/wp.png");
	wKnightTex.loadFromFile("images/wN.png");
	wBishopTex.loadFromFile("images/wB.png");
	wRookTex.loadFromFile("images/wR.png");
	wQueenTex.loadFromFile("images/wQ.png");
	wKingTex.loadFromFile("images/wK.png");
	bPawnTex.loadFromFile("images/bp.png");
	bKnightTex.loadFromFile("images/bN.png");
	bBishopTex.loadFromFile("images/bB.png");
	bRookTex.loadFromFile("images/bR.png");
	bQueenTex.loadFromFile("images/bQ.png");
	bKingTex.loadFromFile("images/bK.png");

	wPawnTex.setSmooth(true);
	wKnightTex.setSmooth(true);
	wBishopTex.setSmooth(true);
	wRookTex.setSmooth(true);
	wQueenTex.setSmooth(true);
	wKingTex.setSmooth(true);
	bPawnTex.setSmooth(true);
	bKnightTex.setSmooth(true);
	bBishopTex.setSmooth(true);
	bRookTex.setSmooth(true);
	bQueenTex.setSmooth(true);
	bKingTex.setSmooth(true);
}

void drawSquares()
{
	for (int rank = 0; rank < 8; rank++)
    {
    	for (int file = 0; file < 8; file++)
    	{
    		int index = rank * 8 + file;
    		squares[index] = RectangleShape(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    		squares[index].setPosition(SQUARE_SIZE * rank, SQUARE_SIZE * file);
    		
    		if ((rank+file) % 2 == 0)
    			squares[index].setFillColor(Color(200, 200, 200));
    		else
    			squares[index].setFillColor(Color(100, 100, 100));
    	}
    }
}

void printBoard()
{
	for (int i = 0; i < 64; i++)
	{
		if (i % 8 == 0)
			printf("\n%d", 8 - (i / 8));

		if (getBit(boards[P], i)) printf(" P");
		else if (getBit(boards[p], i)) printf(" p");
		else if (getBit(boards[N], i)) printf(" N");
		else if (getBit(boards[n], i)) printf(" n");
		else if (getBit(boards[B], i)) printf(" B");
		else if (getBit(boards[b], i)) printf(" b");
		else if (getBit(boards[R], i)) printf(" R");
		else if (getBit(boards[r], i)) printf(" r");
		else if (getBit(boards[Q], i)) printf(" Q");
		else if (getBit(boards[q], i)) printf(" q");
		else if (getBit(boards[K], i)) printf(" K");
		else if (getBit(boards[k], i)) printf(" k");
		else printf(" -");
	}

	printf("\n  a b c d e f g h\n");

	printf("\nSide: %s\n", (whiteToMove ? "white" : "black"));

	printf("Enpassant: %s\n", (enpassant != noSqr) ? squareToCoords[enpassant] : "no");

	printf("Castling: %c%c%c%c\n", ((castling & wk) ? 'K' : '-'), ((castling & wq) ? 'Q' : '-'),
		  ((castling & bk) ? 'k' : '-'), ((castling & bq) ? 'q' : '-'));

	printf("\nBitboard: %lluULL\n", (uint64_t)occupancy[both]);
}

void loadPieces()
{
	for (int i = 0; i < 64; i++)
	{
		if (i == 0 || i == 7) pieces[i].setTexture(wRookTex);
		if (i == 1 || i == 6) pieces[i].setTexture(wKnightTex);
		if (i == 2 || i == 5) pieces[i].setTexture(wBishopTex);
		if (i == 3) pieces[i].setTexture(wQueenTex);
		if (i == 4) pieces[i].setTexture(wKingTex);
		if (i >= 8 && i <= 15) pieces[i].setTexture(wPawnTex);
		
		if (i == 63 || i == 56) pieces[i].setTexture(bRookTex);
		if (i == 62 || i == 57) pieces[i].setTexture(bKnightTex);
		if (i == 61 || i == 58) pieces[i].setTexture(bBishopTex);
		if (i == 60) pieces[i].setTexture(bQueenTex);
		if (i == 59) pieces[i].setTexture(bKingTex);
		if (i >= 48 && i <= 55) pieces[i].setTexture(bPawnTex);

		pieces[i].setPosition((SQUARE_SIZE * (i % 8))-5.0f, (SQUARE_SIZE * (i / 8))-5.0f);
	}
}

static inline bool makeMove(Move move)
{
	setBit(&boards[move.piece], move.to);
	popBit(&boards[move.piece], move.from);

	if (move.isCapture)
	{
		int startPiece, endPiece;

		if (whiteToMove)
		{
			startPiece = p;
			endPiece = k;
		}
		else
		{
			startPiece = P;
			endPiece = K;
		}
		
		for (int bbPiece = startPiece; bbPiece <= endPiece; bbPiece++)
		{
			if (getBit(boards[bbPiece], move.to))
			{
				popBit(&boards[bbPiece], move.to);
				break;
			}
		}
	}

	if (move.promotedPiece)
	{
		popBit(&boards[whiteToMove ? P : p], move.to);
		setBit(&boards[move.promotedPiece], move.to);
	}

	if (enpassant)
		whiteToMove ? popBit(&boards[p], move.to + 8) : popBit(&boards[P], move.to - 8);

	enpassant = noSqr;

	if (move.isDoublePawnPush)
		whiteToMove ? enpassant = move.to + 8 : enpassant = move.to - 8;
	
	if (move.isCastling)
	{
		switch(move.to)
		{
		// white king side
		case g1:
			popBit(&boards[R], h1);
			setBit(&boards[R], f1);
			break;
		// white queen side
		case c1:
			popBit(&boards[R], a1);
			setBit(&boards[R], d1);
			break;
		// black king side
		case g8:
			popBit(&boards[r], h8);
			setBit(&boards[r], f8);
			break;
		// black castles queen side
		case c8:
			popBit(&boards[r], a8);
			setBit(&boards[r], d8);
			break;
		}
	}

	castling &= castlingRights[move.from];
	castling &= castlingRights[move.to];

	memset(occupancy, 0ULL, 0);

	for (int bbPiece = P; bbPiece <= K; bbPiece++)
		occupancy[white] |= boards[bbPiece];

	for (int bbPiece = p; bbPiece <= k; bbPiece++)
		occupancy[black] |= boards[bbPiece];

	occupancy[both] = occupancy[black] | occupancy[white];

	whiteToMove ^= 1;

	/*if(isSquareAttacked((whiteToMove ? getFirstSetBit(board[k]) : getFirstSetBit(board[K])), whiteToMove))
	{
		memcpy(board, boardCopy, sizeof(boardCopy));
		memcpy(occupied, occupanciesCopy, sizeof(occupanciesCopy));
		enpassant = enpassantCopy;
		castling = castlingCopy;
		whiteToMove = sideCopy;
		return false;
	}
	else
		return true;*/

	return true;
}

int getPieceAt(int square)
{
	for (int piece = P; piece <= k; piece++)
		if (getBit(boards[piece], square))
			return piece;
}

int main()
{
    RenderWindow window(VideoMode(SCREEN_SIZE, SCREEN_SIZE), "SFML Chess");

	Event event;

	int selectedX = -1;
	int selectedY = 0;

	bool secondClick = false;

	initBitboards();
	printBoard();

	loadTextures();
	loadPieces();
	
	drawSquares();

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
			
			if (event.type == Event::MouseButtonReleased)
			{
				Vector2i mousePos = Mouse::getPosition(window);
				int tempX = mousePos.x / SQUARE_SIZE;
				int tempY = mousePos.y / SQUARE_SIZE;
				
				if (secondClick)
				{
					int fromIndex = selectedX * 8 + selectedY;
					int toIndex = tempX * 8 + tempY;

					if (move is in movelist)
					makeMove(Move(fromIndex, toIndex, getPieceAt(fromIndex), 0, 0, 0, 0, 0));
					pieces[fromIndex].setPosition(Vector2f((tempX * SQUARE_SIZE)-5.0f, (tempY * SQUARE_SIZE)-5.0f));
					secondClick = false;
				}

				if (tempX >= 0 && tempX < 8 && tempY >= 0 && tempY < 8)
				{
					selectedX = mousePos.x / SQUARE_SIZE;
					selectedY = mousePos.y / SQUARE_SIZE;
					secondClick = true;
				}
			}
        }

        window.clear();
		
        for (int i = 0; i < 64; i++)
        	window.draw(squares[i]);

		for (int i = 0; i < 64; i++)
			window.draw(pieces[i]);

		if (selectedX != -1)
		{
			RectangleShape selector = RectangleShape(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
			selector.setPosition(Vector2f(selectedX * SQUARE_SIZE, selectedY * SQUARE_SIZE));
			selector.setFillColor(Color(0, 0, 255, 100));

			window.draw(selector);
		}

		window.display();
    }

    return 0;
}
