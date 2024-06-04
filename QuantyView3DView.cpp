// QuantyView3DView.cpp : implementation of the CQuantyView3DView class
//

#include<iostream>
#include<fstream>

#include "stdafx.h"
#include "QuantyView3D.h"

#include "QuantyView3DDoc.h"
#include "QuantyView3DView.h"

#include "setjmp.h"
#using <mscorlib.dll>
#using <System.xml.dll>
using namespace System;
using namespace System::Xml;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuantyView3DView

IMPLEMENT_DYNCREATE(CQuantyView3DView, CGV3dView)

BEGIN_MESSAGE_MAP(CQuantyView3DView, CGV3dView)
	//{{AFX_MSG_MAP(CQuantyView3DView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_33937, &CQuantyView3DView::extract_bound)
	ON_COMMAND(ID_33938, &CQuantyView3DView::convert_TIN)
END_MESSAGE_MAP()

CQuantyView3DView::CQuantyView3DView()
{
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));  
}

CQuantyView3DView::~CQuantyView3DView()
{
}

void CQuantyView3DView::OnDraw(CDC* pDC)
{
	CQuantyView3DDoc* pDoc = (CQuantyView3DDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	CGV3dView::OnDraw(pDC);// 准备好绘图,以下开始画需要显示的物体
}

/////////////////////////////////////////////////////////////////////////////
// CQuantyView3DView diagnostics

#ifdef _DEBUG
//////////////////////////////////////////////////////////////////////////
////QuantyView_Tech_Mark_Begin
//==================================================================//
//QuantyView_Doc_Mark_Begin											//
// 函 数 名:	AssertValid
// 功    能:	视图有效性检验
// 参    数:	无
//   (入口):	无
//   (出口):	无
// 返    回:	无
// 调用方法:	
//QuantyView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-10-16, Wengzp: Add notes.
// 修改日期:	
//==================================================================//
void CQuantyView3DView::AssertValid() const
////QuantyView_Tech_Mark_End
{
	CView::AssertValid();
}

//////////////////////////////////////////////////////////////////////////
////QuantyView_Tech_Mark_Begin
//==================================================================//
//QuantyView_Doc_Mark_Begin											//
// 函 数 名:	Dump	
// 功    能:	输出内容
// 参    数:	CDumpContext& dc
//   (入口):	dc:	输出内容对象
//   (出口):	无
// 返    回:	无
// 调用方法:	
//QuantyView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-10-16, Wengzp: Add notes.
// 修改日期:	
//==================================================================//
void CQuantyView3DView::Dump(CDumpContext& dc) const
////QuantyView_Tech_Mark_End
{
	CView::Dump(dc);
}
#endif //_DEBUG
//////////////////////////////////////////////////////////////////////////
////QuantyView_Tech_Mark_Begin
//==================================================================//
//QuantyView_Doc_Mark_Begin											//
// 函 数 名:	GetDocument
// 功    能:	获取视图对应的文档
// 参    数:	无
//   (入口):	无
//   (出口):	无
// 返    回:	CQuantyView3DDoc*:	视图对应的文档指针
// 调用方法:	
//QuantyView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-10-16, Wengzp: Add notes.
// 修改日期:	
//==================================================================//
CGV3dDocument* CQuantyView3DView::GetDocument() // non-debug version is inline
////QuantyView_Tech_Mark_End
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGV3dDocument)));
	return (CGV3dDocument*)m_pDocument;
}

BOOL CQuantyView3DView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//if(pMsg->message==WM_MOUSEMOVE)
	//	m_ToolTip.RelayEvent(pMsg);

	if (CGV3dView::PreTranslateMessage(pMsg))  return TRUE; 
	return ((m_hAccelTable != NULL) &&  ::TranslateAccelerator (m_hWnd, m_hAccelTable, pMsg));
}

void CQuantyView3DView::OnInitialUpdate()
{
	// TODO: 在此添加专用代码和/或调用基类
	//m_ToolTip.Create(this,WS_POPUP |TTS_ALWAYSTIP | TTS_BALLOON);
	//m_ToolTip.AddTool(this);
	//m_ToolTip.SetTipTextColor(RGB(0,0,255));
	//m_ToolTip.SetDelayTime(10);
	//m_ToolTip.ShowWindow(SW_HIDE);
	//m_ToolTip.Activate(FALSE);
	CGV3dView::OnInitialUpdate();
}


