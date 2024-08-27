/**************************************************************************/
/*  locale_editor_plugin.h                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/
#ifndef LOCALE_EDITOR_PLUGIN_H
#define LOCALE_EDITOR_PLUGIN_H

#include "editor/plugins/editor_plugin.h"

class WindowWrapper;
class HSplitContainer;
class HBoxContainer;
class VBoxContainer;
class ItemList;

class LocaleEditorPlugin : public EditorPlugin {
	GDCLASS(LocaleEditorPlugin, EditorPlugin);

    static LocaleEditorPlugin *singleton;

    Button *bottom_panel_button = nullptr;
    WindowWrapper *window_wrapper = nullptr;
    HSplitContainer *hsplit = nullptr;

    // root container
    HBoxContainer *root_container = nullptr;

    // bundle list on left-hand side
    VBoxContainer *bundle_list_container = nullptr;
    ItemList *bundle_list = nullptr;

    // main container on right-hand side
    VBoxContainer *main_container = nullptr;
    Label *current_bundle_label = nullptr;

    // HBoxContainer *bundle_list_ui = nullptr;
    // ItemList *bundle_list = nullptr;

private:
    void build_bundle_list_ui();
    void build_main_ui();    
    void update_bundle_list();
    void _window_visibility_changed(bool p_visible);
    void _notification(int p_what);
    void _project_settings_changed();
    void _resource_saved(const Ref<Resource> &p_res);
    void _visiblity_changed();

public:
    LocaleEditorPlugin();
    ~LocaleEditorPlugin();

    static LocaleEditorPlugin *get_singleton() { return singleton; }
    void notify_translation_changed();

};

#endif // LOCALE_EDITOR_PLUGIN_H