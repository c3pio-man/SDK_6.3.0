
add_library(Qt5::EinkIntegrationPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(EinkIntegrationPlugin RELEASE "platforms/libqpocketbook2.so")

list(APPEND Qt5Gui_PLUGINS Qt5::EinkIntegrationPlugin)
