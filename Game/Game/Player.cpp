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
#include "PlayerJump.h"
#include "PlayerFalling.h"
#include "PlayerAttack.h"

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
	states_[(size_t)PlayerStateType::kJump] = make_shared<PlayerJump>(this);
	states_[(size_t)PlayerStateType::kFalling] = make_shared<PlayerFalling>(this);
	states_[(size_t)PlayerStateType::kAttack] = make_shared<PlayerAttack>(this);

	right_ = make_shared<Texture>();
	right_->Load(L"Resources/WarriorRightSheet.bmp", 6, 17);
	right_->SetPivot({ 0.385f, 0.97f });

	left_ = make_shared<Texture>();
	left_->Load(L"Resources/WarriorLeftSheet.bmp", 6, 17);
	left_->SetPivot({ 0.625f, 0.97f });

	AddSpriteRenderer();
	GetSpriteRenderer()->SetSprite(right_);

	AddAnimator();
	GetAnimator()->AddClip((size_t)PlayerClipType::kIdle, true, 0, 6);
	GetAnimator()->AddClip((size_t)PlayerClipType::kWalk, true, 6, 8);
	GetAnimator()->AddClip((size_t)PlayerClipType::kJump, false, 41, 3);
	GetAnimator()->AddClip((size_t)PlayerClipType::kFalling, false, 44, 5);
	GetAnimator()->AddClip((size_t)PlayerClipType::kAttack, false, 14, 12);

	AddRigidbody2D();
	AddBoxCollider2D();
	GetBoxCollider2D()->SetOffset({ 0.f, -50.f });
	GetBoxCollider2D()->SetScale({ 100.f, 100.f });

	StateMachine::Initiate();
}

void Player::InputUpdate()
{
	horizontal = (INPUT_MANAGER->GetKey(VK_RIGHT) - INPUT_MANAGER->GetKey(VK_LEFT)) * move_speed_;

	GetRigidbody2D()->SetVelocity({ horizontal, GetRigidbody2D()->GetVelocity().y_ });

	if (INPUT_MANAGER->GetKey(VK_RIGHT))
	{
		direction_ = 1;
	}

	if (INPUT_MANAGER->GetKey(VK_LEFT))
	{
		direction_ = -1;
	}

	if (INPUT_MANAGER->GetKeyDown(VK_UP))
	{
		is_ground_ = false;
		GetRigidbody2D()->SetVelocity({ GetRigidbody2D()->GetVelocity().x_, -500.f });
	}

	if (INPUT_MANAGER->GetKeyDown(VK_LSHIFT))
	{
		move_speed_ = 400.f;
	}

	if (INPUT_MANAGER->GetKeyUp(VK_LSHIFT))
	{
		move_speed_ = 200.f;
	}

	if (is_ground_)
	{
		if (INPUT_MANAGER->GetKeyDown(VK_DOWN))
		{
			is_ground_ = false;
			GetRigidbody2D()->SetVelocity({ 500.f * direction_, -500.f });
		}
	}

	if (INPUT_MANAGER->GetKey(VK_LCONTROL))
	{
		is_attack_ = true;

		if (current_state_ != states_[(size_t)PlayerStateType::kAttack])
		{
			ChangeState(states_[(size_t)PlayerStateType::kAttack]);
		}
	}
}

void Player::Update()
{
	Object::Update();
	StateMachine::Update();

	if (!is_attack_)
	{
		if (is_ground_)
		{
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
		}

		if (GetRigidbody2D()->GetVelocity().y_ < 0.f)
		{
			if (current_state_ != states_[(size_t)PlayerStateType::kJump])
			{
				ChangeState(states_[(size_t)PlayerStateType::kJump]);
			}
		}

		if (GetRigidbody2D()->GetVelocity().y_ > 0.f)
		{
			if (current_state_ != states_[(size_t)PlayerStateType::kFalling])
			{
				ChangeState(states_[(size_t)PlayerStateType::kFalling]);
			}
		}
	}

	if (direction_ == 1)
	{
		GetSpriteRenderer()->SetSprite(right_);
	}
	else if (direction_ == -1)
	{
		GetSpriteRenderer()->SetSprite(left_);
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
	is_ground_ = true;
	GetRigidbody2D()->SetVelocity({ 0.f, 0.f });
}

void Player::OnTriggerStay(Object* other)
{
}

void Player::OnTriggerExit(Object* other)
{
	is_ground_ = false;
}
