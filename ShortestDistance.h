#pragma once

#include "MT3Line.h"

class ShortestDistance
{

public:

    /// <summary>
    /// シングルトンインスタンスの取得
    /// </summary>
    /// <returns></returns>
    static ShortestDistance* GetInstance();

    /// <summary>
    /// 点と直線の距離
    /// </summary>
    /// <param name="p">点</param>
    /// <param name="l">直線</param>
    /// <param name="h">点から下した垂線の足(戻り値)</param>
    /// <param name="t">ベクトル係数(戻り値)</param>
    /// <returns>最短距離</returns>
    static float PointLineDist(const Vector3& p, const Line& l, Vector3& h, float &t);

    /// <summary>
    /// 点と線分の距離
    /// </summary>
    /// <param name="p">点</param>
    /// <param name="l">線分</param>
    /// <param name="h">最短距離となる端点(戻り値)</param>
    /// <param name="t">端点位置(戻り値)</param>
    /// <returns>最短距離</returns>
    static float PointSegmentDist(const Vector3& p, const Segment& seg, Vector3& h, float& t);

private:
    ShortestDistance() = default;
    ~ShortestDistance() = default;
    ShortestDistance(const ShortestDistance&) = delete;
    ShortestDistance& operator=(const ShortestDistance&) = delete;

};

