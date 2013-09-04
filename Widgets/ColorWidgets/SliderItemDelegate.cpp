#include "SliderItemDelegate.h"

SliderItemDelegate::SliderItemDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{

}


void SliderItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();

	if (option.state & QStyle::State_Selected)
	{
		painter->fillRect(option.rect, option.palette.highlight());
	}

	int width = option.rect.width();
	int height = option.rect.height();

	QString text = index.data(Qt::DisplayRole).toString();
	QColor color = index.data(Qt::BackgroundRole).value<QColor>();
	QRect colorRect (option.rect.x()+5, option.rect.y()+5, 50, height-10);

	painter->fillRect(colorRect, color);
	painter->drawRect(colorRect);
	painter->drawText(QRect(option.rect.x()+60, option.rect.y()+5, width-60, height-10), Qt::AlignVCenter, text);

	painter->restore();
}


QSize SliderItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QSize(100, 40);
}


//QWidget *SliderItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	QLineEdit *editor = new QLineEdit(parent);
//	return editor;
//}


//void SliderItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{

//}


//void SliderItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{

//}
