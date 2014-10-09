#include "stdafx.h"
#include "Block.h"

Block::Block()
{
	this->blockType = BLOCK_DIRT;
	this->activationState = false;
}
Block::~Block()
{

}
bool Block::IsActive()
{
	return this->activationState;
}
void Block::SetActive(bool active)
{
	this->activationState = active;
}
void Block::SetBlockType(unsigned char blockType)
{
	this->blockType = blockType;
}
unsigned char Block::GetBlockType()
{
	return this->blockType;
}