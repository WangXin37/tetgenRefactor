#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <assert.h>

//**********************************************
//Point in 2d
//**********************************************

struct Point2D : public std::array<double, 2> {
public:
    Point2D() {
        (*this)[0] = 0;
        (*this)[1] = 0;
    }
    Point2D(const double x, const double y) {
        (*this)[0] = x;
        (*this)[1] = y;
    }

    Point2D(const Point2D& point) {
        (*this)[0] = point[0];
        (*this)[1] = point[1];
    }

    const double& x() {
        return (*this)[0];
    }

    const double& y() {
        return (*this)[1];
    }

    Point2D operator-() const {
        return { -(*this)[0],-(*this)[1] };
    }

    Point2D operator+(const Point2D& point) const {
        return Point2D{ (*this)[0] + point[0], (*this)[1] + point[1] };
    }

    Point2D& operator+=(const Point2D& point) {
        (*this)[0] += point[0];
        (*this)[1] += point[1];
        return *this;
    }

    Point2D operator-(const Point2D& point) const {
        return Point2D{ (*this)[0] - point[0], (*this)[1] - point[1] };
    }

    Point2D& operator-=(const Point2D& point) {
        (*this)[0] -= point[0];
        (*this)[1] -= point[1];
        return *this;
    }

    Point2D operator*(const double& scalar) const {
        return Point2D{ (*this)[0] * scalar, (*this)[1] * scalar };
    }

    Point2D& operator*=(const double& scalar) {
        (*this)[0] *= scalar;
        (*this)[1] += scalar;
        return *this;
    }

    Point2D operator/(const double& scalar) const {
        return Point2D{ (*this)[0] / scalar, (*this)[1] / scalar };
    }

    Point2D& operator/=(const double& scalar) {
        (*this)[0] /= scalar;
        (*this)[1] /= scalar;
        return *this;
    }

    bool operator==(const Point2D& other) const {
        if (std::abs((*this)[0] - other[0]) < 1e-9 && std::abs((*this)[1] - other[1]) < 1e-9)
            return true;
        else
            return false;
    }

    double Length() {
        return std::sqrt((*this)[0] * (*this)[0] + (*this)[1] * (*this)[1]);
    }

    double Length2() {
        return (*this)[0] * (*this)[0] + (*this)[1] * (*this)[1];
    }

    void Normalize() {
        (*this) /= Length();
    }

    Point2D GetNormalized() {
        return (*this) / Length();
    }

    Point2D GetRotate90() const { //cw
        return Point2D{ -(*this)[1],(*this)[0] };
    }

    double Cross(const Point2D& p) const {
        return (*this)[0] * p[1] - (*this)[1] * p[0];
    }

    double Dot(const Point2D& p) const {
        return (*this)[0] * p[0] + (*this)[1] * p[1];
    }
};

//**********************************************
//Point in 3d
//**********************************************

struct Point3D : public std::array<double, 3> {
public:
    Point3D() {
        (*this)[0] = 0;
        (*this)[1] = 0;
        (*this)[2] = 0;
    }
    Point3D(const double x, const double y, const double z) {
        (*this)[0] = x;
        (*this)[1] = y;
        (*this)[2] = z;
    }

    Point3D(const Point3D& point) {
        (*this)[0] = point[0];
        (*this)[1] = point[1];
        (*this)[2] = point[2];
    }

    const double& x() {
        return (*this)[0];
    }

    const double& y() {
        return (*this)[1];
    }

    const double& z() {
        return (*this)[2];
    }

    Point3D operator-() const {
        return { -(*this)[0],-(*this)[1], -(*this)[2] };
    }

    Point3D operator+(const Point3D& point) const {
        return Point3D{ (*this)[0] + point[0], (*this)[1] + point[1], (*this)[2] + point[2] };
    }

    Point3D& operator+=(const Point3D& point) {
        (*this)[0] += point[0];
        (*this)[1] += point[1];
        (*this)[2] += point[2];
        return *this;
    }

    Point3D operator-(const Point3D& point) const {
        return Point3D{ (*this)[0] - point[0], (*this)[1] - point[1], (*this)[2] - point[2] };
    }

    Point3D& operator-=(const Point3D& point) {
        (*this)[0] -= point[0];
        (*this)[1] -= point[1];
        (*this)[2] -= point[2];
        return *this;
    }

    Point3D operator*(const double& scalar) const {
        return Point3D{ (*this)[0] * scalar, (*this)[1] * scalar,(*this)[2] * scalar };
    }

    Point3D& operator*=(const double& scalar) {
        (*this)[0] *= scalar;
        (*this)[1] += scalar;
        (*this)[2] += scalar;
        return *this;
    }

