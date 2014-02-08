#ifndef CVVISUAL_SIGNALEMITTER_HPP
#define CVVISUAL_SIGNALEMITTER_HPP

//std
#include <functional>
#include <stdexcept>

//QT
#include <QObject>
#include <QString>

#include "../dbg/dbg.hpp"

namespace cvv { namespace qtutil{

/**
 * @brief The Signal class can be used for privat or static signals and in case of
 * conflicts between templates and Q_OBJECT.
 */
class Signal: public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief Constructor
	 * @param parent The parent
	 */
	Signal(QObject* parent = nullptr):QObject{parent}{TRACEPOINT;}

	~Signal(){TRACEPOINT;}

	/**
	 * @brief Emits the signal.
	 * @param args The arguments
	 */
	void emitSignal(){TRACEPOINT;emit signal();TRACEPOINT;}
signals:
	/**
	 * @brief The signal emited by emitSignal.
	 */
	void signal();
};

/**
 * @brief The Slot class can be used for static slots and in case of conflicts between
 * templates and Q_OBJECT.
 */
class Slot: public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief Constructor
	 * @param f Function called by the slot slot()
	 * @throw std::invalid_argument If f is invalide
	 * @param parent The parent
	 */
	Slot(const std::function<void()>& f, QObject* parent = nullptr):
		QObject{parent}, function_{f}
	{
		TRACEPOINT;
		if(!f)throw std::invalid_argument{"invalide function"};
		TRACEPOINT;
	}

	~Slot(){TRACEPOINT;}

public slots:
	/**
	 * @brief The slot calling function()
	 */
	void slot(){TRACEPOINT;function_();TRACEPOINT;}
private:
	/**
	 * @brief The function called by the slot slot()
	 */
	std::function<void()> function_;
};

// ///////////////////////////////////////////////////////////////
// manual "templating" for classes Signal and Slot
// ///////////////////////////////////////////////////////////////



class SignalQString: public QObject
{
	Q_OBJECT
public:
	SignalQString(QObject* parent = nullptr):
		QObject{parent}{TRACEPOINT;}

	~SignalQString(){TRACEPOINT;}

	void emitSignal(const QString& t)
		{TRACEPOINT;emit signal(t);TRACEPOINT;}
signals:
	void signal( QString t);
};

class SlotQString: public QObject
{
	Q_OBJECT
public:
	SlotQString(const std::function<void(QString)>& f, QObject* parent = nullptr):
		QObject{parent}, function_{f}
	{
		TRACEPOINT;
		if(!f) throw std::invalid_argument{"invalide function"};
		TRACEPOINT;
	}

	~SlotQString(){TRACEPOINT;}

public slots:
	void slot(QString t)
		{TRACEPOINT;function_(t);TRACEPOINT;}

private:
	std::function<void(QString)> function_;
};

}} // end namespaces qtutil, cvv
#endif //CVVISUAL_SIGNALEMITTER_HPP
