#pragma once

#include <string>
#include <vector>

enum class TextureType
{
    Texture2D,
    Texture2DArray
};

class Texture
{
public:
    Texture();
    Texture(const std::string &path);

    virtual void Bind() const;
    static void Unbind();

    void SetTexture(const std::string &path);

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    int GetNumChannels() const { return m_NumChannels; }

    TextureType GetType() const { return m_Type; }

protected:
    void SetType(TextureType type) { m_Type = type; }

private:
    void Init();

private:
    unsigned int m_Id = 0;

    int m_Width = 0;
    int m_Height = 0;
    int m_NumChannels = 0;

    std::string m_Path;

    // Used for checking in shaders
    TextureType m_Type;
};

class Texture3D : public Texture
{
public:
    Texture3D(int maxWidth, int maxHeight);
    Texture3D(int maxWidth, int maxHeight, const std::vector<std::string> &paths);

    void Bind() const override;
    static void Unbind();

    void AddTexture(const std::string &path);

    int GetMaxWidth() const { return m_MaxWidth; }
    int GetMaxHeight() const { return m_MaxHeight; }

private:
    void Init();

private:
    unsigned int m_Id = 0;

    int m_MaxWidth = 0;
    int m_MaxHeight = 0;

    std::vector<std::string> m_Paths;
};
