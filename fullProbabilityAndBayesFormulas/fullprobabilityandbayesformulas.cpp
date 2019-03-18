#include "fullprobabilityandbayesformulas.h"
#include "QVBoxLayout"
#include "QCheckBox"
#include "QTextCodec"
#include "QMessageBox"
#include "qdebug.h"

fullProbabilityAndBayesFormulas::fullProbabilityAndBayesFormulas(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	currentHypothesis = 1;

	QTextCodec *c = QTextCodec::codecForLocale();

	ui.hypothesesConditionalProbabilities->setTabToolTip( 0, c->toUnicode( "Все найденные условные вероятности гипотез" ) );
	ui.hypothesesConditionalProbabilities->setTabToolTip( 1, c->toUnicode( "Условные вероятности выбранных гипотез" ) );

	ui.hypothesisNumber->setText( "P(H1)" );
	ui.conditionalHypothesisNumber->setText( "P(A | H1)" );

	hypothesisProbabilityFieldWasClicked = false;

	connect( ui.calculateProbabilityBtn, SIGNAL( clicked() ), this, SLOT( calculateProbability() ) );
	connect( ui.fullGroupEventsNumber, SIGNAL( editingFinished() ), this, SLOT( addHypothesesBoxes() ) );
	connect( ui.saveHypothesesProbabilitiesBtn, SIGNAL( clicked() ), this, SLOT( enterNextHypothesis() ) );
	connect( ui.clearAllBtn, SIGNAL( clicked() ), this, SLOT( clearAllData() ) );

	ui.calculateProbabilityBtn->setDisabled( true );

	// Скрыть виджеты для отображения результатов, так как не определена формула для вычислений

	ui.conditionalProbabilitiesResultsText->setDisabled( true );
	ui.hypothesesConditionalProbabilities->setDisabled( true );
	ui.fullProbabilityResultText->setDisabled( true );
	ui.fullProbabilityResult->setDisabled( true );
}

fullProbabilityAndBayesFormulas::~fullProbabilityAndBayesFormulas()
{

}

void fullProbabilityAndBayesFormulas::enterNextHypothesis()
{
	if ( currentHypothesis <= ui.fullGroupEventsNumber->value() )
	{
		int numOfEvents = ui.fullGroupEventsNumber->value();
		QTextCodec *c = QTextCodec::codecForLocale();

		hypothesesProbabilities.append( ui.hypothesisProbability->value() );
		hypothesesConditionalProbabilities.append( ui.hypothesisConditionalProbability->value() );
		currentHypothesis++;

		ui.hypothesisProbability->setValue( ui.hypothesisProbability->minimum() );
		ui.hypothesisConditionalProbability->setValue( ui.hypothesisConditionalProbability->minimum() );

		if ( currentHypothesis <= numOfEvents )
		{
			ui.hypothesisNumber->setText( "P(H" + QString::number( currentHypothesis ) + ")" );
			ui.conditionalHypothesisNumber->setText( "P(A | H" + QString::number( currentHypothesis ) + ")" );
		}

		if ( currentHypothesis > numOfEvents )
		{
			ui.saveHypothesesProbabilitiesBtn->setDisabled( true );
			ui.calculateProbabilityBtn->setEnabled( true );

			// Проверка правильности ввода данных

			double sumOfHypothesesProbabilities = 0;
			for ( int hypothesis = 0; hypothesis < ui.fullGroupEventsNumber->value(); hypothesis++ )
				sumOfHypothesesProbabilities += hypothesesProbabilities[ hypothesis ];

			if ( sumOfHypothesesProbabilities != 1 )
			{
				currentHypothesis = 1;
				hypothesesProbabilities.clear();
				hypothesesConditionalProbabilities.clear();
				ui.hypothesisNumber->setText( "P(H1)" );
				ui.conditionalHypothesisNumber->setText( "P(A | H1)" );

				QMessageBox::warning( this, c->toUnicode( "Неверные входные данные" ), c->toUnicode( "Сумма вероятностей гипотез не равна единице" ) );

				ui.saveHypothesesProbabilitiesBtn->setEnabled( true );
				ui.calculateProbabilityBtn->setDisabled( true );
			}
		}	
	}
}

void fullProbabilityAndBayesFormulas::saveInputData()
{
	fullGroupEventsNumber = ui.fullGroupEventsNumber->value();

	// Очистка контейнера перед добавлением данных
	selectedHypothesesNumbers.clear();

	// Определение и отбор выбранных пользователем гипотез для вычисления их вероятностей
	for ( int hypothesis = 0; hypothesis < fullGroupEventsNumber; hypothesis++ )
		if ( hypothesesBoxes[ hypothesis ]->isChecked() )
			selectedHypothesesNumbers.append( hypothesis );
}


void fullProbabilityAndBayesFormulas::calculateProbability()
{
	saveInputData();
	displayFormula();

	// Вычисление по формуле полной вероятности

	fullProbability = 0.0000;

	for ( int numOfEvents = 0; numOfEvents < ui.fullGroupEventsNumber->value(); numOfEvents++ )
		fullProbability += hypothesesProbabilities[ numOfEvents ] * hypothesesConditionalProbabilities[ numOfEvents ];

	// Вычисления по формуле Байеса

	for ( int numOfEvents = 0; numOfEvents < ui.fullGroupEventsNumber->value(); numOfEvents++ )
		calculatedConditionalHypothesesProbabilities.append( ( hypothesesProbabilities[ numOfEvents ] * hypothesesConditionalProbabilities[ numOfEvents ] ) / fullProbability );

	displayCalculatedResults();
}

