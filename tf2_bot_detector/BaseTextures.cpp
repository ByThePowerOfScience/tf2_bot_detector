#include "BaseTextures.h"
#include "Bitmap.h"
#include "Filesystem.h"
#include "Log.h"
#include "TextureManager.h"

#include <mh/text/string_insertion.hpp>

#include <filesystem>
#include <string>

using namespace std::string_literals;
using namespace tf2_bot_detector;

namespace
{
	class BaseTextures final : public IBaseTextures
	{
	public:
		BaseTextures(std::shared_ptr<ITextureManager> textureManager);

		const ITexture* GetHeart_16() const override { return m_Heart_16.get(); }
		const ITexture* GetVACShield_16() const override { return m_VACShield_16.get(); }
		const ITexture* GetGameBanIcon_16() const override { return m_GameBanIcon_16.get(); }

	private:
		std::shared_ptr<ITextureManager> m_TextureManager;

		std::shared_ptr<ITexture> m_Heart_16;
		std::shared_ptr<ITexture> m_VACShield_16;
		std::shared_ptr<ITexture> m_GameBanIcon_16;

		std::shared_ptr<ITexture> TryLoadTexture(std::filesystem::path file) const;
	};
}

std::unique_ptr<IBaseTextures> IBaseTextures::Create(std::shared_ptr<ITextureManager> textureManager)
{
	return std::make_unique<BaseTextures>(textureManager);
}

BaseTextures::BaseTextures(std::shared_ptr<ITextureManager> textureManager) :
	m_TextureManager(textureManager),

	m_Heart_16(TryLoadTexture("images/heart_16.png")),
	m_VACShield_16(TryLoadTexture("images/vac_icon_16.png")),
	m_GameBanIcon_16(TryLoadTexture("images/game_ban_icon_16.png"))
{
}

std::shared_ptr<ITexture> BaseTextures::TryLoadTexture(std::filesystem::path file) const
{
	file = IFilesystem::Get().ResolvePath(file, PathUsage::Read);

	try
	{
		return m_TextureManager->CreateTexture(Bitmap(file));
	}
	catch (const std::exception& e)
	{
		LogException(MH_SOURCE_LOCATION_CURRENT(), e, "Failed to load {}", file);
	}

	return nullptr;
}
