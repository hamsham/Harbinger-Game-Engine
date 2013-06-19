/*
 * stack class
*/

#ifndef __HL_QUEUE__
#define __HL_QUEUE__

namespace hamLibs {
namespace containers {

template <typename type>
class queue {
	private:
		struct node {
			type data;
			node *next = nullptr;
		};
		node		*first = nullptr;		//List navigation
		node		*last = nullptr;
		int	numNodes;
		
		
	public:
		queue		();
		~queue		();

		//data acquisition
		type*		front		() const;
		type*		peekNext	() const;
		type*		back		() const;

		//insertion & deletion
		void		push		(const type& object);
		void		pop			();
		void		clear		();

		//miscellaneous
		int	size		() const;
		bool		empty		() const;
};

//-----------------------------------------------------------------------------
//			Construction & Destruction
//-----------------------------------------------------------------------------
template <typename type>
queue<type>::queue() :
	first( nullptr ),
	last( nullptr ),
	numNodes( 0 )
{}

template <typename type>
queue<type>::~queue() {
	clear();
}

//-----------------------------------------------------------------------------
//			Data Acquisition
//-----------------------------------------------------------------------------
template <typename type>
type* queue<type>::front() const {
	return (first) ? (&first->data) : nullptr;
}

template <typename type>
type* queue<type>::peekNext() const {
	if (first && first->next)
		return &(first->next->data);
	return nullptr;
}

template <typename type>
type* queue<type>::back() const {
	return (last) ? &(last->data) : nullptr;
}

//-----------------------------------------------------------------------------
//			Insertion
//-----------------------------------------------------------------------------
template <typename type>
void queue<type>::push(const type& object) {
	node* temp = new node;
	temp->data = object;
	temp->next = first;
	first = temp;
	++numNodes;
	
	if (numNodes == 1)
		last = first;
}

//-----------------------------------------------------------------------------
//			Deletion
//-----------------------------------------------------------------------------
template <typename type>
void queue<type>::pop() {
	if (first == nullptr)
		return;
	
	node* temp( first->next );
	delete first;
	--numNodes;
	
	first = temp;	
	if (first == nullptr)
		last = nullptr;
}

template <typename type>
void queue<type>::clear() {
	while (first != nullptr) {
		pop();
	}
}

//-----------------------------------------------------------------------------
//			Miscellaneous
//-----------------------------------------------------------------------------
template <typename type>
int queue<type>::size() const {
	return numNodes;
}

template <typename type>
bool queue<type>::empty() const {
	return (first == nullptr) ? true : false;
}

} //end containers namespace
} //end hamlibs namespace
#endif //__HL_QUEUE__