void fullProbabilityAndBayesFormulas::addHypothesesBoxes()
{
	QTextCodec *c = QTextCodec::codecForLocale();

	if(ui.hypothesesVariants->widget() != 0)
		clearScrollAreaContent();

	QVBoxLayout *checkBoxesLayout = new QVBoxLayout;
	for ( int numOfHypothesis = 1; numOfHypothesis <= ui.fullGroupEventsNumber->value(); numOfHypothesis++ )
	{
		hypothesesBoxes.append( new QCheckBox( c->toUnicode("Гипотеза ") + QString::number( numOfHypothesis ) ) );
		checkBoxesLayout->addWidget( hypothesesBoxes[ numOfHypothesis - 1 ] );
	}

	ui.hypothesesVariants->widget()->setLayout( checkBoxesLayout );
}

void fullProbabilityAndBayesFormulas::displayFormula()
{
	QPixmap *formula;

	if ( ui.fullProbabilityRadioBtn->isChecked() )
	{
		formula = new QPixmap( "fullProbabilityFormula.jpg" );
		ui.formula->setPixmap( *formula );	
	}

	else if ( ui.bayesRadioBtn->isChecked() )
	{
		formula = new QPixmap( "bayesFormula.jpg" );
		ui.formula->setPixmap( *formula );	
	}
}

void fullProbabilityAndBayesFormulas::displayCalculatedResults()
{
	if ( ui.fullProbabilityRadioBtn->isChecked() )
	{
		ui.fullProbabilityResultText->setEnabled( true );
		ui.fullProbabilityResult->setEnabled( true );

		ui.fullProbabilityResult->setText( QString::fromStdString( std::to_string( long double( fullProbability ) ) ) );
	}

	else if ( ui.bayesRadioBtn->isChecked() )
	{
		ui.conditionalProbabilitiesResultsText->setEnabled( true );
		ui.hypothesesConditionalProbabilities->setEnabled( true );

		// Очистка List Widget'ов перед добавлением элементов

		ui.allConditionalProbabilities->clear();
		ui.selectedConditionalProbabilities->clear();

		// Добавление всех найденных условных вероятностей гипотез в первый лист и выбранных условных вероятностей во второй

		for ( int hypothesisNumber = 0; hypothesisNumber < ui.fullGroupEventsNumber->value(); hypothesisNumber++ )
		{
			QString hypothesisConditionalProbabilityText = "P(H" + QString::number( hypothesisNumber + 1 ) + " | A" + ") = " + QString::fromStdString ( std::to_string( long double( calculatedConditionalHypothesesProbabilities[ hypothesisNumber ] ) ) );
			QListWidgetItem *hypothesisConditionalProbability = new QListWidgetItem;
			hypothesisConditionalProbability->setText( hypothesisConditionalProbabilityText );
			ui.allConditionalProbabilities->addItem( hypothesisConditionalProbability );

			if( selectedHypothesesNumbers.contains( hypothesisNumber ) )
			{
				QListWidgetItem *hypothesisConditionalProbability = new QListWidgetItem;
				hypothesisConditionalProbability->setText( hypothesisConditionalProbabilityText );
				ui.selectedConditionalProbabilities->addItem( hypothesisConditionalProbability );
			}
		}
	}
}

void fullProbabilityAndBayesFormulas::clearAllData()
{
	// Очистка числа событий, составляющих полную группу (в форме + поле класса)
	fullGroupEventsNumber = 1;
	ui.fullGroupEventsNumber->setValue( ui.fullGroupEventsNumber->minimum() );

	// Очистка контейнеров, содержащих введённые пользователем вероятности гипотез и условные вероятности события
	hypothesesProbabilities.clear();
	hypothesesConditionalProbabilities.clear();

	// Очистка выбранных пользователем гипотез

	clearScrollAreaContent();

	// Выбрать формулу полной вероятности в группе радио-кнопок
	ui.fullProbabilityRadioBtn->setChecked( true );

	// Удалить формулу

	QPixmap *emptyPixmap = new QPixmap( 0, 0 );
	ui.formula->setPixmap( *emptyPixmap );

	//Текущая гипотеза - первая
	currentHypothesis = 1;

	// Дезактивировать кнопку для вычисления вероятности(ей)
	ui.calculateProbabilityBtn->setDisabled( true );

	// Активировать кнопку для сохранения вероятностей

	ui.saveHypothesesProbabilitiesBtn->setEnabled( true );

	// Скрыть виджеты для отображения результатов, так как не определена формула для вычислений
	ui.conditionalProbabilitiesResultsText->setDisabled( true );
	ui.hypothesesConditionalProbabilities->setDisabled( true );
	ui.fullProbabilityResultText->setDisabled( true );
	ui.fullProbabilityResult->setDisabled( true );

	// Очистить результаты предыдущих вычислений
	fullProbability = 0.0000;
	calculatedConditionalHypothesesProbabilities.clear();

	ui.fullProbabilityResult->setText( "" );
	ui.allConditionalProbabilities->clear();
	ui.selectedConditionalProbabilities->clear();

	ui.hypothesisNumber->setText( "P(H1)" );
	ui.conditionalHypothesisNumber->setText( "P(A | H1)" );
}

void fullProbabilityAndBayesFormulas::clearScrollAreaContent()
{
	QList <QWidget *> innerScrollAreaWidgets = ui.hypothesesVariants->widget()->findChildren<QWidget *>( QString(), Qt::FindDirectChildrenOnly );
	foreach( QWidget *child, innerScrollAreaWidgets )
	{
		delete child;	
	}

	delete ui.hypothesesVariants->widget()->layout();

	hypothesesBoxes.clear();
	selectedHypothesesNumbers.clear();

	//hypothesisProbabilityFieldWasClicked = false;
}