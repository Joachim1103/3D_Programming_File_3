    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoord;

    uniform sampler2D hillTexture;
    uniform vec4 objectColor;
    uniform int useTexture;

    void main() 
    {
        if (useTexture != 0)
            FragColor = texture(hillTexture, TexCoord);
        else
            FragColor = objectColor;
    }