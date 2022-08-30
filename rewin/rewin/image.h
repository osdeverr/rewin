#pragma once
#include "static_control.h"

namespace rewin
{
	typedef void* ImageResourceHandle;

	enum class ImageType
	{
		Invalid = -1,
		Bitmap,
		Cursor,
		Icon
	};

	class ImageResource
	{
	public:
		ImageResource(ImageResourceHandle handle, ImageType type)
			: mHandle{ handle }, mType{ type }
		{}

		static ImageResource LoadFromFile(const std::string& filename, ImageType type = ImageType::Bitmap);
		static ImageResource LoadFromResource(int rid, ImageType type = ImageType::Bitmap);

		void Destroy();

		ImageResourceHandle GetHandle() const { return mHandle; }
		ImageType GetType() const { return mType; }

	private:
		ImageResourceHandle mHandle = 0;
		ImageType mType = ImageType::Invalid;
	};

	class Image : public StaticControl
	{
	public:
		Image(
			const Coords& pos, const Coords& size,
			const ImageResource& resource
		);

		Image(
			const Coords& pos, const Coords& size,
			ImageResource* pResource
		);

		virtual void Activate(Widget* pParent, int id) override;

		~Image();

		const ImageResource* GetResource() const { return mResource; }

	private:
		ImageResource* mResource = nullptr;
		bool mOwnResource = false;
	};
}
