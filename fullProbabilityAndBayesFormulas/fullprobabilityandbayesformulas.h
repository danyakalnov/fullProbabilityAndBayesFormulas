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

	uint fullGroupEventsNumber; // ����� �������, ���������� ������ ������
	QList <double> hypothesesProbabilities; // ����������� �������
	QList <double> hypothesesConditionalProbabilities; // �������� ����������� �������
	QList <uint> selectedHypothesesNumbers; // ������ ��������� ������������� �������

	bool hypothesisProbabilityFieldWasClicked;
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
};

#endif // FULLPROBABILITYANDBAYESFORMULAS_H
