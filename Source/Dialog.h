#ifndef __Dialog__
#define __Dialog__

#include "stdafx.h"
#include "Resource.h"

class CreateBeamDialog : public CDialog
{
    double _length = 0.0;

public:
    CreateBeamDialog() : CDialog(CreateBeamDialog::IDD) {}
    void SetLength(double length);
    double GetLength();

    enum { IDD = IDD_CREATE_BEAM };

protected:
    void DoDataExchange(CDataExchange* pDX);
};

class PropertiesDialog : public CDialog
{
private:
    double _modulusOfElasticity = 0.0;
    double _momentOfInertia = 0.0;
    double _axialArea = 0.0;

public:
    PropertiesDialog() : CDialog(PropertiesDialog::IDD) {}
    void SetModulusOfElasticity(double value);
    double GetModulusOfElasticity();
    void SetMomentOfInertia(double value);
    double GetMomentOfInertia();
    void SetAxialArea(double value);
    double GetAxialArea();

    enum { IDD = IDD_PROPERTIES };

protected:
    void DoDataExchange(CDataExchange* pDX);
};

class FixedSupportDialog : public CDialog
{
    BOOL _position = FALSE;

public:
    FixedSupportDialog() : CDialog(FixedSupportDialog::IDD) {}
    void SetPosition(BOOL value);
    BOOL GetPosition();
    void OnPositionStart();
    void OnPositionEnd();

    enum { IDD = IDD_FIXED_SUPPORT };

protected:
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
};

class HingedSupportDialog : public CDialog
{
    double _position;

public:
    HingedSupportDialog() : CDialog(HingedSupportDialog::IDD)
    {
        _position = 0.0;
    }
    void SetPosition(double value);
    double GetPosition();

    enum { IDD = IDD_HINGED_SUPPORT };

protected:
    void DoDataExchange(CDataExchange* pDX);
};

class RollerSupportDialog : public CDialog
{
    double _position;

public:
    RollerSupportDialog() : CDialog(RollerSupportDialog::IDD)
    {
        _position = 0.0;
    }
    void SetPosition(double value);
    double GetPosition();

    enum { IDD = IDD_ROLLER_SUPPORT };

protected:
    void DoDataExchange(CDataExchange* pDX);
};

class PointLoadDialog : public CDialog
{
    double _position;
    double _value;

public:
    PointLoadDialog() : CDialog(PointLoadDialog::IDD)
    {
        _position = 0.0;
        _value = 0.0;
    }
    void SetPosition(double value);
    double GetPosition();
    void SetValue(double value);
    double GetValue();

    enum { IDD = IDD_POINT_LOAD };

protected:
    void DoDataExchange(CDataExchange * DX);
};

class LinearDistributedLoadDialog : public CDialog
{
    double _position;
    double _value;
    double _length;

public:
    LinearDistributedLoadDialog() : CDialog(LinearDistributedLoadDialog::IDD)
    {
        _position = 0.0;
        _value = 0.0;
        _length = 0.0;
    }
    void SetPosition(double value);
    double GetPosition();
    void SetValue(double value);
    double GetValue();
    void SetLength(double value);
    double GetLength();

    enum { IDD = IDD_LINEAR_DISTRIBUTED_LOAD };

protected:
    void DoDataExchange(CDataExchange* pDX);
};

class AboutDialog : public CDialog
{
public:
    AboutDialog() : CDialog(AboutDialog::IDD)
    {
    }

    enum { IDD = IDD_ABOUT };
};

#endif
