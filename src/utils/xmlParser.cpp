
#include "xmlParser.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SINGLE_QUOTES 1
#define DOUBLE_QUOTES 2

/*
	@function skip_whit_spaces is a helper function to increment the position in the string to the next non-white character

	@param it : The position iterator in the string

	@return : The incremented iterator at a non-white character
*/
char* skip_white_spaces(char* it)
{
	while(*it == ' ' || *it == '\t' || *it == '\n') {
		it++;
	}
	return it;
}
/*
	@function skip_whit_spaces is a helper function to increment the position in the string to the next specified character

	@param it : 	The position iterator in the string
	@param stop : 	The stop character

	@return : The incremented iterator at said character, or the end
*/
char* skip_to(char* it, char stop)
{
	while(it && *it != stop) {
		it++;
	}
	return it;
}
/*
	@function no_leading_nor_trailing_spaces creates a string whithout leadind nor trailing space 

	@param it_fist : The first position in the input string
	@param it_last : The last position in the input string

	@return : A trimed string

	@note : This is highly inconsistent with the char*s and strings and all
*/
std::string no_leading_nor_trailing_spaces(char* it_first, char* it_last)
{
	// Move the first position to the first non-white space (basically skip_white_spaces)
	while(it_first && (*it_first == ' ' || *it_first == '\t' || *it_first == '\n')) {
		it_first++;	
	}
	// Move the last position to the last non-white space (basically skip_white_spaces inverted)
	while((it_last-1) && (*(it_last-1) == ' ' || *(it_last-1) == '\t' || *(it_last-1) == '\n')) {
		it_last--;
	}

	if(it_first && it_last) {
		return std::string(it_first, it_last); // Create a string with the incremented / decremented characters
	}
	return std::string();
}
/*
	@function print_tabs is a helper function to print nodes and document, it prints the required amount of tabs

	@param tabs : The number of tabs to print
*/
void print_tabs(int tabs)
{
	for(int i = 0; i < tabs; ++i) {
		std::cout<<'\t';
	}
}

XML_node::XML_node():
	mDocument(nullptr), // Everything shall be
	mPrevNode(nullptr), // set to nullptr
	mNextNode(nullptr),
	mFirstChild(nullptr),
	mLastChild(nullptr),
	mParent(nullptr)
{
	
}
XML_node::~XML_node()
{
	XML_node* child = firstChild();
	while(child) { // for each child, delete it
		XML_node* next = child->next();
		delete child;
		child = next;
	}
}

void XML_node::unlink()
{
	if(mParent == this) {
		// Set the node's handle handles to valid values
		if(mPrevNode && mPrevNode->mNextNode)
			mPrevNode->mNextNode = mNextNode;
		if(mNextNode && mNextNode->mPrevNode)
			mNextNode->mPrevNode = mPrevNode;

		// Reset the handles
		mPrevNode = nullptr;
		mNextNode = nullptr;
		mParent = nullptr;
		mDocument = nullptr;
	}
}

