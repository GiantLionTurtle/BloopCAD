
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef XMLPARSER_HPP_
#define XMLPARSER_HPP_

#include <string>

/*
	@note : This file is still a hot mess, it goes back and forth between string, const char* and char* and
	there is a lot of redundant code, but it works for what it is used, so it will likely stay that way for a bit

	@note : It will however be made robust for ill formed files in a near future
*/

class XML_document; // Forward declaration needed for pointer to Document
class XML_element; // Forward declaration needed for pointer to node

/*
	@class XML_node describes "something" in an XML Document it can have children, a parent node, a parent Document
	and it can be part of a chained list of nodes
*/
class XML_node {
public:
	friend class XML_document;
protected:
	XML_document* mDocument; 				// The Document that owns the node
	XML_node* mParent;						// It's parent node, if it has one
	XML_node* mPrevNode, *mNextNode; 		// Handles for linked chain
	XML_node* mFirstChild, * mLastChild;	// Handles for children linked chain
public:
	/*
		@function XML_node creates an empty node with null pointers
		in all it's fields
	*/
	XML_node();
	/*
		@function ~XML_node destroys an XML_node object and deallocate itself.
		It also destroys all it's children.
	*/
	virtual ~XML_node();

	/*
		@function set_doc sets the identity of the Document owning the node
	*/
	void set_doc(XML_document* doc) { mDocument = doc; }

	/*
		@function unlink strands the node ; it is no longer in the chained list and has no parent nor Document
	*/
	void unlink();

	/*
		@function add_firstChild adds a child node to the node at the first position in the linked chain

		@param node : A pointer to the node to add
	*/
	void add_firstChild(XML_node* node);
	/*
		@function add_lastChild adds a child node to the node at the last position in the linked chain

		@param node : A pointer to the node to add
	*/	
	void add_lastChild(XML_node* node);
	/*
		@function add_lastChild adds a child node to the node after a known node in the linked chain

		@param at : 	A pointer to the node after which to add the desired node
		@param node : 	A pointer to the node to add
	*/	
	void add_child(XML_node* at, XML_node* node);

	/*
		@function get_child_by_name attempts to find a XML_element child within the children linked list

		@param name : The name of the researched element

		@return : A pointer to the element that has the searched name or nullptr if it does not exist
	*/
	XML_element* get_child_by_name(std::string const& name);

	/*
		@function create_classifiedNode creates a specialized node from a char pointer that points to the begining of a node

		@param it : 		The char iterator to parse for node type
		@param node [out] : A pointer to a pointer to the node to be created

		@return : The incremented char iterator
	*/
	static char* create_classifiedNode(char* it, XML_node** node);
	/*
		@function parse is an overridable function for node specializations to parse themselves

		@param it : The char iterator to parse the node
		
		@return : The incremented char iterator
	*/
	virtual char* parse(char* it) = 0;

	/*
		@function firstChild 

		@return : The first child in the children linked list
	*/
	XML_node* firstChild() {return mFirstChild; };
	/*
		@function lastChild 

		@return : The last child in the children linked list
	*/	
	XML_node* lastChild() { return mLastChild; };

	/*
		@function prev

		@return : The previous node in the node's linked list
	*/
	XML_node* prev() { return mPrevNode; }
	/*
		@function next

		@return : The next node in the node's linked list
	*/	
	XML_node* next() { return mNextNode; }

	/*
		@function print is an overridable function for node specializations to print themselves

		@param tabs : The number of tabs to add to each print statement for aesthetic purposes
	*/
	virtual void print(int tabs = 0) = 0;
	/*
		@function to_xml is an overridable function for node specializations to create their xml code

		@param level : The recursion level of the node add proper indentation to each statement for aesthetic purposes
	*/
	virtual std::string to_xml(int level = 0) = 0;
};

/*
	@class XML_attribute describes a tuple of a name and a string value, it has an element parent and is part of
	an attributes linked chain
*/
class XML_attribute {
public:
	friend class XML_element;
private:
	XML_element* mParent; 					// Parent node
	XML_attribute* mPrevAttr, * mNextAttr;	// Handles to the linked list
	std::string mName, mValue;				// Name and value, the content of the attribute
public:
	/*
		@function XML_attribute creates an empty attribute object, it has nullptr in all it's pointer fields
		(it has no position in a linked list and is not owned)
	*/
	XML_attribute();
	XML_attribute(std::string const& name_, std::string const& value_);

