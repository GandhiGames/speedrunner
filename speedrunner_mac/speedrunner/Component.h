#pragma once

class Object;

class Component
{
	friend class Object;
public:
	Component(Object* owner) : m_owner(owner)
	{
	}

	virtual void Awake() {};
	virtual void Start() {};

protected:
	Object* m_owner;

};
