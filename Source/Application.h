#ifndef __Beamax__
#define __Beamax__

class ApplicationWindow : public CMDIFrameWnd
{
private:
    CToolBar _toolBar;

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    DECLARE_MESSAGE_MAP()
    DECLARE_DYNAMIC(ApplicationWindow)
};

class Application : public CWinApp
{
private:
    COleTemplateServer _oleTemplateServer;

public:
    Application();
    virtual BOOL InitInstance();
    void DefaultAnalysis();
    void OnFileCreate();

    double _beamLength;
    BOOL _beamCreate;

protected:
    afx_msg void OnAbout(void);

    DECLARE_MESSAGE_MAP()
};

#endif