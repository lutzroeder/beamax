#include "stdafx.h"
#include "Resource.h"
#include "Application.h"
#include "Dialog.h"
#include "Object.h"
#include "Document.h"
#include "View.h"

Application _application;

IMPLEMENT_DYNAMIC(ApplicationWindow, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(ApplicationWindow, CMDIFrameWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_COMMAND(ID_HELP_INDEX, CMDIFrameWnd::OnHelpIndex)
END_MESSAGE_MAP()

static UINT _toolBarButtons[] =
{
    ID_FILE_NEW, ID_FILE_OPEN, ID_FILE_SAVE, ID_SEPARATOR,
    IDM_PROPERTIES, ID_SEPARATOR,
    IDM_FIXED_SUPPORT, IDM_HINGED_SUPPORT, IDM_ROLLER_SUPPORT, ID_SEPARATOR,
    IDM_POINT_LOAD, IDM_LINEAR_DISTRIBUTED_LOAD, ID_SEPARATOR,
    IDM_DELETE, ID_SEPARATOR,
    IDM_VIEW_CONTINUOUS_BEAM, IDM_VIEW_SHEAR_FORCE,
    IDM_VIEW_BENDING_MOMENT, IDM_VIEW_DISPLACEMENT, ID_SEPARATOR,
    IDM_VIEW_NUMERICAL_VALUES
};

int ApplicationWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // initialize toolbar
    if (!_toolBar.Create(this) || !_toolBar.LoadBitmap(IDR_MAIN_FRAME) || !_toolBar.SetButtons(_toolBarButtons, sizeof(_toolBarButtons) / sizeof(UINT)))
    {
        return -1;
    }
    _toolBar.SetBarStyle(_toolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
    _toolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&_toolBar);

    _toolBar.SetWindowText("ToolBar");

    // translate view buttons into check boxes
    int image;
    UINT style;
    UINT id;

    for (int i = 15; i <= 17; i++)
    {
        _toolBar.GetButtonInfo(i, id, style, image);
        _toolBar.SetButtonInfo(i, id, TBBS_CHECKBOX, image);
    }
    _toolBar.GetButtonInfo(20, id, style, image);
    _toolBar.SetButtonInfo(20, id, TBBS_CHECKBOX, image);

    return 0;
}

BEGIN_MESSAGE_MAP(Application, CWinApp)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
    ON_COMMAND(ID_FILE_NEW, OnFileCreate)
    ON_COMMAND(IDM_ABOUT, OnAbout)
    ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

extern "C" const GUID CLSID_System = { 0xC48F2271, 0x54E5, 0x11D1, { 0xBC, 0x97, 0x00, 0x80, 0x5F, 0x8C, 0x5D, 0x94 } };

Application::Application()
{
    _beamLength = 10.0;
    _beamCreate = TRUE;
}

BOOL Application::InitInstance()
{
    // initialize OLE libraries
    if (!AfxOleInit())
    {
        return FALSE;
    }

    // enable application settings
    LoadStdProfileSettings();

    // register document and view class
    CMultiDocTemplate * DocTemplate;
    DocTemplate = new CMultiDocTemplate(IDR_DOCUMENT, RUNTIME_CLASS(Document), RUNTIME_CLASS(CMDIChildWnd), RUNTIME_CLASS(View));
    AddDocTemplate(DocTemplate);

    ApplicationWindow* applicationWindow = new ApplicationWindow;
    if (!applicationWindow->LoadFrame(IDR_MAIN_FRAME))
    {
        return FALSE;
    }

    m_pMainWnd = applicationWindow;

    // enable drag and drop
    m_pMainWnd->DragAcceptFiles();

    // register object factories
    _oleTemplateServer.ConnectTemplate(CLSID_System, DocTemplate, FALSE);
    COleTemplateServer::RegisterAll();

    // if automation return
    if (RunEmbedded() || RunAutomated())
    {
        return TRUE;
    }

    // add objects to system registry
    _oleTemplateServer.UpdateRegistry(OAT_DISPATCH_OBJECT);
    COleObjectFactory::UpdateRegistryAll();

    m_pMainWnd->ShowWindow(m_nCmdShow);
    m_pMainWnd->UpdateWindow();

    // phrase command line
    if (m_lpCmdLine[0] != '\0')
    {
        OpenDocumentFile(m_lpCmdLine);
    }

    return TRUE;
}

void Application::OnFileCreate()
{
    CreateBeamDialog dialog;

    dialog.SetLength(_beamLength);
    if (dialog.DoModal() == IDOK)
    {
        _beamLength = dialog.GetLength();
        _beamCreate = TRUE;
    }
    else
    {
        _beamCreate = FALSE;
    }

    OnFileNew();
}

void Application::OnAbout(void)
{
    AboutDialog dialog;
    dialog.DoModal();
}
