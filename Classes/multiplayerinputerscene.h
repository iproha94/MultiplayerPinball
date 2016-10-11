#ifndef MULTIPLAYERINPUTERSCENE_H
#define MULTIPLAYERINPUTERSCENE_H
#include "cocos2d.h"
#include "basescene.h"
#include "extensions/GUI/CCControlExtension/CCControl.h"
#include "ui/CocosGUI.h"
#include "string.h"


class MultiplayerInputerScene: public BaseScene
{
public:

  static cocos2d::Scene* createScene();

  virtual bool init();

  void  action(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

  void textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType typy);

  std::string get_player_name();

  std::string get_server_IP();

  CREATE_FUNC(MultiplayerInputerScene);

private:
  std::string player_name;

  std::string server_IP;


};

#endif




