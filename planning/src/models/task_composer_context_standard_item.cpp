/**
 * @author Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @copyright Copyright (C) 2022 Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @par License
 * GNU Lesser General Public License Version 3, 29 June 2007
 * @par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * @par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * @par
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <tesseract_qt/planning/models/task_composer_context_standard_item.h>
#include <tesseract_qt/planning/models/task_composer_node_info_map_standard_item.h>
#include <tesseract_qt/planning/models/task_composer_data_storage_standard_item.h>

#include <tesseract_qt/common/models/standard_item_type.h>
#include <tesseract_qt/common/models/standard_item_utils.h>
#include <tesseract_qt/common/icon_utils.h>

#include <tesseract_task_composer/core/task_composer_context.h>

namespace tesseract_gui
{
TaskComposerContextStandardItem::TaskComposerContextStandardItem(const tesseract_planning::TaskComposerContext& context)
  : QStandardItem(icons::getUnknownIcon(), "Task Composer Context")
{
  ctor(context);
}

TaskComposerContextStandardItem::TaskComposerContextStandardItem(const QString& text,
                                                                 const tesseract_planning::TaskComposerContext& context)
  : QStandardItem(icons::getUnknownIcon(), text)
{
  ctor(context);
}

TaskComposerContextStandardItem::TaskComposerContextStandardItem(const QIcon& icon,
                                                                 const QString& text,
                                                                 const tesseract_planning::TaskComposerContext& context)
  : QStandardItem(icon, text)
{
  ctor(context);
}

int TaskComposerContextStandardItem::type() const
{
  return static_cast<int>(StandardItemType::MP_TASK_COMPOSER_CONTEXT);
}

void TaskComposerContextStandardItem::ctor(const tesseract_planning::TaskComposerContext& input)
{
  appendRow(createStandardItemString("name", input.name));
  appendRow(createStandardItemBool("dotgraph", input.dotgraph));
  appendRow(new TaskComposerDataStorageStandardItem("data_storage", *input.data_storage));
  /** @todo Add profiles */
  appendRow(createStandardItemBool("successful", input.isSuccessful()));
  appendRow(createStandardItemBool("aborted", input.isAborted()));

  // Convert the map from plain objects to a map with unique_ptr values.
  const auto& raw_map = input.task_infos.getInfoMap();
  std::map<boost::uuids::uuid, std::unique_ptr<tesseract_planning::TaskComposerNodeInfo>> unique_map;
  for (const auto& kv : raw_map)
  {
    // Assuming TaskComposerNodeInfo is copy-constructible. If not, you may need to adjust this.
    unique_map.emplace(kv.first, std::make_unique<tesseract_planning::TaskComposerNodeInfo>(kv.second));
  }
  appendRow(new TaskComposerNodeInfoMapStandardItem(QString("node_infos"), unique_map));
}
}  // namespace tesseract_gui
