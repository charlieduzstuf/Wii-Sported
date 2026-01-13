#ifdef PLATFORM_PC
// PC implementation of Revolution SDK MTX library
// Math operations are platform-independent, just need standard C implementations

#include <revolution/MTX.h>
#include <math.h>
#include <string.h>

void PSMTXIdentity(Mtx m) {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
}

void PSMTXCopy(const Mtx src, Mtx dst) {
    if (src != dst) {
        memcpy(dst, src, sizeof(Mtx));
    }
}

void PSMTXConcat(const Mtx a, const Mtx b, Mtx result) {
    Mtx tmp;
    int i, j;
    
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            tmp[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j];
            if (j == 3) tmp[i][j] += a[i][3];
        }
    }
    
    PSMTXCopy(tmp, result);
}

void PSMTXTranspose(const Mtx src, Mtx dst) {
    Mtx tmp;
    
    tmp[0][0] = src[0][0];
    tmp[0][1] = src[1][0];
    tmp[0][2] = src[2][0];
    tmp[0][3] = 0.0f;
    
    tmp[1][0] = src[0][1];
    tmp[1][1] = src[1][1];
    tmp[1][2] = src[2][1];
    tmp[1][3] = 0.0f;
    
    tmp[2][0] = src[0][2];
    tmp[2][1] = src[1][2];
    tmp[2][2] = src[2][2];
    tmp[2][3] = 0.0f;
    
    PSMTXCopy(tmp, dst);
}

u32 PSMTXInverse(const Mtx src, Mtx dst) {
    f32 det;
    Mtx tmp;
    
    // Calculate determinant
    det = src[0][0] * (src[1][1] * src[2][2] - src[1][2] * src[2][1])
        - src[0][1] * (src[1][0] * src[2][2] - src[1][2] * src[2][0])
        + src[0][2] * (src[1][0] * src[2][1] - src[1][1] * src[2][0]);
    
    if (det == 0.0f) return 0;
    
    det = 1.0f / det;
    
    // Calculate inverse
    tmp[0][0] = (src[1][1] * src[2][2] - src[1][2] * src[2][1]) * det;
    tmp[0][1] = (src[0][2] * src[2][1] - src[0][1] * src[2][2]) * det;
    tmp[0][2] = (src[0][1] * src[1][2] - src[0][2] * src[1][1]) * det;
    
    tmp[1][0] = (src[1][2] * src[2][0] - src[1][0] * src[2][2]) * det;
    tmp[1][1] = (src[0][0] * src[2][2] - src[0][2] * src[2][0]) * det;
    tmp[1][2] = (src[0][2] * src[1][0] - src[0][0] * src[1][2]) * det;
    
    tmp[2][0] = (src[1][0] * src[2][1] - src[1][1] * src[2][0]) * det;
    tmp[2][1] = (src[0][1] * src[2][0] - src[0][0] * src[2][1]) * det;
    tmp[2][2] = (src[0][0] * src[1][1] - src[0][1] * src[1][0]) * det;
    
    // Translation part
    tmp[0][3] = -(src[0][3] * tmp[0][0] + src[1][3] * tmp[0][1] + src[2][3] * tmp[0][2]);
    tmp[1][3] = -(src[0][3] * tmp[1][0] + src[1][3] * tmp[1][1] + src[2][3] * tmp[1][2]);
    tmp[2][3] = -(src[0][3] * tmp[2][0] + src[1][3] * tmp[2][1] + src[2][3] * tmp[2][2]);
    
    PSMTXCopy(tmp, dst);
    return 1;
}

void PSMTXRotRad(Mtx m, char axis, f32 rad) {
    f32 s = sinf(rad);
    f32 c = cosf(rad);
    
    PSMTXIdentity(m);
    
    switch (axis) {
        case 'x':
        case 'X':
            m[1][1] = c;
            m[1][2] = -s;
            m[2][1] = s;
            m[2][2] = c;
            break;
        case 'y':
        case 'Y':
            m[0][0] = c;
            m[0][2] = s;
            m[2][0] = -s;
            m[2][2] = c;
            break;
        case 'z':
        case 'Z':
            m[0][0] = c;
            m[0][1] = -s;
            m[1][0] = s;
            m[1][1] = c;
            break;
    }
}

