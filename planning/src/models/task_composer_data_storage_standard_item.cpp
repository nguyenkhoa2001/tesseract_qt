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

#include <tesseract_qt/planning/models/task_composer_data_storage_standard_item.h>
#include <tesseract_qt/common/models/standard_item_type.h>
#include <tesseract_qt/common/models/standard_item_utils.h>
#include <tesseract_qt/common/factories/any_poly_standard_item_factory.h>
#include <tesseract_qt/common/icon_utils.h>

#include <tesseract_task_composer/core/task_composer_data_storage.h>

namespace tesseract_gui
{
TaskComposerDataStorageStandardItem::TaskComposerDataStorageStandardItem(
    const tesseract_planning::TaskComposerDataStorage& data)
  : QStandardItem(icons::getDataIcon(), "Task Composer Data Storage")
{
  ctor(data);
}

TaskComposerDataStorageStandardItem::TaskComposerDataStorageStandardItem(
    const QString& text,
    const tesseract_planning::TaskComposerDataStorage& data)
  : QStandardItem(icons::getDataIcon(), text)
{
  ctor(data);
}

TaskComposerDataStorageStandardItem::TaskComposerDataStorageStandardItem(
    const QIcon& icon,
    const QString& text,
    const tesseract_planning::TaskComposerDataStorage& data)
  : QStandardItem(icon, text)
{
  ctor(data);
}

int TaskComposerDataStorageStandardItem::type() const
{
  return static_cast<int>(StandardItemType::MP_TASK_COMPOSER_DATA_STORAGE);
}

void TaskComposerDataStorageStandardItem::ctor(const tesseract_planning::TaskComposerDataStorage& data)
{
  auto copy = data.getData();
  for (const auto& entry : copy)
  {
    QList<QStandardItem*> items = AnyPolyStandardItemManager::create(entry.second);  // NOLINT
    if (!items.empty())
    {
      auto* base_item = new QStandardItem(QString::fromStdString(entry.first));  // NOLINT
      auto* base_item_desc = new QStandardItem(items.front()->text());           // NOLINT
      base_item->appendRow(items);
      appendRow({ base_item, base_item_desc });
    }
    else
    {
      auto* name = new QStandardItem(icons::getUnknownIcon(), QString::fromStdString(entry.first));
      auto* value = new QStandardItem();  // NOLINT
      value->setData("Unknown any type", Qt::DisplayRole);
      appendRow({ name, value });
    }
  }
}
}  // namespace tesseract_gui