void CQuantyView3DView::readkml(CString fileName)
{

	String ^systemstring = gcnew String(fileName);

	int flag1=0,flag2=0,len=0,num=0,flag3=0,flag4=0,flag5=0,flag6=0,flag7=0,flag8=0,flag9=0,flag_name=0,flag_des=0;
	double x,y,z;
	//BSTR bs;
	CArray< POINT3d *, POINT3d *> point_arr;
	CString bs_text,text1,text2,name_text,des_text;
	//char *text;
	CGV3dPoint *pPnt;
	POINT3d *pPoint_point3d;
	CGV3dPolyline *pLine;
	POINT3d *pData_point3d;
	CGV3dPolygon* pnewgon;


	CGV3dDocument* pDoc = (CGV3dDocument*)GetDocument();
	ASSERT_VALID(pDoc);
	CGV3dMap * pMap = pDoc->m_Project.GetCurrentMap();//获得当前图幅	
	if( !pMap)
	{
		AfxMessageBox("请设置当前图幅！");
		return;
	}
	CGV3dLayer *pLayer=pDoc->m_Project.GetCurrentLayer();//得到当前图层
	if(!pLayer)
	{
		AfxMessageBox("请先设置当前层!");
		return;
	}

	XmlTextReader ^reader=gcnew XmlTextReader(systemstring);

	while (reader->Read()) 
	{
		switch (reader->NodeType) 
		{
		case XmlNodeType::Element: // The node is an element.

			if(reader->Name=="Placemark")
				flag7=1;
			if(reader->Name=="name")
				flag8=1;
			if(reader->Name=="description")
				flag9=1;
			if(reader->Name=="LineString")
				flag1=1;
			if(reader->Name=="coordinates")
				flag2=1;
			if(reader->Name=="Point")
				flag3=1;
			if(reader->Name=="coordinates")
				flag4=1;
			if(reader->Name=="LinearRing")
				flag5=1;
			if(reader->Name=="coordinates")
				flag6=1;

			break;
		case XmlNodeType::Text: //Display the text in each element.
			if(flag7==1&&flag8==1)
			{
				name_text=reader->Value->ToString();
				flag_name=1;
			}
			if(flag7==1&&flag9==1)
			{
				des_text=reader->Value->ToString();
				flag_des=1;
			}
			if(flag3==1&&flag4==1)//点的处理

			{
				text1=reader->Value->ToString(); 
				len=text1.GetLength();

				bs_text=text1.Trim();

				int index1=0;
				int index2=bs_text.Find(" ");
				while(index2!=-1)
				{
					int num_position=index2-index1;
					text1=bs_text.Mid(index1,num_position);
					sscanf(text1, "%lf,%lf,%lf", &x,&y,&z);
					pPnt = new CGV3dPoint();
					ASSERT(pPnt);

					pPnt->SetPosition(x, y, z);
					pLayer->AddObject(pPnt);

					int index3=index2+1;
					bs_text=bs_text.Mid(index3);
					index2=bs_text.Find(" ");
					if(index2==-1)
					{
						text1=bs_text;
						sscanf(text1, "%lf,%lf,%lf", &x,&y,&z);

						pPnt = new CGV3dPoint();
						ASSERT(pPnt);

						pPnt->SetPosition(x, y, z);
						if(flag_name==1)
							pPnt->SetName(name_text);
						if(flag_des==1)
							pPnt->SetAttribute(des_text);
						pLayer->AddObject(pPnt);


					}

				}

			}

			if(flag1==1&&flag2==1)//路径处理

			{
				point_arr.RemoveAll();
				text1=reader->Value->ToString(); 
				len=text1.GetLength();

				bs_text=text1.Trim();

				int index1=0;
				int index2=bs_text.Find(" ");
				while(index2!=-1)
				{
					int num_position=index2-index1;
					text1=bs_text.Mid(index1,num_position);
					sscanf(text1, "%lf,%lf,%lf", &x,&y,&z);

					pPoint_point3d =new POINT3d[1];

					pPoint_point3d[0][0]=x;
					pPoint_point3d[0][1]=y;
					pPoint_point3d[0][2]=z;
					point_arr.Add(pPoint_point3d);
					int index3=index2+1;
					bs_text=bs_text.Mid(index3);
					index2=bs_text.Find(" ");
					if(index2==-1)
					{
						text1=bs_text;
						sscanf(text1, "%lf,%lf,%lf", &x,&y,&z);

						pPoint_point3d =new POINT3d[1];

						pPoint_point3d[0][0]=x;
						pPoint_point3d[0][1]=y;
						pPoint_point3d[0][2]=z;
						point_arr.Add(pPoint_point3d);


					}

				}


				pLine = new CGV3dPolyline();
				ASSERT(pLine);
				UINT numPnt=point_arr.GetSize();
				pData_point3d =new POINT3d [numPnt];
				for(UINT i=0;i<numPnt;i++)
				{
					pData_point3d[i][0]=(*point_arr.GetAt(i))[0];
					pData_point3d[i][1]=(*point_arr.GetAt(i))[1];
					pData_point3d[i][2]=(*point_arr.GetAt(i))[2];
				}
				pLine->SetAll(numPnt,pData_point3d);
				if(flag_name==1)
					pLine->SetName(name_text);
				if(flag_des==1)
					pLine->SetAttribute(des_text);
				pLayer->AddObject(pLine);  
				for(int ii=0;ii<point_arr.GetCount();ii++)    
					delete (point_arr.GetAt(ii));
				point_arr.RemoveAll();	 
				if(pData_point3d)
				{
					delete pData_point3d;
					pData_point3d=NULL;
				}

			}
			if(flag5==1&&flag6==1)//多边形处理

			{
				CArray<XYZ, XYZ>data;
				XYZ xyz;
				data.RemoveAll();
				text1=reader->Value->ToString(); 
				len=text1.GetLength();

				bs_text=text1.Trim();

				int index1=0;
				int index2=bs_text.Find(" ");
				while(index2!=-1)
				{
					int num_position=index2-index1;
					text1=bs_text.Mid(index1,num_position);
					sscanf(text1, "%lf,%lf,%lf", &x,&y,&z);

					xyz.x=x;
					xyz.y=y;
					xyz.z=z;
					data.Add(xyz);

					int index3=index2+1;
					bs_text=bs_text.Mid(index3);
					index2=bs_text.Find(" ");
					if(index2==-1)
					{
						text1=bs_text;
						sscanf(text1, "%lf,%lf,%lf", &x,&y,&z);

						xyz.x=x;
						xyz.y=y;
						xyz.z=z;
						data.Add(xyz);
					}

				}


				pnewgon = new CGV3dPolygon;
				pnewgon->SetAll(data);
				if(flag_name==1)
					pnewgon->SetName(name_text);
				if(flag_des==1)
					pnewgon->SetAttribute(des_text);
				pLayer->AddObject(pnewgon);



			}


			break;
		case XmlNodeType::EndElement: //Display the end of the element.

			if(reader->Name=="LineString")
				flag1=0;
			if(reader->Name=="coordinates")
			{
				flag2=0;
				flag_name=0;
				flag_des=0;
			}

			if(reader->Name=="Point")
				flag3=0;	 
			if(reader->Name=="coordinates")
				flag4=0;
			if(reader->Name=="LinearRing")
				flag5=0;
			if(reader->Name=="coordinates")
				flag6=0;
			if(reader->Name=="Placemark")
				flag7=0;
			if(reader->Name=="name")
				flag8=0;
			if(reader->Name=="description")
				flag9=0;
			break;
		}
	}
}



