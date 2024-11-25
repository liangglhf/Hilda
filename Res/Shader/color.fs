#version 330 core
out vec4 FragColor;
  
struct Material {
    //ϵ������ka,kd,ks
    sampler2D diffuse;
    sampler2D specular;
    //sampler2D emission;
    float shininess;//����ȣ��������й�
}; 

//��
struct DirLight {
    vec3 direction;
    //����˥��
    float constant;//������
    float linear;//һ�����������
    float quadratic;//������
    //�����
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

//�ӽǹ�
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;//���շ�Χ������Բ׶
    float outerCutOff;//����˥����Χ������Բ׶
    //����˥��
    float constant;//������
    float linear;//һ�����������
    float quadratic;//������
    //�����
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

//����ԭ��
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    //̫����
    vec3 result = CalcDirLight(dirLight, norm, viewDir);   
    //�ӽǹ�
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}

//̫����
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //������
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // ������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    // �߹�
    //Phone��Blinn Phone�˴����㲻ͬ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//���õ��Ƿ����
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //����˥��
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));      
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //Phoneģ��
    return (ambient + diffuse + specular);
}

//���Դ
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //�߹�
    //Phone��Blinn Phone�˴����㲻ͬ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//���õ��Ƿ����
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //����˥��
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));      
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //Phoneģ��
    return (ambient + diffuse + specular);
}

//�ӽǹ�
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //������
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //�߹�
    //Phone��Blinn Phone�˴����㲻ͬ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//���õ��Ƿ����
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //����˥��
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    //���շ�Χ
    float theta = dot(lightDir, normalize(-light.direction));//��Բ׶
    float epsilon = (light.cutOff - light.outerCutOff);//��Բ׶����Բ׶�н�
    //��Բ׶����Բ׶֮����ս��в�ֵ���õ�ģ���Ĺ��չ���
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);//clamp��������ֵԼ����0��1   
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    //Phoneģ��
    return (ambient + diffuse + specular);
}