#ifndef FULLPROBABILITYANDBAYESFORMULAS_H
#define FULLPROBABILITYANDBAYESFORMULAS_H

#include <QtWidgets/QMainWindow>
#include "ui_fullprobabilityandbayesformulas.h"
#include <QCheckBox>

class fullProbabilityAndBayesFormulas : public QMainWindow
{
	Q_OBJECT

public:
	fullProbabilityAndBayesFormulas(QWidget *parent = 0);
	~fullProbabilityAndBayesFormulas();

private:
	Ui::fullProbabilityAndBayesFormulasClass ui;

	uint fullGroupEventsNumber; // Число событий, образующих полную группу
	QList <double> hypothesesProbabilities; // Вероятности гипотез
	QList <double> hypothesesConditionalProbabilities; // Условные вероятности гипотез
	QList <uint> selectedHypothesesNumbers; // Номера выбранных пользователем гипотез

	uint currentHypothesis;
	QList <QCheckBox *> hypothesesBoxes;

	double fullProbability;
	QList <double> calculatedConditionalHypothesesProbabilities;

	void clearScrollAreaContent();

private slots:

	void saveInputData();

	void calculateProbability();

	void addHypothesesBoxes();

	void enterNextHypothesis();

	void displayFormula();

	void displayCalculatedResults();

	void clearAllData();

	void clearProbabilitiesList();
};

#endif // FULLPROBABILITYANDBAYESFORMULAS_H
