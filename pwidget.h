/* -*- C++ -*-
 *
 * patience -- main program
 *   Copyright (C) 1995  Paul Olav Tvete
 *
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
 *
 *
 * Heavily modified by Mario Weilguni <mweilguni@sime.com>
 *
 */

#ifndef PWIDGET_H
#define PWIDGET_H

//Added by qt3to4:
#include <QPixmap>
#include <QLabel>
#include <QShowEvent>

#include <kxmlguiwindow.h>
#include <krecentfilesaction.h>

class DemoBubbles;
class PatienceView;
class KToggleAction;
class KSelectAction;
class KRecentFilesAction;
class QAction;
class QLabel;
class QStackedWidget;
class cardMap;
class DealerInfo;

class pWidget: public KXmlGuiWindow {
    Q_OBJECT

public:
    pWidget();
    ~pWidget();

public slots:
    void undoMove();
    void redoMove();
    void newGameType(int id);
    void startNew(long gameNumber = -1);
    void restart();
    void slotShowGameSelectionScreen();

    void openGame();
    void openGame(const KUrl &url, bool addToRecentFiles = true);
    void saveGame();

    void newGame();
    void chooseGame();
    void undoPossible(bool poss);
    void redoPossible(bool poss);
    void gameLost();
    void slotGameInfo(const QString &);
    void slotUpdateMoves();
    void helpGame();
    void enableAutoDrop();
    void enableSolver();
    void enableRememberState();
    void showStats();

    void slotGameSolverStart();
    void slotGameSolverWon();
    void slotGameSolverLost();
    void slotGameSolverUnknown();
    void slotPickRandom();
    void slotSelectDeck();

    void slotSnapshot();

    void slotGameSelected(int id);

protected:
    virtual void closeEvent(QCloseEvent * e);
    virtual void showEvent(QShowEvent *e);

private slots:
    void slotSnapshot2();

private:
    void setGameCaption();
    bool allowedToStartNewGame();

private:
    // Members
    QAction        *undo, *redo;
    KToggleAction  *dropaction;
    KToggleAction  *solveraction;
    KToggleAction  *rememberstateaction;
    KRecentFilesAction  *recent;

    cardMap        *m_cards; // possibly move to PatienceView
    QMap<int, const DealerInfo*>  m_dealer_map;
    QMap<int, const DealerInfo*>::const_iterator  m_dealer_it;

    QStackedWidget *m_stack;
    PatienceView   *dill;
    DemoBubbles    *m_bubbles;
};

#endif
