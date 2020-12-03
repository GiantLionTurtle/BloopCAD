
#ifndef XMLPARSER_HPP_
#define XMLPARSER_HPP_

#include <string>

class XML_document;
class XML_element;

class XML_node {
public:
	friend class XML_document;
protected:
	XML_document* mDocument;

	XML_node* mParent;
	XML_node* mPrevNode, *mNextNode; 		// Handles for linked chain
	XML_node* mFirstChild, * mLastChild;	// Handles for children linked chain
public:
	XML_node();
	virtual ~XML_node();

	void set_doc(XML_document* doc) { mDocument = doc; }

	void unlink(XML_node* node);
	static void unlink_stranded(XML_node* node);

	void add_firstChild(XML_node* node);
	void add_lastChild(XML_node* node);
	void add_child(XML_node* at, XML_node* node);
	XML_element* get_child_by_name(std::string const& name);


	static char* create_classified_node(char* it, XML_node** node);
	virtual char* parse(char* it) = 0;

	XML_node* firstChild() {return mFirstChild; };
	XML_node* lastChild() { return mLastChild; };

	XML_node* prev() { return mPrevNode; }
	XML_node* next() { return mNextNode; }

	virtual void print(int tabs = 0) = 0;
	virtual std::string to_xml(int level = 0) = 0;
};

class XML_attribute {
public:
	friend class XML_element;
private:
	XML_element* mParent;
	XML_attribute* mPrevAttr, * mNextAttr;

	std::string mName, mValue;
public:
	XML_attribute();

	XML_attribute* prev() { return mPrevAttr; }
	XML_attribute* next() { return mNextAttr; }

	std::string name() { return mName; }
	std::string value() { return mValue; }
	void set_name(std::string name) { mName = name; }
	void set_value(std::string val) { mValue = val; }
};

class XML_element : public XML_node {
public:
	enum closing_types {CLOSED, OPENED};
protected:
	closing_types mClosingType;

	XML_attribute* mFirstAttrib, * mLastAttrib;

	std::string mName;
	std::string mContent;

	std::string mStartString, mSelfCloseString;
public:
	XML_element();
	~XML_element();

	void set_name(std::string name) { mName = name; }
	std::string name() const { return mName; }

	std::string content() const { return mContent; }

	void set_closingType(closing_types type) { mClosingType = type; }
	closing_types closingType() { return mClosingType; }

	void add_firstAttribute(XML_attribute* attr);
	void add_lastAttribute(XML_attribute* attr);
	void add_attribute(XML_attribute* at, XML_attribute* attr);

	XML_attribute* get_attribute_by_name(std::string const& name);

	XML_attribute* firstAttribute() { return mFirstAttrib; }
	XML_attribute* lastAttribute() { return mLastAttrib; }

	char* parse_attributes(char* it);
	char* parse_name(char* it);
	char* check_endTag(char* it, const char* stop, int stop_len, bool& out);
	virtual char* parse(char* it);

	virtual void print(int tabs = 0);
	virtual std::string to_xml(int level = 0);
};

class XML_comment : public XML_node {
private:
	std::string mContent;
public:
	XML_comment();
	~XML_comment();

	virtual char* parse(char* it);
	virtual void print(int tabs = 0);
	virtual std::string to_xml(int level = 0);
};

class XML_declaration : public XML_element {
public:
	XML_declaration() { mStartString = "<?"; mSelfCloseString = "?>"; }
};

class XML_document {
private:
	XML_node* mFirstChild, * mLastChild;	// Handles for children linked chain
public:
	XML_document();
	~XML_document();
	
	void parse(char* str);

	void add_firstChild(XML_node* node);
	void add_lastChild(XML_node* node);
	void add_child(XML_node* at, XML_node* node);

	XML_node* firstChild() { return mFirstChild; }
	XML_node* lastChild() { return mLastChild; }

	XML_element* get_child_by_name(std::string const& name);

	void print();
	void save(std::string const& filePath);
};

#endif