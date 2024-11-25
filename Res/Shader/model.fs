#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

//��
struct LampLight {
    vec3 position;
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

uniform vec3 viewPos;
uniform LampLight lampLight;
uniform SpotLight spotLight;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

//��
vec3 CalcLampLight(LampLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //������
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    // ������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    // �߹�
    //Phone��Blinn Phone�˴����㲻ͬ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0f);//���õ��Ƿ����
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

    //Phoneģ��
    return (ambient + diffuse + specular);
}

//�ӽǹ�
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //������
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    //������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    //�߹�
    //Phone��Blinn Phone�˴����㲻ͬ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0f);//���õ��Ƿ����
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

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

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 lampColor = CalcLampLight(lampLight, norm, FragPos, viewDir);// �ƹ�
    vec3 spotColor = CalcSpotLight(spotLight, norm, FragPos, viewDir);// �ӽǹ�
    vec3 lightColor = spotColor + lampColor;
    vec4 texColor = texture(texture_diffuse1, TexCoords); // ����
    vec3 finalColor = mix(texColor.rgb, lightColor * texColor.rgb, 0.7);// 70%Ч����Դ�ڹ���
    
    FragColor = vec4(finalColor, texColor.a);
}