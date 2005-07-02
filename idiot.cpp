/*
   idiot.cpp  implements a patience card game

     Copyright (C) 1995  Paul Olav Tvete

   Permission to use, copy, modify, and distribute this software and its
   documentation for any purpose and without fee is hereby granted,
   provided that the above copyright notice appear in all copies and that
   both that copyright notice and this permission notice appear in
   supporting documentation.

   This file is provided AS IS with no warranties of any kind.  The author
   shall have no liability with respect to the infringement of copyrights,
   trade secrets or any patents by this file or any part thereof.  In no
   event will the author be liable for any lost revenue or profits or
   other special, indirect and consequential damages.

   4 positions, remove lowest card(s) of suit
*/


#include "idiot.h"
#include <klocale.h>
#include "deck.h"
#include "cardmaps.h"


Idiot::Idiot( KMainWindow* parent, const char* _name)
  : Dealer( parent, _name )
{
    deck = Deck::new_deck( this );
    deck->move(10, 10);

    away = new Pile( 5, this );
    away->setTarget(true);
    away->setRemoveFlags(Pile::disallow);

    const int distx = cardMap::CARDX() + cardMap::CARDX() / 10 + 1;

    // Create 4 piles where the cards will be placed during the game.
    for( int i = 0; i < 4; i++ ) {
        play[i] = new Pile( i + 1, this);

        play[i]->move(10 + cardMap::CARDX() * 18 / 10 + distx * i, 10);
        play[i]->setAddFlags( Pile::addSpread );
        play[i]->setRemoveFlags( Pile::disallow );
    }

    away->move(10 + cardMap::CARDX() * 5 / 2 + distx * 4, 10);
    setActions(Dealer::Hint | Dealer::Demo);
}


void Idiot::restart()
{
    deck->collectAndShuffle();
    deal();
}

inline bool higher( const Card* c1, const Card* c2)
{
    // Sanity check.
    if (!c1 || !c2 || c1 == c2)
        return false;

    // Must be same suit.
    if (c1->suit() != c2->suit())
        return false;

    // Aces form a special case.
    if (c2->rank() == Card::Ace)
        return true;
    if (c1->rank() == Card::Ace)
        return false;

    return (c1->rank() < c2->rank());
}


bool Idiot::canMoveAway(Card *c)
{
    return ( higher( c, play[ 0 ]->top() ) ||
             higher( c, play[ 1 ]->top() ) ||
             higher( c, play[ 2 ]->top() ) ||
             higher( c, play[ 3 ]->top() ) );
}


bool Idiot::cardClicked(Card *c)
{
    // If the deck is clicked, deal 4 more cards.
    if (c->source() == deck) {
        deal();
	++moves;
	emit setMoves(moves);
        return true;
    }

    // Only the top card of a pile can be clicked.
    if (c != c->source()->top())
        return false;

    bool  didMove = true;
    if( canMoveAway(c) )
	// Add to 'away', face up, no spread
        away->add(c, false, false);
    else if( play[ 0 ]->isEmpty() )
	// Add to pile 1, face up, spread.
        play[0]->add(c, false, true);
    else if( play[ 1 ]->isEmpty() )
	// Add to pile 2, face up, spread.
        play[1]->add(c, false, true);
    else if( play[ 2 ]->isEmpty() )
	// Add to pile 3, face up, spread.
        play[2]->add( c, false, true);
    else if( play[ 3 ]->isEmpty() )
	// Add to pile 4, face up, spread.
        play[3]->add(c, false, true);
    else
	didMove = false;

    // If a card was actually moved, increase the shown moves.
    if (didMove) {
	++moves;
	emit setMoves(moves);
    }

    return true; // may be a lie, but noone cares
}


// The game is won when:
//  1. all cards are dealt.
//  2. all piles contain exactly one ace.
//  3. the rest of the cards are thrown away (follows automatically from 1, 2.
//
bool Idiot::isGameWon() const
{
    // Criterium 1.
    if (!deck->isEmpty())
        return false;

    // Criterium 2.
    for (int i = 0; i < 4; i++) {
        if (play[i]->cardsLeft() != 1 || play[i]->top()->rank() != Card::Ace)
            return false;
    }

    return true;
}


// This patience doesn't support double click.
//

bool Idiot::cardDblClicked(Card *)
{
    return false; // nothing - nada
}


// Deal 4 cards face up - one on each pile.
//

void Idiot::deal()
{
    if ( !deck->isEmpty() ) {
        for ( int i = 0; i < 4; i++ )
	    // Move the top card of the deck, faceup, spread out.
            play[ i ]->add( deck->nextCard(), false, true );
    }
}

void Idiot::getHints()
{
    bool cardMoved = false;
    for ( int i = 0; i < 4; i++ )
        if ( canMoveAway( play[i]->top() ) ) {
            cardMoved = true;
            newHint(new MoveHint(play[i]->top(), away));
        }

    if (cardMoved)
        return;

    // now let's try to be a bit clever with the empty piles
    for( int i = 0; i < 4; i++ )
        if (play[i]->isEmpty())
        {
            // Find a card to move there
            int biggestPile = -1;
            int sizeBiggestPile = -1;
            for( int j = 0; j < 4; j++ )
            {
                if ( i != j && play[j]->cardsLeft()>1 )
                {
                    // Ace on top of the pile ? -> move it
                    if ( play[j]->top()->rank() == Card::Ace )
                    {
                        biggestPile = j;
                        break;
                    }
                    // Otherwise choose the biggest pile
                    if ( play[j]->cardsLeft() > sizeBiggestPile )
                    {
                        sizeBiggestPile = play[j]->cardsLeft();
                        biggestPile = j;
                    }
                }
            }
            if ( biggestPile != -1 )
            {
                newHint(new MoveHint(play[biggestPile]->top(), play[i]));
                return;
            }
        }
}


Card *Idiot::demoNewCards()
{
    if( deck->isEmpty() )
        return 0;
    deal();
    return play[0]->top();
}


static class LocalDealerInfo2 : public DealerInfo
{
public:
    LocalDealerInfo2() : DealerInfo(I18N_NOOP("&Aces Up"), 2) {}
    virtual Dealer *createGame(KMainWindow *parent) { return new Idiot(parent); }
} ldi4;


#include "idiot.moc"
