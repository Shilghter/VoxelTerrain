#define BLOCK_GRASS 0
#define BLOCK_DIRT 1
#define BLOCK_STONE 2
#define BLOCK_SIZE 0.5f

class Block
{

public:
	Block();
	~Block();
	bool IsActive();
	void SetActive(bool active);
	void SetBlockType(unsigned char blockType);
	unsigned char GetBlockType();
private:
	bool activationState;
	unsigned char blockType;
};