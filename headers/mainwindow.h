#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QSlider>
#include <QPointer>
#include <memory>
#include <QSqlDatabase>
#include <QJsonObject>
#include <QHBoxLayout>

// Forward declarations
namespace Ui {
class MainWindow;
}
class AnswerBox;
class CircleTimer;
class GeminiAI;
class TypingAnimation;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QJsonObject getQuestion(){return question;};
    ~MainWindow();
    // ... existing public members ...
    int getCurrentQuestionIndex() const { return currentQuestionIndex; }
    void setCurrentQuestionIndex(int index) { currentQuestionIndex = index; }
    QVector<QJsonObject> getQuizQuestions() const { return quizQuestions; } // Getter for the questions list
    void displayQuestion(int index); // New function to display a question
    void endQuizSession(); // New function to handle the end of the quiz
    void handleAnswer(bool isCorrect); // function to handle answers
    void updateScoreDisplay();
    void saveHighScore(int newScore);
    void cleanupQuizWidgets();
    void showDomainSelectionScreen();
    void cleanupQuizSession();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_startBtn_clicked();
    void on_settingsBtn_clicked();
    void on_infoBtn_clicked();
    void on_statsBtn_clicked();
    void onDomainButtonClicked();
    void updateVolume(int value);
    void onBackButtonClicked();
    void onPauseClicked();
    void onResumeclicked();
    void on_restartBtn_clicked();
    void onAnswerBoxClicked(AnswerBox *box);

private:
    // UI Initialization
    void initializeUI();
    void initializeNullPointers();
    void setupMainButtons();
    void setupButton(QPushButton *button, const QString &iconName);
    void applyShadowEffect(QWidget *widget, int blurRadius, const QColor &color);
    void createBackButton();
    void createPauseButton();

    // Game Flow
    void DomainsChoose();
    void questionsPage(const QString &domain, int QstNum);
    void startProgressBar(QProgressBar *progressBar, QLabel *waitLabel, TypingAnimation *typing);
    QJsonObject generateQuestion(QString domain, int session);
    QString getDomainFromButton(QPushButton *button);
    void generateQuestionsForDomain(const QString &domain);
    void startQuizSession(const QString &domain, int session);

    // Widget Management
    template <typename T> void clearWidgets();
    void deleteButtons(const std::initializer_list<QPushButton *> &buttons);
    void replaceCentralWidgetLayout(QLayout *newLayout);
    void updateWidgetPositionsOnResize();
    void setupDomainSelectionLayout(QLabel *domainHead, QPushButton *buttons[]);
    void setupQuizLayout(const QString &domain, int sessionId);
    void createQuizPageLayout(QLabel *domainNameTxt, QLabel *domainName, QLabel *scoreTxt,
                                          QLabel *score, QLabel *questionLabel,
                              AnswerBox *B1, AnswerBox *B2, AnswerBox *B3, AnswerBox *B4);

    int currentQuestionIndex = 0; // Index of the current question
    QVector<QJsonObject> quizQuestions; // All questions for this session


    // Widget Creators
    QProgressBar *createProgressBar();
    QLabel *createLabel(const QString &text, int fontSize, Qt::Alignment alignment);
    QPushButton *createDomainButton(const QString &iconName);
    QPointer<QWidget> m_endQuizContainer;

    // Pause Menu
    void pausewindow();
    QString createInstructionsContent();
    void adjustDomainLayout(QHBoxLayout *firstRowLayout, QHBoxLayout *secondRowLayout, QPushButton *buttons[]);

    // Database
    bool connectToDatabase();
    bool createMcqTable();

    // UI Components
    std::unique_ptr<Ui::MainWindow> ui;
    QPushButton *exitBtn = nullptr;
    QPushButton *infoBtn;
    QGridLayout *answersLayout = nullptr;

    // Pause Menu Components
    QWidget *pauseOverlay = nullptr;
    QWidget *pausecontainer = nullptr;
    QLabel *volumetxt = nullptr;
    QLabel *loadingLabel = nullptr;
    QSlider *volumeSlider = nullptr;

    // Game Components
    GeminiAI *geminiAI = nullptr;
    SettingsDialog *settingsDialog = nullptr;
    bool ispaused = false;

    // Game Stats
    int highScore = 0;
    int totalGamesPlayed = 0;
    int gamesWon = 0;
    int averageScore = 0;
    int longestStreak = 0;
    int fastestTime = -1;
    int totalCorrectAnswers = 0;
    int totalIncorrectAnswers = 0;
    int currentScore = 0;
    int correctStreak = 0;
    QTime answerTime; // To track when question was displayed
    QLabel* scoreLabel = nullptr; // Pointer to the score display label

    // variables
    int sessionId = 0;
    QJsonObject question;

    QLabel *score;
    // For quiz end container
    QPointer<QPushButton> backbutton;      // Use QPointer for automatic nulling
    CircleTimer *timer = nullptr;        // For timer object
    QPointer<QPushButton> pauseBtn;        // For pause button
    QTimer* m_cleanupTimer = nullptr;
    QWidget* m_resultsContainer = nullptr;
    TypingAnimation* typingEffect = nullptr;

};

#endif // MAINWINDOW_H
