//Anh Luu U08722141
//Khanh DOng U14837275 
#include "HuffmanTree.hpp"

// Recursively deconstruct the tree
HuffmanTree::~HuffmanTree() {
	deleteTree(root_node);
}

// Deconstructor Helper
void deleteTree(HuffmanNode* node) {
	if (!node->isLeaf()) { //If it is not a leaf
			// Postorder traversal
			deleteTree(node->left);
			deleteTree(node->right);
	}
	delete node; // Delete the node
}

// Compression
std::string HuffmanTree::compress(const std::string inStr) {
	std::map<char, int> charFreq; // Create list of frequency of characters via map
	for(unsigned int i = 0; i < inStr.length(); i++) {
		if(charFreq.count(inStr.at(i)) >= 1)  // If char exists in map already
      charFreq.at(inStr.at(i)) += 1; // Increment freq of that character 
		else {
			auto p = std::make_pair(inStr.at(i), 1);
			charFreq.insert(p); // Inserted the new char in the map with freq of 1
		}
	}

	// Inserting the characters and their frequencies into a priority queue 
	HeapQueue<HuffmanNode*, HuffmanNode::Compare> pQueue; //HeapQueue
	auto iter = charFreq.begin();
	while(iter != charFreq.end()) { //Using HeapQueue to build the binary tree
		HuffmanNode* newNode;
    	newNode = new HuffmanNode(iter->first, iter->second);
		pQueue.insert(newNode);
		iter++;
	}

	// Until there is one element left in the priority queue
	while(pQueue.size() != 1) {
		// Remove two characters/frequencies pairs from the priority queue
		auto pair1 = pQueue.min();
		pQueue.removeMin(); //remove once
		auto pair2 = pQueue.min();
		pQueue.removeMin(); // remove twice

		// Create an intermediate node to be their parent using the sum of the frequencies for those children
		int intermediateFreq = (pair1->getFrequency() + pair2->getFrequency());
		HuffmanNode* intermediate_node = new HuffmanNode('\0', intermediateFreq, nullptr, pair1, pair2);

		// Insert Intermedidate Node into PQueue
		pair1->parent = intermediate_node; pair2->parent = intermediate_node;
		pQueue.insert(intermediate_node); //inserting the intermediate node back into the queue
	}
	// Remaining last pair
	root_node = pQueue.min();
	// Encode the characters
	std::string encodedString = "";
	std::map<char, std::string> charCodeMap; // Map with the char and code
	preorderCoding(&charCodeMap, root_node, encodedString);
	// Replace the characters in the string with their new variable-length prefix codes
	for(char i : inStr) 
		encodedString += charCodeMap.at(i); 
	return encodedString;
}

void preorderCoding(std::map<char, std::string>* charCodeMap, HuffmanNode* node, std::string strCode) {
	if (!node->isLeaf()) { //if it is not a leaf
		preorderCoding(charCodeMap, node->left, strCode + '0'); // left add 0
		preorderCoding(charCodeMap, node->right, strCode + '1'); // right add 1
	}
	else {
		//visit the node
		auto codePair = std::make_pair(node->getCharacter(), strCode);
		charCodeMap->insert(codePair); //insert the char, code pair
	}
	return;
}

// Recursively post order serialize the tree starting from the root node
std::string HuffmanTree::serializeTree() const{ 
	return postorderSerialize(root_node);
}

const std::string postorderSerialize(HuffmanNode* node) {
	std::string serializeStr = "";
	// If node is a leaf (external node) then you add a L plus the character to the serialize tree string
	if (node->isLeaf()) { // If leaf node
			serializeStr += "L"; // add the 'L' 
			serializeStr += node->getCharacter(); // add the character
			return serializeStr; // add to serialize string
	}
	// If branch (internal node) then you add a B to the serialize tree string 
	return (postorderSerialize(node->left) + postorderSerialize(node->right) + 'B');
}

// decompress to retrieve the original string
std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree) {
  //1. create binary tree / huffman tree (with serializedTree) with stack
  //2. read the huffman tree using input code 
  //3. output final code
  
  std::string output = ""; //string output to be returned
  std::stack<HuffmanNode*> huffTree; //declaring a stack of datatype HuffmanNode* 

  //creating a binary tree of the letters 
  for(unsigned int i = 0; i < serializedTree.length(); i++){ //iterate through the string serializedTree 
    if(serializedTree[i] == 'L'){
      i++; //because the char in front of 'L' is what will be "added" to the binary tree
      HuffmanNode* temp = new HuffmanNode(serializedTree[i], 0);
      huffTree.push(temp); //pushing the node into a stack
    }
    else if(serializedTree[i] == 'B'){ 
      HuffmanNode* right = huffTree.top();
      huffTree.pop();
      HuffmanNode* left = huffTree.top();
      huffTree.pop(); 

      //creating a parent node with the top two elements of the stack as its children 
      HuffmanNode* parent = new HuffmanNode(0, 0, nullptr, left, right);
      huffTree.push(parent); //pushing the parent node back into the stack
    }
  }

  HuffmanNode* root = huffTree.top(); //the root is the "top" item in the stack
  unsigned int i = 0;
	HuffmanNode* temp; //declare a HuffmanNode* node
  bool isRoot = false;
	
  while(i < inputCode.length()){ 
    if (!isRoot){ 
      temp = root; 
      isRoot = true;
    }
    else if(isRoot){
      if(temp->isLeaf()){ //check if the temp node is at a leaf
        output += temp->getCharacter(); //concatenate the character to the string 
        isRoot = false; //set temp equals to root again (to start again)
      }
      else{ //traverse through the binary tree 
        if(inputCode[i] == '0'){
          temp = temp->left; 
        }
        else if(inputCode[i] == '1'){
          temp = temp->right;
        }
				i++;
      }
    }
  }
  output += (temp->getCharacter()); //add the last character
  return output; //return the string
}


