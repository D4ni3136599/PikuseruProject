#include "Engine.hpp"
#include "Graphics.hpp"
#include "UI.hpp"
#include "Transform.hpp"
#include "Controller.hpp"
#include "Physics.hpp"
#include "Time.hpp"
#include "Log.hpp"
#include "Audio.hpp"
#include "Input.hpp"
#include "GameState.hpp"
#include <iostream>
#include "Debug.hpp"

using Aspen::Engine::Engine;
using Aspen::GameState::GameState;
using Aspen::GameState::GameStateManager;
using Aspen::Graphics::Graphics;
using Aspen::Object::Object;

double radians = 0.01;

void ChangeScene(Aspen::Graphics::UI::Button *button, std::string scene, GameStateManager *gsm)
{
  gsm->SetCurrentState(scene);
}

class Player : public Aspen::Graphics::Animation
{
  public:
  Player(Object *parent = nullptr, std::string name = "Player") :
    Aspen::Graphics::Animation(
      new Aspen::Graphics::UniformSpritesheet("./resources/TurtleEatingGrass.png", 128, 128, 8, nullptr, "TurtleSpriteSheet"),
      1.0f / 5.0f, parent, name)
      {
      CreateChild<Aspen::Physics::AABBCollider>()->SetSize(128, 64);
      CreateChild<Aspen::Physics::Rigidbody>();
      }
  void OnCollision(Aspen::Physics::Collision c)
  {
    Aspen::Log::Debug("Help I've fallen and I can't get up");
    
  }

};

class Platform : public Aspen::Graphics::Sprite
{
public:
  Platform(Object *parent = nullptr, std::string name = "Platform") :
    Aspen::Graphics::Sprite("./resources/GrassyBlock.png", parent, name)
  {
    GetTransform()->SetPosition(100, 420);
    CreateChild<Aspen::Physics::AABBCollider>()->SetSize(200, 60);
  }

void OnCollision(Aspen::Physics::Collision c)
{
  Aspen::Log::Debug("I have been in a horrible collision");
}
};



class MainMenu : public GameState
{
  Aspen::Graphics::UI::Text *title;
  Player *player;
  Platform *platform;

  public:
    MainMenu(Object *parent = nullptr, std::string name = "MainMenu") : GameState(parent, name)
    {  
      //text physics
      player = new Player();
      title = new Aspen::Graphics::UI::Text("You are wrong", "default", 64, this, "Title");
      title->GetTransform()->SetPosition(313, 400);
      title->GetTransform()->SetRotation(0);
      title->GetTransform()->SetScale(1.12, 0.790);
      AddChild(title);

      player = new Player(this, "Player");
      player->GetTransform()->SetPosition(100,200);
      AddChild(player);

      platform = new Platform();
      platform->GetTransform()->SetPosition(0, 0);
      AddChild(platform);

      platform = new Platform();
      platform->GetTransform()->SetPosition(0, 100);
      AddChild(platform);

      platform = new Platform();
      platform->GetTransform()->SetPosition(0, 200);
      AddChild(platform);

      platform = new Platform();
      platform->GetTransform()->SetPosition(300, 300);
      AddChild(platform);
  }

  void OnUpdate()
  {

    if(Aspen::Input::KeyHeld(SDLK_w))
    {
      Aspen::Log::Info("W is held");
      player->GetTransform()->ModifyPosition(0, -20);
      //title->GetTransform()->ModifyRotation(radians);
      //radians += 0.001;
    }
    if(Aspen::Input::KeyHeld(SDLK_s))
    {
      Aspen::Log::Info("S is held");
      player->GetTransform()->ModifyPosition(0, 3);
      //title->GetTransform()->ModifyRotation(radians);
      //radians += 0.001;
    }
    if(Aspen::Input::KeyHeld(SDLK_d))
    {
      Aspen::Log::Info("D is held");
      player->GetTransform()->ModifyPosition(3, 0);
      //title->GetTransform()->ModifyRotation(radians);
      //radians += 0.001;
    }
    if(Aspen::Input::KeyHeld(SDLK_a))
    {
      Aspen::Log::Info("A is held");
      player->GetTransform()->ModifyPosition(-3, 0);
      //title->GetTransform()->ModifyRotation(radians);
      //radians += 0.001;
    }
    if(Aspen::Input::KeyHeld(SDLK_SPACE))
    {
      radians = 0;
    }
    else
    {
       title->GetTransform()->ModifyRotation(radians);
       radians += 0.001;
       Aspen::Log::Info("Velocity is ", radians);
    }
    
  }




};

class Game : public GameState
{};

int main(int argc, char **argv)
{
  Aspen::Log::Log::SetFile("./Aspen.log");

  Engine engine(Aspen::Engine::START_FLAGS::ALL ^ (
    Aspen::Engine::START_FLAGS::CREATE_GRAPHICS |
    Aspen::Engine::START_FLAGS::CREATE_GRAPHICS_DEBUGGER |
    Aspen::Engine::START_FLAGS::CREATE_GRAPHICS_FONTCACHE
  ));
  Aspen::Graphics::Graphics *gfx = new Aspen::Graphics::Graphics(1080, 720);
  gfx->CreateChild<Aspen::Debug::Debug>();
  gfx->CreateChild<Aspen::Graphics::FontCache>();
  engine.AddChild(gfx);

  engine.FindChildOfType<Aspen::Physics::Physics>()->SetGravityStrength(1);
  engine.FindChildOfType<Aspen::Physics::Physics>()->SetDrag(0.1);
  engine.FindChildOfType<Aspen::Time::Time>()->TargetFramerate(60);
  engine.FindChildOfType<Aspen::Graphics::Graphics>()->FindChildOfType<Aspen::Graphics::FontCache>()->LoadFont("resources/ABeeZee-Regular.ttf", "default");

  engine.FindChildOfType<GameStateManager>()->LoadState<MainMenu>(true);
  //engine.FindChildOfType<GameStateManager>()->LoadState<Game>(false);

  while (engine)
    engine();
  return 0;
}
