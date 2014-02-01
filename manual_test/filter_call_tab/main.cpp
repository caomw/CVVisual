#include <sstream>

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>

#include "../../src/gui/filter_call_tab.hpp"
#include "../../src/impl/filter_call.hpp"
#include "../../src/controller/view_controller.hpp"
#include "../../include/opencv2/call_meta_data.hpp"
#include "../../src/view/defaultfilterview.hpp"
#include "../../src/view/dual_filter_view.hpp"
#include "../../src/util/util.hpp"

#include <opencv2/core/core.hpp>

int main(/*int argc, char *argv[]*/)
{

	/* Create some data for the FilterCallTab: */
	cv::Mat in{100,100,CV_8U};
	cv::Mat out{100,100,CV_8U};
	cvv::impl::CallMetaData data{};
	QString type{"test_type"};
	cvv::impl::FilterCall fc{in, out, data, type, "some description", ""};
	cvv::controller::ViewController vc{};

	cvv::gui::FilterCallTab::addFilterViewToMap("DefaultFilterView",
		[] (std::vector<cv::Mat> images, QWidget* parent) { return cvv::util::make_unique<cvv::view::DefaultFilterView>(images, parent); });
	cvv::gui::FilterCallTab::addFilterViewToMap("DualFilterView",
		[] (std::vector<cv::Mat> images, QWidget* parent) { return cvv::util::make_unique<cvv::view::DualFilterView>(images, parent); });

	cvv::gui::FilterCallTab v{"TestFTab", fc, vc};
	cvv::gui::FilterCallTab w{"TestFTab", fc, vc, "DualFilterView"};
	v.show();
	w.show();
	vc.exec();
	return 0;
}
