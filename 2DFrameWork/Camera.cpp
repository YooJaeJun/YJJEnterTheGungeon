#include "framework.h"

Camera::Camera()
{
    //첫 카메라 위치
    position.x = 0.0f;
    position.y = 0.0f;
    //뷰포트
    viewport.width = app.GetWidth();
    viewport.height = app.GetHeight();
    D3D.GetDC()->RSSetViewports(1, viewport.Get11());
    // 줌
    zoomFactor = Vector3(1.0f, 1.0f, 1.0f);
}

void Camera::Set()
{
    //뷰행렬 -> 역이동행렬
    V = Matrix::CreateTranslation(-position.x, -position.y, -1.0f);
    // 크기 행렬 (줌인 줌아웃)
    S = Matrix::CreateScale(Vector3(zoomFactor.x, zoomFactor.y, zoomFactor.z));

    //프로젝션 -> 투영행렬      // 원근 투영 대신 직교 투영 쓸 것
    P = Matrix::CreateOrthographic(app.GetWidth(), app.GetHeight(), 0.0f, 10.0f);

    VP = V * S * P;
}

void Camera::Zoom(const float zDelta)
{
    zoomFactor.x += zDelta / 1200.0f;
    zoomFactor.y += zDelta / 1200.0f;
    zoomFactor.z += zDelta / 1200.0f;
    zoomFactor.x = Utility::Saturate(zoomFactor.x, 0.08f, 1.0f);
    zoomFactor.y = Utility::Saturate(zoomFactor.y, 0.08f, 1.0f);
    zoomFactor.z = Utility::Saturate(zoomFactor.z, 0.08f, 1.0f);
}

void Camera::ResizeScreen()
{
    viewport.width = app.GetWidth();
    viewport.height = app.GetHeight();
    D3D.GetDC()->RSSetViewports(1, viewport.Get11());
}
