#pragma once

#include <memory>

class C_Collidable
{
	friend class Object;

public:
	virtual void OnCollisionEnter(Object& owner, std::shared_ptr<Object> other) {};
	virtual void OnCollisionStay(Object& owner, std::shared_ptr<Object> other) {};
	virtual void OnCollisionExit(Object& owner, std::shared_ptr<Object> other) {};

	virtual void OnTriggerEnter(Object& owner, std::shared_ptr<Object> other) {};
	virtual void OnTriggerStay(Object& owner, std::shared_ptr<Object> other) {};
	virtual void OnTriggerExit(Object& owner, std::shared_ptr<Object> other) {};
};