void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z) {
    PSMTXIdentity(m);
    m[0][3] = x;
    m[1][3] = y;
    m[2][3] = z;
}

void PSMTXScale(Mtx m, f32 x, f32 y, f32 z) {
    PSMTXIdentity(m);
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
}

void C_MTXLookAt(Mtx m, const Vec* camPos, const Vec* camUp, const Vec* target) {
    Vec vLook, vRight, vUp;
    
    // Forward vector (look direction)
    vLook.x = camPos->x - target->x;
    vLook.y = camPos->y - target->y;
    vLook.z = camPos->z - target->z;
    
    // Normalize
    f32 len = sqrtf(vLook.x * vLook.x + vLook.y * vLook.y + vLook.z * vLook.z);
    if (len > 0.0f) {
        vLook.x /= len;
        vLook.y /= len;
        vLook.z /= len;
    }
    
    // Right vector = camUp x vLook
    vRight.x = camUp->y * vLook.z - camUp->z * vLook.y;
    vRight.y = camUp->z * vLook.x - camUp->x * vLook.z;
    vRight.z = camUp->x * vLook.y - camUp->y * vLook.x;
    
    len = sqrtf(vRight.x * vRight.x + vRight.y * vRight.y + vRight.z * vRight.z);
    if (len > 0.0f) {
        vRight.x /= len;
        vRight.y /= len;
        vRight.z /= len;
    }
    
    // Up vector = vLook x vRight
    vUp.x = vLook.y * vRight.z - vLook.z * vRight.y;
    vUp.y = vLook.z * vRight.x - vLook.x * vRight.z;
    vUp.z = vLook.x * vRight.y - vLook.y * vRight.x;
    
    m[0][0] = vRight.x;
    m[0][1] = vRight.y;
    m[0][2] = vRight.z;
    m[0][3] = -(vRight.x * camPos->x + vRight.y * camPos->y + vRight.z * camPos->z);
    
    m[1][0] = vUp.x;
    m[1][1] = vUp.y;
    m[1][2] = vUp.z;
    m[1][3] = -(vUp.x * camPos->x + vUp.y * camPos->y + vUp.z * camPos->z);
    
    m[2][0] = vLook.x;
    m[2][1] = vLook.y;
    m[2][2] = vLook.z;
    m[2][3] = -(vLook.x * camPos->x + vLook.y * camPos->y + vLook.z * camPos->z);
}

// Stub implementations for less common functions
void PSMTXConcatArray(const Mtx a, const Mtx b, Mtx result, u32 count) {
    u32 i;
    for (i = 0; i < count; i++) {
        PSMTXConcat(a, b, result);
    }
}

u32 PSMTXInvXpose(const Mtx src, Mtx dst) {
    Mtx tmp;
    if (!PSMTXInverse(src, tmp)) return 0;
    PSMTXTranspose(tmp, dst);
    return 1;
}

void PSMTXRotTrig(Mtx m, f32 sinA, f32 cosA, char axis) {
    PSMTXIdentity(m);
    
    switch (axis) {
        case 'x':
        case 'X':
            m[1][1] = cosA;
            m[1][2] = -sinA;
            m[2][1] = sinA;
            m[2][2] = cosA;
            break;
        case 'y':
        case 'Y':
            m[0][0] = cosA;
            m[0][2] = sinA;
            m[2][0] = -sinA;
            m[2][2] = cosA;
            break;
        case 'z':
        case 'Z':
            m[0][0] = cosA;
            m[0][1] = -sinA;
            m[1][0] = sinA;
            m[1][1] = cosA;
            break;
    }
}

