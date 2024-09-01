file(GLOB_RECURSE weekend_src CONFIGURE_DEPENDS "src/*.cpp" "src/include/*.hpp")

project(RayTracingInOneWeekend)

include_directories(RayTracingInOneWeekend PUBLIC "src/include")

# 根据每次更新main函数，分开多个版本，按章节区分
add_executable(RayTracingInOneWeekend "src/main_10.cpp")