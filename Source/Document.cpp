#include <stdio.h> 
#include <math.h>
#include "stdafx.h"
#include "Application.h"
#include "Dialog.h"
#include "Object.h"
#include "Document.h"

IMPLEMENT_DYNCREATE(Document, CDocument)

BEGIN_MESSAGE_MAP(Document, CDocument)
    ON_COMMAND(IDM_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

Document::Document()
{
    EnableAutomation();
    AfxOleLockApp();

    // default values
    _beamLength = ((Application*)AfxGetApp())->_beamLength;
    _modulusOfElasticity = 210000 * 1000;
    _momentOfInertia = 0.0000836;
    _axialArea = 0.00538;

    // no object is selected
    _selected = NULL;
}

Document::~Document()
{
    CoFreeUnusedLibraries();
    AfxOleUnlockApp();
}

BOOL Document::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
    {
        return FALSE;
    }

    ((Application*)AfxGetApp())->_beamLength = 10;

    if (((Application*)AfxGetApp())->_beamCreate)
    {
        ((Application*)AfxGetApp())->_beamCreate = TRUE;
        return TRUE;
    }
    else
    {
        ((Application*)AfxGetApp())->_beamCreate = TRUE;
        return FALSE;
    }
}

void Document::Serialize(CArchive& archive)
{
    if (archive.IsLoading())
    {
        archive >> _beamLength;
        archive >> _modulusOfElasticity >> _momentOfInertia;
        _objectList.Serialize(archive);
    }
    else
    {
        archive << _beamLength;
        archive << _modulusOfElasticity << _momentOfInertia;
        _objectList.Serialize(archive);
    }
}

BOOL Document::InsertObject(ObjectCast* item)
{
    if (item == NULL)
        return FALSE;

    if (CLASSOF(item, "HingedSupport") || CLASSOF(item, "FixedSupport") || CLASSOF(item, "RollerSupport"))
    {
        ObjectCast* object;
        POSITION position = _objectList.GetHeadPosition();
        while (position != NULL)
        {
            object = (ObjectCast*)_objectList.GetNext(position);
            if (object != NULL)
            {
                if (CLASSOF(item, "HingedSupport") || CLASSOF(item, "FixedSupport") || CLASSOF(item, "RollerSupport"))
                {
                    if (fabs(((SupportCast *)object)->GetPosition() - ((SupportCast *)item)->GetPosition()) < EPSILON)
                    {
                        return FALSE;
                    }
                }
            }
        }
    }

    if (_objectList.IsEmpty())
    {
        _objectList.AddHead(item);
    }
    else
    {
        _objectList.AddTail(item);
    }

    return TRUE;
}

void Document::DeleteObject(ObjectCast* item)
{
    POSITION position = _objectList.GetHeadPosition();
    position = _objectList.Find(item);
    if (position != NULL)
    {
        _objectList.RemoveAt(position);
    }
}

int Document::SortLoadLevels(CDC* pDC, double scaleX)
{
    int maximumLevel = 1;
    LoadCast* currentObject = NULL;

    // set the level of all object to zero
    ObjectCast* object = NULL;
    POSITION position = _objectList.GetHeadPosition();
    while (position != NULL)
    {
        object = (ObjectCast*)_objectList.GetNext(position);
        if ((object != NULL) && (CLASSOF(object, "PointLoad") || CLASSOF(object, "LinearDistributedLoad")))
        {
            ((LoadCast*)object)->SetLevel(0);
        }
    }

    // sort all load objects
    do
    {
        currentObject = NULL;
        double currentStart = 0;
        double currentLength = 0;

        // scan for the biggest load object at level 0
        ObjectCast* object;
        POSITION position = _objectList.GetHeadPosition();
        while (position != NULL)
        {
            object = (ObjectCast*)_objectList.GetNext(position);
            if ((object != NULL) && (CLASSOF(object, "PointLoad") || CLASSOF(object, "LinearDistributedLoad")))
            {
                double start;
                double length;
                ((LoadCast*)object)->GetExtent(pDC, start, length, scaleX);
                if ((((LoadCast*)object)->GetLevel() == 0) && (length > currentLength))
                {
                    currentObject = (LoadCast*)object;
                    currentStart = start;
                    currentLength = length;
                }
            }
        }

        // exit if no object found
        if (currentObject == NULL)
        {
            return maximumLevel;
        }

        // increase the level of the object until it fits 
        int currentLevel = 0;
        BOOL match = FALSE;
        do
        {
            match = FALSE;
            currentLevel++;
            currentObject->SetLevel(currentLevel);
            ObjectCast* object;
            POSITION position = _objectList.GetHeadPosition();
            while (position != NULL)
            {
                object = (ObjectCast*)_objectList.GetNext(position);
                if ((object != NULL) && (object != currentObject) && (CLASSOF(object, "PointLoad") || CLASSOF(object, "LinearDistributedLoad")))
                {
                    int level;
                    double start;
                    double length;
                    level = ((LoadCast*)object)->GetLevel();
                    ((LoadCast*)object)->GetExtent(pDC, start, length, scaleX);
                    if (level == currentLevel)
                    {
                        if (((currentStart >= start) &&
                            (currentStart <= (start + length))) ||
                            (((currentStart + currentLength) >= start) &&
                            ((currentStart + currentLength) <= (start + length))))
                        {
                            match = TRUE;
                        }
                    }
                }
            }
        } while (match);

        // update maximum level
        if (maximumLevel < currentLevel) maximumLevel = currentLevel;
    } while (currentObject != NULL);

    return maximumLevel;
}