void PSMTXRotAxisRad(Mtx m, const Vec* axis, f32 rad) {
    f32 s = sinf(rad);
    f32 c = cosf(rad);
    f32 t = 1.0f - c;
    f32 x = axis->x, y = axis->y, z = axis->z;
    
    m[0][0] = t * x * x + c;
    m[0][1] = t * x * y - s * z;
    m[0][2] = t * x * z + s * y;
    m[0][3] = 0.0f;
    
    m[1][0] = t * x * y + s * z;
    m[1][1] = t * y * y + c;
    m[1][2] = t * y * z - s * x;
    m[1][3] = 0.0f;
    
    m[2][0] = t * x * z - s * y;
    m[2][1] = t * y * z + s * x;
    m[2][2] = t * z * z + c;
    m[2][3] = 0.0f;
}

void PSMTXTransApply(const Mtx src, Mtx dst, f32 x, f32 y, f32 z) {
    PSMTXCopy(src, dst);
    dst[0][3] += x;
    dst[1][3] += y;
    dst[2][3] += z;
}

void PSMTXScaleApply(const Mtx src, Mtx dst, f32 x, f32 y, f32 z) {
    int i;
    for (i = 0; i < 3; i++) {
        dst[i][0] = src[i][0] * x;
        dst[i][1] = src[i][1] * y;
        dst[i][2] = src[i][2] * z;
        dst[i][3] = src[i][3];
    }
}

void PSMTXQuat(Mtx m, const Quaternion* q) {
    f32 xx = q->x * q->x;
    f32 yy = q->y * q->y;
    f32 zz = q->z * q->z;
    f32 xy = q->x * q->y;
    f32 xz = q->x * q->z;
    f32 yz = q->y * q->z;
    f32 wx = q->w * q->x;
    f32 wy = q->w * q->y;
    f32 wz = q->w * q->z;
    
    m[0][0] = 1.0f - 2.0f * (yy + zz);
    m[0][1] = 2.0f * (xy - wz);
    m[0][2] = 2.0f * (xz + wy);
    m[0][3] = 0.0f;
    
    m[1][0] = 2.0f * (xy + wz);
    m[1][1] = 1.0f - 2.0f * (xx + zz);
    m[1][2] = 2.0f * (yz - wx);
    m[1][3] = 0.0f;
    
    m[2][0] = 2.0f * (xz - wy);
    m[2][1] = 2.0f * (yz + wx);
    m[2][2] = 1.0f - 2.0f * (xx + yy);
    m[2][3] = 0.0f;
}

void C_MTXLightFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f, f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    f32 tmp;
    
    tmp = 1.0f / (r - l);
    m[0][0] = (2.0f * n) * tmp * scaleS;
    m[0][1] = 0.0f;
    m[0][2] = (r + l) * tmp * scaleS - transS;
    m[0][3] = 0.0f;
    
    tmp = 1.0f / (t - b);
    m[1][0] = 0.0f;
    m[1][1] = (2.0f * n) * tmp * scaleT;
    m[1][2] = (t + b) * tmp * scaleT - transT;
    m[1][3] = 0.0f;
    
    tmp = 1.0f / (f - n);
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = -f * tmp;
    m[2][3] = (f * n) * tmp;
}

void C_MTXLightPerspective(Mtx m, f32 fovy, f32 aspect, f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    f32 angle = fovy * 0.5f;
    f32 cot = 1.0f / tanf(angle);
    
    m[0][0] = (cot / aspect) * scaleS;
    m[0][1] = 0.0f;
    m[0][2] = -transS;
    m[0][3] = 0.0f;
    
    m[1][0] = 0.0f;
    m[1][1] = cot * scaleT;
    m[1][2] = -transT;
    m[1][3] = 0.0f;
    
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = -1.0f;
    m[2][3] = 0.0f;
}

void C_MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    f32 tmp;
    
    tmp = 1.0f / (r - l);
    m[0][0] = 2.0f * tmp * scaleS;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = ((r + l) * tmp * scaleS) - transS;
    
    tmp = 1.0f / (t - b);
    m[1][0] = 0.0f;
    m[1][1] = 2.0f * tmp * scaleT;
    m[1][2] = 0.0f;
    m[1][3] = ((t + b) * tmp * scaleT) - transT;
    
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 0.0f;
    m[2][3] = 1.0f;
}

#endif // PLATFORM_PC
