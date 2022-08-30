#include "image.h"

#include <Windows.h>

namespace rewin
{
	namespace
	{
		int ImageTypeToWinapiType(ImageType type)
		{
			switch (type)
			{
			case ImageType::Bitmap:
				return IMAGE_BITMAP;
			case ImageType::Cursor:
				return IMAGE_CURSOR;
			case ImageType::Icon:
				return IMAGE_ICON;
			default:
				return -1;
			};
		}

		int ImageTypeToSCFlags(ImageType type)
		{
			switch (type)
			{
			case ImageType::Bitmap:
				return SS_BITMAP;
			case ImageType::Cursor:
			case ImageType::Icon:
				return SS_ICON;
			default:
				return 0;
			};
		}
	}

	ImageResource ImageResource::LoadFromFile(const std::string& filename, ImageType type)
	{
		auto handle = LoadImageA(nullptr, filename.c_str(), ImageTypeToWinapiType(type), 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		return ImageResource{ handle, type };
	}

	ImageResource ImageResource::LoadFromResource(int rid, ImageType type)
	{
		auto handle = LoadImageA(nullptr, MAKEINTRESOURCE(rid), ImageTypeToWinapiType(type), 0, 0, LR_DEFAULTSIZE);
		return ImageResource{ handle, type };
	}

	void ImageResource::Destroy()
	{
		switch (mType)
		{
		case ImageType::Bitmap:
			DeleteObject(mHandle);
		case ImageType::Cursor:
			DestroyCursor((HCURSOR)mHandle);
		case ImageType::Icon:
			DestroyIcon((HICON)mHandle);
		};
	}

	Image::Image(const Coords& pos, const Coords& size, const ImageResource& resource)
		: StaticControl{ pos, size, "", SS_REALSIZECONTROL | SS_NOTIFY | ImageTypeToSCFlags(resource.GetType()) }, mResource{ new ImageResource(resource) }
	{
	}

	Image::Image(const Coords& pos, const Coords& size, ImageResource* pResource)
		: StaticControl{ pos, size, "", SS_REALSIZECONTROL | SS_NOTIFY | ImageTypeToSCFlags(pResource->GetType()) }, mResource{ pResource }, mOwnResource{ false }
	{
	}

	void Image::Activate(Widget* pParent, int id)
	{
		StaticControl::Activate(pParent, id);

		SendWindowMessage((WindowMessageType)STM_SETIMAGE, ImageTypeToWinapiType(mResource->GetType()), (WindowParam)mResource->GetHandle());
	}

	Image::~Image()
	{
		if (mOwnResource)
			mResource->Destroy();
	}
}
