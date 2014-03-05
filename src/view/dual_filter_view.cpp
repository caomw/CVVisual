#include <exception>
#include <memory>
#include <iostream> //for debugging
#include <string>
#include <unordered_map>
#include <vector>

#include "opencv2/core/core.hpp"

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QtGui>

#include "../dbg/dbg.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/autofilterwidget.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "../qtutil/zoomableimageoptpanel.hpp"
#include "../qtutil/synczoomwidget.hpp"
#include "../qtutil/util.hpp"
#include "../util/util.hpp"
#include "dual_filter_view.hpp"

namespace cvv
{
namespace view
{

// neuer Konstruktor
DualFilterView::DualFilterView(std::array<cv::Mat, 2> images, QWidget *parent)
    : FilterView{ parent }, rawImages_(images)
{
	TRACEPOINT;
	auto layout = util::make_unique<QHBoxLayout>();
	auto imageLayout = util::make_unique<QHBoxLayout>();
	auto imwid = util::make_unique<QWidget>();
	auto accor = util::make_unique<qtutil::Accordion>();

	accor->setMinimumWidth(300);
	accor->setMaximumWidth(300);
	
	auto filterSelector =
	    util::make_unique<qtutil::AutoFilterWidget<2, 1>>(this);
	filterSelector->enableUserSelection(false);
	qtutil::AutoFilterWidget<2, 1> *filterSel = filterSelector.get();
	accor->insert("Select a Filter", std::move(filterSelector));

	auto lambda = [this, &imageLayout, &accor, filterSel](
	    const cv::Mat &image, size_t count)
	{
		auto zoomIm = util::make_unique<qtutil::ZoomableImage>();

		if (count == 1)
		{
			auto filterSignals = filterSel->addEntry(
			    QString("middle image"),
			    { { util::makeRef<const cv::Mat>(rawImages_.at(0)),
				util::makeRef<const cv::Mat>(
				    rawImages_.at(1)) } },
			    { { util::makeRef<cv::Mat>(zoomIm->mat()) } });

			// connect entry=> zoomableimage
			connect(filterSignals.front().getPtr(),
			        SIGNAL(signal(cv::Mat &)), zoomIm.get(),
			        SLOT(setMatR(cv::Mat &)));
		}

		accor->insert(
		    QString("ImageInformation: ") + QString::number(count),
		    std::move(
		        util::make_unique<qtutil::ZoomableOptPanel>(*zoomIm)));

		if (count != 1)
		{
			zoomIm->setMat(image);
		}
		else
		{
			zoomIm->setMat(image.clone());
		}
		imageLayout.get()->addWidget(zoomIm.get());
		
		return zoomIm.release();
	};
	
	std::vector<qtutil::ZoomableImage*> syncVec;
	
	syncVec.push_back(lambda(rawImages_.at(0), 0));
	syncVec.push_back(lambda(rawImages_.at(0), 1));
	syncVec.push_back(lambda(rawImages_.at(1), 2));
	
	accor->insert("Zoom synchronization",
		std::move(util::make_unique<qtutil::SyncZoomWidget>(syncVec)), true, 1);
	
	imageLayout->setStretch(0,1);
	imageLayout->setStretch(1,1);
	imageLayout->setStretch(2,1);
	imwid->setLayout(imageLayout.release());

	layout->addWidget(accor.release());
	layout->addWidget(imwid.release());

	setLayout(layout.release());

	TRACEPOINT;
}

// Vektorkonstruktor
DualFilterView::DualFilterView(const std::vector<cv::Mat> &images,
                               QWidget *parent)
    : DualFilterView(convertToArray(images), parent)
{
}

std::array<cv::Mat, 2>
DualFilterView::convertToArray(const std::vector<cv::Mat> &matVec) const
{
	TRACEPOINT;
	if (matVec.size() != 2)
	{
		throw std::runtime_error("Wrong number of elements in vector");
	}
	TRACEPOINT;
	return { matVec.at(0), matVec.at(1) };
}
}
} // namespaces
