#include "stdafx.h"
#include "Object.h"


std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uidC{ 0.0f, 1.0f };
glm::mat4 proj = glm::mat4(1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 50.0f, 50.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
glm::mat4 view = glm::mat4(1.0f);
glm::vec3 cameraAngle = { 0.0f, 0.0f, 0.0f };
glm::vec3 lightPos = { 0.0f, 100.0f, 100.0f };
glm::vec3 lightColor = {1.0f, 1.0f, 1.0f};
bool light_onf = true;
float light_hardness = 0.3f;

pair<float, float> WintoOpenGL(POINT pt)
{
	pair<float, float> a = { pt.x / 400. - 1, 1 - (pt.y / 400.) };
	return a;
}

float dist(glm::vec3 p1, glm::vec3 p2)
{
	return sqrt(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2) + powf(p2.z - p1.z, 2));
}

bool obb(Object& a, Object& b)
{
    double c[3][3];
    double absC[3][3];
    double d[3];

    double r0, r1, r;
    int i;

    const double cutoff = 0.999999;
    bool existsParallelPair = false;

    glm::vec3 diff = a.vCenterPos - b.vCenterPos;

    for (i = 0; i < 3; ++i)
    {
        c[0][i] = glm::dot(a.vAxisDir[0], b.vAxisDir[i]);
        absC[0][i] = abs(c[0][i]);
        if (absC[0][i] > cutoff)
            existsParallelPair = true;
    }
    d[0] = glm::dot(diff, a.vAxisDir[0]);
    r = abs(d[0]);
    r0 = a.fAxisLen[0];
    r1 = b.fAxisLen[0] * absC[0][0] + b.fAxisLen[1] * absC[0][1] + b.fAxisLen[2] * absC[0][2];

    if (r > r0 + r1)
        return FALSE;



    for (i = 0; i < 3; ++i)
    {
        c[1][i] = glm::dot(a.vAxisDir[1], b.vAxisDir[i]);
        absC[1][i] = abs(c[1][i]);
        if (absC[1][i] > cutoff)
            existsParallelPair = true;
    }
    d[1] = glm::dot(diff, a.vAxisDir[1]);
    r = abs(d[1]);
    r0 = a.fAxisLen[1];
    r1 = b.fAxisLen[0] * absC[1][0] + b.fAxisLen[1] * absC[1][1] + b.fAxisLen[2] * absC[1][2];

    if (r > r0 + r1)
        return FALSE;



    for (i = 0; i < 3; ++i)
    {
        c[2][i] = glm::dot(a.vAxisDir[2], b.vAxisDir[i]);
        absC[2][i] = abs(c[2][i]);
        if (absC[2][i] > cutoff)
            existsParallelPair = true;
    }
    d[2] = glm::dot(diff, a.vAxisDir[2]);
    r = abs(d[2]);
    r0 = a.fAxisLen[2];
    r1 = b.fAxisLen[0] * absC[2][0] + b.fAxisLen[1] * absC[2][1] + b.fAxisLen[2] * absC[2][2];

    if (r > r0 + r1)
        return FALSE;



    r = abs(glm::dot(diff, b.vAxisDir[0]));
    r0 = a.fAxisLen[0] * absC[0][0] + a.fAxisLen[1] * absC[1][0] + a.fAxisLen[2] * absC[2][0];
    r1 = b.fAxisLen[0];

    if (r > r0 + r1)
        return FALSE;



    r = abs(glm::dot(diff, b.vAxisDir[1]));
    r0 = a.fAxisLen[0] * absC[0][1] + a.fAxisLen[1] * absC[1][1] + a.fAxisLen[2] * absC[2][1];
    r1 = b.fAxisLen[1];

    if (r > r0 + r1)
        return FALSE;



    r = abs(glm::dot(diff, b.vAxisDir[2]));
    r0 = a.fAxisLen[0] * absC[0][2] + a.fAxisLen[1] * absC[1][2] + a.fAxisLen[2] * absC[2][2];
    r1 = b.fAxisLen[2];

    if (r > r0 + r1)
        return FALSE;



    if (existsParallelPair == true)
        return TRUE;



    r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
    r0 = a.fAxisLen[1] * absC[2][0] + a.fAxisLen[2] * absC[1][0];
    r1 = b.fAxisLen[1] * absC[0][2] + b.fAxisLen[2] * absC[0][1];
    if (r > r0 + r1)
        return FALSE;



    r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
    r0 = a.fAxisLen[1] * absC[2][1] + a.fAxisLen[2] * absC[1][1];
    r1 = b.fAxisLen[0] * absC[0][2] + b.fAxisLen[2] * absC[0][0];
    if (r > r0 + r1)
        return FALSE;



    r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
    r0 = a.fAxisLen[1] * absC[2][2] + a.fAxisLen[2] * absC[1][2];
    r1 = b.fAxisLen[0] * absC[0][1] + b.fAxisLen[1] * absC[0][0];
    if (r > r0 + r1)
        return FALSE;



    r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
    r0 = a.fAxisLen[0] * absC[2][0] + a.fAxisLen[2] * absC[0][0];
    r1 = b.fAxisLen[1] * absC[1][2] + b.fAxisLen[2] * absC[1][1];
    if (r > r0 + r1)
        return FALSE;



    r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
    r0 = a.fAxisLen[0] * absC[2][1] + a.fAxisLen[2] * absC[0][1];
    r1 = b.fAxisLen[0] * absC[1][2] + b.fAxisLen[2] * absC[1][0];
    if (r > r0 + r1)
        return FALSE;



    r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
    r0 = a.fAxisLen[0] * absC[2][2] + a.fAxisLen[2] * absC[0][2];
    r1 = b.fAxisLen[0] * absC[1][1] + b.fAxisLen[1] * absC[1][0];
    if (r > r0 + r1)
        return FALSE;



    r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
    r0 = a.fAxisLen[0] * absC[1][0] + a.fAxisLen[1] * absC[0][0];
    r1 = b.fAxisLen[1] * absC[2][2] + b.fAxisLen[2] * absC[2][1];
    if (r > r0 + r1)
        return FALSE;



    r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
    r0 = a.fAxisLen[0] * absC[1][1] + a.fAxisLen[1] * absC[0][1];
    r1 = b.fAxisLen[0] * absC[2][2] + b.fAxisLen[2] * absC[2][0];
    if (r > r0 + r1)
        return FALSE;



    r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
    r0 = a.fAxisLen[0] * absC[1][2] + a.fAxisLen[1] * absC[0][2];
    r1 = b.fAxisLen[0] * absC[2][1] + b.fAxisLen[1] * absC[2][0];
    if (r > r0 + r1)
        return FALSE;



    return TRUE;
}


