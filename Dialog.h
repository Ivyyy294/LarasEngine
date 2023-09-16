#pragma once
#include "GameObject.h"

namespace LarasEngine
{
    class Dialog :
        public GameObject
    {
    public:
        Dialog (const TypId& _typId);

        virtual void Draw(QPainter* painter) override;
        virtual void Update () override;

        inline void AddText(const std::string& text) { textVec.push_back(text); };
        virtual size_t GetLayerId() const override { return LAYER_DIALOG; };
    protected:
        std::vector <std::string> textVec;
        size_t indexCurrentText;
        size_t maxCharCount;
        QPen pen;
        QFont font;
        QRectF textRect;
        bool drawNext;
        size_t framesCount;
        size_t textSpeed;
        virtual void ReadJson(const QJsonObject& json) override;
    };
}


