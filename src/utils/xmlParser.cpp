
#include "xmlParser.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SINGLE_QUOTES 1
#define DOUBLE_QUOTES 2

char* skip_white_spaces(char* it)
{
	while(*it == ' ' || *it == '\t' || *it == '\n') {
		it++;
	}
	return it;
}
char* skip_to(char* it, char stop)
{
	while(it && *it != stop) {
		it++;
	}
	return it;
}
std::string no_leading_nor_trailing_spaces(char* it_first, char* it_last)
{
	while(it_first && (*it_first == ' ' || *it_first == '\t' || *it_first == '\n')) {
		it_first++;	
	}
	while((it_last-1) && (*(it_last-1) == ' ' || *(it_last-1) == '\t' || *(it_last-1) == '\n')) {
		it_last--;	
	}

	if(it_first && it_last) {
		return std::string(it_first, it_last);
	}
	return std::string();
}

void print_tabs(int tabs)
{
	for(int i = 0; i < tabs; ++i) {
		std::cout<<'\t';
	}
}

XML_node::XML_node():
	mDocument(nullptr),
	mPrevNode(nullptr),
	mNextNode(nullptr),
	mFirstChild(nullptr),
	mLastChild(nullptr),
	mParent(nullptr)
{
	
}

void XML_node::unlink(XML_node* node)
{
	if(node->mParent == this) {
		node->mPrevNode->mNextNode = node->mNextNode;
		node->mNextNode->mPrevNode = node->mPrevNode;

		node->mPrevNode = nullptr;
		node->mNextNode = nullptr;
	}
}
void XML_node::unlink_stranded(XML_node* node)
{
	if(node->mParent) {

		node->mParent->unlink(node);
		node->mParent = nullptr;
	}
}

void XML_node::add_firstChild(XML_node* node)
{
	unlink_stranded(node);

	if(mFirstChild) {
		mFirstChild->mPrevNode = node;
		node->mPrevNode = nullptr;
		node->mNextNode = mFirstChild;
		mFirstChild = node;
	} else {
		mFirstChild = mLastChild = node;
	}
	node->mParent = this;
}
void XML_node::add_lastChild(XML_node* node)
{
	unlink_stranded(node);

	if(mLastChild) {
		mLastChild->mNextNode = node;
		node->mPrevNode = mLastChild;
		node->mNextNode = nullptr;
		mLastChild = node;
	} else {
		mLastChild = mFirstChild = node;
	}
	node->mParent = this;
}
void XML_node::add_child(XML_node* at, XML_node* node)
{
	unlink_stranded(node);

	if(at->mParent == this) {
		at->mNextNode->mPrevNode = node;
		node->mNextNode = at->mPrevNode->mNextNode;
		at->mNextNode = node;
		node->mPrevNode = at;
		node->mParent = this;
	}
}

char* XML_node::create_classified_node(char* it, XML_node** node)
{
	if(strncmp(it, "<!--", 4) == 0) {
		it+=4;
		*node = new XML_comment();
	} else if(strncmp(it, "<?", 2) == 0) {
		it+=2;
		*node = new XML_declaration();
	} else if(strncmp(it, "<", 1) == 0) {
		it+=1;
		*node = new XML_element();
	} else {
		*node = nullptr;
	}
	return it;
}


XML_attribute::XML_attribute():
	mPrevAttr(nullptr),
	mNextAttr(nullptr)
{

}

XML_element::XML_element():
	mFirstAttrib(nullptr),
	mLastAttrib(nullptr),
	mContent(""),
	mName(""),
	mStartString("<"),
	mSelfCloseString("/>")
{

}

void XML_element::add_firstAttribute(XML_attribute* attr)
{
	if(mFirstAttrib) {
		mFirstAttrib->mPrevAttr = attr;
		attr->mPrevAttr = nullptr;
		attr->mNextAttr = mFirstAttrib;
		mFirstAttrib = attr;
	} else {
		mFirstAttrib = mLastAttrib = attr;
	}
	attr->mParent = this;
}
void XML_element::add_lastAttribute(XML_attribute* attr)
{
	if(mLastAttrib) {
		mLastAttrib->mNextAttr = attr;
		attr->mPrevAttr = mLastAttrib;
		attr->mNextAttr = nullptr;
		mLastAttrib = attr;
	} else {
		mLastAttrib = mFirstAttrib = attr;
	}
	attr->mParent = this;
}
void XML_element::add_attribute(XML_attribute* at, XML_attribute* attr)
{
	if(at->mParent == this) {
		at->mNextAttr->mPrevAttr = attr;
		attr->mNextAttr = at->mPrevAttr->mNextAttr;
		at->mNextAttr = attr;
		attr->mPrevAttr = at;
		attr->mParent = this;
	}
}

