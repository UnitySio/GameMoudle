#pragma once

#include <Windows.h>

class BoxCollider2D;
class Rigidbody2D;

class Object
{
private:
	WCHAR name_[1024];
	Vector2 position_;
	Vector2 scale_;
	Vector2 pivot_;

	std::shared_ptr<BoxCollider2D> box_collider2d_;
	std::shared_ptr<Rigidbody2D> rigidbody2d_;
public:
	Object();
	Object(const Object& kOrigin);
	virtual ~Object() = default;

	virtual void Update(float delta_time);
	virtual void LateUpdate(float delta_time);
	virtual void Render(HDC hdc);

	virtual void OnTriggerEnter(std::shared_ptr<BoxCollider2D> other);
	virtual void OnTriggerStay(std::shared_ptr<BoxCollider2D> other);
	virtual void OnTriggerExit(std::shared_ptr<BoxCollider2D> other);

	void SetName(LPCWSTR name);
	void SetPosition(Vector2 position);
	void SetScale(Vector2 scale);
	void SetPivot(Vector2 pivot);
	void Translate(Vector2 vector2);

	LPCWSTR GetName();
	Vector2 GetPosition();
	Vector2 GetScale();
	Vector2 GetPivot();
	Vector2 GetPivotPosition();

	std::shared_ptr<BoxCollider2D> GetBoxCollider2D();
	std::shared_ptr<Rigidbody2D> GetRigidbody2D();

	void AddBoxCollider2D();
	void AddRigidbody2D();
};
