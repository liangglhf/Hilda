#version 330 core
out vec4 FragColor;
  
struct Material {
    //系数，即ka,kd,ks
    sampler2D diffuse;
    sampler2D specular;
    //sampler2D emission;
    float shininess;//反光度，跟材质有关
}; 

//灯
struct DirLight {
    vec3 direction;
    //光照衰减
    float constant;//常数项
    float linear;//一次项，即线性项
    float quadratic;//二次项
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


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

//函数原型
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    //太阳光
    vec3 result = CalcDirLight(dirLight, norm, viewDir);   
    //视角光
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}

//太阳光
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    // 高光
    //Phone和Blinn Phone此处计算不同
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//幂用的是反光度
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //光照衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));      
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //Phone模型
    return (ambient + diffuse + specular);
}

//点光源
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //高光
    //Phone和Blinn Phone此处计算不同
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//幂用的是反光度
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //光照衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));      
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //Phone模型
    return (ambient + diffuse + specular);
}

//视角光
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //高光
    //Phone和Blinn Phone此处计算不同
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//幂用的是反光度
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

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