fwLoadProperties()
fwUseForwardInclude(
    fwCore
)

find_package(GLM REQUIRED)

fwForwardInclude(
    ${GLM_INCLUDE_DIR}
)