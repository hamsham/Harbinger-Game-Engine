/* 
 * File:   hamString.h
 * Author: hammy
 *
 * Created on July 7, 2012, 6:41 PM
 */

#ifndef __HL_STRING_H__
#define	__HL_STRING_H__

namespace hamLibs {
namespace containers {

class string {
	private:
		static const int nPos;
		
		char*		array;
		int	Size;
	
	public:
		string		();
		string		(char c);
		string		(const char* str);
		string		(const string& str);
		~string		();

		//operators
		string&		operator =	(const string& str);
		string&		operator =	(const char* str);
		string&		operator =	(char c);
		
		string&		operator +=	(const string& str);
		string&		operator +=	(const char* str);
		string&		operator +=	(char c);
		
		string		operator +	(const string& str) const;
		string		operator +	(const char* str) const;
		string		operator +	(char c) const;
		
		bool		operator ==	(const string& str) const;
		bool		operator ==	(const char* str) const;
		bool		operator ==	(char c) const;
		
		bool		operator !=	(const string& str) const;
		bool		operator !=	(const char* str) const;
		bool		operator !=	(char c) const;
		
		char&		operator []	(int);
		char		operator []	(int) const;

		//character deletion
		void		clear		();
		void		popFront	();
		void		popBack		();
		void		pop			(int index);

		//random accecss insertion
		void		push		(int index, const char* str, int strSize);
		void		pushAfter	(int index, const char* str, int strSize);
		
		void		push		(int index, const string& str);
		void		pushAfter	(int index, const string& str);
		
		void		push		(int index, const char* str);
		void		pushAfter	(int index, const char* str);
		
		void		push		(int index, char c);
		void		pushAfter	(int index, char c);
		
		//direct insertion
		void		pushFront	(const char* str, int strSize);
		void		pushBack	(const char* str, int strSize);
		
		void		pushFront	(const string& str);
		void		pushBack	(const string& str);
		
		void		pushFront	(const char* str);
		void		pushBack	(const char* str);
		
		void		pushFront	(char c);
		void		pushBack	(char c);

		//traversal
		char&		front		();
		char&		back		();
		
		//searching
		int	find		(const char* str, int strLength) const;
		int	find		(const string& str) const;
		int	find		(const char* str) const;
		int	find		(char c) const;
		
		int	rFind		(const char* str, int strLength) const;
		int	rFind		(const string& str) const;
		int	rFind		(const char* str) const;
		int	rFind		(char c) const;

		//utility functions
		void		copy		(const string& str);
		void		copy		(const char* str);
		void		copy		(char c);
		
		void		append		(const string& str);
		void		append		(const char* str, int strLength);
		void		append		(const char* str);
		void		append		(char c);
		
		void		resize		(int newSize, char c);
		void		resize		(int newSize);
		
		//miscellaneous
		string	subStr		(int pos, int length) const;
		bool		empty		() const;
		int	size		() const;
		void		swap		(string&);
		const char*	cStr		() const;
};

} // end containers namespace
} //end hamLibs namespace

#endif	/* __HL_STRING_H__ */
