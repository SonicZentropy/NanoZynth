/*==============================================================================
//  ValueTreeWindow.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/19
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Display of Value Tree for debugging, etc
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/
#ifndef VALUETREEWINDOW_H_INCLUDED
#define VALUETREEWINDOW_H_INCLUDED

#include "JuceHeader.h"

//==============================================================================
class ValueTreeItem : public TreeViewItem,
	private ValueTree::Listener
{
public:
	ValueTreeItem(const ValueTree& v, UndoManager& um)
		: tree(v), undoManager(um)
	{
		tree.addListener(this);
	}

	String getUniqueName() const override
	{
		return tree["name"].toString();
	}

	bool mightContainSubItems() override
	{
		return tree.getNumChildren() > 0;
	}

	void paintItem(Graphics& g, int width, int height) override
	{
		g.setColour(Colours::black);
		g.setFont(15.0f);
		
		String propertySummary = tree.getType().toString() + ": ";

		for (int i = 0; i < tree.getNumProperties(); ++i)
		{
			const Identifier name = tree.getPropertyName(i).toString();
			String propertyValue = tree.getProperty(name).toString();
			
			if (name.toString().equalsIgnoreCase("name"))
			{
				//propertySummary += " " + name.toString() + "=\"" + propertyValue + "\"";
			} else
			{
				propertySummary += " " + name.toString() + "=" + propertyValue.toStdString().substr(0, 8);
			}
			
		}

	

		g.drawText(propertySummary,
			4, 0, width - 4, height,
			Justification::centredLeft, true);
		
	}

	void itemOpennessChanged(bool isNowOpen) override
	{
		if (isNowOpen && getNumSubItems() == 0)
			refreshSubItems();
		else
			clearSubItems();
	}

	var getDragSourceDescription() override
	{
		return "Drag Demo";
	}

	bool isInterestedInDragSource(const DragAndDropTarget::SourceDetails& dragSourceDetails) override
	{
		return dragSourceDetails.description == "Drag Demo";
	}

	void itemDropped(const DragAndDropTarget::SourceDetails&, int insertIndex) override
	{
		moveItems(*getOwnerView(),
			getSelectedTreeViewItems(*getOwnerView()),
			tree, insertIndex, undoManager);
	}

	static void moveItems(TreeView& treeView, const Array<ValueTree>& items,
		ValueTree newParent, int insertIndex, UndoManager& undoManager)
	{
		if (items.size() > 0)
		{
			ScopedPointer<XmlElement> oldOpenness(treeView.getOpennessState(false));

			for (int i = items.size(); --i >= 0;)
			{
				ValueTree& v = items.getReference(i);

				if (v.getParent().isValid() && newParent != v && !newParent.isAChildOf(v))
				{
					if (v.getParent() == newParent && newParent.indexOf(v) < insertIndex)
						--insertIndex;

					v.getParent().removeChild(v, &undoManager);
					newParent.addChild(v, insertIndex, &undoManager);
				}
			}

			if (oldOpenness != nullptr)
				treeView.restoreOpennessState(*oldOpenness, false);
		}
	}

	static Array<ValueTree> getSelectedTreeViewItems(TreeView& treeView)
	{
		Array<ValueTree> items;

		const int numSelected = treeView.getNumSelectedItems();

		for (int i = 0; i < numSelected; ++i)
			if (const ValueTreeItem* vti = dynamic_cast<ValueTreeItem*> (treeView.getSelectedItem(i)))
				items.add(vti->tree);

		return items;
	}

private:
	ValueTree tree;
	UndoManager& undoManager;

	void refreshSubItems()
	{
		clearSubItems();

		for (int i = 0; i < tree.getNumChildren(); ++i)
			addSubItem(new ValueTreeItem(tree.getChild(i), undoManager));
	}

	void valueTreePropertyChanged(ValueTree&, const Identifier&) override
	{
		repaintItem();
	}

	void valueTreeChildAdded(ValueTree& parentTree, ValueTree&) override { treeChildrenChanged(parentTree); }
	void valueTreeChildRemoved(ValueTree& parentTree, ValueTree&, int) override { treeChildrenChanged(parentTree); }
	void valueTreeChildOrderChanged(ValueTree& parentTree, int, int) override { treeChildrenChanged(parentTree); }
	void valueTreeParentChanged(ValueTree&) override {}

	void treeChildrenChanged(const ValueTree& parentTree)
	{
		if (parentTree == tree)
		{
			refreshSubItems();
			treeHasChanged();
			setOpen(true);
		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeItem)
};

//==============================================================================
//==============================================================================
class ValueTreeDebugComponent : public Component,
	public DragAndDropContainer,
	private ButtonListener,
	private Timer
{
public:
	ValueTreeDebugComponent()
		: undoButton("Undo"),
		redoButton("Redo")
	{
		addAndMakeVisible(tree);

		tree.setDefaultOpenness(true);
		tree.setMultiSelectEnabled(true);
		tree.setRootItem(rootItem = new ValueTreeItem(createRootValueTree(), undoManager));
		tree.setColour(TreeView::backgroundColourId, Colours::white);

		addAndMakeVisible(undoButton);
		addAndMakeVisible(redoButton);
		undoButton.addListener(this);
		redoButton.addListener(this);

		startTimer(500);
		this->setSize(800, 800);
		this->setVisible(true);
	}

	ValueTreeDebugComponent(ValueTree* inValueTree)
		: undoButton("Undo"),
		redoButton("Redo")
	{
		addAndMakeVisible(tree);

		tree.setDefaultOpenness(true);
		tree.setMultiSelectEnabled(true);
		tree.setRootItem(rootItem = new ValueTreeItem(*inValueTree, undoManager));
		tree.setColour(TreeView::backgroundColourId, Colours::white);

		addAndMakeVisible(undoButton);
		addAndMakeVisible(redoButton);
		undoButton.addListener(this);
		redoButton.addListener(this);

		startTimer(500);
		this->setSize(800, 800);
		this->setVisible(true);
	}

	ValueTreeDebugComponent(ValueTree inValueTree)
		: undoButton("Undo"),
		redoButton("Redo")
	{
		addAndMakeVisible(tree);

		tree.setDefaultOpenness(true);
		tree.setMultiSelectEnabled(true);
		tree.setRootItem(rootItem = new ValueTreeItem(inValueTree, undoManager));
		tree.setColour(TreeView::backgroundColourId, Colours::white);

		addAndMakeVisible(undoButton);
		addAndMakeVisible(redoButton);
		undoButton.addListener(this);
		redoButton.addListener(this);

		startTimer(500);
		this->setSize(800, 800);
		this->setVisible(true);
	}


	~ValueTreeDebugComponent()
	{
		tree.setRootItem(nullptr);
	}

	void paint(Graphics& g) override
	{
		//fillTiledBackground(g);
		g.fillAll(Colour(0xff303030));
	}

	void resized() override
	{
		Rectangle<int> r(getLocalBounds().reduced(8));

		Rectangle<int> buttons(r.removeFromBottom(22));
		undoButton.setBounds(buttons.removeFromLeft(100));
		buttons.removeFromLeft(6);
		redoButton.setBounds(buttons.removeFromLeft(100));

		r.removeFromBottom(4);
		tree.setBounds(r);
	}

	static ValueTree createTree(const String& desc)
	{
		ValueTree t("Root");
		t.setProperty("name", desc, nullptr);
		return t;
	}

	static ValueTree createRootValueTree()
	{
		ValueTree vt = createTree("Root Value Tree");

		return vt;
	}

	static ValueTree createRandomTree(int& counter, int depth)
	{
		ValueTree t = createTree("Item " + String(counter++));

		if (depth < 3)
			for (int i = 1 + Random::getSystemRandom().nextInt(7); --i >= 0;)
				t.addChild(createRandomTree(counter, depth + 1), -1, nullptr);

		return t;
	}

	void deleteSelectedItems()
	{
		Array<ValueTree> selectedItems(ValueTreeItem::getSelectedTreeViewItems(tree));

		for (int i = selectedItems.size(); --i >= 0;)
		{
			ValueTree& v = selectedItems.getReference(i);

			if (v.getParent().isValid())
				v.getParent().removeChild(v, &undoManager);
		}
	}

	bool keyPressed(const KeyPress& key) override
	{
		if (key == KeyPress::deleteKey)
		{
			deleteSelectedItems();
			return true;
		}

		if (key == KeyPress('z', ModifierKeys::commandModifier, 0))
		{
			undoManager.undo();
			return true;
		}

		if (key == KeyPress('z', ModifierKeys::commandModifier | ModifierKeys::shiftModifier, 0))
		{
			undoManager.redo();
			return true;
		}

		if (key == KeyPress('r'))
		{
			DBG("R key pressed");
			rootItem->treeHasChanged();
			tree.repaint();
		}

		return Component::keyPressed(key);
	}

	void buttonClicked(Button* b) override
	{
		if (b == &undoButton)
			undoManager.undo();
		else if (b == &redoButton)
			undoManager.redo();
	}

private:
	TreeView tree;
	TextButton undoButton, redoButton;
	ScopedPointer<ValueTreeItem> rootItem;

	UndoManager undoManager;

	void timerCallback() override
	{
		undoManager.beginNewTransaction();
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeDebugComponent);
};

//==============================================================================


/** Display a separate desktop window for viewed and editing a value tree's
property fields.
*/
/*
class ValueTreeEditor : public DocumentWindow
{
	/ ** Display properties for a tree. * /
	class PropertyEditor : public PropertyPanel
	{
	public:
		PropertyEditor()
		{
			noEditValue = "not editable";
		}

		void setSource(ValueTree& tree)
		{
			clear();
			t = tree;
			const int maxChars = 200;
			Array<PropertyComponent *> pc;

			for (int i = 0; i < t.getNumProperties(); ++i)
			{
				const Identifier name = t.getPropertyName(i).toString();
				Value v = t.getPropertyAsValue(name, nullptr);
				TextPropertyComponent* tpc;

				if (v.getValue().isObject())
				{
					tpc = new TextPropertyComponent(noEditValue, name.toString(), maxChars, false);
					tpc->setEnabled(false);
				} else
				{
					tpc = new TextPropertyComponent(v, name.toString(), maxChars, false);
				}

				pc.add(tpc);
			}

			addProperties(pc);
		}

	private:
		Value noEditValue;
		ValueTree t;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyEditor);
	};

	class Item : public TreeViewItem,
		public ValueTree::Listener
	{
	public:
		Item(PropertyEditor* propertiesEditor, ValueTree tree)
			:propertiesEditor(propertiesEditor),
			t(tree)
		{
			t.addListener(this);
		}

		~Item()
		{
			clearSubItems();
		}

		bool mightContainSubItems() override
		{
			return t.getNumChildren() > 0;
		}

		void itemOpennessChanged(bool isNowOpen) override
		{
			clearSubItems();
			if (!isNowOpen) return;
			int children = t.getNumChildren();
			for (int i = 0; i < children; ++i)
			{
				addSubItem(new Item(propertiesEditor, t.getChild(i)));
			}
		}

		void valueTreeChildRemoved(ValueTree& parentTree,
			ValueTree& childWhichHasBeenRemoved,
			int indexFromWhichChildWasRemoved) override
		{
		}

		void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override
		{
			
		}

		void paintItem(Graphics& g, int w, int h) override
		{
			Font font;
			Font smallFont(11.0);
			if (isSelected())
				g.fillAll(Colours::white);
			const float padding = 20.0f;
			String typeName = t.getType().toString();
			const float nameWidth = font.getStringWidthFloat(typeName);
			const float propertyX = padding + nameWidth;

			g.setColour(Colours::black);
			g.setFont(font);
			g.drawText(t.getType().toString(), 0, 0, w, h, Justification::left, false);
			g.setColour(Colours::blue);
			g.setFont(smallFont);
			String propertySummary;

			for (int i = 0; i < t.getNumProperties(); ++i)
			{
				const Identifier name = t.getPropertyName(i).toString();
				propertySummary += " [" + name.toString() + "] " + t.getProperty(name).toString();
			}
			g.drawText(propertySummary, propertyX, 0, w - propertyX, h, Justification::left, true);
		}

		void itemSelectionChanged(bool isNowSelected) override
		{
			if (isNowSelected)
			{
				t.removeListener(this);
				propertiesEditor->setSource(t);
				t.addListener(this);
			}
		}

		/ * Enormous list of ValueTree::Listener options... * /
		void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override
		{
			if (t != treeWhosePropertyHasChanged) return;
			t.removeListener(this);
			//            if (isSelected())
			//                propertiesEditor->setSource(t);
			repaintItem();
			t.addListener(this);
		}

		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override
		{
			treeHasChanged();
		}

		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved)
		{
			treeHasChanged();
		}

		void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved)
		{
			treeHasChanged();
		}

		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override
		{
			treeHasChanged();
		}

		void valueTreeRedirected(ValueTree& treeWhichHasBeenChanged) override
		{
			treeHasChanged();
		}

	private:
		PropertyEditor* propertiesEditor;
		ValueTree t;
		Array<Identifier> currentProperties;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Item);
	};

	/ ** Display a tree. * /
	class Editor : public Component
	{
	public:
		Editor() : layoutResizer(&layout, 1, false)
		{
			layout.setItemLayout(0, -0.1, -0.9, -0.6);
			layout.setItemLayout(1, 5, 5, 5);
			layout.setItemLayout(2, -0.1, -0.9, -0.4);
			setSize(1000, 700);

			addAndMakeVisible(treeView);
			addAndMakeVisible(propertyEditor);
			addAndMakeVisible(layoutResizer);
		}

		~Editor()
		{
			treeView.setRootItem(nullptr);
		}

		void resized() override
		{
			Component* comps[] = {&treeView, &layoutResizer, &propertyEditor};
			layout.layOutComponents(comps, 3, 0, 0, getWidth(), getHeight(), true, true);
		}

		void setTree(ValueTree newTree)
		{
			if (newTree == ValueTree::invalid)
			{
				treeView.setRootItem(nullptr);
			} else if (tree != newTree)
			{
				tree = newTree;
				treeView.setRootItem(new Item(&propertyEditor, tree));
			}
		}
	public:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor);
		ValueTree tree;
		TreeView treeView;
		PropertyEditor propertyEditor;
		StretchableLayoutManager layout;
		StretchableLayoutResizerBar layoutResizer;
	};
public:
	ValueTreeEditor()
		: DocumentWindow("Value Tree Editor",
			Colours::lightgrey,
			DocumentWindow::allButtons)
	{
		editor = new Editor();
		setContentOwned(editor, true);
		setResizable(true, false);
		setUsingNativeTitleBar(true);
		centreWithSize(getWidth(), getHeight());
		setVisible(true);
	}

	~ValueTreeEditor()
	{
		editor->setTree(ValueTree::invalid);
		DBG("In valuetreeeditor destruct");
	}

	void closeButtonPressed() override
	{
		setVisible(false);
	}

	void setSource(ValueTree& v)
	{
		editor->setTree(v);
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeEditor);
	ScopedPointer<Editor> editor;
};
*/

#endif // VALUETREEWINDOW_H_INCLUDED
