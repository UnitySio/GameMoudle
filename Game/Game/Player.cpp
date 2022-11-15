#include "pch.h"
#include "Player.h"
#include "Window.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Rigidbody2D.h"
#include "BoxCollider2D.h"
#include "InputManager.h"
#include "PlayerIdle.h"
#include "PlayerWalk.h"
#include "SceneManager.h"
#include "Bullet.h"
#include "Scene.h"

using namespace std;

shared_ptr<State> Player::GetInitiateState()
{
	return states_[(size_t)PlayerStateType::kIdle];
}

Player::Player() :
	move_speed_(200.f),
	direction_(1),
	is_ground_(),
	is_attack_(),
	horizontal()
{
	states_[(size_t)PlayerStateType::kIdle] = make_shared<PlayerIdle>(this);
	states_[(size_t)PlayerStateType::kWalk] = make_shared<PlayerWalk>(this);

	right_ = make_shared<Texture>();
	right_->Load(L"Resources/WarriorRightSheet.bmp", 6, 17);
	right_->SetPivot({ 0.385f, 0.97f });

	left_ = make_shared<Texture>();
	left_->Load(L"Resources/WarriorLeftSheet.bmp", 6, 17);
	left_->SetPivot({ 0.625f, 0.97f });

	AddSpriteRenderer();
	GetSpriteRenderer()->SetTexture(right_);

	AddAnimator();
	GetAnimator()->AddClip(L"IDLE", true, 0, 6);
	GetAnimator()->AddClip(L"WALK", true, 6, 8);

	AddRigidbody2D();
	AddBoxCollider2D();
	GetBoxCollider2D()->SetOffset({ 0.f, -50.f });
	GetBoxCollider2D()->SetSize({ 64.f, 100.f });

	StateMachine::Initiate();
}

void Player::Update()
{
	Object::Update();
	StateMachine::Update();

	horizontal = (INPUT->GetKey(VK_RIGHT) - INPUT->GetKey(VK_LEFT)) * move_speed_;

	GetRigidbody2D()->SetVelocity({ horizontal, GetRigidbody2D()->GetVelocity().y_ });

	if (INPUT->GetKey(VK_RIGHT))
	{
		direction_ = 1;
	}

	if (INPUT->GetKey(VK_LEFT))
	{
		direction_ = -1;
	}

	if (INPUT->GetKeyDown(VK_UP))
	{
		GetRigidbody2D()->SetVelocity({ GetRigidbody2D()->GetVelocity().x_, -500.f });
	}

	if (INPUT->GetKeyDown(VK_LSHIFT))
	{
		move_speed_ = 400.f;
		//SCENE->Destroy(shared_from_this());
	}

	if (INPUT->GetKeyUp(VK_LSHIFT))
	{
		move_speed_ = 200.f;
	}

	shared_ptr<Object> bullet = make_shared<Bullet>();
	Vector2 temp = SCENE->GetCurrentScene()->FindObject(L"Ground")->GetPosition() - GetPosition();
	Vector2 temp1 = MOUSE_POSITION - GetPosition();
	float m = sqrt(pow(temp.x_, 2) + pow(temp.y_, 2));
	Vector2 nn = temp1.Normalized();
	Vector2 n;
	if (m > 0)
	{
		n = { temp.x_ / m, temp.y_ / m };
	}

	WCHAR word[1024];
	_stprintf_s(word, L"X: %f, Y: %f\n", n.y_, nn.y_);
	OutputDebugString(word);

	(*(Bullet*)bullet.get()).SetDirection(n);
	SCENE->Instantiate(bullet, LayerType::kDefault, L"Bullet", GetPosition() + Vector2({0, -66.f}), {}, {});

	if (GetRigidbody2D()->GetVelocity().x_ != 0.f)
	{
		if (current_state_ != states_[(size_t)PlayerStateType::kWalk])
		{
			ChangeState(states_[(size_t)PlayerStateType::kWalk]);
		}
	}
	else
	{
		if (current_state_ != states_[(size_t)PlayerStateType::kIdle])
		{
			ChangeState(states_[(size_t)PlayerStateType::kIdle]);
		}
	}

	if (direction_ == 1)
	{
		GetSpriteRenderer()->SetTexture(right_);
	}
	else if (direction_ == -1)
	{
		GetSpriteRenderer()->SetTexture(left_);
	}
}

void Player::LateUpdate()
{
	Object::LateUpdate();

	if (!is_ground_)
	{
		GetRigidbody2D()->SetGravityAcceleration(Vector2().Down() * 800);
	}
}

void Player::PhysicsUpdate()
{
	Object::PhysicsUpdate();
}

void Player::Render()
{
	Object::Render();
}

void Player::OnTriggerEnter(Object* other)
{
	if (GetPosition().y_ < other->GetPosition().y_ - 15.f)
	{
		is_ground_ = true;
		GetRigidbody2D()->SetVelocity({ GetRigidbody2D()->GetVelocity().x_, 0.f});
	}
}

void Player::OnTriggerStay(Object* other)
{
}

void Player::OnTriggerExit(Object* other)
{
	is_ground_ = false;
}
