#include <QVBoxLayout>
#include <QPushButton>

#include "zoomableimageoptpanel.hpp"
#include "util.hpp"

namespace cvv{ namespace qtutil {

ZoomableOptPanel::ZoomableOptPanel(const ZoomableImage& zoomIm,QWidget *parent):QWidget{parent}
{
	TRACEPOINT;
	zoomSpin 	= new QDoubleSpinBox{};
	labelConvert 	= new QLabel{};
	labelDim 	= new QLabel{};
	labelType 	= new QLabel{};
	labelChannel 	= new QLabel{};
	labelSize 	= new QLabel{};
	labelDepth 	= new QLabel{};
	QPushButton *buttonFullImage = new QPushButton{"show full Image"};


	zoomSpin->setMinimum(0.0);
	zoomSpin->setMaximum(2000.0);

	QVBoxLayout *basicLayout = new QVBoxLayout{};

	basicLayout->addWidget(zoomSpin);
	basicLayout->addWidget(labelConvert);
	basicLayout->addWidget(labelDim);
	basicLayout->addWidget(labelType);
	basicLayout->addWidget(labelChannel);
	basicLayout->addWidget(labelSize);
	basicLayout->addWidget(labelDepth);
	basicLayout->addWidget(buttonFullImage);

	setLayout(basicLayout);

	/*connect(&zoomIm,SIGNAL(updateConversionResult(ImageConversionResult)),this,
		SLOT(updateConvertStatus(ImageConversionResult)));

	connect(zoomSpin,SIGNAL(valueChanged(double)),&zoomIm,SIGNAL(setZoom(qreal)));
	connect(&zoomIm,SIGNAL(updateArea(QRectF,qreal)),zoomSpin,SLOT(setValue(double)));

	connect(buttonFullImage,SIGNAL(clicked()),&zoomIm,SLOT(showFullImage()));
	*/
	updateMat(zoomIm.mat());
	TRACEPOINT;
}

void ZoomableOptPanel::updateConvertStatus(ImageConversionResult result){
	TRACEPOINT;
	QString qs{"Convert Status: "};
	switch(result)
	{
		case ImageConversionResult::SUCCESS : 
			labelConvert->setText(qs.append("SUCCES"));
			break;
		case ImageConversionResult::MAT_EMPTY:
			labelConvert->setText(qs.append("Empty Mat"));
			break;
		case ImageConversionResult::MAT_NOT_2D:
			labelConvert->setText(qs.append("Unsupported Dimension"));
			break;
		case ImageConversionResult::FLOAT_OUT_OF_0_TO_1:
			labelConvert->setText(qs.append("Float values out of range [0,1]"));
			break;
		case ImageConversionResult::NUMBER_OF_CHANNELS_NOT_SUPPORTED:
			labelConvert->setText(qs.append("Unsupported number of channels"));
			break;
		case ImageConversionResult::MAT_INVALID_SIZE:
			labelConvert->setText(qs.append("Invalid Size"));
			break;
		case ImageConversionResult::MAT_UNSUPPORTED_DEPTH:
			labelConvert->setText(qs.append("Unsupported Depth "));
			break;
		default : 
			labelConvert->setText(qs.append("Unknown result from convert function"));
			break;
	}
	TRACEPOINT;
}

void ZoomableOptPanel::updateMat(cv::Mat mat){
	TRACEPOINT;
	if(mat.empty()){
		labelDim->setText("empty");
		labelType->setText("empty");
		labelChannel->setText("empty");
		labelSize->setText("empty");
		labelDepth->setText("empty");
	}else
	{
		labelDim->setText(QString("Dimension: %1").arg(mat.dims));
		labelChannel->setText(QString("Number of Channels: %1").arg(mat.channels()));
		labelSize->setText(QString("Size: %1/%2").arg(mat.rows).arg((mat.cols)));
		labelDepth->setText(QString("Depth: %1").arg(mat.depth()));

		switch(mat.type())
		{
			case CV_8U: labelType->setText("Type: CV_8U");
				break;
			case CV_8S: labelType->setText("Type: CV_5U");
				break;
			case CV_16U: labelType->setText("Type: CV_16U");
				break;
			case CV_16S: labelType->setText("Type: CV_16S");
				break;
			case CV_32S: labelType->setText("Type: CV_32S");
				break;
			case CV_32F: labelType->setText("Type: CV_32F");
				break;
			case CV_64F: labelType->setText("Type: CV_64S");
				break;
			default:
				labelType->setText("Type: Unkown");
				break;
		}
	}
	TRACEPOINT;
}

void ZoomableOptPanel::setZoom(QRectF visibleArea,qreal zoomfac)
{
	zoomSpin->setValue(zoomfac);
	(void) visibleArea;
}
}}
