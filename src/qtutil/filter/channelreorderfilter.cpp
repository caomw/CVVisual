#include "channelreorderfilter.hpp"

#include <QLabel>

#include "../util.hpp"

namespace cvv
{
namespace qtutil
{

ChannelReorderFilter::ChannelReorderFilter(QWidget *parent)
    : FilterFunctionWidget<1, 1>{ parent }, layout_{ nullptr },
      channel_{ nullptr }, channelAssignment_{}
{
	TRACEPOINT;
	setToolTip(
	    "nonexistant channels from source will be seen as a zero mat");
	TRACEPOINT;
	auto lay = util::make_unique<QVBoxLayout>();
	layout_ = *lay;
	auto channel = util::make_unique<QSpinBox>();
	channel_ = *channel;

	TRACEPOINT;
	// channelselector
	channel_->setRange(1, 10);
	QObject::connect(channel_.getPtr(), SIGNAL(valueChanged(int)), this,
	                 SLOT(setChannel(int)));

	TRACEPOINT;
	// build ui
	layout_->addWidget(
	    util::make_unique<QLabel>("Number of channels").release());
	layout_->addWidget(channel.release());
	layout_->addWidget(util::make_unique<QLabel>(
	    "Assignment for the old channels").release());
	setLayout(lay.release());
	TRACEPOINT;

	channel_->setValue(3);
	TRACEPOINT;
}

std::pair<bool, QString> ChannelReorderFilter::checkInput(InputArray in) const
{
	TRACEPOINT;
	if (in.at(0)->channels() < 1)
	{
		TRACEPOINT;
		return { false, "<1 channel" };
	}
	TRACEPOINT;
	return { true, "" };
}

void ChannelReorderFilter::applyFilter(InputArray in, OutputArray out) const
{
	TRACEPOINT;
	auto chans = splitChannels(in.at(0).get());
	cv::Mat zeros = cv::Mat::zeros(chans.front().rows, chans.front().cols,
	                               chans.front().type());
	std::vector<cv::Mat> toMerge{};
	for (std::size_t i = 0; i < channelAssignment_.size(); i++)
	{
		if (static_cast<std::size_t>(
		        channelAssignment_.at(i)->value()) < chans.size())
		{
			TRACEPOINT;
			toMerge.push_back(
			    chans.at(channelAssignment_.at(i)->value()));
		}
		else
		{
			TRACEPOINT;
			toMerge.push_back(zeros);
		}
	}
	TRACEPOINT;
	out.at(0).get() = mergeChannels(toMerge);
	TRACEPOINT;
}

void ChannelReorderFilter::setChannel(std::size_t n)
{
	TRACEPOINT;
	if (n == channelAssignment_.size())
	{
		// stop rec + update
		TRACEPOINT;
		signalFilterSettingsChanged().emitSignal();
		TRACEPOINT;
		return;
	}
	else if (n < channelAssignment_.size())
	{
		// remove one channel
		TRACEPOINT;
		QSpinBox *box = channelAssignment_.back().getPtr();
		channelAssignment_.pop_back();
		layout_->removeWidget(box);
		box->setParent(nullptr);
		box->deleteLater();
		TRACEPOINT;
	}
	else
	{
		// add one channel
		TRACEPOINT;
		auto box = util::make_unique<QSpinBox>();
		box->setRange(0, 9);
		box->setSingleStep(1);
		box->setValue(channelAssignment_.size());
		channelAssignment_.emplace_back(*box);
		// connect
		QObject::connect(box.get(), SIGNAL(valueChanged(int)),
		                 &(this->signalFilterSettingsChanged()),
		                 SIGNAL(signal()));
		layout_->addWidget(box.release());
		TRACEPOINT;
	}
	// rec
	setChannel(n);
	TRACEPOINT;
}
}
}