void XML_node::add_firstChild(XML_node* node)
{
	node->unlink(); // The goal is to add a node that is alone

	if(mFirstChild) {
		mFirstChild->mPrevNode = node; // The first node is no longer first
		node->mPrevNode = nullptr; // The added node has no previous node (should be taken care of in unlink)
		node->mNextNode = mFirstChild; // The former first node is now the second node
		mFirstChild = node; // The first node is now the added node
	} else { // The chain is empty, pretty simple case
		mFirstChild = mLastChild = node;
	}
	node->mParent = this;
}
void XML_node::add_lastChild(XML_node* node)
{
	node->unlink(); // The node shall be alone

	if(mLastChild) {
		mLastChild->mNextNode = node; // The former last child will have the added node as a next
		node->mPrevNode = mLastChild; // The node is last, so the las it it's prev
		node->mNextNode = nullptr; // As the last node, it doesn't have a next (should be taken care of in unlink)
		mLastChild = node; // Last child is now the added node
	} else { // The chain is empty, simple case
		mLastChild = mFirstChild = node;
	}
	node->mParent = this;
}
void XML_node::add_child(XML_node* at, XML_node* node)
{
	node->unlink(); // The node should be alone

	if(at->mParent == this) { // Cannot add a node after a node that doesn't belong to this node
		// Make space for the new node
		at->mNextNode->mPrevNode = node;
		node->mNextNode = at->mPrevNode->mNextNode;
		at->mNextNode = node; // put node in created spot
		node->mPrevNode = at;
		node->mParent = this;
	}
}
XML_element* XML_node::get_child_by_name(std::string const& name)
{
	XML_node* node = mFirstChild;
	while(node) { // Iterate through all children
		XML_element* elem = dynamic_cast<XML_element*>(node); // Check if it is an element
		if(elem && elem->name() == name) { // Actually check the name
			return elem;
		} 
		node = node->next();
	}
	return nullptr;
}

char* XML_node::create_classifiedNode(char* it, XML_node** node)
{
	// Compare the string with all known start sequences
	if(strncmp(it, "<!--", 4) == 0) {
		it+=4;
		*node = new XML_comment();
	} else if(strncmp(it, "<?", 2) == 0) {
		it+=2;
		*node = new XML_declaration();
	} else if(strncmp(it, "<", 1) == 0) { // This is the last because it works for the other two
		it+=1;
		*node = new XML_element();
	} else {
		*node = nullptr;
	}
	return it;
}


XML_attribute::XML_attribute():
	mPrevAttr(nullptr), // Nullptr for the win!
	mNextAttr(nullptr)
{

}
XML_attribute::XML_attribute(std::string const& name_, std::string const& value_):
	mPrevAttr(nullptr),
	mNextAttr(nullptr),
	mName(name_),
	mValue(value_)
{

}

XML_element::XML_element():
	mFirstAttrib(nullptr),
	mLastAttrib(nullptr),
	mContent(""),
	mName("unamednode"),
	mClosingType(OPENED),
	mStartString("<"), // Simple opening sequence
	mSelfCloseString("/>") // Simple closing sequence
{

}
XML_element::XML_element(std::string const& name_):
	mFirstAttrib(nullptr),
	mLastAttrib(nullptr),
	mContent(""),
	mName(name_),
	mClosingType(OPENED),
	mStartString("<"), // Simple opening sequence
	mSelfCloseString("/>") // Simple closing sequence
{

}

XML_element::~XML_element()
{
	XML_attribute* attrib = firstAttribute();
	while(attrib) { // Iterate through all attribute
		XML_attribute* next = attrib->next();
		delete attrib; // Delete will call the destructor
		attrib = next;
	}
}

void XML_element::set_attribute(std::string const& name, std::string const& value)
{
	XML_attribute* attrib = firstAttribute();
	while(attrib) { // Iterate through all attribute
		if(attrib->name() == name) {
			attrib->set_value(value);
			return;
		}
		attrib = attrib->next();;
	}
	XML_attribute* newattrib = new XML_attribute(name, value);
	add_lastAttribute(newattrib);
}