    Point3D operator/(const double& scalar) const {
        return Point3D{ (*this)[0] / scalar, (*this)[1] / scalar,(*this)[2] / scalar };
    }

    Point3D& operator/=(const double& scalar) {
        (*this)[0] /= scalar;
        (*this)[1] /= scalar;
        (*this)[2] /= scalar;
        return *this;
    }

    bool operator==(const Point3D& other) const {
        if (std::abs((*this)[0] - other[0]) < 1e-9 && std::abs((*this)[1] - other[1]) < 1e-9
            && std::abs((*this)[2] - other[2]) < 1e-9)
            return true;
        else
            return false;
    }

    double Length2() {
        return (*this)[0] * (*this)[0] + (*this)[1] * (*this)[1] + (*this)[2] * (*this)[2];
    }

    double Length() {
        return std::sqrt(Length2());
    }

    void Normalize() {
        (*this) /= Length();
    }

    Point3D GetNormalized() {
        return (*this) / Length();
    }
};

//**********************************************
//TetgenRefactor Logger
//**********************************************
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "tetgenr_version.h"

#define Log_Debug(...) SPDLOG_LOGGER_DEBUG(Logger::GetInstance().GetLoggerPtr(), __VA_ARGS__);
#define Log_Info(...) SPDLOG_LOGGER_INFO(Logger::GetInstance().GetLoggerPtr(),__VA_ARGS__);
#define Log_Warn(...) SPDLOG_LOGGER_WARN(Logger::GetInstance().GetLoggerPtr(),__VA_ARGS__);
#define Log_Error(...) SPDLOG_LOGGER_ERROR(Logger::GetInstance().GetLoggerPtr(),__VA_ARGS__);
#define Log_Trace(...) SPDLOG_LOGGER_TRACE(Logger::GetInstance().GetLoggerPtr(),__VA_ARGS__);

#define TETMESH_EXIT(...)   \
{                           \
    Log_Error(__VA_ARGS__); \
    assert(0);              \
    exit(EXIT_FAILURE);     \
}

#define FIND_MESH_FILE_TAG(LINE,STR)               \
    if(LINE.find(STR) == std::string::npos)        \
    {                                              \
        TETMESH_EXIT("Invalid mesh input:{}",STR); \
    }


class Logger
{
private:
    std::shared_ptr<spdlog::logger> spdlogPtr = nullptr;
    Logger() = default;
public:
    ~Logger(){}

    static Logger& GetInstance() {
        static Logger instance;
        return instance;
    }

    std::shared_ptr<spdlog::logger>  GetLoggerPtr() {
        return  spdlogPtr;
    }

    void Init(const bool isconsole = true, const bool isfile = false,
        const spdlog::level::level_enum consolelevel = spdlog::level::level_enum::trace,
        const spdlog::level::level_enum filelevel = spdlog::level::level_enum::trace,
        const std::string filename = "tetgenR_log.txt") {
        spdlog::drop_all();
        spdlogPtr.reset();
        spdlogPtr = nullptr;

        const std::string pattern = "%Y-%m-%d %H:%M:%S.%e [%n] [%l] %v";
        std::vector<spdlog::sink_ptr> sinks;
        if (isconsole) {
            auto sink1 = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            sink1->set_level(consolelevel);
            sink1->set_pattern(pattern);
            sinks.push_back(sink1);
        }
        if (isfile) {
            auto sink2 = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true);
            sink2->set_level(filelevel);
            sink2->set_pattern(pattern);
            sinks.push_back(sink2);
        }
        if (sinks.empty()) {
            return;
        }
        spdlogPtr = std::make_shared<spdlog::logger>("TetgenR", begin(sinks), end(sinks));
        spdlogPtr->set_level(spdlog::level::level_enum::trace);
        spdlog::register_logger(spdlogPtr);
        return;
    }

    void PrintVersion() {
        Log_Info("----------------------------------------------");
        Log_Info("TetgenR Version: {}", TETGENR_VERSION);
        Log_Info("TetgenR Internal Version: {}", TETGENR_INTERNAL_VERSION);
        Log_Info("TetgenR Git Hash: {}", TETGENR_GIT_HASH);
        Log_Info("TetgenR Git Branch: {}", TETGENR_GIT_BRANCH);
        Log_Info("TetgenR Git Tag: {}", TETGENR_GIT_TAG);
        Log_Info("TetgenR Build Time: {}", TETGENR_BUILD_TIME);
        Log_Info("TetgenR Build Type: {}", TETGENR_BUILD_TYPE);
        Log_Info("----------------------------------------------");
    }
};