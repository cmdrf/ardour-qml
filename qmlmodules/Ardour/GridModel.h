#ifndef BEATMARKERMODEL_H
#define BEATMARKERMODEL_H

#include <temporal/tempo.h>

#include <QAbstractListModel>
#include <QtQmlIntegration>

/// Model that provides positions for grid lines
/** Provides grid line positions between startSamples and endSamples. Does some optimisation so
	you can bind these properties to your current scroll position without GridModel resetting itself
	all the time. */
class GridModel : public QAbstractListModel
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(qint64 startSamples READ startSamples WRITE setStartSamples NOTIFY startSamplesChanged FINAL)
	Q_PROPERTY(qint64 endSamples READ endSamples WRITE setEndSamples NOTIFY endSamplesChanged FINAL)
	Q_PROPERTY(int barModulo READ barModulo WRITE setBarModulo NOTIFY barModuloChanged FINAL)
	Q_PROPERTY(int beatDivision READ beatDivision WRITE setBeatDivision NOTIFY beatDivisionChanged FINAL)

public:
	enum
	{
		TimeRole = Qt::UserRole
	};

	explicit GridModel(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

	qint64 startSamples() const {return m_startSamples;}
	void setStartSamples(qint64 newStartSamples);

	qint64 endSamples() const {return m_endSamples;}
	void setEndSamples(qint64 newEndSamples);

	int barModulo() const {return m_barModulo;}
	void setBarModulo(int newBarModulo);

	int beatDivision() const {return m_beatDivision;}
	void setBeatDivision(int newBeatDivision);

Q_SIGNALS:
	void startSamplesChanged();
	void endSamplesChanged();
	void barModuloChanged();
	void beatDivisionChanged();

private Q_SLOTS:
	void updateMap();

private:
	void maybeUpdateMap();

	Temporal::TempoMapPoints m_points;
	qint64 m_startSamples = 0; ///< Start samples chosen by user
	qint64 m_endSamples = 0; ///< End sample chosen by user
	qint64 m_actualStartSamples = 0; ///< Start sample currently describing m_points
	qint64 m_actualEndSamples = 0; ///< End sample currently describing m_points
	int m_barModulo = 0;
	int m_beatDivision = 1;
};

#endif // BEATMARKERMODEL_H
