#include "C_Tag.h"


C_Tag::C_Tag(Object* owner) : Component(owner),
m_tag(TAG::DEFAULT)
{
}

TAG C_Tag::Get() const
{
	return m_tag;
}

void C_Tag::Set(TAG tag)
{
	m_tag = tag;
}

bool C_Tag::Compare(std::shared_ptr<C_Tag> other)
{
    return m_tag == other->m_tag;
}

bool C_Tag::Compare(TAG other)
{
    return m_tag == other;
}