BOOL Document::Analyse()
{
    Beam* analysis = new Beam();

    // create beam
    analysis->SetLength(_beamLength);
    analysis->SetEI(_modulusOfElasticity, _momentOfInertia);

    // transfer objects
    ObjectCast* object;
    POSITION position = _objectList.GetHeadPosition();
    while (position != NULL)
    {
        object = (ObjectCast*)_objectList.GetNext(position);
        if (object != NULL)
        {
            if (CLASSOF(object, "FixedSupport"))
                analysis->CreateFixedSupport(((SupportCast *)object)->GetPosition());
            if (CLASSOF(object, "HingedSupport"))
                analysis->CreateHingedSupport(((SupportCast *)object)->GetPosition());
            if (CLASSOF(object, "RollerSupport"))
                analysis->CreateRollerSupport(((SupportCast *)object)->GetPosition());
            if (CLASSOF(object, "PointLoad"))
                analysis->CreatePointLoad(((LoadCast*)object)->GetPosition(), ((LoadCast*)object)->GetValue());
            if (CLASSOF(object, "LinearDistributedLoad"))
                analysis->CreateLinearDistributedLoad(((LoadCast*)object)->GetPosition(), ((LoadCast*)object)->GetValue(), ((LoadCast*)object)->GetLength());
        }
    }

    // start analysis
    analysis->Analyse();

    // delete old results
    _shearForceList.RemoveAll();
    _bendingMomentList.RemoveAll();
    _displacementList.RemoveAll();

    // retrieve results
    Section* shearForce;
    Section* bendingMoment;
    Section* displacement;
    HRESULT result = 0;
    do
    {
        shearForce = new Section;
        bendingMoment = new Section;
        displacement = new Section;
        result = analysis->GetNextSection(shearForce, bendingMoment, displacement);
        _shearForceList.AddHead((CObject *)shearForce);
        _bendingMomentList.AddHead((CObject *)bendingMoment);
        _displacementList.AddHead((CObject *)displacement);
    } while (result == S_OK);

    delete analysis;

    return TRUE;
}

void Document::OnProperties()
{
    PropertiesDialog dialog;

    dialog.SetModulusOfElasticity(_modulusOfElasticity / 1000);
    dialog.SetMomentOfInertia(_momentOfInertia * (100 * 100 * 100 * 100));
    dialog.SetAxialArea(_axialArea * (100 * 100));

    if (dialog.DoModal() == IDOK)
    {
        _modulusOfElasticity = dialog.GetModulusOfElasticity() * 1000;
        _momentOfInertia = dialog.GetMomentOfInertia() / (100 * 100 * 100 * 100);
        _axialArea = dialog.GetAxialArea() / (100 * 100);
    }

    UpdateAllViews(NULL);
}

BOOL Document::CreateBeam(double length, double modulusOfElasticity, double momentOfInertia, double axialArea)
{
    _beamLength = length;
    _modulusOfElasticity = modulusOfElasticity;
    _momentOfInertia = momentOfInertia;
    _axialArea = axialArea;
    return TRUE;
}

