#ifndef CVVISUAL_FILTER_CALL_TAB_HPP
#define CVVISUAL_FILTER_CALL_TAB_HPP

#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>

#include "call_tab.hpp"
#include "../view/filter_view.hpp"
#include "../controller/view_controller.hpp"
#include "../impl/filter_call.hpp"
#include "../util/util.hpp"

namespace cvv {
namespace gui {

/** Filter Call Tab
 * The inner part of a tab or window
 * containing a FilterView.
 * Allows to switch views and to access the help.
 */
class FilterCallTab: public CallTab
{
Q_OBJECT

public:

	/**
	 * @brief FilterCallTab
	 * Short constructor which initialises the Call Tab with default view from settings.
	 * @param tabName
	 * @param fc the FilterCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to.
	 */
	FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc);

	/**
	 * @brief FilterCallTab
	 * Constructor initialising the Call Tab.
	 * @param tabName
	 * @param fc the FilterCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to
	 * @param viewId the ID of the view to be shown inside this CallTab.
	 */
	FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc, const QString& viewId);

	/**
	 * @brief getId
	 * @return the ID of the CallTab which is equal to the ID of the associated call.
	 */
	size_t getId() const;

	/**
	 * Adds a FilterView with a name to the thread local map of all FilterViews.
	 * @param filterViewId the Id or name of the FilterView.
	 */
	static void addFilterViewToMap(const QString& filterViewId, const cvv::view::FilterView& filterView);

private slots:

	/**
	 * Called when the index of the view selection changes.
	 * @param text of the current selection in the view selection.
	 */
	void currentIndexChanged(const QString& text) const;

	/**
	 * Called when the help button is clicked.
	 */
	void helpButtonClicked() const;

private:

	thread_local static QMap<QString, cvv::view::FilterView> filterViewMap;
	util::Reference<const cvv::impl::FilterCall> filterCall;
	util::Reference<const cvv::controller::ViewController> viewController;
	QString filterViewId;
	cvv::view::FilterView filterView;

	QPushButton helpButton;
	QComboBox filterViewSelection;
};

}}//namespaces

#endif