/* 
 * File:   dictionary.h
 * Author: hammy
 *
 * Created on August 28, 2012, 4:53 PM
 */

#ifndef __HL_DICTIONARY_H__
#define __HL_DICTIONARY_H__

#include <string>

namespace hamLibs {
namespace containers {

template <typename type>
class dictionary {

    typedef const char* const cstr;
    
	private:
		struct node {
			type*	definition;
			node*	alphabet[26];
			
			explicit	node		();
			explicit	node		(const node& nodeCopy);
			~node			();
			node&	operator=	(const node& nodeCopy);
			void		clear	();
			bool		hasSubNodes() const;
		};
		
		node	mainNode;
		
		int		getArrayIndex		(char c) const;
		node*	iterateToNode		(cstr nodeName) const;
		std::string iterateToNextNode	(cstr nodeName) const;
		std::string iterateToPrevNode	(cstr nodeName) const;
		
	public:
		class iterator {
			friend class dictionary<type>;
			
			private:
				const dictionary* dictToIter;
				const dictionary::node* currentNode;
				std::string currentWord;
				
			public:
				iterator();
				iterator( const iterator& iterCopy );
				~iterator();
				
				iterator operator ++ (int); // postfix
				iterator operator -- (int);
				iterator& operator ++ (); // prefix
				iterator& operator -- ();
				iterator& operator = ( const iterator& iterCopy );
				iterator& operator = ( const dictionary& dict );
				
				bool atFront () const;
				bool atEnd() const;
				
				void goToFront();
				void goToEnd();
				void goToWord( cstr word );
				
				const char* getCurrentWord() const;
				type* getData() const;
		};
		
		dictionary			(){}
		dictionary			(const dictionary& dictionCopy) {mainNode = dictionCopy.mainNode;}
		~dictionary			() {}
		dictionary& operator=	(const dictionary& dictCopy);
		
		type* getDefinition		(cstr word) const;
		void setDefinition		(cstr word, const type& inDefinition);
		void clearDefinition	(cstr word);
		bool knowWord			(cstr word);
		bool knowDefinition		(cstr word);
		bool knowSubWords		(cstr word); //check if there are any sub-nodes within a node
		void addWord			(cstr word);
		void deleteWord		(cstr word); //clears a node's letter (if it doesn't have any sub-nodes) and its definition
		void clear			() {mainNode.clear();}
		