BOOL Document::CreateFixedSupport(double position)
{
    SupportCast * SupportObject = NULL;
    if ((position != 0) && (position != _beamLength)) return FALSE;
    SupportObject = new FixedSupport;
    if (SupportObject == NULL) return FALSE;
    SupportObject->SetPosition(position);
    if (!InsertObject(SupportObject))
    {
        delete SupportObject;
        return FALSE;
    }
    _selected = SupportObject;
    return TRUE;
}

BOOL Document::CreateHingedSupport(double position)
{
    SupportCast * SupportObject = NULL;
    if ((position < 0) || (position > _beamLength)) return FALSE;
    SupportObject = new HingedSupport;
    if (SupportObject == NULL) return FALSE;
    SupportObject->SetPosition(position);
    if (!InsertObject(SupportObject))
    {
        delete SupportObject;
        return FALSE;
    }
    _selected = SupportObject;
    return TRUE;
}

BOOL Document::CreateRollerSupport(double position)
{
    SupportCast * SupportObject = NULL;
    if ((position < 0) || (position > _beamLength)) return FALSE;
    SupportObject = new RollerSupport;
    if (SupportObject == NULL) return FALSE;
    SupportObject->SetPosition(position);
    if (!InsertObject(SupportObject))
    {
        delete SupportObject;
        return FALSE;
    }
    _selected = SupportObject;
    return TRUE;
}

BOOL Document::CreatePointLoad(double position, double value)
{
    LoadCast* loadObject = NULL;
    if ((position < 0) || (position > _beamLength)) return FALSE;
    loadObject = new PointLoad;
    if (loadObject == NULL) return FALSE;
    loadObject->SetValue(value);
    loadObject->SetPosition(position);
    if (!InsertObject(loadObject))
    {
        delete loadObject;
        return FALSE;
    }
    _selected = loadObject;
    return TRUE;
}

BOOL Document::CreateLinearDistributedLoad(double position, double value,
    double length)
{
    LoadCast* loadObject = NULL;
    if ((length < 0) || (position < 0) || ((position + length) > _beamLength))
    {
        return FALSE;
    }
    loadObject = new LinearDistributedLoad;
    if (loadObject == NULL)
    {
        return FALSE;
    }
    loadObject->SetValue(value);
    loadObject->SetPosition(position);
    loadObject->SetLength(length);
    if (!InsertObject(loadObject))
    {
        delete loadObject;
        return FALSE;
    }
    _selected = loadObject;
    return TRUE;
}

double Document::GetShearForce(double position)
{
    Section* object;
    POSITION pointer = _shearForceList.GetHeadPosition();
    while (pointer != NULL)
    {
        object = (Section*)_shearForceList.GetNext(pointer);
        if (object != NULL)
        {
            if ((position >= object->Start) && (position <= (object->Start + object->Length)))
            {
                double x = position - object->Start;
                return (object->A4 * x * x * x * x + object->A3 * x * x * x + object->A2 * x * x + object->A1 * x + object->A0);
            }
        }
    }
    return 0;
}

double Document::GetBendingMoment(double position)
{
    Section* object;
    POSITION pointer = _bendingMomentList.GetHeadPosition();
    while (pointer != NULL)
    {
        object = (Section*)_bendingMomentList.GetNext(pointer);
        if (object != NULL)
        {
            if ((position >= object->Start) && (position <= (object->Start + object->Length)))
            {
                double x = position - object->Start;
                return (object->A4 * x * x * x * x + object->A3 * x * x * x + object->A2 * x * x + object->A1 * x + object->A0);
            }
        }
    }
    return 0;
}

double Document::GetDisplacement(double position)
{
    Section* object;
    POSITION pointer = _displacementList.GetHeadPosition();
    while (pointer != NULL)
    {
        object = (Section*)_displacementList.GetNext(pointer);
        if (object != NULL)
        {
            if ((position >= object->Start) &&
                (position <= (object->Start + object->Length)))
            {
                double x = position - object->Start;
                return (object->A4 * x * x * x * x + object->A3 * x * x * x + object->A2 * x * x + object->A1 * x + object->A0);
            }
        }
    }
    return 0;
}
