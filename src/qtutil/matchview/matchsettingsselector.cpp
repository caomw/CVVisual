
#include <QVBoxLayout>

#include "matchsettingsselector.hpp"
#include "../../util/util.hpp"


namespace cvv{ namespace qtutil{

MatchSettingsSelector::MatchSettingsSelector(const std::vector<cv::DMatch> &univers, QWidget *parent):
	MatchSettings{parent},
	RegisterHelper<MatchSettings,std::vector<cv::DMatch>>{},
	univers_{univers}
{
	auto layout=util::make_unique<QVBoxLayout>();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(comboBox_);

	connect(&signalElementSelected(),SIGNAL(signal(QString)),this,SLOT(changedSetting()));
	layout_=layout.get();
	setLayout(layout.release());
	TRACEPOINT;
	if(this->has(this->selection())){
		TRACEPOINT;
		changedSetting();
	}
	TRACEPOINT;
}

void MatchSettingsSelector::setSettings(CVVMatch &match)
{
	setting_->setSettings(match);
}

void MatchSettingsSelector::changedSetting()
{
	TRACEPOINT;
	auto setting=(*this)()(univers_);
	TRACEPOINT;
	if(setting){
		TRACEPOINT;
		if(setting_){
			TRACEPOINT;
			layout_->removeWidget(setting_);
			disconnect(setting_,SIGNAL(settingsChanged(MatchSettings&)),
				   this,SIGNAL(settingsChanged(MatchSettings&)));
		}
		TRACEPOINT;
		setting_->deleteLater();
		TRACEPOINT;
		setting_=setting.get();
		TRACEPOINT;
		layout_->addWidget(setting.release());
		TRACEPOINT;
		connect(setting_,SIGNAL(settingsChanged(MatchSettings&)),
			this,SIGNAL(settingsChanged(MatchSettings&)));
	}
}

}}