	/*
		@function prev

		@return : The previous element in the linked list (or nullptr if it is the first attribute)
	*/
	XML_attribute* prev() { return mPrevAttr; }
	/*
		@function next

		@return : The next element in the linked list (or nullptr if it is the last attribute)
	*/
	XML_attribute* next() { return mNextAttr; }

	/*
		@function name 

		@return : The attribute's name
	*/
	std::string name() { return mName; }
	/*
		@function value 

		@return : The attribute's value
	*/	
	std::string value() { return mValue; }
	/*
		@function set_name sets the name of the attribute

		@param name : The name to be set
	*/
	void set_name(std::string name) { mName = name; }
	/*
		@function set_value sets the value of the attribute

		@param val : The value to be set
	*/
	void set_value(std::string val) { mValue = val; }
};

/*
	@class XML_element describes an xml node that has text content and attributes
	@parent : XML_node
*/
class XML_element : public XML_node {
public:
	enum closing_types {CLOSED, OPENED}; // <div/> or <div></div>, the first one can't have children
protected:
	closing_types mClosingType; 					// If it can contain children, from the markup
	XML_attribute* mFirstAttrib, * mLastAttrib;		// The chained list handles for the attributes
	std::string mName;								// The name of the element
	std::string mContent;							// The text content or value of the element
	std::string mStartString, mSelfCloseString;		// The closing and starting string (used for specialization of XML_declaration)
public:
	/*
		@function XML_element creates an xml element with no attribute, name, content or chilldren
		all pointers are se to nullptr
	*/
	XML_element();
	XML_element(std::string const& name_);

	/*
		@function ~XML_element destroys an xml element object by deleting it's attribute and letting the 
		XML_node destructor take care of the rest
	*/
	~XML_element();

	/*
		@function name

		@return : The element's name
	*/
	std::string name() const { return mName; }
	/*
		@function content 

		@return : The element's text content
	*/
	std::string content() const { return mContent; }

	void set_attribute(std::string const& name, std::string const& value);

	/*
		@function set_name sets the element's name

		@param name : The name to be set
	*/
	void set_name(std::string const& name) { mName = name; }
	/*
		@function set_content sets the element's text content

		@param content : The content to be set
	*/
	void set_content(std::string const& content) { mContent = content;}

	/*
		@function closingType 

		@return : The element's closing type
	*/
	closing_types closingType() { return mClosingType; }
	/*
		@function set_closingType sets the element's closing type

		@param type : The closing type to be set
	*/
	void set_closingType(closing_types type) { mClosingType = type; }

	/*
		@function add_firstAttribute adds an attribute to the element at the first position in the linked chain

		@param attr : A pointer to the attribute to add
	*/
	void add_firstAttribute(XML_attribute* attrib);
	/*
		@function add_lastAttribute adds an attribute to the element at the ;ast position in the linked chain

		@param attr : A pointer to the attribute to add
	*/
	void add_lastAttribute(XML_attribute* attrib);
	/*
		@function add_attribute adds an attribute to the element at the an arbitrary position in the linked chain

		@param at : 	The attribute that will preceed the added attribute in the chain
		@param attr : 	A pointer to the attribute to add
	*/
	void add_attribute(XML_attribute* at, XML_attribute* attrib);

	/*
		@function get_attribute_by_name attempts to find an attribute with a given name in the chain

		@param name : The name of the searched attribute

		@return : The attribute corresponding to that name or nullptr if it doesn't exist
	*/
	XML_attribute* get_attribute_by_name(std::string const& name);

	/*
		@function firstAttribute 

		@return : The first attribute of the attribute chain
	*/
	XML_attribute* firstAttribute() { return mFirstAttrib; }
	/*
		@funcrtion lastAttribute

		@return : The last attribute of the attribute chain
	*/
	XML_attribute* lastAttribute() { return mLastAttrib; }

