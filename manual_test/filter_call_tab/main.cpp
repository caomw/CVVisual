#include <sstream>

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>

#include "../../src/gui/filter_call_tab.hpp"
#include "../../src/impl/filter_call.hpp"
#include "../../include/opencv2/call_meta_data.hpp"
#include "../../src/view/defaultfilterview.hpp"
#include "../../src/view/dual_filter_view.hpp"
#include "../../src/view/singlefilterview.hpp"
#include "../../src/util/util.hpp"

#include <opencv2/core/core.hpp>

int main(int argc, char *argv[])
{

	/* Create some data for the FilterCallTab: */
	cv::Mat in{ 100, 100, CV_8U };
	cv::Mat out{ 100, 100, CV_8U };
	cvv::impl::CallMetaData data{};
	QString type{ "test_type" };
	QApplication controller{ argc, argv };
	cvv::impl::FilterCall fc{ in, out, data, type, "some description", "" };

	cvv::gui::FilterCallTab::registerFilterView<
	    cvv::view::DefaultFilterView>("DefaultFilterView");
	cvv::gui::FilterCallTab::registerFilterView<cvv::view::DualFilterView>(
	    "DualFilterView");
	cvv::gui::FilterCallTab::registerFilterView<
	    cvv::view::SingleFilterView>("SingleFilterView");

	cvv::gui::FilterCallTab u{ fc };

	u.show();
	controller.exec();
	return 0;
}
