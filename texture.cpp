
//纹理设置
//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//glEnableVertexAttribArray(1);

//------------------------------------------------------------------------------------
//纹理
//创建对象
//unsigned int texture1, texture2;
//glGenTextures(1, &texture1);
//glGenTextures(1, &texture2);
////翻转
//stbi_set_flip_vertically_on_load(true);
////texture1
//glActiveTexture(GL_TEXTURE0);//激活纹理单元0
//glBindTexture(GL_TEXTURE_2D, texture1);//把texture1绑定到纹理单元0
//// 为当前绑定的纹理对象设置环绕、过滤方式
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//Repeat即超出部分重复纹理
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Linear即双线性插值
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//// 加载并生成纹理
//int width, height, nrChannels;
//unsigned char* data = stbi_load("Res/Texture/container.jpg", &width, &height, &nrChannels, 0);//输入
//if (data) {
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//生成，第一个0是Mipmap级别
//    glGenerateMipmap(GL_TEXTURE_2D);//Mipmap
//}
//else {
//    std::cout << "Failed to load texture" << std::endl;
//}
//
////texture2
//glActiveTexture(GL_TEXTURE1);//激活纹理单元1
//glBindTexture(GL_TEXTURE_2D, texture2);//把texture2绑定到纹理单元1
//// 为当前绑定的纹理对象设置环绕、过滤方式
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//Repeat即超出部分重复纹理
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Linear即双线性插值
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//// 加载并生成纹理
//data = stbi_load("Res/Texture/awesomeface.png", &width, &height, &nrChannels, 0);//输入
//if (data) {
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//生成，第一个0是Mipmap级别
//    glGenerateMipmap(GL_TEXTURE_2D);//Mipmap
//}
//else {
//    std::cout << "Failed to load texture" << std::endl;
//}
//stbi_image_free(data);//释放内存