	/*
		@function parse_attributes extracts any amount of attributes and sets the closing type
		from a position in the file

		@param it : The position iterator in the file

		@return : The incremented iterator		
	*/
	char* parse_attributes(char* it);
	/*
		@function parse_name exxtracts the name of the element from a position in the file

		@param it : The position iterator in the file

		@return : The incremented iterator		
	*/
	char* parse_name(char* it);
	/*
		@function check_endTag detects if the end tag of the element has been reached at a point in the file

		@param it : 		The position iterator in the file
		@param stop : 		The end tag string
		@param stop_len : 	The length of the end tag string
		@param out [out] : 	The result of the detection ; true : reached ; false : not reached, continue parsing

		@return : The incremented iterator		
	*/
	char* check_endTag(char* it, const char* stop, int stop_len, bool& out);
	/*
		@function parse extracts all the information about the element out of the file and 
		creates the object with all it's children 

		@param it : The start position iterator in the file of the parsing

		@return : The incremented iterator
	*/
	virtual char* parse(char* it);

	void deduce_closingType();

	/*
		@function print is an overrided function to print the element with all it's attributes
		and children. It is not printed as formal xml

		@param tabs : The number of tabs to add to print statements to create a tree
	*/
	virtual void print(int tabs = 0);
	/*
		@function to_xml creates a string in formal xml representing the element with it's attributes
		and children

		@param level : The recursion level of this element
	*/
	virtual std::string to_xml(int level = 0);
};

/*
	@class XML_comment describes a comment node, it is basically plain text as it cannot have children or attributes
	@parent : XML_node
*/
class XML_comment : public XML_node {
private:
	std::string mContent; // What the comment says
public:
	/*
		@function XML_comment creates a comment object
	*/
	XML_comment();
	/*
		@function ~XML_comment destroys a comment object. It doesn't do much because a comment has no heap allocation 
		(except for the string but it is handled by the stl)
	*/
	~XML_comment();

	/*
		@function parse locates the start and end tag from a position in the text and extract it's content

		@param it : The iterator for the position in the file

		@return : The incremented position iterator
	*/
	virtual char* parse(char* it);

	/*
		@function print is an overrided function to print the content of the comment
		
		@param tabs : The number of tabs to add to each line of the print statement 
		to create a nice tree
	*/
	virtual void print(int tabs = 0);
	/*
		@function to_xml creates a valid xml string representing the comment

		@param level : The level of recursion it is in, so that it fits nicely with the rest of the 
		file (it is a bit awkard tho, since comments are somewhat free)
	*/
	virtual std::string to_xml(int level = 0);
};

/*
	@class XML_declaration describes an xml declaration (encoding type and version). It is basically an 
	XML_element with different markups
	@parent : XML_element
*/
class XML_declaration : public XML_element {
public:
	/*
		@function XML_declaration creates an XML_declaration object with the appropriate markup strings
	*/
	XML_declaration(): XML_element("xml") { mStartString = "<?"; mSelfCloseString = "?>"; }
};

/*
	@class XML_document describes a Document, containing multiple nodes in a linked chain
*/
class XML_document {
private:
	XML_node* mFirstChild, * mLastChild;	// Handles for children linked chain
public:
	/*
		@function XML_document creates an empty Document with nullptr initialized chain handles
	*/
	XML_document();
	/*
		@function ~XML_document destroys the object and destroys every children
	*/
	~XML_document();

	/*
		@function parse extracts all xml information from a string begining at a certain point

		@param str : The position iterator in the string to parse 

		@return : The incremented iterator
	*/
	char* parse(char* str);

	/*
		@function add_firstChild adds a child node to the node at the first position in the linked chain

		@param node : A pointer to the node to add
	*/
	void add_firstChild(XML_node* node);
	/*
		@function add_lastChild adds a child node to the node at the last position in the linked chain

		@param node : A pointer to the node to add
	*/	
	void add_lastChild(XML_node* node);
	/*
		@function add_lastChild adds a child node to the node after a known node in the linked chain

		@param at : 	A pointer to the node after which to add the desired node
		@param node : 	A pointer to the node to add
	*/	
	void add_child(XML_node* at, XML_node* node);

	/*
		@function firstChild 

		@return : The first child in the children linked list
	*/
	XML_node* firstChild() {return mFirstChild; };
	/*
		@function lastChild 

		@return : The last child in the children linked list
	*/	
	XML_node* lastChild() { return mLastChild; };

	/*
		@function get_child_by_name attempts to find a child element with the required name

		@param name : The searched name 

		@return : The element with the corresponding name, or nullptr
	*/
	XML_element* get_child_by_name(std::string const& name);

	/*
		@function print prints the xml Document in a non-xml-conform way, it just display data
	*/
	void print();
	/*
		@function save writes the Document as an xml file 

		@param filePath : The location of the file to save
	*/
	void save(std::string const& filePath);
};

#endif