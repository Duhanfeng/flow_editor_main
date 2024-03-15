//
// Created by cxc on 2024/3/13.
//

#pragma once
#include <map>
#include <memory>
#include <unordered_map>
#include <flow_editor/data_def.hpp>
#include <flow_editor/flow_model.hpp>

namespace fe
{
class SceneConfig
{
public:
    //默认样式
    std::shared_ptr<NodeStyle> node_style;                        //节点样式
    std::shared_ptr<ConnectionStyle> connection_style;            //连接样式
    std::shared_ptr<DraftConnectionStyle> draft_connection_style; //草稿连接样式
    std::shared_ptr<TypeColorMap> type_color_map;                 //类型与颜色映射字典

    //显示模式
    SceneShowMode scene_show_mode = SceneShowMode::Dynamic;             //场景显示模式
    double preview_scale = 0.5;                                         //小于该缩放值时,进入预览模式
    NodeLayoutStyle node_layout_style = NodeLayoutStyle::Horizontal;    //节点布局
    FlowPermission flow_permission = FlowPermission::FlowPermissionAll; //权限
};

} //namespace fe