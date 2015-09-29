#include "value_tree_editor.h"
#include "../../components/NotepadComponent/NotepadComponent.h"
#include <iomanip>
#include <sstream>

namespace Zen
{

ValueTreeEditor::ValueTreeEditor() :
	DocumentWindow("Value Tree Editor",
		Colours::lightgrey,
		DocumentWindow::allButtons)
{
	editor = new Editor();
	tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop);
	tabbedComponent->addTab("ValueTree", Colours::lightgrey, editor, false);
	bufferVisualiser = new Visualiser();
	tabbedComponent->addTab("BufferVis", Colours::lightgrey, bufferVisualiser->getComponent(), false);

	tabbedComponent->addTab("Notes", Colours::lightgrey, new NotepadComponent("Notepad", ""), true);
	tabbedComponent->setCurrentTabIndex(0);
	setSize(450, 300);
	tabbedComponent->setBounds(0, 0, getWidth(), getHeight());
	
	setContentNonOwned(tabbedComponent, true);
	setResizable(true, false);
	setUsingNativeTitleBar(true);
	centreWithSize(getWidth(), getHeight());
	setVisible(true);
}

ValueTreeEditor::~ValueTreeEditor()
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
	tabbedComponent = nullptr;
	bufferVisualiser = nullptr;
}

void ValueTreeEditor::addTraceLabel(const String& inName, Value& theValue)
{
	this->editor->addTraceLabel(inName, theValue);
}

void ValueTreeEditor::addTraceLabel(const String& inName, const String& inText)
{
	this->editor->addTraceLabel(inName, inText);
}

void ValueTreeEditor::setLabelText(const String& labelName, const float inText)
{
	this->editor->setLabelText(labelName, String(inText));
}

void ValueTreeEditor::setLabelText(const String& labelName, const String& inText)
{
	this->editor->setLabelText(labelName, inText);
}

void ValueTreeEditor::addOrSetTraceLabel(const String& inName, const String& inText)
{
	this->editor->addOrSetTraceLabel(inName, inText);
}

void ValueTreeEditor::closeButtonPressed()
{
	setVisible(false);
}

void ValueTreeEditor::setSource(ValueTree& v)
{
	editor->setTree(v);
}



ValueTreeEditor::Editor::Editor() :
	treeView("treeView-Name"),
	layoutResizer(&layout, 1, false)
{
	layout.setItemLayout(0, -0.1, -0.9, -0.6);
	layout.setItemLayout(1, 5, 5, 5);
	layout.setItemLayout(2, -0.1, -0.9, -0.3);
	layout.setItemLayout(3, 120, -0.9, -0.1);

	addTraceLabel("Debug Label", "Debug");
	setSize(1000, 700);
	treeView.setDefaultOpenness(true);
	addAndMakeVisible(treeView);
	addAndMakeVisible(propertyEditor);
	addAndMakeVisible(layoutResizer);
	addAndMakeVisible(labelsComponent);
	startTimer(500);
}

ValueTreeEditor::Editor::~Editor()
{
	treeView.deleteRootItem();
	treeView.setRootItem(nullptr);
	if (rootItem != nullptr)
		rootItem->setOwnerViewPublic(nullptr);
	rootItem = nullptr;
	theLabels.clear();
//	tabComponent = nullptr;
}

void ValueTreeEditor::Editor::resized()
{
	Component* comps[] = {&treeView, &layoutResizer, &propertyEditor, &labelsComponent};
	layout.layOutComponents(comps, 4, 0, 0, getWidth(), getHeight(), true, true);
}

void ValueTreeEditor::Editor::addTraceLabel(const String& inName, Value& theValue)
{
	addTraceLabel(inName, theValue.toString());
}

void ValueTreeEditor::Editor::addTraceLabel(const String& inName, const String& inText)
{
	Label* theLabel = new Label(inName, formatLabelText(inName, inText));

	theLabels.add(theLabel);
	theLabel->setFont(Font(12.00f, Font::bold));
	theLabel->setJustificationType(Justification::centredLeft);
	theLabel->setEditable(false, false, false);
	theLabel->setColour(Label::textColourId, Colours::black);
	theLabel->setColour(TextEditor::textColourId, Colours::black);
	theLabel->setColour(TextEditor::backgroundColourId, Colour(0x00FFFF00));
	theLabel->setTopLeftPosition(((labelsComponent.getNumChildComponents() - 1) / 4) * 150, ((labelsComponent.getNumChildComponents() - 1) % 4) * 15);
	theLabel->setSize(150, 15);

	labelsComponent.addAndMakeVisible(theLabel);
	resized();
}

