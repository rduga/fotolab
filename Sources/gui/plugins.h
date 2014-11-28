#ifndef PLUGINS_H
#define PLUGINS_H

// TODO how to create qt plugins

class AbstractPlugin
{
public:
	AbstractPlugin(QString);

	virtual ~AbstractPlugin();

	QVariant* getProperty(const QString& prop) {
		return &QVariant();
	}


	QGraphicsItem* toGraphics(const QGraphicsItem& inputitem) {

	}

protected:
	QSettings* settings;
	QGraphicsItem* inputitem;

	virtual void processPlugin();



};

/*
 * uchovava hodnoty
 * vstup:
 * 		- obrazok
 * 		- data ineho pluginu
 * vystup
 * 		- qgraphicsitem
 *
 * - metody pre pristup k premennym
 */

#endif // PLUGINS_H
