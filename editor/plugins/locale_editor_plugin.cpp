/**************************************************************************/
/*  locale_editor_plugin.cpp                                              */
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

#include "locale_editor_plugin.h"

#include "core/config/project_settings.h"
#include "core/string/translation.h"
#include "editor/editor_command_palette.h"
#include "editor/editor_node.h"
#include "editor/gui/editor_bottom_panel.h"
#include "editor/window_wrapper.h"
#include "scene/gui/item_list.h"
#include "scene/gui/label.h"
#include "scene/gui/split_container.h"
#include "scene/main/node.h"

LocaleEditorPlugin *LocaleEditorPlugin::singleton = nullptr;

void LocaleEditorPlugin::build_bundle_list_ui() {
    bundle_list_container = memnew(VBoxContainer);

    Label *label = memnew(Label);
    label->set_text(TTR("Bundles"));
    bundle_list_container->add_child(label);

    bundle_list = memnew(ItemList);
    bundle_list->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    bundle_list->set_auto_translate_mode(Node::AutoTranslateMode::AUTO_TRANSLATE_MODE_DISABLED);
    bundle_list_container->add_child(bundle_list);
}

void LocaleEditorPlugin::build_main_ui() {
    main_container = memnew(VBoxContainer);

    current_bundle_label = memnew(Label);
    current_bundle_label->set_text(TTR("Current Bundle: "));
    main_container->add_child(current_bundle_label);   
}

void LocaleEditorPlugin::update_bundle_list() {
    bundle_list->clear();
    
    HashSet<String> bundles;

    Vector<String> locales = ProjectSettings::get_singleton()->get_setting_with_override("internationalization/locale/translations");
    for (Vector<String>::Iterator it = locales.begin(); it != locales.end(); ++it) {
        String locale_resource_path = *it;
        Ref<Translation> translation = ResourceLoader::load(locale_resource_path);
        if (translation.is_valid()) {
            String bundle_name = translation->get_bundle_name();
            if (!bundles.has(bundle_name)) {
                bundles.insert(bundle_name);
                bundle_list->add_item(bundle_name);
            }
        }
    }
}

void LocaleEditorPlugin::_window_visibility_changed(bool p_visible) {
    update_bundle_list();
}

void LocaleEditorPlugin::_notification(int p_what) {
	switch (p_what) {
        case Node::NOTIFICATION_READY:
            ProjectSettings::get_singleton()->connect("settings_changed", callable_mp(this, &LocaleEditorPlugin::_project_settings_changed));
            EditorNode::get_singleton()->connect("resource_saved", callable_mp(this, &LocaleEditorPlugin::_resource_saved));
            break;

        case CanvasItem::NOTIFICATION_VISIBILITY_CHANGED:
            _visiblity_changed();
            break;
        
        default:
            break;
    }
}

void LocaleEditorPlugin::_project_settings_changed() {
    update_bundle_list();
}

void LocaleEditorPlugin::_resource_saved(const Ref<Resource> &p_resource) {
    update_bundle_list();
}

void LocaleEditorPlugin::_visiblity_changed() {
    bool is_visible = hsplit->is_visible_in_tree();
    if (is_visible) {
        update_bundle_list();
    }
}

LocaleEditorPlugin::LocaleEditorPlugin() {	
    singleton = this;

    window_wrapper = memnew(WindowWrapper);
	window_wrapper->set_window_title(vformat(TTR("%s - Godot Engine"), TTR("Localization Editor")));
	window_wrapper->set_margins_enabled(true);
    window_wrapper->connect("window_visibility_changed", callable_mp(this, &LocaleEditorPlugin::_window_visibility_changed));

    bottom_panel_button = EditorNode::get_bottom_panel()->add_item(
        TTR("Locale Editor"), 
        window_wrapper, 
        ED_SHORTCUT_AND_COMMAND("bottom_panels/toggle_locale_editor_bottom_panel", 
        TTR("Toggle Locale Editor Bottom Panel"), 
        KeyModifierMask::ALT | Key::L)
    );

    hsplit = memnew(HSplitContainer);
    hsplit->set_split_offset(200);
    hsplit->connect("visibility_changed", callable_mp(this, &LocaleEditorPlugin::_visiblity_changed));
    window_wrapper->set_wrapped_control(hsplit);

    // create sub containers
    build_bundle_list_ui();
    build_main_ui();

    // add sub containers to root container
    hsplit->add_child(bundle_list_container);
    hsplit->add_child(main_container);

    // update bundle list
    update_bundle_list();
}

LocaleEditorPlugin::~LocaleEditorPlugin() {
}

void LocaleEditorPlugin::notify_translation_changed() {
    update_bundle_list();
}
