#include "typeerasure.h"

class Type1Player
{
public:
    void Play() { std::cout << "Type1Player::Play()\n"; }
};

class Type2Player
{
public:
    void Play() { std::cout << "Type2Player::Play()\n"; }
};

int main()
{
    Player player;

    // Sfinae prevent recursive ctor.
    Player player0(player);

    try
    {
        player0.Play();
    }
    catch (...)
    {
        std::cout << "No play function\n";
    }

    // Both Type1Player and Type2Player are different types.
    // But they have the function Play(), so they are able to be put into the same Player class.

    Type1Player hasPlayPlayer1;
    Player player1(hasPlayPlayer1);
    player1.Play();

    Type2Player hasPlayPlayer2;
    Player player2(hasPlayPlayer2);
    player2.Play();

    return 0;
}
