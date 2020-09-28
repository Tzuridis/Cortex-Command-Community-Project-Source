#include "SDLTexture.h"

namespace RTE {
	SDLTexture::SDLTexture() {
		m_TextureFile.Reset();
		m_Texture = nullptr;
		m_SelfCreated = false;
		m_ClipRect = NULL;
	}

	SDLTexture::SDLTexture(SDL_Texture *pSurface) {
		m_TextureFile.Reset();
		m_Texture = pSurface;
		m_SelfCreated = false;
		m_ClipRect = NULL;
	}

	bool SDLTexture::Create(int width, int height, int depth) {
		m_SelfCreated = true;

		m_TextureFile.Reset();
		m_Texture =
		    SDL_CreateTexture(g_FrameMan.GetRenderer(), g_FrameMan.GetPixelformat(),
		                      SDL_TEXTUREACCESS_STREAMING, width, height);
		if (m_Texture == NULL) {
			return false;
		}
		return true;
	}

	bool SDLTexture::Create(const std::string filename) {
		m_SelfCreated = false;

		m_TextureFile.Create(filename.c_str());

		m_Texture = m_TextureFile.GetAsTexture();
		RTEAssert(m_Texture, "Could not load bitmap from file into SDLTexture!");

		return true;
	}

	void SDLTexture::Destroy() {
		if (m_SelfCreated && m_Texture)
			SDL_DestroyTexture(m_Texture);

		m_Texture = nullptr;
	}

	void SDLTexture::Draw(GUIBitmap *pDestBitmap, int x, int y, GUIRect *pRect) {}

	void SDLTexture::DrawTrans(GUIBitmap *pDestBitmap, int x, int y,
	                           GUIRect *pRect) {}

	void SDLTexture::DrawTransScaled(GUIBitmap *pDestBitmap, int x, int y,
	                                 int width, int height) {}

	void SDLTexture::DrawLine(int x1, int y1, int x2, int y2,
	                          unsigned long color) {}

	void SDLTexture::DrawRectangle(int x, int y, int width, int height,
	                               unsigned long color, bool filled) {}

	unsigned long SDLTexture::GetPixel(int x, int y) {
		Uint32 *pixels = (Uint32 *)m_Pixels;

		return pixels[(y * m_Pitch / 4) + x];
	}

	void SDLTexture::SetPixel(int x, int y, unsigned long color) {
		Uint32 *pixels = (Uint32 *)m_Pixels;
		pixels[(y * m_Pitch / 4) + x] = color;
	}

	bool SDLTexture::LockTexture(SDL_Rect *rect) {
		RTEAssert(SDL_LockTexture(m_Texture, rect, &m_Pixels, &m_Pitch) == 0,
		          "Failed to lock Texture with error: " + SDL_GetError());

		return true;
	}

	bool SDLTexture::UnlockTexture() {
		RTEAssert(SDL_UnlockTexture(m_Texture) == 0,
		          "Failed to unlock Texture with error: " + SDL_GetError());

		m_Pixels = nullptr;

		m_Pitch = 0;

		return true;
	}

	void SDLTexture::GetClipRect(GUIRect *rect) {
		rect->left = m_ClipRect.x;
		rect->top = m_ClipRect.y;
		rect->right = m_ClipRect.x + m_ClipRect.w;
		rect->bottom = m_ClipRect.y + m_ClipRect.h;
	}

	void SDLTexture::SetClipRect(GUIRect *rect) {
		m_ClipRect.x = rect->left;
		m_ClipRect.y = rect->top;
		m_ClipRect.w = rect->right - rect->left;
		m_ClipRect.h = rect->bottom - rect->top;
	}

	void SDLTexture::AddClipRect(GUIRect *rect) {
		m_ClipRect.x = std::max(m_ClipRect.x, rect->left);
		m_ClipRect.y = std::max(m_ClipRect.y, rect->top);
		m_ClipRect.w = std::min(m_ClipRect.w, rect->right - rect->left);
		m_ClipRect.h = std::min(m_ClipRect.h, rect->bottom - rect->top);
	}
} // namespace RTE