bool obb_ray(Object& a, glm::vec3 origin, glm::vec3 direction)
{
    float tMin = -numeric_limits<float>::infinity();
    float tMax = numeric_limits<float>::infinity();

    glm::vec3 delta = a.vCenterPos - origin;
    glm::vec3 obbAxis[3];

    // Calculate OBB axis vectors
    for (int i = 0; i < 3; ++i) {
        obbAxis[i] = a.vAxisDir[i];
    }

    for (int i = 0; i < 3; ++i) {
        float e = glm::dot(obbAxis[i], delta);
        float f = glm::dot(direction, obbAxis[i]);

        // Check for parallel ray
        if (abs(f) > 0.001f) {
            float t1 = (e + a.fAxisLen[i] * 0.5f) / f;
            float t2 = (e - a.fAxisLen[i] * 0.5f) / f;

            if (t1 > t2) {
                swap(t1, t2);
            }

            if (t1 > tMin) {
                tMin = t1;
            }

            if (t2 < tMax) {
                tMax = t2;
            }

            if (tMin > tMax) {
                return false;
            }

            if (tMax < 0) {
                return false;
            }
        }
        else {
            // Ray is parallel to OBB axis, check if ray origin is within OBB
            if (-e - a.fAxisLen[i] * 0.5f > 0 || -e + a.fAxisLen[i] * 0.5f < 0) {
                return false;
            }
        }
    }

    return true;
}

GLubyte* LoadDIBitmap(const char* filename, BITMAP* bitmap)
{
    FILE* fp;
    GLubyte* bits;
    unsigned int infosize;
    BITMAPFILEHEADER header;

    //--- 바이너리 읽기 모드로 파일을 연다
    if ((fp = fopen(filename, "rb")) == NULL)
        return NULL;

    //--- 비트맵 파일 헤더를 읽는다.
    if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
        fclose(fp);
        return NULL;
    }

    //--- 파일이 BMP 파일인지 확인한다.
    if (header.bfType != 'MB') {
        fclose(fp);
        return NULL;
    }

    //--- 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
    infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
    if (fread(bitmap, 1, infosize, fp) < (unsigned int)infosize) {
        fclose(fp);
        return NULL;
    }

    //--- 비트맵의 크기 설정
    unsigned int bitsize = bitmap->bmWidthBytes * abs(bitmap->bmHeight);

    //--- 비트맵의 크기만큼 메모리를 할당한다.
    if ((bits = (GLubyte*)malloc(bitsize)) == NULL) {
        fclose(fp);
        return NULL;
    }

    //--- 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
    if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
        free(bits);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return bits;
}

GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info) {
    FILE* fp;
    GLubyte* bits;
    int bitsize, infosize;
    BITMAPFILEHEADER header;

    if ((fp = fopen(filename, "rb")) == NULL)
        return NULL;

    if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
        fclose(fp);
        return NULL;
    }

    if (header.bfType != 0x4D42) {
        fclose(fp);
        return NULL;
    }

    infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);

    if ((*info = (BITMAPINFO*)malloc(infosize)) == NULL) {
        fclose(fp);
        return NULL;
    }

    if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
        free(*info);
        fclose(fp);
        return NULL;
    }

    if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
        bitsize = ((*info)->bmiHeader.biWidth *
            (*info)->bmiHeader.biBitCount + 7) / 8.0 *
        abs((*info)->bmiHeader.biHeight);

    if ((bits = (GLubyte*)malloc(bitsize)) == NULL) {
        free(*info);
        fclose(fp);
        return NULL;
    }

    if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
        free(*info);
        free(bits);
        fclose(fp);
        return NULL;
    }

    std::ofstream outputFile("output_image.bmp", std::ios::binary);
    if (outputFile.is_open()) {
        BITMAPFILEHEADER fileHeader;
        fileHeader.bfType = 0x4D42;
        fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + infosize + bitsize;
        fileHeader.bfReserved1 = 0;
        fileHeader.bfReserved2 = 0;
        fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + infosize;

        outputFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
        outputFile.write(reinterpret_cast<char*>(&(*info)->bmiHeader), sizeof((*info)->bmiHeader));
        outputFile.write(reinterpret_cast<char*>(bits), bitsize);

        outputFile.close();
    }
    else {
        std::cerr << "Error opening output file." << std::endl;
    }

    fclose(fp);
    return bits;
}