#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "../../timecard.h"

#include "../Board.h"
#include "Dimensions.h"
#include "StatusBar.h"
#include "Navigator.h"

class Interface
{
protected:
  Board     *_board;
  StatusBar  _statusBar;
  Navigator  _navigator;

public:
  Interface(Board *board);

  void update(void);
  void initLayout(void);
  void onSecondChange(void);
  void onMinuteChange(void);
};

extern Interface *interface;

#endif // __INTERFACE_H__
