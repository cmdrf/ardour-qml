#ifndef TEMPOMAP_H
#define TEMPOMAP_H

#include "TimePos.h"

#include <QAbstractListModel>

class TempoMap : public QAbstractListModel
{
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

public:
	enum Type
	{
		MusicTimePointType,
		TempoPointType,
		MeterPointType
	};
	Q_ENUM(Type)

	enum
	{
		TimeRole = Qt::UserRole,
		TypeRole,
		NoteValueRole,
		DivisionsPerBarRole,
		NoteTypesPerMinuteRole
	};

	explicit TempoMap(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

private Q_SLOTS:
	void updateMap();

private:
	struct Entry
	{
		TimePos time;
		Type type;
		int noteValue;
		int divisionsPerBar;
		double noteTypesPerMinute;
	};
	QVector<Entry> m_entries;
};

#endif // TEMPOMAP_H