void ValueTreeEditor::Editor::addOrSetTraceLabel(const String& inName, const String& inText)
{
	labelsMap.insert_or_assign(inName, inText);
}

void ValueTreeEditor::Editor::setLabelText(const String& labelName, const float inText)
{
	setLabelText(labelName, String(inText));
}

bool ValueTreeEditor::Editor::setLabelText(const String& labelName, const String& inText)
{
	bool labelFoundAndSet = false;
	for (int labelPointer = 0; labelPointer < theLabels.size(); labelPointer++)
	{
		if (theLabels[labelPointer]->getName() == labelName)
		{
			String tempStr(formatLabelText(labelName, inText));
			theLabels[labelPointer]->setText(tempStr, dontSendNotification);
			labelFoundAndSet = true;
		}
	}
	return labelFoundAndSet;
}

String ValueTreeEditor::Editor::formatLabelText(const String& labelName, const String& inText)
{
	return labelName + ":  " + inText;
}

void ValueTreeEditor::Editor::setTree(ValueTree newTree)
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

void ValueTreeEditor::Editor::timerCallback()
{
	//DBG("In timer callback");
	for (auto ii = labelsMap.begin(); ii != labelsMap.end(); ++ii)
	{
		bool labelExistsAndWasChanged;
		labelExistsAndWasChanged = setLabelText((*ii).first, (*ii).second);
		if (!labelExistsAndWasChanged)
			addTraceLabel((*ii).first, (*ii).second);
	}
}



String ValueTreeEditor::Item::getUniqueName() const
{
	if (t.getParent() == ValueTree::invalid) return "1";

	return String(t.getParent().indexOf(t));
}

void ValueTreeEditor::Item::valueTreeRedirected(ValueTree& treeWhichHasBeenChanged)
{
	if (treeWhichHasBeenChanged == t)
		updateSubItems();

	treeHasChanged();
}

void ValueTreeEditor::Item::valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged)
{
	treeHasChanged();
}

void ValueTreeEditor::Item::valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int, int)
{
	if (parentTreeWhoseChildrenHaveMoved == t)
		updateSubItems();

	treeHasChanged();
}

void ValueTreeEditor::Item::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int)
{
	if (parentTree == t)
		updateSubItems();

	treeHasChanged();
}

void ValueTreeEditor::Item::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded)
{
	if (parentTree == t)
		updateSubItems();

	treeHasChanged();
}

void ValueTreeEditor::Item::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
	if (t != treeWhosePropertyHasChanged) return;

	t.removeListener(this);
//            if (isSelected())
//                propertiesEditor->setSource(t);
	repaintItem();
	t.addListener(this);
}

void ValueTreeEditor::Item::itemSelectionChanged(bool isNowSelected)
{
	if (isNowSelected)
	{
		t.removeListener(this);
		propertiesEditor->setSource(t);
		t.addListener(this);
	}
}

void ValueTreeEditor::Item::paintItem(Graphics& g, int w, int h)
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

void ValueTreeEditor::Item::updateSubItems()
{
	ScopedPointer<XmlElement> openness = getOpennessState();
	clearSubItems();
	int children = t.getNumChildren();

	for (int i = 0; i < children; ++i)
		addSubItem(new Item(propertiesEditor, t.getChild(i)));

	if (openness)
		restoreOpennessState(*openness);
}

void ValueTreeEditor::Item::itemOpennessChanged(bool isNowOpen)
{
	if (isNowOpen) updateSubItems();
}

bool ValueTreeEditor::Item::mightContainSubItems()
{
	return t.getNumChildren() > 0;
}

ValueTreeEditor::Item::~Item()
{
	clearSubItems();
}

ValueTreeEditor::Item::Item(PropertyEditor* inPropertiesEditor, ValueTree tree) :
	t(tree)
{
	t.addListener(this);
	propertiesEditor = inPropertiesEditor;
}

ValueTreeEditor::PropertyEditor::PropertyEditor()
{
	noEditValue = "not editable";
}

void ValueTreeEditor::PropertyEditor::setSource(ValueTree& tree)
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
} //Namespace Zen