void CQuantyView3DView::extract_bound()
{
	// TODO: Add your command handler code here
	CGV3dDocument* pDoc = (CGV3dDocument*)GetDocument();
    ASSERT_VALID(pDoc);
    CGV3dLayer *pLayer=pDoc->m_Project.GetCurrentLayer();//得到当前图层
    if(!pLayer)
    {
        AfxMessageBox("请先设置当前层!");
        return;
    }
    CGV3dObject* pObj = pLayer->GetObjectAt(0);
    CGV3dPolygon* pGon = (CGV3dPolygon*)(pObj);
    double aRea = pGon->GetArea();
    GV3dWARRAY psArr;
    pGon->GetPolygons(psArr);
    //void GetAll(const UINT n, POINT3d * pdData);
    UINT nnn = pGon->GetDataSize();
    POINT3d* pdData = new POINT3d[nnn]; 
    pGon->GetAll(nnn,pdData);
    int gSize = psArr.GetSize();
    int mSize = 0;
    int dSize = 0;
    int** aa = new int*[nnn];
    for(int i=0;i<nnn;i++){
        aa[i] = new int[nnn];
        for(int j=0;j<nnn;j++){
            aa[i][j] = -1;
        }
    }
    for(int i=0;i<gSize;i++){
        if(i == 0){
            mSize = psArr.GetAt(i);
            continue;
        }
        if(i == 1){
            dSize = psArr.GetAt(i);
            continue;
        }
        if(dSize == 0){
            mSize -= 1;
        }
        if(mSize == 0){
            break;
        }

        //----------------------------------------------------------------------------------------------------------------------------

        if(i!=0 && i!=1){
            if(dSize == 0){
                dSize = psArr.GetAt(i);
            }else{
                for(int j=1; j<dSize; j++){
                    aa[psArr.GetAt(i+j-1)][psArr.GetAt(i+j)] += 1;
                    aa[psArr.GetAt(i+j)][psArr.GetAt(i+j-1)] += 1;
                }
                aa[psArr.GetAt(i)][psArr.GetAt(i+dSize-1)] += 1;
                aa[psArr.GetAt(i+dSize-1)][psArr.GetAt(i)] += 1;
                i += dSize-1;
                dSize = 0;
                
            }
        }
    }
    //------------------------------------------------------------------------------------------------------------------------------------------------------
    for(int i=0;i<nnn;i++){
        for(int j=0;j<nnn;j++){
            if(aa[i][j] == 0 && aa[j][i] == 0){
                //pGline
                int ss = aa[i][j];
                aa[i][j] = -1;
                CGV3dPolyline* pGline = new CGV3dPolyline();
                GV3dWARRAY pptem;
                pptem.Add(1);
                pptem.Add(2);
                pptem.Add(i);
                pptem.Add(j);
                pGline->SetPolylines(pptem);
                pGline->SetAll(nnn,pdData);
                pLayer->AddObject(pGline);
            }
        }
    }
}


