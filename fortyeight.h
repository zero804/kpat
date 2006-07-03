/*
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * This file is provided AS IS with no warranties of any kind.  The author
 * shall have no liability with respect to the infringement of copyrights,
 * trade secrets or any patents by this file or any part thereof.  In no
 * event will the author be liable for any lost revenue or profits or
 * other special, indirect and consequential damages.
*/
#ifndef _FORTY_EIGHT_H
#define _FORTY_EIGHT_H

#include "dealer.h"

class HorLeftPile : public Pile
{
    Q_OBJECT

public:
    HorLeftPile( int _index, Dealer* parent = 0);
    virtual QSize cardOffset( bool _spread, bool _facedown, const Card *before) const;
    virtual void initSizes();
};

class Fortyeight : public Dealer
{
    Q_OBJECT

public:
    Fortyeight( KMainWindow* parent=0 );
    virtual bool isGameLost() const;

public slots:
    void deal();
    virtual void restart();
    void deckClicked(Card *c);

protected:
    virtual bool checkAdd( int checkIndex, const Pile *c1, const CardList& c2) const;
    virtual Card *demoNewCards();
    virtual QString getGameState() const;
    virtual void setGameState( const QString & stream );

private:
    Pile *stack[8];
    Pile *target[8];
    HorLeftPile *pile;
    Deck *deck;
    bool lastdeal;
};

#endif


//-------------------------------------------------------------------------//