char* XML_element::parse_attributes(char* it)
{
	it = skip_white_spaces(it);
	int inquotes = 0;
	bool insegment = false;
	int currSegmentLength = 0;
	while(*it != '\0') {
		if((inquotes == SINGLE_QUOTES && *(it) == '\'') || (inquotes == DOUBLE_QUOTES && *(it) == '\"')) {
			mLastAttrib->set_value(std::string(it-currSegmentLength+1, currSegmentLength-1));
			currSegmentLength = 0;
			inquotes = false;
			insegment = false;
		} else if(!inquotes) {
			if(*it == '\'') {
				currSegmentLength = 0;
				inquotes = SINGLE_QUOTES;
			} else if(*it == '\"') {
				currSegmentLength = 0;
				inquotes = DOUBLE_QUOTES;
			} else if(!insegment && isalnum(*it)) {
				currSegmentLength = 0;
				insegment = true;
			} else if(!isalnum(*it)) {
				if(insegment) {
					add_lastAttribute(new XML_attribute());
					mLastAttrib->set_name(std::string(it-currSegmentLength, currSegmentLength));
					insegment = false;
					currSegmentLength = 0;
				}

				if(*it+mSelfCloseString.length() && strncmp(it, mSelfCloseString.c_str(), mSelfCloseString.length()) == 0) {
					set_closingType(XML_element::CLOSED);
					it+=2;
					break;
				} else if(*it == '>') {
					set_closingType(XML_element::OPENED);
					it++;
					break;
				}
			}
		}
		currSegmentLength++;
		it++;
	}
	return it;
}
char* XML_element::parse_name(char* it)
{
	int lenght = 0;
	while(isalnum(*(it+lenght))) {
		lenght++;
	}
	set_name(std::string(it, lenght));
	it+=lenght;
	return it;
}
char* XML_element::check_endTag(char* it, const char* stop, int stop_len, bool& out)
{
	if(strncmp(it, "</", 2) == 0 && strncmp(it+2, stop, stop_len) == 0) {
		out = true;
		return skip_to(it+2+stop_len, '>')+1;
	}
	out = false;
	return it;
}
char* XML_element::parse(char* it)
{
	it = skip_white_spaces(it);
	it = parse_name(it);
	it = parse_attributes(it);
	if(mClosingType == OPENED) {
		XML_node* node = nullptr;
		do {
			it = skip_white_spaces(it);
			char* it_tmp = it;
			it = skip_to(it, '<');
			if(it != it_tmp) {
				mContent += no_leading_nor_trailing_spaces(it_tmp, it);
			}

			bool ex;
			it = check_endTag(it, mName.c_str(), mName.length(), ex);
			if(ex) {
				return it;
			}
			it = create_classified_node(it, &node);

			if(node) {
				it = node->parse(it);
				add_lastChild(node);
			}
			
			it = skip_white_spaces(it);
			it = check_endTag(it, mName.c_str(), mName.length(), ex);
			if(ex) {
				return it;
			}
		} while(node);
	}
	return it;
}

void XML_element::print(int tabs)
{
	print_tabs(tabs);
	std::cout<<" "<<mName<<" => <"<<mContent<<">\n";
	XML_attribute* attrib = mFirstAttrib;
	while(attrib) {
		print_tabs(tabs+1);
		std::cout<<"["<<attrib->name()<<",  "<<attrib->value()<<"]\n";
		attrib = attrib->mNextAttr;
	}
	XML_node* child = mFirstChild;
	while(child) {
		child->print(tabs+1);
		child = child->next();
	}
}
std::string XML_element::to_xml(int level)
{
	std::string level_str = "";
	for(int l = 0; l < level; ++l)
		level_str += '\t';
	std::string out = level_str + mStartString + mName;

	XML_attribute* attrib = mFirstAttrib;
	while(attrib) {
		out += " " + attrib->name() + "=\"" + attrib->mValue + "\"";
		attrib = attrib->next();
	}

	if(mClosingType == OPENED) {
		out+=">";
		out+=mContent;
		XML_node* child = mFirstChild;
		bool had_child = child;
		while(child) {
			out += "\n" + child->to_xml(level+1);
			child = child->next();
		}
		if(had_child)
			out += "\n" + level_str;
		out += "</" + mName + ">";
	} else {
		out += mSelfCloseString;
	}
	
	return out;
}

char* XML_comment::parse(char* it)
{
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
		if(mContent[i] == '\n')
			print_tabs(tabs+1);
	}
	std::cout<<"\n";
}
std::string XML_comment::to_xml(int level)
{
	std::string level_str = "";
	for(int l = 0; l < level; ++l)
		level_str += '\t';
	
	return level_str + "<!--" + mContent + "-->";
}

XML_document::XML_document():
	mFirstChild(nullptr),
	mLastChild(nullptr)
{

}

void XML_document::parse(char* str)
{
	char* it = str;
	
	XML_node* node = nullptr;
	do {
		it = skip_white_spaces(it);
		it = XML_node::create_classified_node(it, &node);
		XML_element* elem = dynamic_cast<XML_element*>(node);
		if(elem) {
			it = elem->parse(it);
			add_lastChild(elem);
		}
	} while(node);
}

void XML_document::add_firstChild(XML_node* node)
{
	XML_node::unlink_stranded(node);

	if(mFirstChild) {
		mFirstChild->mPrevNode = node;
		node->mPrevNode = nullptr;
		node->mNextNode = mFirstChild;
		mFirstChild = node;
	} else {
		mFirstChild = mLastChild = node;
	}
	node->mParent = nullptr;
	node->mDocument = this;
}
void XML_document::add_lastChild(XML_node* node)
{
	XML_node::unlink_stranded(node);

	if(mLastChild) {
		mLastChild->mNextNode = node;
		node->mPrevNode = mLastChild;
		node->mNextNode = nullptr;
		mLastChild = node;
	} else {
		mLastChild = mFirstChild = node;
	}
	node->mParent = nullptr;
	node->mDocument = this;
}
void XML_document::add_child(XML_node* at, XML_node* node)
{
	XML_node::unlink_stranded(node);

	if(at->mDocument == this) {
		at->mNextNode->mPrevNode = node;
		node->mNextNode = at->mPrevNode->mNextNode;
		at->mNextNode = node;
		node->mPrevNode = at;
		node->mParent = nullptr;
		node->mDocument = this;
	}
}

void XML_document::print()
{
	XML_node* child = mFirstChild;
	while(child) {
		child->print();
		child = child->next();
	}
}

void XML_document::save(std::string const& filePath)
{
	std::ofstream stream(filePath);
	XML_node* child = mFirstChild;
	while(child) {
		stream<<child->to_xml()<<"\n"<<std::endl;
		child = child->next();
	}

	stream.close();
}
