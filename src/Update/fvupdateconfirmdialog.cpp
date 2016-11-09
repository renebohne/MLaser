#include "fvupdateconfirmdialog.h"
#include "fvavailableupdate.h"
#include "fvupdater.h"
#include "ui_fvupdateconfirmdialog.h"
#include <QCloseEvent>
#include "fvupdater.h"

FvUpdateConfirmDialog::FvUpdateConfirmDialog(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::FvUpdateConfirmDialog)
{
	m_ui->setupUi(this);

	// Delete on close
	setAttribute(Qt::WA_DeleteOnClose, true);

	// Set the "close app, then reopen" string
	QString closeReopenString = m_ui->downloadThisUpdateLabel->text().arg(QString::fromUtf8(FV_APP_NAME));
	m_ui->downloadThisUpdateLabel->setText(closeReopenString);

	// Connect buttons
	connect(m_ui->confirmButtonBox, SIGNAL(accepted()),
			FvUpdater::sharedUpdater(), SLOT(UpdateInstallationConfirmed()));
	connect(m_ui->confirmButtonBox, SIGNAL(rejected()),
			FvUpdater::sharedUpdater(), SLOT(UpdateInstallationNotConfirmed()));

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("更新"));
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(m_ui->frame);
    pLayout->setSpacing(0);
     pLayout->addWidget(m_ui->frameOKBtn);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

FvUpdateConfirmDialog::~FvUpdateConfirmDialog()
{
	delete m_ui;
}

bool FvUpdateConfirmDialog::UpdateWindowWithCurrentProposedUpdate()
{
	FvAvailableUpdate* proposedUpdate = FvUpdater::sharedUpdater()->GetProposedUpdate();
	if (! proposedUpdate) {
		return false;
	}

	QString downloadLinkString = m_ui->updateFileLinkLabel->text()
			.arg(proposedUpdate->GetEnclosureUrl().toString());
	m_ui->updateFileLinkLabel->setText(downloadLinkString);

	return true;
}

void FvUpdateConfirmDialog::closeEvent(QCloseEvent* event)
{
	FvUpdater::sharedUpdater()->updateConfirmationDialogWasClosed();
	event->accept();
}