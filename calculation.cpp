#include <node_api.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

struct ConeData {
    std::vector<float> vertices;
    std::vector<int> indices;
};

ConeData generateCone(float height, float radius, int segments) {
    ConeData cone;

    // Вершина конуса
    cone.vertices.push_back(0.0f);         // x
    cone.vertices.push_back(height);       // y
    cone.vertices.push_back(0.0f);         // z

    // Боковые вершины конуса
    float segmentAngle = 2.0f * M_PI / segments;
    for (int i = 0; i < segments; ++i) {
        float theta = i * segmentAngle;
        float x = radius * std::cos(theta);
        float z = radius * std::sin(theta);

        cone.vertices.push_back(x);         // x
        cone.vertices.push_back(0.0f);      // y
        cone.vertices.push_back(z);         // z
    }

    // Индексы для отрисовки треугольников
    for (int i = 1; i <= segments; ++i) {
        cone.indices.push_back(0);           // Вершина конуса
        cone.indices.push_back(i);           // Боковая вершина текущего сегмента
        cone.indices.push_back((i % segments) + 1);  // Боковая вершина следующего сегмента
    }

    return cone;
}

napi_value GenerateConeData(napi_env env, napi_callback_info info) {
    napi_status status;

    // Получение аргументов функции
    size_t argc = 3;
    napi_value args[3];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to get callback info");
        return nullptr;
    }

    // Проверка типов аргументов
    for (int i = 0; i < argc; ++i) {
        napi_valuetype valuetype;
        status = napi_typeof(env, args[i], &valuetype);
        if (status != napi_ok) {
            napi_throw_error(env, nullptr, "Failed to get argument type");
            return nullptr;
        }

        if (valuetype != napi_number) {
            napi_throw_type_error(env, nullptr, "Invalid argument type. All arguments must be numbers");
            return nullptr;
        }
    }

    // Извлечение значений аргументов
    double height, radius;
    int segments;
    status = napi_get_value_double(env, args[0], &height);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to get height value");
        return nullptr;
    }

    status = napi_get_value_double(env, args[1], &radius);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to get radius value");
        return nullptr;
    }

    status = napi_get_value_int32(env, args[2], &segments);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to get segments value");
        return nullptr;
    }

    // Генерация данных конуса
    ConeData cone = generateCone(static_cast<float>(height), static_cast<float>(radius), segments);

    // Создание объекта-результата
    napi_value result;
    status = napi_create_object(env, &result);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to create result object");
        return nullptr;
    }

    // Заполнение массива вершин
    napi_value verticesArray;
    status = napi_create_array(env, &verticesArray);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to create vertices array");
        return nullptr;
    }

    for (size_t i = 0; i < cone.vertices.size(); ++i) {
        napi_value value;
        status = napi_create_double(env, cone.vertices[i], &value);
        if (status != napi_ok) {
            napi_throw_error(env, nullptr, "Failed to create vertex value");
            return nullptr;
        }

        status = napi_set_element(env, verticesArray, i, value);
        if (status != napi_ok) {
            napi_throw_error(env, nullptr, "Failed to set vertex value");
            return nullptr;
        }
    }

    status = napi_set_named_property(env, result, "vertices", verticesArray);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to set vertices property");
        return nullptr;
    }

    // Заполнение массива индексов
    napi_value indicesArray;
    status = napi_create_array(env, &indicesArray);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to create indices array");
        return nullptr;
    }

    for (size_t i = 0; i < cone.indices.size(); ++i) {
        napi_value value;
        status = napi_create_int32(env, cone.indices[i], &value);
        if (status != napi_ok) {
            napi_throw_error(env, nullptr, "Failed to create index value");
            return nullptr;
        }

        status = napi_set_element(env, indicesArray, i, value);
        if (status != napi_ok) {
            napi_throw_error(env, nullptr, "Failed to set index value");
            return nullptr;
        }
    }

    status = napi_set_named_property(env, result, "indices", indicesArray);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to set indices property");
        return nullptr;
    }

    return result;
}

napi_value Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_value fn;

    status = napi_create_function(env, nullptr, 0, GenerateConeData, nullptr, &fn);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to create function");
        return nullptr;
    }

    status = napi_set_named_property(env, exports, "generateConeData", fn);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "Failed to set function property");
        return nullptr;
    }

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)