void XML_element::add_firstAttribute(XML_attribute* attrib)
{
	if(mFirstAttrib) {
		mFirstAttrib->mPrevAttr = attrib; // First attribute will become the second
		attrib->mPrevAttr = nullptr; // The new first attribute will not have prev
		attrib->mNextAttr = mFirstAttrib; // The former first is now second
		mFirstAttrib = attrib; // First attrib is the added one
	} else {
		mFirstAttrib = mLastAttrib = attrib; // Simple case ; Chain is empty
	}
	attrib->mParent = this;
}
void XML_element::add_lastAttribute(XML_attribute* attrib)
{
	if(mLastAttrib) {
		mLastAttrib->mNextAttr = attrib; // The last attribute becomes the one before the last one
		attrib->mPrevAttr = mLastAttrib; // The former last attribute becomes the previous of the new one
		attrib->mNextAttr = nullptr; // The new last attribute doesn't have a next
		mLastAttrib = attrib; // The last is the assigned
	} else {
		mLastAttrib = mFirstAttrib = attrib; // Simple cast ; Chain is empty
	}
	attrib->mParent = this;
}
void XML_element::add_attribute(XML_attribute* at, XML_attribute* attr)
{
	if(at->mParent == this) { // Can't add attribute after node not owned by this node
		// Make space for the new node and put it there
		at->mNextAttr->mPrevAttr = attr;
		attr->mNextAttr = at->mPrevAttr->mNextAttr;
		at->mNextAttr = attr;
		attr->mPrevAttr = at;
		attr->mParent = this;
	}
}
XML_attribute* XML_element::get_attribute_by_name(std::string const& name)
{
	XML_attribute* attrib = mFirstAttrib;
	while(attrib) { // Iterate through all attribs
		if(attrib->name() == name) { // Check for their name
			return attrib;
		} 
		attrib = attrib->next();
	}
	return nullptr;	
}

char* XML_element::parse_attributes(char* it)
{
	it = skip_white_spaces(it); // No nasty whitespace will ruin this function

	int inquotes = 0; // If the iterator in quoted content, and if so, what type
	bool insegment = false; // If the iterator is in an attribute name
	int currSegmentLength = 0; // Length since last time the string was sampled to be put... anywhere really
	while(*it != '\0') { // Account for nullterminated strings (it would indeed be an ill formed xml file if the end was reached in the attribute parser.. but who knows?)
		if((inquotes == SINGLE_QUOTES && *(it) == '\'') || (inquotes == DOUBLE_QUOTES && *(it) == '\"')) { // Exiting quotes
			mLastAttrib->set_value(std::string(it-currSegmentLength+1, currSegmentLength-1)); //Set the Attrib's value since the quote ended
			currSegmentLength = 0; // Reset the length of the next "thing"
			inquotes = false; 
			insegment = false; // Until an interesting character shows up, the iterator is not in segment
		} else if(!inquotes) {
			if(*it == '\'') { // Now in single quotes!
				currSegmentLength = 0; // Record from this point
				inquotes = SINGLE_QUOTES;
			} else if(*it == '\"') { // Now in double quotes!
				currSegmentLength = 0; // Record from this point
				inquotes = DOUBLE_QUOTES;
			} else if(!insegment && isalnum(*it)) { // If it is in segment and an alphanumeric character shows up.. it's normal (should it account for underscores?)
				currSegmentLength = 0;
				insegment = true;
			} else if(!isalnum(*it)) {
				if(insegment) { // Not alnum is the end of the name (should it account for underscore?)
					add_lastAttribute(new XML_attribute()); // Push an attribute
					mLastAttrib->set_name(std::string(it-currSegmentLength, currSegmentLength)); // Set it's name
					insegment = false;
					currSegmentLength = 0;
				}

				if(*it+mSelfCloseString.length() && strncmp(it, mSelfCloseString.c_str(), mSelfCloseString.length()) == 0) { // If the iterator is at /> or similar, it is a closed node
					set_closingType(XML_element::CLOSED);
					it+=2;
					break;
				} else if(*it == '>') { // This is an opened node e.g <div><innernode/></div>
					set_closingType(XML_element::OPENED);
					it++;
					break;
				}
			}
		}
		currSegmentLength++; // The current *thing* being recorded is one longer
		it++; // Advance in file
	}
	return it;
}
char* XML_element::parse_name(char* it)
{
	int lenght = 0;
	while(isalnum(*(it+lenght))) { // Find the non alpha numeric character
		lenght++;
	}
	set_name(std::string(it, lenght)); // Create a string from the begining to the non alpha numeric character
	it+=lenght;
	return it;
}
char* XML_element::check_endTag(char* it, const char* stop, int stop_len, bool& out)
{
	if(strncmp(it, "</", 2) == 0 && strncmp(it+2, stop, stop_len) == 0) { // Find end tag of form </div>
		out = true;
		return skip_to(it+2+stop_len, '>')+1; // Go to the end of the closing tag
	}
	out = false;
	return it;
}
char* XML_element::parse(char* it)
{
	it = skip_white_spaces(it); // White spaces are the devil
	it = parse_name(it); // Find the name
	it = parse_attributes(it); // Find the attributes
	if(mClosingType == OPENED) { // If it can have children, parse children until the end tag is reached
		XML_node* node = nullptr;
		do {
			it = skip_white_spaces(it); // Them white spaces amirite?
			char* it_tmp = it; // Save it to create the content of the 
			it = skip_to(it, '<'); // add the content until the begining of a markup (kind of wack for multiline content)
			if(it != it_tmp) { // If the iterator is at the markup, add the last stretch to the content
				mContent += no_leading_nor_trailing_spaces(it_tmp, it);
			}
			// Has the end tag been reached?
			bool ex;
			it = check_endTag(it, mName.c_str(), mName.length(), ex);
			if(ex) {
				return it;
			}
			it = create_classifiedNode(it, &node); // Create a specialized node

			if(node) { // Check if the node was created
				it = node->parse(it); // Parse it
				add_lastChild(node);
			}
			
			it = skip_white_spaces(it); // They trully are the worst
			it = check_endTag(it, mName.c_str(), mName.length(), ex); // Check again, maybe it is superflouous
			if(ex) {
				return it;
			}
		} while(node);
	}
	return it;
}

