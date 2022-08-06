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
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstmybasesrc
 *
 * The mybasesrc element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! mybasesrc ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/base/gstbasesrc.h>
#include "gstmybasesrc.h"

GST_DEBUG_CATEGORY_STATIC (gst_mybasesrc_debug_category);
#define GST_CAT_DEFAULT gst_mybasesrc_debug_category

/* prototypes */


static void gst_mybasesrc_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_mybasesrc_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_mybasesrc_dispose (GObject * object);
static void gst_mybasesrc_finalize (GObject * object);

static GstCaps *gst_mybasesrc_get_caps (GstBaseSrc * src, GstCaps * filter);
static gboolean gst_mybasesrc_negotiate (GstBaseSrc * src);
static GstCaps *gst_mybasesrc_fixate (GstBaseSrc * src, GstCaps * caps);
static gboolean gst_mybasesrc_set_caps (GstBaseSrc * src, GstCaps * caps);
static gboolean gst_mybasesrc_decide_allocation (GstBaseSrc * src,
    GstQuery * query);
static gboolean gst_mybasesrc_start (GstBaseSrc * src);
static gboolean gst_mybasesrc_stop (GstBaseSrc * src);
static void gst_mybasesrc_get_times (GstBaseSrc * src, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end);
static gboolean gst_mybasesrc_get_size (GstBaseSrc * src, guint64 * size);
static gboolean gst_mybasesrc_is_seekable (GstBaseSrc * src);
static gboolean gst_mybasesrc_prepare_seek_segment (GstBaseSrc * src,
    GstEvent * seek, GstSegment * segment);
static gboolean gst_mybasesrc_do_seek (GstBaseSrc * src, GstSegment * segment);
static gboolean gst_mybasesrc_unlock (GstBaseSrc * src);
static gboolean gst_mybasesrc_unlock_stop (GstBaseSrc * src);
static gboolean gst_mybasesrc_query (GstBaseSrc * src, GstQuery * query);
static gboolean gst_mybasesrc_event (GstBaseSrc * src, GstEvent * event);
static GstFlowReturn gst_mybasesrc_create (GstBaseSrc * src, guint64 offset,
    guint size, GstBuffer ** buf);
static GstFlowReturn gst_mybasesrc_alloc (GstBaseSrc * src, guint64 offset,
    guint size, GstBuffer ** buf);
static GstFlowReturn gst_mybasesrc_fill (GstBaseSrc * src, guint64 offset,
    guint size, GstBuffer * buf);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_mybasesrc_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("application/unknown")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMybasesrc, gst_mybasesrc, GST_TYPE_BASE_SRC,
  GST_DEBUG_CATEGORY_INIT (gst_mybasesrc_debug_category, "mybasesrc", 0,
  "debug category for mybasesrc element"));

static void
gst_mybasesrc_class_init (GstMybasesrcClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseSrcClass *base_src_class = GST_BASE_SRC_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS(klass),
      &gst_mybasesrc_src_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS(klass),
      "FIXME Long name", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_mybasesrc_set_property;
  gobject_class->get_property = gst_mybasesrc_get_property;
  gobject_class->dispose = gst_mybasesrc_dispose;
  gobject_class->finalize = gst_mybasesrc_finalize;
  base_src_class->get_caps = GST_DEBUG_FUNCPTR (gst_mybasesrc_get_caps);
  base_src_class->negotiate = GST_DEBUG_FUNCPTR (gst_mybasesrc_negotiate);
  base_src_class->fixate = GST_DEBUG_FUNCPTR (gst_mybasesrc_fixate);
  base_src_class->set_caps = GST_DEBUG_FUNCPTR (gst_mybasesrc_set_caps);
  base_src_class->decide_allocation = GST_DEBUG_FUNCPTR (gst_mybasesrc_decide_allocation);
  base_src_class->start = GST_DEBUG_FUNCPTR (gst_mybasesrc_start);
  base_src_class->stop = GST_DEBUG_FUNCPTR (gst_mybasesrc_stop);
  base_src_class->get_times = GST_DEBUG_FUNCPTR (gst_mybasesrc_get_times);
  base_src_class->get_size = GST_DEBUG_FUNCPTR (gst_mybasesrc_get_size);
  base_src_class->is_seekable = GST_DEBUG_FUNCPTR (gst_mybasesrc_is_seekable);
  base_src_class->prepare_seek_segment = GST_DEBUG_FUNCPTR (gst_mybasesrc_prepare_seek_segment);
  base_src_class->do_seek = GST_DEBUG_FUNCPTR (gst_mybasesrc_do_seek);
  base_src_class->unlock = GST_DEBUG_FUNCPTR (gst_mybasesrc_unlock);
  base_src_class->unlock_stop = GST_DEBUG_FUNCPTR (gst_mybasesrc_unlock_stop);
  base_src_class->query = GST_DEBUG_FUNCPTR (gst_mybasesrc_query);
  base_src_class->event = GST_DEBUG_FUNCPTR (gst_mybasesrc_event);
  base_src_class->create = GST_DEBUG_FUNCPTR (gst_mybasesrc_create);
  base_src_class->alloc = GST_DEBUG_FUNCPTR (gst_mybasesrc_alloc);
  base_src_class->fill = GST_DEBUG_FUNCPTR (gst_mybasesrc_fill);

}

