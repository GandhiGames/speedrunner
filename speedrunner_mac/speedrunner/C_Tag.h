#pragma once

#include <string>

#include "Component.h"
#include "Util.h"

class C_Tag : public Component
{
public:
	C_Tag(Object* owner);

	TAG Get() const;
	void Set(TAG tag);

    bool Compare(std::shared_ptr<C_Tag> other);
    bool Compare(TAG other);
private:
	TAG m_tag;
};

