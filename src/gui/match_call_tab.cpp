#include <vector>
#include <memory>

#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "match_call_tab.hpp"
#include "../view/match_view.hpp"
#include "../controller/view_controller.hpp"
#include "../impl/match_call.hpp"

#include "../dbg/dbg.hpp"

namespace cvv {
namespace gui {

MatchCallTab::MatchCallTab(const cvv::impl::MatchCall& fc, cvv::controller::ViewController& vc):
	matchCall_{fc}, viewController_{vc}
{
	TRACEPOINT;
	setName(matchCall_->description());
	const QString scope{"default_views"};
	const QString key{"default_match_view"};
	QString setting;
	try
	{
		setting = vc.getSetting(scope, key);
	} catch (std::invalid_argument)
	{
		setting = "LineMatchView";
	}
	matchViewId_ = setting;

	createGui();
	TRACEPOINT;
}

MatchCallTab::MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& fc, cvv::controller::ViewController& vc):
	matchCall_{fc}, viewController_{vc}
{
	TRACEPOINT;
	setName(tabName);
	const QString scope{"default_views"};
	const QString key{"default_match_view"};
	QString setting;
	try
	{
		setting = vc.getSetting(scope, key);
	} catch (std::invalid_argument)
	{
		setting = "LineMatchView";
	}
	matchViewId_ = setting;

	createGui();
	TRACEPOINT;
}

MatchCallTab::MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& fc, cvv::controller::ViewController& vc, const QString& viewId):
	matchCall_{fc}, viewController_{vc}
{
	TRACEPOINT;
	setName(tabName);
	matchViewId_ = viewId;

	createGui();
	TRACEPOINT;
}

void MatchCallTab::currentIndexChanged(const QString& text)
{
	TRACEPOINT;
	matchViewId_ = text;
	delete matchView_;
	setView(matchViewId_);
	TRACEPOINT;
}

void MatchCallTab::helpButtonClicked() const
{
	TRACEPOINT;
	viewController_->openHelpBrowser(matchViewId_);
	TRACEPOINT;
}

void MatchCallTab::setAsDefaultButtonClicked()
{
	TRACEPOINT;
	const QString scope{"default_views"};
	const QString key{"default_match_view"};
	const QString value{matchViewId_};
	viewController_->setDefaultSetting(scope, key, value);
	TRACEPOINT;
}

size_t MatchCallTab::getId() const
{
	TRACEPOINT;
	return matchCall_->getId();
}

void MatchCallTab::addMatchViewToMap(const QString& matchViewId, MatchViewBuilder mView)
{
	TRACEPOINT;
	cvv::qtutil::RegisterHelper<cvv::view::MatchView, const cv::Mat&,
			const std::vector<cv::KeyPoint>&,
			const cv::Mat&, const std::vector<cv::KeyPoint>&,
			const std::vector<cv::DMatch>&, QWidget*>::registerElement(matchViewId, mView);
	TRACEPOINT;
}

void MatchCallTab::createGui()
{
	TRACEPOINT;
	comboBox_->setCurrentText(matchViewId_);
	hlayout_ = new QHBoxLayout{this};
	hlayout_->setAlignment(Qt::AlignTop);
	hlayout_->addWidget(new QLabel{"View:"});
	hlayout_->addWidget(comboBox_);
	setAsDefaultButton_ = new QPushButton{"Set as default", this};
	hlayout_->addWidget(setAsDefaultButton_);
	connect(setAsDefaultButton_, SIGNAL(clicked()), this, SLOT(setAsDefaultButtonClicked()));
	helpButton_ = new QPushButton{"Help", this};
	hlayout_->addWidget(helpButton_);
	connect(helpButton_, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));

	upperBar_ = new QWidget{this};
	upperBar_->setLayout(hlayout_);

	vlayout_ = new QVBoxLayout{this};

	vlayout_->addWidget(upperBar_);
	setView(matchViewId_);

	setLayout(vlayout_);
	connect(comboBox_, SIGNAL(currentTextChanged(QString)), this, SLOT(currentIndexChanged(QString)));
	TRACEPOINT;
}

void MatchCallTab::setView(const QString &viewId)
{
	TRACEPOINT;
	try
	{
		auto fct = registeredElements_.at(viewId);
		matchView_ = (fct(matchCall_->img1(), matchCall_->keyPoints1(), matchCall_->img2(),
				matchCall_->keyPoints2(), matchCall_->matches(), this)).release();
		vlayout_->addWidget(matchView_);
	} catch (std::out_of_range&)
	{
		vlayout_->addWidget(new QLabel{"Error: View could not be set up."});
		throw;
	}
	TRACEPOINT;

}

}}//namespaces
