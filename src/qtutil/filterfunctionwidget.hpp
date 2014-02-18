#ifndef CVVISUAL_FILTERFUNCTIONWIDGET_HPP
#define CVVISUAL_FILTERFUNCTIONWIDGET_HPP
//STD
#include <array>
#include <type_traits>

//QT
#include <QWidget>
#include <QString>

//OCV
#include "opencv2/core/core.hpp"

//cvv
#include "signalslot.hpp"
#include "../dbg/dbg.hpp"
#include "../util/util.hpp"

namespace cvv { namespace qtutil{

template<std::size_t In > using CvvInputArray  = std::array<util::Reference<const cv::Mat>,In>;
template<std::size_t Out> using CvvOutputArray = std::array<util::Reference<cv::Mat>,Out>;


/**
 * @brief The type for the input of the filter.
 *
 * Inherit from it if you want to provide a new image filter.
 * Use the widget to let the user choose parameters.
 * Emit stateChanged when user input leads to different parameters.
 *
 * @tparam In The number of input images.
 * @tparam Out The number of output images.
 */
template< std::size_t In, std::size_t Out>
class FilterFunctionWidget: public QWidget
{
	static_assert( Out > 0, "Out should be >0.");
public:

	using InputArray  = CvvInputArray<In>;
	using OutputArray = CvvOutputArray<Out>;

	FilterFunctionWidget(QWidget* parent = nullptr):
		QWidget{parent}{TRACEPOINT;}

	/**
	 * @brief virtual destructor.
	 */
	virtual ~FilterFunctionWidget()
	{
		TRACEPOINT;
	}

	/**
	 * @brief Applys the filter to in and saves the result in out.
	 * @param in The input images.
	 * @param out The output images.
	 */
	virtual void applyFilter(InputArray in,OutputArray out) const = 0;

	/**
	 * @brief Checks whether input can be progressed by the applyFilter function.
	 * @param in The input images.
	 * @return bool = true: the filter can be executed.
	 *		bool = false: the filter cant be executed (e.g. images have wrong depth)
	 *		QString = message for the user (e.g. why the filter can't be progressed.)
	 */
	virtual std::pair<bool, QString> checkInput(InputArray in) const = 0;

	/**
	 * @brief Signal to emit when user input leads to different parameters.
	 */
	Signal signFilterSettingsChanged_;
};

}} // end namespaces qtutil, cvv
#endif // CVVISUAL_FILTERFUNCTIONWIDGET_HPP
