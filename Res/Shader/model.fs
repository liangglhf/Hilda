#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

//灯
struct LampLight {
    vec3 position;
    //反射光
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

//视角光
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;//光照范围，即内圆锥
    float outerCutOff;//光照衰减范围，即外圆锥
    //光照衰减
    float constant;//常数项
    float linear;//一次项，即线性项
    float quadratic;//二次项
    //反射光
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform vec3 viewPos;
uniform LampLight lampLight;
uniform SpotLight spotLight;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

//灯
vec3 CalcLampLight(LampLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //环境光
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    // 高光
    //Phone和Blinn Phone此处计算不同
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0f);//幂用的是反光度
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

    //Phone模型
    return (ambient + diffuse + specular);
}

//视角光
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //环境光
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    //漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    //高光
    //Phone和Blinn Phone此处计算不同
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0f);//幂用的是反光度
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

    //光照衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    //光照范围
    float theta = dot(lightDir, normalize(-light.direction));//内圆锥
    float epsilon = (light.cutOff - light.outerCutOff);//内圆锥与外圆锥夹角
    //内圆锥与外圆锥之间光照进行差值，得到模糊的光照过渡
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);//clamp函数，将值约束在0到1   
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    //Phone模型
    return (ambient + diffuse + specular);
}

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 lampColor = CalcLampLight(lampLight, norm, FragPos, viewDir);// 灯光
    vec3 spotColor = CalcSpotLight(spotLight, norm, FragPos, viewDir);// 视角光
    vec3 lightColor = spotColor + lampColor;
    vec4 texColor = texture(texture_diffuse1, TexCoords); // 纹理
    vec3 finalColor = mix(texColor.rgb, lightColor * texColor.rgb, 0.7);// 70%效果来源于光照
    
    FragColor = vec4(finalColor, texColor.a);
}