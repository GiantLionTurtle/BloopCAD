
#ifndef DOCUMENTTREE_HPP_
#define DOCUMENTTREE_HPP_

#include <Drawables/Base/Drawable.hpp>

#include <gtkmm.h>

class Document;
class DocumentTree;

class DocumentNode : public Gtk::Box {
private:
	Gtk::Box mBody;
	Gtk::Label mCollapser, mContent;
	Gtk::EventBox mCollapserEvents, mBodyEvents;

	DocumentTree* mTree;
	DocumentNode* mParent;
	std::vector<DocumentNode*> mChildren;

	bool mCollapsed;
	int mLevel;
public:
	DocumentNode(std::string label, bool collapsed = false);
	~DocumentNode();

	void add_node(DocumentNode* node);

	void do_realize();

	bool manage_hover(GdkEventCrossing* event);
	bool manage_collapse(GdkEventButton* event);
	virtual bool manage_selection(GdkEventButton* event);
	virtual void set_hover(bool hov);
	virtual void set_selected(bool sel);

	void set_hover_recursive(bool hov);
	void set_selected_recursive(bool sel);

	void collapse();
	void expend();
	void hide_recursive();
	void show_recursive();

	DocumentNode* parent() const { return mParent; }
	void set_parent(DocumentNode* parent_);

	DocumentTree* tree() const { return mTree; }
	void set_tree(DocumentTree* tree_);
};

class DocumentDrawableNode : public DocumentNode, public UILink {
private:
	Drawable* mDrw;
public:
	DocumentDrawableNode(std::string str, Drawable* drw, bool collapsed = false);
	DocumentDrawableNode(Drawable* drw, bool collapsed = false);

	bool manage_selection(GdkEventButton* event);

	virtual void set_hover(bool hov);
	virtual void set_selected(bool sel);

	virtual void notify_hover(bool hov);
	virtual void notify_select(bool sel);
};

class DocumentTree : public Gtk::Box {
private:
	DocumentNode mRoot;
	Document* mDoc;

	DocumentNode* mLastSelected;
public:
	DocumentTree(Document* doc_);
	~DocumentTree();

	Document* doc() { return mDoc; }
	DocumentNode& root() { return mRoot; }

	void add_node(DocumentNode* node);

	void set_lastSelected(DocumentNode* node);
};


#endif