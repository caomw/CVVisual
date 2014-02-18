#include "grayfilterwidget.hpp"

#include <QPushButton>
#include <QLabel>

#include "../filterselectorwidget.hpp"
#include "../../util/util.hpp"
#include "../util.hpp"

namespace cvv { namespace qtutil {

GrayFilterWidget::GrayFilterWidget(QWidget* parent):
	FilterFunctionWidget<1,1>{parent},
	layout_{new QVBoxLayout{}},
	channel_{new QSpinBox{}},
	chanValues_{}
{
	TRACEPOINT;
	//button
	auto button=util::make_unique<QPushButton>("use default rgb to gray");
	QObject::connect(button.get(),SIGNAL(clicked()),this, SLOT(setStd()));

	TRACEPOINT;
	//channelselector
	channel_->setRange(1,10);
	QObject::connect(channel_,SIGNAL(valueChanged(int)),this, SLOT(setChannel(int)));

	TRACEPOINT;
	//build ui
	layout_->addWidget(button.release());
	layout_->addWidget(util::make_unique<QLabel>("Number of channels").release());
	layout_->addWidget(channel_);
	layout_->addWidget(util::make_unique<QLabel>("Percentage for channels").release());
	setLayout(layout_);

	TRACEPOINT;
	//std rgba
	setStd();
	TRACEPOINT;
}

void GrayFilterWidget::applyFilter(InputArray in,OutputArray out) const
{
	TRACEPOINT;
	if(!(checkInput(in).first))
	{
		TRACEPOINT;
		return;
	}
	TRACEPOINT;
	auto channels=splitChannels(in.at(0).get());
	out.at(0).get()=cv::Mat::zeros(in.at(0).get().rows, in.at(0).get().cols,
				in.at(0).get().depth());
	TRACEPOINT;
	for(std::size_t i=0;((i<channels.size()) && (i<chanValues_.size()));i++)
	{
		TRACEPOINT;
		out.at(0).get() += channels.at(i)*(chanValues_.at(i)->value());
		TRACEPOINT;
	}
	TRACEPOINT;
}

std::pair<bool, QString> GrayFilterWidget::checkInput(InputArray) const
{
	TRACEPOINT;
	double sum=0;
	for(auto& elem:chanValues_)
	{
		sum+= (elem->value());
	}
	if(sum>1)
	{
		TRACEPOINT;
		return {false,QString{"total : "}+QString::number(sum)+QString{" > 1"}};
	}
	TRACEPOINT;
	return {true,""};
}

void GrayFilterWidget::setChannel(std::size_t n)
{
	TRACEPOINT;
	if(n==chanValues_.size())
	{
		TRACEPOINT;
		//stop rec
		return;
	}else if(n<chanValues_.size())
	{
		//remove one channel
		TRACEPOINT;
		QDoubleSpinBox* box=chanValues_.back();
		chanValues_.pop_back();
		layout_->removeWidget(box);
		box->setParent(nullptr);
		box->deleteLater();
	}else{
		//add one channel
		TRACEPOINT;
		auto box=util::make_unique<QDoubleSpinBox>();
		box->setRange(0,1);
		box->setSingleStep(0.01);
		chanValues_.emplace_back(box.get());
		//connect
		QObject::connect(box.get(),SIGNAL(valueChanged(double)),
				 &(this->signFilterSettingsChanged_),SIGNAL(signal()));
		layout_->addWidget(box.release());
	}
	//rec
	setChannel(n);
	TRACEPOINT;
}

void GrayFilterWidget::setStd()
{
	TRACEPOINT;
	channel_->setValue(3);
	chanValues_.at(0)->setValue(0.114);
	chanValues_.at(1)->setValue(0.587);
	chanValues_.at(2)->setValue(0.299);
	TRACEPOINT;
}


void registerGray()
{
	TRACEPOINT;
	registerFilter<1,1,cvv::qtutil::GrayFilterWidget>("Gray filter");
	TRACEPOINT;
}
}}