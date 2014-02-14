#ifndef CVVISUAL_OVERVIEWTABLE_HPP
#define	CVVISUAL_OVERVIEWTABLE_HPP

#include <vector>

#include <QWidget>
#include <QList>

#include "overview_panel.hpp"
#include "overview_table_row.hpp"
#include "../stfl/element_group.hpp"
#include "../qtutil/accordion.hpp"
#include "../util/util.hpp"
#include "../controller/view_controller.hpp"
#include "overview_group_subtable.hpp"

namespace cvv { namespace gui {

class OverviewPanel;
class OverviewTableRow;

/**
 * @brief A table displaying the different calls in the overview.
 * It's actually an accordion of subtables to support grouping.
 */
class OverviewTable : public QWidget
{
	Q_OBJECT
	
public:
	
	/**
	 * @brief Constructs a new OverviewTable.
	 * @param controller it's ViewController
	 * @param parent it's parent overview
	 */
	OverviewTable(util::Reference<controller::ViewController> controller, OverviewPanel *parent);
	
	/**
	 * @brief Update the inherited groups of rows and rebuild the UI fully.
	 */
	void updateRowGroups(const std::vector<stfl::ElementGroup<OverviewTableRow>> newGroups);
	
	/**
	 * @brief Hide the thumbnail images in the tables.
	 */
	void hideImages();
	
	/**
	 * @brief Show thumbnail images in the tables.
	 */
	void showImages();
	
	/**
	 * @brief Does this the tables show thumbnail images?
	 */
	bool isShowingImages();

	/**
	 * @brief Updates the UI.
	 * Updates all subtables.
	 */
	void updateUI();

private:
	util::Reference<controller::ViewController> controller;
	OverviewPanel *parent;
	bool doesShowImages = true;
	qtutil::Accordion *subtableAccordion;
	std::vector<OverviewGroupSubtable*> subTables{};

};

}}

#endif

