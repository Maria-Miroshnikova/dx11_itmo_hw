#include "BallComponent.h"
#include "PingPongGame.h"              
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

float def_speed = 0.5f;


// line/line collision
static bool LineLineColl(const Vector2& p1, const Vector2& p2, const Vector2& p3, const Vector2& p4, Vector2& intersection) {

	// calculate the direction of the lines
	float uA = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));
	float uB = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) / ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		intersection.x = (p1.x + (uA * (p2.x - p1.x))) / 800.0f;
		intersection.y = (p1.y + (uA * (p2.y - p1.y))) / 800.0f;
		return true;
	}
	return false;
}

// line/rectangle collision detection
static bool LineRectColl(const Vector2& p1, const Vector2& p2, const DirectX::SimpleMath::Rectangle& rect, bool leftPrimary, Vector2& intersection)
{
	bool top = LineLineColl(p1, p2, Vector2(rect.x, rect.y),Vector2(rect.x + rect.width, rect.y), intersection);
	bool bottom = LineLineColl(p1, p2, Vector2(rect.x, rect.y + rect.height), Vector2(rect.x + rect.width, rect.y + rect.height), intersection);
	bool left, right;
	if (leftPrimary)
	{
		right = LineLineColl(p1, p2, Vector2(rect.x + rect.width, rect.y), Vector2(rect.x + rect.width, rect.y + rect.height), intersection);
		left = LineLineColl(p1, p2, Vector2(rect.x, rect.y), Vector2(rect.x, rect.y + rect.height), intersection);
	}
	else
	{
		left = LineLineColl(p1, p2, Vector2(rect.x, rect.y), Vector2(rect.x, rect.y + rect.height), intersection);
		right = LineLineColl(p1, p2, Vector2(rect.x + rect.width, rect.y), Vector2(rect.x + rect.width, rect.y + rect.height), intersection);
	}

	if (left || right || top || bottom)
	{
		return true;
	}
	return false;
}

float updateSpeed(float speed) {
	return speed + 0.1f;
}

void BallComponent::Update() {
	// прикидываем координату после совершения движения
	auto nextPos = GetPosition() + Direction * Speed * game->DeltaTime;
	
	// отскакивание от верха и низа экрана -
	if (nextPos.y > 1.0f)
		Direction.y = -abs(Direction.y);
	else if (nextPos.y < -1.0f)
		Direction.y = abs(Direction.y);
	// обработка выхода за рамки экрана лево-право - спавн мячика
	else if (nextPos.x < -1.0f || nextPos.x > 1.0f)
	{
		// проверка победы - в положительной и в отрицательной части поля произошел выход за границы
		game->addScore(nextPos.x > 0.0f);

		// респавн мячика
		Speed = def_speed;
		nextPos.x = 0.0f;
		nextPos.y = 0.0f;
		setStartDirection();

		// дефолтный размер платформ
		game->platformLeft->setDefSize();
		game->platformRight->setDefSize();
	}
	// обработка отскакивания от платформы левой
	else if (LineRectColl(GetPosition() * 800, nextPos * 800, game->platformLeft->CollisionBox, false, nextPos))
	{
		Vector2::Lerp(Vector2(1.f, -1.f), Vector2(1.f, 1.f), abs(GetPosition().y - game->platformLeft->GetPosition().y + 0.1f) / 0.2f, Direction);
		Direction.Normalize();

		Speed = updateSpeed(Speed);
		nextPos += Direction * Speed * game->DeltaTime;

		game->platformLeft->reSize();
	}
	// обработка отскакивания от платформы правой
	else if (LineRectColl(GetPosition() * 800, nextPos * 800, game->platformRight->CollisionBox, true, nextPos))
	{
		Vector2::Lerp(Vector2(-1.f, -1.f), Vector2(-1.f, 1.f), abs(GetPosition().y - game->platformRight->GetPosition().y + 0.1f) / 0.2f, Direction);
		Direction.Normalize();

		Speed = updateSpeed(Speed);
		nextPos += Direction * Speed * game->DeltaTime;

		game->platformRight->reSize();
	}

	// отрисовка новой позиции шарика
	SetPosition(nextPos);
	CircleComponent::Update();
}

void BallComponent::setStartDirection() {
	Vector2::Lerp(Vector2(1.0f, 1.0f), Vector2(1.0f, -1.0f), (float)rand() / RAND_MAX, Direction);
	if ((float)rand() / RAND_MAX < 0.5f)
		Direction.x *= -1;
	Direction.Normalize();
}

BallComponent::BallComponent(Game* g) : CircleComponent(g, 0, 0, 0.02, 0)
{
	//Ray r;
	//r.Intersects();
	game = dynamic_cast<PingPongGame*>(g);
	Speed = def_speed;
	setStartDirection();
}