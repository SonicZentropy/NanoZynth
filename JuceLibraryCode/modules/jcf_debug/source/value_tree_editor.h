/*
 * https://github.com/jcredland/juce-toys/
 *
 */

#ifndef VALUE_TREE_EDITOR_H_INCLUDED
#define VALUE_TREE_EDITOR_H_INCLUDED





/**
 Display a separate desktop window for viewed and editing a value
 tree's property fields.

 Instantate a ValueTreeEditor instance, then call
 ValueTreeEditor::setSource(ValueTree &) and it'll display your
 tree.

 For example:

 @code valueTreeEditor = new ValueTreeEditor();
 valueTreeEditor->setSource(myTree);

 @note This code isn't pretty, and the UI isn't pretty - it's for
 debugging and definitely not production use!
 */

//class ValueTreeEditor;
class ValueTreeEditor :
	public DocumentWindow
{


	/** Display properties for a tree. */
	class PropertyEditor :
		public PropertyPanel
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

			Array<PropertyComponent*> pc;

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

	class Item :
		public TreeViewItem,
		public ValueTree::Listener
	{
	public:
		Item(PropertyEditor* inPropertiesEditor, ValueTree tree)
			:
			t(tree)
		{
			t.addListener(this);
			propertiesEditor = inPropertiesEditor;
		}

		~Item()
		{
		//	t.removeListener(this);
			//DBG("Curr Properties size: " + String(currentProperties.size()));
			//currentProperties.clear();
			//propertiesEditor = nullptr;
			clearSubItems();
		}

		bool mightContainSubItems() override
		{
			return t.getNumChildren() > 0;
		}

		void itemOpennessChanged(bool isNowOpen) override
		{
			if (isNowOpen) updateSubItems();
		}

		void updateSubItems()
		{
			ScopedPointer<XmlElement> openness = getOpennessState();
			clearSubItems();
			int children = t.getNumChildren();

			for (int i = 0; i < children; ++i)
				addSubItem(new Item(propertiesEditor, t.getChild(i)));

			if (openness)
				restoreOpennessState(*openness);
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

			String propertySummary;

			for (int i = 0; i < t.getNumProperties(); ++i)
			{
				const Identifier name = t.getPropertyName(i).toString();
				String propertyValue = t.getProperty(name).toString();;
#ifdef JCF_SERIALIZER

				if (t[name].isObject())
				{
					ReferenceCountedObject* p = t[name].getObject();

					if (Serializable* s = dynamic_cast<Serializable*> (p))
						propertyValue = "[[" + s->getDebugInformation() + "]]";
				}

#endif
				if (name.toString().equalsIgnoreCase("name"))
				{
					//propertySummary += " " + name.toString() + "=\"" + propertyValue + "\"";
				} else
				{
					propertySummary += " " + name.toString() + "=" + propertyValue.toStdString().substr(0, 8);
				}
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


		/* Enormous list of ValueTree::Listener options... */
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
			if (parentTree == t)
				updateSubItems();

			treeHasChanged();
		}


		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int) override
		{
			if (parentTree == t)
				updateSubItems();

			treeHasChanged();
		}
		void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int, int) override
		{
			if (parentTreeWhoseChildrenHaveMoved == t)
				updateSubItems();

			treeHasChanged();
		}
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override
		{
			treeHasChanged();
		}
		void valueTreeRedirected(ValueTree& treeWhichHasBeenChanged) override
		{
			if (treeWhichHasBeenChanged == t)
				updateSubItems();

			treeHasChanged();
		}

		/* Works only if the ValueTree isn't updated between calls to getUniqueName. */
		String getUniqueName() const override
		{
			if (t.getParent() == ValueTree::invalid) return "1";

			return String(t.getParent().indexOf(t));
		}

	private:
		//PropertyEditor* propertiesEditor;
		PropertyEditor* propertiesEditor;
		ValueTree t;
		Array<Identifier> currentProperties;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Item);
	};


	/** Display a tree. */
	class Editor :
		public Component, public Timer
	{
	public:

		Editor() :
			treeView("treeView-Name"),
			layoutResizer(&layout, 1, false)
		{
			layout.setItemLayout(0, -0.1, -0.9, -0.6);
			layout.setItemLayout(1, 5, 5, 5);

			layout.setItemLayout(2, -0.1, -0.9, -0.3);
			layout.setItemLayout(3, 120, -0.9, -0.1);

			addTraceLabel("TESTName", "TestText");


			setSize(1000, 700);
			treeView.setDefaultOpenness(true);
			addAndMakeVisible(treeView);
			addAndMakeVisible(propertyEditor);
			addAndMakeVisible(layoutResizer);
			addAndMakeVisible(labelsComponent);
			startTimer(500);

		}
		~Editor()
		{
			treeView.deleteRootItem();
			treeView.setRootItem(nullptr);
			if (rootItem != nullptr)
				rootItem->setOwnerViewPublic(nullptr);
			rootItem = nullptr;
			theLabels.clear();

		}

		void resized() override
		{
			Component* comps[] = {&treeView, &layoutResizer, &propertyEditor, &labelsComponent};
			layout.layOutComponents(comps, 4, 0, 0, getWidth(), getHeight(), true, true);
		}

		void addTraceLabel(const String& inName, const String& inText)
		{
			Label* theLabel = new Label(inName, inName + ": " + inText);

			theLabels.add(theLabel);
			theLabel->setFont(Font(12.00f, Font::bold));
			theLabel->setJustificationType(Justification::centred);
			theLabel->setEditable(false, false, false);
			theLabel->setColour(Label::textColourId, Colours::black);
			theLabel->setColour(TextEditor::textColourId, Colours::black);
			theLabel->setColour(TextEditor::backgroundColourId, Colour(0x00FFFF00));
			theLabel->setTopLeftPosition(((labelsComponent.getNumChildComponents() - 1) / 4) * 150, ((labelsComponent.getNumChildComponents() - 1) % 4) * 30);
			theLabel->setSize(150, 30);



			labelsComponent.addAndMakeVisible(theLabel);
		}

		void addTraceLabel(const String& inName, Value& theValue)
		{
			addTraceLabel(inName, theValue.toString());
		}

		void addOrSetTraceLabel(const String& inName, const String& inText)
		{
			//labelName = inName;
			//labelText = inText;
			labelsMap.insert_or_assign(inName, inText);
		}

		bool setLabelText(const String& labelName, const String& inText)
		{

			bool labelFoundAndSet = false;
			for (int labelPointer = 0; labelPointer < theLabels.size(); labelPointer++)
			{
				if (theLabels[labelPointer]->getName() == labelName)
				{

					theLabels[labelPointer]->setText(labelName + ": " + inText, dontSendNotification);
					labelFoundAndSet = true;
				}
			}
			return labelFoundAndSet;
		}

		void setLabelText(const String& labelName, const float inText)
		{
			setLabelText(labelName, String(inText));
		}

		void setTree(ValueTree newTree)
		{
			if (newTree == ValueTree::invalid)
			{
				treeView.setRootItem(nullptr);
			} else if (tree != newTree)
			{
				tree = newTree;
				rootItem = new Item(&propertyEditor, tree);
				treeView.setRootItem(rootItem);
			}
		}

	protected:
		virtual void timerCallback() override
		{
			for (auto ii = labelsMap.begin(); ii != labelsMap.end(); ++ii)
			{
				DBG("Testing TestMap: " + (*ii).first + " : " + (*ii).second);
				bool labelExistsAndWasChanged;
				labelExistsAndWasChanged = setLabelText((*ii).first, (*ii).second);
				if (!labelExistsAndWasChanged)
					addTraceLabel((*ii).first, (*ii).second);
			}
		}

	public:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor);

		ScopedPointer<Item> rootItem;
		ValueTree tree;
		TreeView treeView;
		PropertyEditor propertyEditor;
		Component labelsComponent;
		OwnedArray<Label> theLabels;
		StretchableLayoutManager layout;
		StretchableLayoutResizerBar layoutResizer;

	protected:
		String labelName, labelText;
		std::map<String, String> labelsMap;


	};

