/* GStreamer
 * Copyright (C) 2022 FIXME <fixme@example.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _GST_MYAUDIOSRC_H_
#define _GST_MYAUDIOSRC_H_

#include <gst/audio/gstaudiosrc.h>

G_BEGIN_DECLS

#define GST_TYPE_MYAUDIOSRC   (gst_myaudiosrc_get_type())
#define GST_MYAUDIOSRC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MYAUDIOSRC,GstMyaudiosrc))
#define GST_MYAUDIOSRC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MYAUDIOSRC,GstMyaudiosrcClass))
#define GST_IS_MYAUDIOSRC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MYAUDIOSRC))
#define GST_IS_MYAUDIOSRC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MYAUDIOSRC))

typedef struct _GstMyaudiosrc GstMyaudiosrc;
typedef struct _GstMyaudiosrcClass GstMyaudiosrcClass;

struct _GstMyaudiosrc
{
  GstAudioSrc base_myaudiosrc;

};

struct _GstMyaudiosrcClass
{
  GstAudioSrcClass base_myaudiosrc_class;
};

GType gst_myaudiosrc_get_type (void);

G_END_DECLS

#endif
