
//��������
//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//glEnableVertexAttribArray(1);

//------------------------------------------------------------------------------------
//����
//��������
//unsigned int texture1, texture2;
//glGenTextures(1, &texture1);
//glGenTextures(1, &texture2);
////��ת
//stbi_set_flip_vertically_on_load(true);
////texture1
//glActiveTexture(GL_TEXTURE0);//��������Ԫ0
//glBindTexture(GL_TEXTURE_2D, texture1);//��texture1�󶨵�����Ԫ0
//// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//Repeat�����������ظ�����
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Linear��˫���Բ�ֵ
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//// ���ز���������
//int width, height, nrChannels;
//unsigned char* data = stbi_load("Res/Texture/container.jpg", &width, &height, &nrChannels, 0);//����
//if (data) {
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//���ɣ���һ��0��Mipmap����
//    glGenerateMipmap(GL_TEXTURE_2D);//Mipmap
//}
//else {
//    std::cout << "Failed to load texture" << std::endl;
//}
//
////texture2
//glActiveTexture(GL_TEXTURE1);//��������Ԫ1
//glBindTexture(GL_TEXTURE_2D, texture2);//��texture2�󶨵�����Ԫ1
//// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//Repeat�����������ظ�����
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Linear��˫���Բ�ֵ
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//// ���ز���������
//data = stbi_load("Res/Texture/awesomeface.png", &width, &height, &nrChannels, 0);//����
//if (data) {
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//���ɣ���һ��0��Mipmap����
//    glGenerateMipmap(GL_TEXTURE_2D);//Mipmap
//}
//else {
//    std::cout << "Failed to load texture" << std::endl;
//}
//stbi_image_free(data);//�ͷ��ڴ