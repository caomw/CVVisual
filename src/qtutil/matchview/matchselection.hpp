#ifndef CVVISUAL_MATCH_SELECTOR
#define CVVISUAL_MATCH_SELECTOR

#include <QWidget>

#include "opencv2/features2d/features2d.hpp"

namespace cvv{ namespace qtutil{

/**
 * @brief this class select matches from a given selection
 */
class MatchSelection:public QWidget{

	Q_OBJECT

public:
	/**
	 * @brief the constructor
	 */
	MatchSelection(QWidget * parent =nullptr):QWidget{parent}{}
	virtual std::vector<cv::DMatch> select(const std::vector<cv::DMatch>& selection) = 0;

private:

	MatchSelection* currentSelection_;
};

}}
#endif
