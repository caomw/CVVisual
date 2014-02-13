#ifndef CVVISUAL_ZOOMABLE_IMAGE_OPT_PANEL
#define CVVISUAL_ZOOMABLE_IMAGE_OPT_PANEL

#include <QLabel>
#include <QString>
#include <QWidget>
#include <QDoubleSpinBox>


#include "opencv2/core/core.hpp"
#include "zoomableimage.hpp"
#include "util.hpp"
#include "../dbg/dbg.hpp"


namespace cvv { namespace qtutil{

class ZoomableOptPanel : public QWidget{

Q_OBJECT

public:
	ZoomableOptPanel(const ZoomableImage&,QWidget *parent=nullptr);

public slots:
	void updateMat(cv::Mat mat);
	void updateConvertStatus(ImageConversionResult result);
	void setZoom(QRectF,qreal);

private:
	QDoubleSpinBox *zoomSpin;
	QLabel *labelConvert;
	QLabel *labelDim;
	QLabel *labelType;
	QLabel *labelChannel;
	QLabel *labelSize;
	QLabel *labelDepth;

};

}}

#endif