void XML_element::deduce_closingType()
{
	if(mFirstChild == nullptr) {
		mClosingType = CLOSED;
	} else {
		mClosingType = OPENED;
	}
}

void XML_element::print(int tabs)
{
	print_tabs(tabs);
	std::cout<<" "<<mName<<" => <"<<mContent<<">\n";
	XML_attribute* attrib = mFirstAttrib;
	while(attrib) { // Print all attribs
		print_tabs(tabs+1);
		std::cout<<"["<<attrib->name()<<",  "<<attrib->value()<<"]\n";
		attrib = attrib->mNextAttr;
	}
	XML_node* child = mFirstChild;
	while(child) { // Print all the children nodes with one more tab
		child->print(tabs+1);
		child = child->next();
	}
}
std::string XML_element::to_xml(int level)
{
	deduce_closingType();

	std::string level_str = ""; // Helper string to do all the tabulations
	for(int l = 0; l < level; ++l)
		level_str += '\t';
	std::string out = level_str + mStartString + mName; // Open the tag and add the name : <div

	XML_attribute* attrib = mFirstAttrib;
	while(attrib) { // Add the attributes : <div attrib="blop" 
		out += " " + attrib->name() + "=\"" + attrib->mValue + "\"";
		attrib = attrib->next();
	}

	if(mClosingType == OPENED) {
		out+=">"; // Close the opening tag : <div attrib="blop">
		out+=mContent; // Add the content : <div attrib="blop">content

		// Add all the children with one tab
		XML_node* child = mFirstChild;
		bool had_child = child;
		while(child) {
			out += "\n" + child->to_xml(level+1);
			child = child->next();
		}
		if(had_child) // If it had a child, add a line break before closing
			out += "\n" + level_str;
		out += "</" + mName + ">"; 
	} else {
		out += mSelfCloseString; 
	}
	
	return out;
}

XML_comment::XML_comment()
{

}
XML_comment::~XML_comment()
{

}
char* XML_comment::parse(char* it)
{
	// Just add content until "-->" is reached
	int length = 0;
	while(it+3+length && strncmp(it+length, "-->", 3) != 0) {
		length++;
	}
	if(it) {
		mContent = std::string(it, length);
		it += length + 3;
	}
	return it;
}

