#ifndef HUFFTREEHPP
#define HUFFTREEHPP

#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"
#include <string>
#include <stack>
#include <map>

class HuffmanTree : public HuffmanTreeBase {
public:
	HuffmanTree() { root_node = new HuffmanNode('\0', 0); }
	~HuffmanTree();

	virtual std::string compress(const std::string inputStr);
	virtual std::string serializeTree() const;
	virtual std::string decompress(const std::string inputCode, const std::string serializedTree);

private:
    HuffmanNode* root_node;
};

const std::string postorderSerialize(HuffmanNode* node);
void preorderCoding(std::map<char, std::string>* prefixCode, HuffmanNode* node, std::string code);
void deleteTree(HuffmanNode* node);


#endif /* HUFFTREEHPP */ 