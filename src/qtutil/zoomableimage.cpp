
#include <QHBoxLayout>

#include "zoomableimage.hpp"
#include "util.hpp"

namespace cvv{ namespace qtutil{
	ZoomableImage::ZoomableImage(const cv::Mat& mat,QWidget* parent):
		QWidget{parent},mat_(mat)
	{
		label_ = new QLabel{};
		QHBoxLayout *layout=new QHBoxLayout{};
		setLayout(layout) ;
		layout->addWidget(label_);
		updateMat(mat_);
	}

	void ZoomableImage::updateMat(const cv::Mat& mat)
	{
		this->mat_ = mat;
		auto result = convertMatToQPixmap(mat_);
		if(result.first==ImageConversionResult::SUCCESS)
		{
			label_->setPixmap(result.second);
		}
		emit updateInfo(result.first);


	}
}}