public:
	
	ValueTreeEditor() :
		DocumentWindow("Value Tree Editor",
			Colours::lightgrey,
			DocumentWindow::allButtons)
	{
		editor = new Editor();
		setContentNonOwned(editor, true);
		setResizable(true, false);
		setUsingNativeTitleBar(true);
		centreWithSize(getWidth(), getHeight());
		setVisible(true);
	}

	~ValueTreeEditor()
	{
		editor->setTree(ValueTree::invalid);
		if (editor->isOnDesktop())
		{
			editor->removeFromDesktop();
		}
		editor = nullptr;
		if (this->isOnDesktop())
		{
			this->removeFromDesktop();
		}
	}

	void addTraceLabel(const String& inName, const String& inText)
	{
		this->editor->addTraceLabel(inName, inText);
	}

	void addTraceLabel(const String& inName, Value& theValue)
	{
		this->editor->addTraceLabel(inName, theValue);
	}

	void setLabelText(const String& labelName, const String& inText)
	{
		this->editor->setLabelText(labelName, inText);
	}

	void setLabelText(const String& labelName, const float inText)
	{
		this->editor->setLabelText(labelName, String(inText));
	}

	void addOrSetTraceLabel(const String& inName, const String& inText)
	{
		this->editor->addOrSetTraceLabel(inName, inText);
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
	ScopedPointer<Editor> editor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeEditor);
};

#endif  // VALUE_TREE_EDITOR_H_INCLUDED