void XML_comment::print(int tabs)
{
	print_tabs(tabs);
	std::cout<<"Comment:\n";
	print_tabs(tabs+1);
	for(int i = 0; i < mContent.length(); ++i) {
		std::cout<<mContent[i];
		if(mContent[i] == '\n') // If it is a multiline comment, add tabs
			print_tabs(tabs+1);
	}
	std::cout<<"\n";
}
std::string XML_comment::to_xml(int level)
{	
	std::string level_str = "";
	for(int l = 0; l < level; ++l)
		level_str += '\t';
	
	return level_str + "<!--" + mContent + "-->"; // Kind of wack for multiline comments but alright
}

XML_document::XML_document():
	mFirstChild(nullptr),
	mLastChild(nullptr)
{

}
XML_document::~XML_document()
{
	XML_node* child = firstChild();
	while(child) { // Go through every all children
		XML_node* next = child->next();
		delete child; // Delete calls the destructor
		child = next;
	}
}

char* XML_document::parse(char* str)
{
	char* it = str;	
	XML_node* node = nullptr;
	do {
		it = skip_white_spaces(it); // White spaces are the worst
		it = XML_node::create_classifiedNode(it, &node); // Create a specialized node
		XML_element* elem = dynamic_cast<XML_element*>(node); // It it is 
		if(elem) {
			it = elem->parse(it);
			add_lastChild(elem);
		}
	} while(node);
	return it;
}

void XML_document::add_firstChild(XML_node* node)
{
	node->unlink(); // The added should be alone

	if(mFirstChild) {
		mFirstChild->mPrevNode = node; // The former first node will have this node as prev
		node->mPrevNode = nullptr; // The new first node doesn't have a prev (should be taken care of in unlink)
		node->mNextNode = mFirstChild; // The former first node is the new first node's next node
		mFirstChild = node;	 // The first node is now the new node
	} else {
		mFirstChild = mLastChild = node; // Simple case ; empty chain
	}
	node->mParent = nullptr; // Top level node
	node->mDocument = this;
}
void XML_document::add_lastChild(XML_node* node)
{
	node->unlink(); // Add a node that is alone and can't be accessed

	if(mLastChild) {
		mLastChild->mNextNode = node; // The former last node has the new node as next
		node->mPrevNode = mLastChild; // The new last node has the former last node as prev
		node->mNextNode = nullptr; // The new last node doesn't have a next (should be taken care of in unlink)
		mLastChild = node; // The last child is now the added node
	} else {
		mLastChild = mFirstChild = node;
	}
	node->mParent = nullptr; // Top level node
	node->mDocument = this;
}
void XML_document::add_child(XML_node* at, XML_node* node)
{
	node->unlink();

	if(at->mDocument == this) {
		// Create space for node and add it
		at->mNextNode->mPrevNode = node;
		node->mNextNode = at->mPrevNode->mNextNode;
		at->mNextNode = node;
		node->mPrevNode = at;
		node->mParent = nullptr; // Top level node
		node->mDocument = this;
	}
}

XML_element* XML_document::get_child_by_name(std::string const& name)
{
	XML_node* node = mFirstChild;
	while(node) { // GO through all the children nodes
		XML_element* elem = dynamic_cast<XML_element*>(node); // Attempt a cast into element
		if(elem && elem->name() == name) { // If it is an element check it's name
			return elem;
		} 
		node = node->next();
	}
	return nullptr;
}

void XML_document::print()
{
	XML_node* child = mFirstChild;
	while(child) { // Go through every child and print it
		child->print();
		child = child->next();
	}
}

void XML_document::save(std::string const& filePath)
{
	std::ofstream stream(filePath);
	XML_node* child = mFirstChild;
	while(child) { // GO through every child and ask for it's xml transcription
		stream<<child->to_xml()<<"\n"<<std::endl;
		child = child->next();
	}
	stream.close();
}