		iterator begin			() const;
		iterator end			() const;
};

//-----------------------------------------------------------------------------
//		Node Structure
//-----------------------------------------------------------------------------
template <typename type>
dictionary<type>::node::node() :
	definition	(nullptr)
{
	//NULL-ify all of the elements in the array
	int iter = 26;
	while (iter--) {
		alphabet[iter] = nullptr;
	}
}

template <typename type>
dictionary<type>::node::node(const node& nodeCopy) :
	definition( nullptr )
{
	int iter(26);
	while (iter--)
		alphabet[ iter ] = nullptr;
	*this = ( nodeCopy );
}

template <typename type>
 dictionary<type>::node::~node() {
	clear();
}

template <typename type> typename
dictionary<type>::node& dictionary<type>::node::operator = (const node& nodeCopy) {
	clear();
	
	if (nodeCopy.definition != nullptr) {
		if (definition)
			delete definition;
		definition = new type(*(nodeCopy.definition));
		
	}
	
	int iter(26);
	while (iter--) {
		if (nodeCopy.alphabet[ iter ]) {
			alphabet[ iter ] = new node (*nodeCopy.alphabet[ iter ]); //begin recursive awesomeness!
		}
	}
	return *this;
}

template <typename type>
void dictionary<type>::node::clear() {
	delete definition;
	definition = nullptr;
	
	int iter(26);
	while (iter--) {
		delete alphabet[ iter ];
		alphabet[ iter ] = nullptr;
	}
}

template <typename type>
bool dictionary<type>::node::hasSubNodes() const {
	int iter(26);
	while (iter--) {
		if ( alphabet[ iter ] != nullptr )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//		Utilities
//-----------------------------------------------------------------------------
template <typename type>
dictionary<type>& dictionary<type>::operator = (const dictionary& dictCopy) {
	mainNode = dictCopy.mainNode;
	return *this;
}

template <typename type> typename
dictionary<type>::node* dictionary<type>::iterateToNode(cstr nodeName) const {
	int pos(0), index(0);
	node* nodeTracker( const_cast< node* >(&mainNode) ); //dictionary iterator

	do {
		index = getArrayIndex( nodeName[pos] );
		if (index == -1 || !nodeTracker->alphabet[ index ]) return nullptr;
		nodeTracker = nodeTracker->alphabet[ index ];
		++pos;
	} while (nodeName[ pos ]);					//end the loop when a null-termination is reached
	return nodeTracker;						//return the position of the requested node
}

template <typename type>
std::string dictionary<type>::iterateToNextNode(cstr nodeName) const {
	std::string nodeIdentifier;
	if (nodeName != nullptr) {
		nodeIdentifier = nodeName;
	}
	else {
		nodeIdentifier = "a";
	}
	node* nodeIter = iterateToNode( nodeName );
	int iter = 0;
	
	//search for a possible child node (going from a-z)
	if (nodeIter) {
		while (iter < 26) {
			if (nodeIter->alphabet[ iter ]) {
				//move to the next node by incrementing the specified letter by "iter"
				nodeIdentifier.push_back( 'a' + iter );
				return nodeIdentifier;
			}
			++iter;
		}
	}
	
	//if no child node was found in the previous loop, go to the parent node, then return the next sibling node
	while (nodeIdentifier.size() > 0) {
		if (nodeIdentifier[ nodeIdentifier.size()-1 ] == 'z' ) {
			nodeIdentifier.erase( nodeIdentifier.size()-1 );
			continue;
		}
		nodeIdentifier[ nodeIdentifier.size()-1 ] += 1;
		nodeIter = iterateToNode( nodeIdentifier.c_str() );
		if (nodeIter)
			return nodeIdentifier;
	}
	return nodeIdentifier;
}

template <typename type>
std::string dictionary<type>::iterateToPrevNode(cstr nodeName) const {
	std::string nodeIdentifier;
	if (nodeName != nullptr)
		nodeIdentifier = nodeName;
	else
		return nodeIdentifier;
	
	node* nodeIter = nullptr; //iterateToNode( nodeName );
	
	//go to the next sibling node
	while (nodeIdentifier.size() > 0) {
		if (nodeIdentifier[ nodeIdentifier.size()-1 ] == 'a') {
			nodeIdentifier.erase( nodeIdentifier.size()-1 );
			if (nodeIdentifier.size() > 0)
				//return the parent node if the children have all been searched
				return nodeIdentifier;
		}
		nodeIdentifier[ nodeIdentifier.size()-1 ] -= 1;
		nodeIter = iterateToNode( nodeIdentifier.c_str() );
		//return the next sibling node's child if possible
		if (nodeIter) {
			if (nodeIter->hasSubNodes()) {
				nodeIdentifier.push_back( 'z' );
				continue;
			}
			//else
			return nodeIdentifier;
		}
	}
	
	return nodeIdentifier;
}

template <typename type>
int dictionary<type>::getArrayIndex(char c) const {
	//make sure the letter in question actually is a letter
	if ((c < 'a') && (c > 'Z') || (c < 'A') || (c > 'z')) return -1;
	return (c < 'a') ? c - 'A' : c - 'a';	//convert all uppercase letters to lowercase (See asciitable.com)
	//get the index of the next letter in dictionary<type>::alphabet[]
}

//-----------------------------------------------------------------------------
//		Definitions
//-----------------------------------------------------------------------------
template <typename type>
type* dictionary<type>::getDefinition(cstr word) const {
	node* nodeTracker( iterateToNode(word) );
	return nodeTracker->definition;
}

template <typename type>
void dictionary<type>::setDefinition(cstr word, const type& inDefinition) {
	node* nodeTracker( iterateToNode(word) );
	if (nodeTracker) {
		if (nodeTracker->definition)
			delete nodeTracker->definition;
		nodeTracker->definition = new type( inDefinition );
	}
}

template <typename type>
void dictionary<type>::clearDefinition(cstr word) {
	node* nodeTracker( iterateToNode(word) );
	if (nodeTracker) {
		delete nodeTracker->definition;
		nodeTracker->definition = nullptr;
	}
}

template <typename type>
bool dictionary<type>::knowWord(cstr word) {
	node* nodeTracker( iterateToNode(word) );
	return (nodeTracker) ? true : false;
}

template <typename type>
bool dictionary<type>::knowDefinition(cstr word) {
	node* nodeTracker( iterateToNode(word) );
	return (nodeTracker && nodeTracker->definition) ? true : false;
}

template <typename type>
bool dictionary<type>::knowSubWords(cstr word) {
	node* nodeTracker( iterateToNode(word) );
	if (nodeTracker) {
		int iter(26);
		while (iter--) {
			if (nodeTracker[ iter ]) return true;
		}
	}
	return false;
}

template <typename type>
void dictionary<type>::addWord(cstr word) {
	int pos(0), index(0);
	node* nodeIter( const_cast< node* >(&mainNode) );

	while ( word[ pos ] ) { //end when a null-termination is reached
		index = getArrayIndex( word[ pos ] );		//get the index of the next letter in dictionary<type>::alphabet[]
		if (index == -1) return;

		if (nodeIter->alphabet[ index ] == nullptr) {
			nodeIter->alphabet[ index ] = new node;	//add a new word to the dictionary if possible
		}
		nodeIter = nodeIter->alphabet[index];	//move to the next letter in dictionary<type>::alphabet
		++pos;
	}
}

template <typename type>
void dictionary<type>::deleteWord(cstr word) {
	node* nodeTracker( iterateToNode(word) );
	if (nodeTracker == nullptr || nodeTracker == &mainNode)
		return;
	
	if (nodeTracker->hasSubNodes() == true) {
		delete nodeTracker->definition;
		nodeTracker->definition = nullptr;
	}
	else {
		delete nodeTracker;
	}
}

//-----------------------------------------------------------------------------
//		Iterations
//-----------------------------------------------------------------------------
template <typename type>
dictionary<type>::iterator::iterator() :
	dictToIter( nullptr ),
	currentNode( nullptr ),
	currentWord( "" )
{}

template <typename type>
dictionary<type>::iterator::iterator( const dictionary<type>::iterator& iterCopy ) :
	dictToIter( iterCopy.dictToIter ),
	currentNode( iterCopy.currentNode ),
	currentWord( iterCopy.currentWord )
{}

template <typename type>
dictionary<type>::iterator::~iterator() {}

template <typename type> typename
dictionary<type>::iterator dictionary<type>::begin() const {
	node* nodeIter( const_cast< node* >(&mainNode) );
	std::string currWord;
	
	int iter(0);
	while (iter < 26) {
		if ( nodeIter->alphabet[ iter ] != nullptr ) {
			currWord.push_back( iter + 'a' );
			break;
		}
		else {
			++iter;
		}
	}
	iterator temp;
	temp.dictToIter = this;
	temp.goToWord( (currWord.size() > 0) ? currWord.c_str() : nullptr );
	return temp;
}

template <typename type> typename
dictionary<type>::iterator dictionary<type>::end() const {
	node* nodeIter( const_cast< node* >(&mainNode) );
	std::string currWord;
	
	int iter(25);
	while (iter >= 0) {
		if ( nodeIter->alphabet[ iter ] != nullptr ) {
			currWord.push_back( iter + 'a' );
			nodeIter = nodeIter->alphabet[ iter ];
			iter = 25;
		}
		else {
			--iter;
		}
	}
	iterator temp;
	temp.dictToIter = this;
	temp.goToWord( (currWord.size() > 0) ? currWord.c_str() : nullptr );
	return temp;
}

template <typename type> typename
dictionary<type>::iterator dictionary<type>::iterator::operator ++(int) {
	operator++();
	return *this;
}

template <typename type> typename
dictionary<type>::iterator dictionary<type>::iterator::operator --(int) {
	operator--();
	return *this;
}

template <typename type> typename
dictionary<type>::iterator& dictionary<type>::iterator::operator ++() {
	if (dictToIter != nullptr) {
		currentWord = dictToIter->iterateToNextNode( currentWord.c_str() );
		currentNode = dictToIter->iterateToNode( currentWord.c_str() );
	}
	return *this;
}

template <typename type> typename
dictionary<type>::iterator& dictionary<type>::iterator::operator --() {
	if (dictToIter != nullptr) {
		currentWord = dictToIter->iterateToPrevNode( currentWord.c_str() );
		currentNode = dictToIter->iterateToNode( currentWord.c_str() );
	}
	return *this;
}

template <typename type> typename
dictionary<type>::iterator& dictionary<type>::iterator::operator = ( const iterator& iterCopy ) {
	dictToIter = iterCopy.dictToIter;
	currentNode = iterCopy.currentNode;
	currentWord = iterCopy.currentWord;
}

template <typename type> typename
dictionary<type>::iterator& dictionary<type>::iterator::operator = ( const dictionary& dict ) {
	*this = dict.begin();
}

template <typename type>
bool dictionary<type>::iterator::atFront () const {
	if (dictToIter == nullptr)
		return true;
	if ( currentNode == nullptr
	&& dictToIter->iterateToPrevNode( currentWord.c_str() ).size() == 0 )
		return true;
	return false;
}

template <typename type>
bool dictionary<type>::iterator::atEnd() const {
	if (dictToIter == nullptr)
		return true;
	if ( currentNode == nullptr
	&& dictToIter->iterateToNextNode( currentWord.c_str() ).size() == 0 )
		return true;
	return false;
}

template <typename type>
void dictionary<type>::iterator::goToFront() {
	if (dictToIter != nullptr)
		*this = dictToIter->begin();
}

template <typename type>
void dictionary<type>::iterator::goToEnd() {
	if (dictToIter != nullptr)
		*this = dictToIter->end();
}

template <typename type>
void dictionary<type>::iterator::goToWord( cstr word ) {
	if (dictToIter == nullptr || word == nullptr)
		return;
	
	currentNode = dictToIter->iterateToNode( word );
	if ( currentNode != nullptr )
		currentWord = word;
}

template <typename type>
const char* dictionary<type>::iterator::getCurrentWord() const {
	if (dictToIter != nullptr)
		return currentWord.c_str();
	return nullptr;
}

template <typename type>
type* dictionary<type>::iterator::getData() const {
	if (dictToIter != nullptr && currentNode != nullptr)
		return currentNode->definition;
	return nullptr;
}

} //end containers namespace
} //end hamLibs namespace

#endif	/* __HL_DICTIONARY_H__ */
