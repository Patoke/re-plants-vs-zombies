#include "CursorWidget.h"
#include "Image.h"

using namespace Sexy;

CursorWidget::CursorWidget()
{
	mImage = NULL;
	mMouseVisible = false;	
}

void CursorWidget::Draw(Graphics* g)
{
	if (mImage != NULL)
		g->DrawImage(mImage, 0, 0);
}

void CursorWidget::SetImage(Image* theImage)
{
	mImage = theImage;
	if (mImage != NULL)
		Resize(mX, mY, theImage->mWidth, theImage->mHeight);
}

Point CursorWidget::GetHotspot()
{
	if (mImage == NULL)
		return Point(0, 0);
	return Point(mImage->GetWidth()/2, mImage->GetHeight()/2);
}
