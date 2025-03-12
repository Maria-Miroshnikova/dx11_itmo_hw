#pragma once
#include "RectangleComponent.h"

class PlatformComponent : public RectangleComponent {
   
public:

    DirectX::SimpleMath::Rectangle CollisionBox;
    float Speed;
    float Size_coeff = 1; // насколько ужать. 1 = не ужимать, полный размер. 
  //  float left_corner_x;
  //  float left_corner_y;
    
    PlatformComponent(Game* g);
    PlatformComponent(Game* g, bool isLeft);
    void Update() override;

    void reSize();
    void setDefSize();
};