static void
gst_mybasesrc_init (GstMybasesrc *mybasesrc)
{
}

void
gst_mybasesrc_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (object);

  GST_DEBUG_OBJECT (mybasesrc, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mybasesrc_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (object);

  GST_DEBUG_OBJECT (mybasesrc, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mybasesrc_dispose (GObject * object)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (object);

  GST_DEBUG_OBJECT (mybasesrc, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_mybasesrc_parent_class)->dispose (object);
}

void
gst_mybasesrc_finalize (GObject * object)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (object);

  GST_DEBUG_OBJECT (mybasesrc, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_mybasesrc_parent_class)->finalize (object);
}

/* get caps from subclass */
static GstCaps *
gst_mybasesrc_get_caps (GstBaseSrc * src, GstCaps * filter)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "get_caps");

  return NULL;
}

/* decide on caps */
static gboolean
gst_mybasesrc_negotiate (GstBaseSrc * src)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "negotiate");

  return TRUE;
}

/* called if, in negotiation, caps need fixating */
static GstCaps *
gst_mybasesrc_fixate (GstBaseSrc * src, GstCaps * caps)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "fixate");

  return NULL;
}

/* notify the subclass of new caps */
static gboolean
gst_mybasesrc_set_caps (GstBaseSrc * src, GstCaps * caps)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "set_caps");

  return TRUE;
}

/* setup allocation query */
static gboolean
gst_mybasesrc_decide_allocation (GstBaseSrc * src, GstQuery * query)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "decide_allocation");

  return TRUE;
}

/* start and stop processing, ideal for opening/closing the resource */
static gboolean
gst_mybasesrc_start (GstBaseSrc * src)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "start");

  return TRUE;
}

static gboolean
gst_mybasesrc_stop (GstBaseSrc * src)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "stop");

  return TRUE;
}

/* given a buffer, return start and stop time when it should be pushed
 * out. The base class will sync on the clock using these times. */
static void
gst_mybasesrc_get_times (GstBaseSrc * src, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "get_times");

}

/* get the total size of the resource in bytes */
static gboolean
gst_mybasesrc_get_size (GstBaseSrc * src, guint64 * size)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "get_size");

  return TRUE;
}

/* check if the resource is seekable */
static gboolean
gst_mybasesrc_is_seekable (GstBaseSrc * src)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "is_seekable");

  return TRUE;
}

/* Prepare the segment on which to perform do_seek(), converting to the
 * current basesrc format. */
static gboolean
gst_mybasesrc_prepare_seek_segment (GstBaseSrc * src, GstEvent * seek,
    GstSegment * segment)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "prepare_seek_segment");

  return TRUE;
}

/* notify subclasses of a seek */
static gboolean
gst_mybasesrc_do_seek (GstBaseSrc * src, GstSegment * segment)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "do_seek");

  return TRUE;
}

/* unlock any pending access to the resource. subclasses should unlock
 * any function ASAP. */
static gboolean
gst_mybasesrc_unlock (GstBaseSrc * src)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "unlock");

  return TRUE;
}

/* Clear any pending unlock request, as we succeeded in unlocking */
static gboolean
gst_mybasesrc_unlock_stop (GstBaseSrc * src)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "unlock_stop");

  return TRUE;
}

/* notify subclasses of a query */
static gboolean
gst_mybasesrc_query (GstBaseSrc * src, GstQuery * query)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "query");

  return TRUE;
}

/* notify subclasses of an event */
static gboolean
gst_mybasesrc_event (GstBaseSrc * src, GstEvent * event)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "event");

  return TRUE;
}

/* ask the subclass to create a buffer with offset and size, the default
 * implementation will call alloc and fill. */
static GstFlowReturn
gst_mybasesrc_create (GstBaseSrc * src, guint64 offset, guint size,
    GstBuffer ** buf)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "create");

  return GST_FLOW_OK;
}

/* ask the subclass to allocate an output buffer. The default implementation
 * will use the negotiated allocator. */
static GstFlowReturn
gst_mybasesrc_alloc (GstBaseSrc * src, guint64 offset, guint size,
    GstBuffer ** buf)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "alloc");

  return GST_FLOW_OK;
}

/* ask the subclass to fill the buffer with data from offset and size */
static GstFlowReturn
gst_mybasesrc_fill (GstBaseSrc * src, guint64 offset, guint size, GstBuffer * buf)
{
  GstMybasesrc *mybasesrc = GST_MYBASESRC (src);

  GST_DEBUG_OBJECT (mybasesrc, "fill");

  return GST_FLOW_OK;
}

static gboolean
plugin_init (GstPlugin * plugin)
{

  /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
  return gst_element_register (plugin, "mybasesrc", GST_RANK_NONE,
      GST_TYPE_MYBASESRC);
}

/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
#ifndef VERSION
#define VERSION "0.0.FIXME"
#endif
#ifndef PACKAGE
#define PACKAGE "FIXME_package"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "FIXME_package_name"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "http://FIXME.org/"
#endif

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    mybasesrc,
    "FIXME plugin description",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)

