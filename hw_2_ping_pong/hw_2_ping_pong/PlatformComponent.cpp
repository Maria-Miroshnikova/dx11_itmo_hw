#include "PlatformComponent.h"
#include "SimpleMath.h"
#include "Game.h"

using namespace DirectX;

const SimpleMath::Vector4 leftColor = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
const SimpleMath::Vector4 rightColor = SimpleMath::Vector4(0.0f, 1.0f, 1.0f, 1.0f);

const float SIZE_half_def = 0.1f;

PlatformComponent::PlatformComponent(Game* g) : RectangleComponent(g)  {
	SimpleMath::Vector4 color = rightColor;

	SimpleMath::Vector4 pointsTmp[8] = {
		SimpleMath::Vector4(0.01f, SIZE_half_def, 0.5f, 1.0f),	color,
		SimpleMath::Vector4(-0.01f, -SIZE_half_def, 0.5f, 1.0f),	color,
		SimpleMath::Vector4(0.01f, -SIZE_half_def, 0.5f, 1.0f),	color,
		SimpleMath::Vector4(-0.01f, SIZE_half_def, 0.5f, 1.0f),	color,
	};
	std::swap(points, pointsTmp);

	Speed = 1.7f;
//	CollisionBox = SimpleMath::Rectangle(-0.01f * 800, -0.1f * 800, 0.02f * 800, 0.2f * 800);
	// первые два параметра - (x.y), левый нижний угол
	// вторые два - это ширина (вправо) и высота (вверх)
	// итого коллижн бокс совпадает по высоте с платформой и выпирает только вправо от нее
	CollisionBox = SimpleMath::Rectangle(-0.01f * 800, -SIZE_half_def * 800, 0.02f * 800, 2 * SIZE_half_def * 800);
}

PlatformComponent::PlatformComponent(Game* g, bool isLeft) : RectangleComponent(g) {
	SimpleMath::Vector4 color = rightColor;
	if (isLeft)
		color = leftColor;

	SimpleMath::Vector4 pointsTmp[8] = {
		SimpleMath::Vector4(0.01f, SIZE_half_def, 0.5f, 1.0f),	color,
		SimpleMath::Vector4(-0.01f, -SIZE_half_def, 0.5f, 1.0f),	color,
		SimpleMath::Vector4(0.01f, -SIZE_half_def, 0.5f, 1.0f),	color,
		SimpleMath::Vector4(-0.01f, SIZE_half_def, 0.5f, 1.0f),	color,
	};
	std::swap(points, pointsTmp);

	Speed = 1.7f;
//	CollisionBox = SimpleMath::Rectangle(-0.01f * 800, -0.1f * 800, 0.02f * 800, 0.2f * 800);
	CollisionBox = SimpleMath::Rectangle(-0.01f * 800, -SIZE_half_def * 800, 0.02f * 800, 2 * SIZE_half_def * 800);
}

void PlatformComponent::Update() {
	// тут мы двигаем наш искусственый коллижн, который не отрисовывается
	// значения подходят?
	//CollisionBox.x = -0.01f * 800 + transform_data.off.x * 800;
	//CollisionBox.y = -SIZE_half_def * 800 + transform_data.off.y * 800;

	CollisionBox.x = -0.01f * 800 + transform_data.off.x * 800;
	CollisionBox.y = -SIZE_half_def * Size_coeff * 800 + transform_data.off.y * 800;
	CollisionBox.height = 2 * (SIZE_half_def * Size_coeff) * 800;
	
// тут мы просим отрисовать платформу	
	RectangleComponent::Update();
}

const float MIN_SIZE_half = 0.051f;
const float resize_coeff_step = 0.1f;

void PlatformComponent::reSize() {
	
	if (Size_coeff * SIZE_half_def > MIN_SIZE_half) {
		Size_coeff -= resize_coeff_step;
		std::cout << "resize: " << Size_coeff << std::endl;
		SetSizeCoeff(SimpleMath::Vector2(1, Size_coeff));
	}
}

void PlatformComponent::setDefSize() {
	Size_coeff = 1.0;
	SetSizeCoeff(SimpleMath::Vector2(1, Size_coeff));
}