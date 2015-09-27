#include "ZenDebugWindow.h"

namespace Zen
{

ZenDebugWindow::ZenDebugWindow()
	:DocumentWindow("Zen Debug Window", Colours::lightgrey, DocumentWindow::allButtons)
{
//	DBGM("In ZenDebugWindow::ZenDebugWindow() ");
	setResizable(true, false);
	setUsingNativeTitleBar(true);
	centreWithSize(getWidth(), getHeight());
	setVisible(true);

	valueTreeDebugComponent = new ValueTreeDebugComponent();
	setContentNonOwned(valueTreeDebugComponent, true);
	setSize(800, 800);
}

ZenDebugWindow::ZenDebugWindow(ValueTree* inValueTree)
	:DocumentWindow("Zen Debug Window", Colours::lightgrey, DocumentWindow::allButtons)
{
//	DBGM("In ZenDebugWindow::ZenDebugWindow(ValueTree* inValueTree) ");
	setResizable(true, false);
	setUsingNativeTitleBar(true);
	centreWithSize(getWidth(), getHeight());
	setVisible(true);

	valueTreeDebugComponent = new ValueTreeDebugComponent(inValueTree);
	setContentNonOwned(valueTreeDebugComponent, true);
	setSize(800, 800);
}


ZenDebugWindow::ZenDebugWindow(ValueTree inValueTree)
	:DocumentWindow("Zen Debug Window", Colours::lightgrey, DocumentWindow::allButtons)
{
//	DBGM("In ZenDebugWindow::ZenDebugWindow(ValueTree inValueTree) ");

	setResizable(true, false);
	setUsingNativeTitleBar(true);
	centreWithSize(getWidth(), getHeight());
	setVisible(true);

	valueTreeDebugComponent = new ValueTreeDebugComponent(inValueTree);
	setContentNonOwned(valueTreeDebugComponent, true);
	setSize(800, 800);
}

ZenDebugWindow::~ZenDebugWindow()
{
//	DBGM("In ZenDebugWindow::~ZenDebugWindow() ");
	valueTreeDebugComponent = nullptr;

}

void ZenDebugWindow::closeButtonPressed()
{
//	DBGM("In ZenDebugWindow::closeButtonPressed() ");
	setVisible(false);
}

void ZenDebugWindow::addValueTree(ValueTree inValueTree)
{
//	DBGM("In ZenDebugWindow::addValueTree(inValueTree) ");
}

} //namespace Zen