void CQuantyView3DView::convert_TIN()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CGV3dDocument* pDoc = (CGV3dDocument*)GetDocument();
    ASSERT_VALID(pDoc);
    CGV3dLayer *pLayer=pDoc->m_Project.GetCurrentLayer();//得到当前图层
    if(!pLayer)
    {
        AfxMessageBox("请先设置当前层!");
        return;
    }
    CGV3dObject* pObj = pLayer->GetObjectAt(0);
    CGV3dSurface* pSur = (CGV3dSurface*)(pObj);
	UINT xy_row = pSur->GetRow();
	UINT xy_column = pSur->GetColumn();
	int mSize = (xy_row-1)*(xy_column-1)*2;
	int dSize =	3;

	UINT nnn = pSur->GetDataSize();
    POINT3d* pdData = new POINT3d[nnn]; 
    pSur->GetAll(nnn,pdData);

	GV3dWARRAY pptem;
	pptem.Add(mSize);

	for(int i=0;i<xy_row -1;i++){
		for(int j=0;j<xy_column -1;j++){
			
			//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			
			POINT3d p1;
			pSur->GetDataAt(i,j,p1);
			int xuhao1 = 0;
			POINT3d pt1;
			for(int xy=0;xy<nnn;xy++){
				//p2[0] = pdData[(i)*(xy_column) + j-1][0];
				pt1[0] = pdData[xy][0];
				pt1[1] = pdData[xy][1];
				pt1[2] = pdData[xy][2];
				if(p1[0] == pt1[0] && p1[1] == pt1[1] && p1[2] == pt1[2]){
					xuhao1 = xy;
					break;
				}
			}
			//0号点00

			POINT3d p2;
			pSur->GetDataAt(i,j+1,p2);
			int xuhao2 = 0;
			POINT3d pt2;
			for(int xy=0;xy<nnn;xy++){
				//p2[0] = pdData[(i)*(xy_column) + j-1][0];
				pt2[0] = pdData[xy][0];
				pt2[1] = pdData[xy][1];
				pt2[2] = pdData[xy][2];
				if(p2[0] == pt2[0] && p2[1] == pt2[1] && p2[2] == pt2[2]){
					xuhao2 = xy;
					break;
				}
			}
			//0号点00

			POINT3d p3;
			pSur->GetDataAt(i+1,j,p3);
			int xuhao3 = 0;
			POINT3d pt3;
			for(int xy=0;xy<nnn;xy++){
				//p2[0] = pdData[(i)*(xy_column) + j-1][0];
				pt3[0] = pdData[xy][0];
				pt3[1] = pdData[xy][1];
				pt3[2] = pdData[xy][2];
				if(p3[0] == pt3[0] && p3[1] == pt3[1] && p3[2] == pt3[2]){
					xuhao3 = xy;
					break;
				}
			}
			//0号点00

			POINT3d p4;
			pSur->GetDataAt(i+1,j+1,p4);
			int xuhao4 = 0;
			POINT3d pt4;
			for(int xy=0;xy<nnn;xy++){
				//p2[0] = pdData[(i)*(xy_column) + j-1][0];
				pt4[0] = pdData[xy][0];
				pt4[1] = pdData[xy][1];
				pt4[2] = pdData[xy][2];
				if(p4[0] == pt4[0] && p4[1] == pt4[1] && p4[2] == pt4[2]){
					xuhao4 = xy;
					break;
				}
			}
			//0号点00
			//------------------------------------------------------------------------------------------------------------------------------------------------------
			pptem.Add(dSize);
			pptem.Add(xuhao1);
			pptem.Add(xuhao2);
			pptem.Add(xuhao3);
			pptem.Add(dSize);
			pptem.Add(xuhao2);
			pptem.Add(xuhao3);
			pptem.Add(xuhao4);

		}
	}
	CGV3dPolygon* pGline = new CGV3dPolygon();
    pGline->SetPolygons(pptem);
    pGline->SetAll(nnn,pdData);
    pLayer->AddObject(pGline);


}
