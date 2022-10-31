#include "pch.h"
#include "PlayerWalk.h"
#include "InputManager.h"
#include "Animator.h"

PlayerWalk::PlayerWalk(Player* owner) :
	owner_(owner)
{
}

void PlayerWalk::Enter()
{
	if (owner_->GetAnimator() != nullptr)
	{
		owner_->GetAnimator()->SetClip((size_t)PlayerClipType::kWalk);
	}
}

void PlayerWalk::Update()
{
}

void PlayerWalk::Exit()
{
}