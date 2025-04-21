#include "Fade.h"
#define M_PI 3.141592

void Fade::Init(const std::string& filePath)
{
    fadeSprite = new Sprite();
    fadeSprite->Init(filePath);
    fadeSprite->SetTexture(TextureManager::GetInstance()->StoreTexture("Resources/fade.png"));
    fadeSprite->SetPosition({ 200.0f,0.0f });
    fadeSprite->SetTextureSize({ 1920.0f,1280.0f });
    fadeSprite->SetSize({ 1920.0f,1280.0f });
    material.color = { 1.0f,1.0f,1.0f,0.0f };
}

void Fade::SetTexture(uint32_t fadeTexture) {
    FADEtextureHandle = fadeTexture;
}
void Fade::SetAlpha(float Alpha) {
    alpha = Alpha;
}

void Fade::Update() {

    fadeSprite->SetColor(material.color);
    fadeSprite->Update();
}

void Fade::Draw() {

    fadeSprite->Draw();
}
void Fade::StartFadeOut()
{
    alpha = 1.0f;
    isFadingOut = true;
    isFadeIn = false;
    fadeOutComplete = false;
}

void Fade::StartFadeIn()
{
    alpha = 0.0f;
    isFadeIn = true;
    isFadingOut = false;
    fadeOutComplete = false;
}

void Fade::UpdateFade()
{
    time += deltaTime; // 時間を進める
    if (isFadingOut)
    {
        UpdateFadeOut();
    }
    else if (isFadeIn)
    {
        UpdateFadeIn();
    }

    material.color = { 1.0f, 1.0f, 1.0f, alpha };
}

void Fade::UpdateFadeOut()
{
    if (alpha > 0.0f)
    {
        alpha -= 0.0013f; // フェード速度
        if (alpha < 0.0f)
        {
            alpha = 0.0f;
            isFadingOut = false;
        }
    }
}

void Fade::UpdateFadeIn()
{
    if (alpha < 1.0f)
    {
        alpha += 0.01f; // フェード速度
        if (alpha > 1.0f)
        {
            alpha = 1.0f;
            isFadeIn = false;
            fadeOutComplete = true;
        }
    }
}
