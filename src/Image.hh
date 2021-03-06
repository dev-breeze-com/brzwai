/**
 * Meawm - Simple and configurable window manager
 *
 * @file   Image.hh
 * @author David Reveman <david@meawm.org>
 * @date   18-Jul-2001 01:45:32
 *
 * @brief Definition of WaImage and WaImageControl classes
 *
 * Function declarations and variable definitions for WaImage and
 * WaImageControl classes.
 *
 * Copyright (c) 1997 - 2000 Brad Hughes (bhughes@tcac.net)
 *
 * Copyright (C) David Reveman. All rights reserved.
 *
 * @author Pierre Innocent <dev@breezeos.com>
 * Copyright (C) Tsert.Inc, All rights reserved.
 *
 */

#ifndef   __Image_hh
#define   __Image_hh

extern "C" {
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifdef    RENDER
#  include <X11/extensions/Xrender.h>
#endif // RENDER

#ifdef    XFT
#  include <X11/Xft/Xft.h>
#endif // XFT

#ifdef    PIXMAP
#  include <Imlib2.h>
#endif // PIXMAP
}

#include <list>
using std::list;

class WaImage;
class WaImageControl;

class WaColor {
private:
    int allocated;
    unsigned char red, green, blue, opacity;
    unsigned long pixel;
    
#ifdef RENDER
    XRenderColor xrenderc;
#endif // RENDER
    
#ifdef XFT
    XftColor xftc;
#endif // XFT    

public:
    WaColor(char r = 0, char g = 0, char b = 0)
        { red = r; green = g; blue = b; pixel = 0; allocated = 0; }

    inline const int &isAllocated(void) const { return allocated; }

    inline const unsigned char &getRed(void) const { return red; }
    inline const unsigned char &getGreen(void) const { return green; }
    inline const unsigned char &getBlue(void) const { return blue; }
    inline const unsigned long &getPixel(void) const { return pixel; }

    inline void setAllocated(int a) { allocated = a; }
    inline void setPixel(unsigned long p) { pixel = p; }
    void setRGB(unsigned short, unsigned short, unsigned short);

#ifdef RENDER
    inline XRenderColor *getXRenderColor(void) { return &xrenderc; }
#endif // RENDER
    
#ifdef XFT
    void setXftOpacity(unsigned char);
    inline XftColor *getXftColor(void) { return &xftc; }
#endif // XFT

};


class WaTexture {
private:
    WaColor color, colorTo, hiColor, loColor;
    unsigned long texture;
    
#ifdef RENDER
    Picture alphaPicture;
    Picture solidPicture;
    int opacity;
#endif // RENDER

#ifdef PIXMAP
    Imlib_Image pixmap;
    Imlib_Context *imlib_context;
#endif // PIXMAP

public:
    WaTexture(void) {
        texture = 0;
        
#ifdef RENDER
        opacity = 0;
        alphaPicture = (Picture) 0;
        solidPicture = (Picture) 0;
#endif // RENDER
        
    }
    inline WaColor *getColor(void) { return &color; }
    inline WaColor *getColorTo(void) { return &colorTo; }
    inline WaColor *getHiColor(void) { return &hiColor; }
    inline WaColor *getLoColor(void) { return &loColor; }

    inline const unsigned long &getTexture(void) const { return texture; }

    inline void setTexture(unsigned long t) { texture = t; }
    inline void addTexture(unsigned long t) { texture |= t; }
    
#ifdef RENDER
    int getOpacity(void);
    inline void setOpacity(int o) { opacity = o; }
    inline void setAlphaPicture(Picture p) { alphaPicture = p; }
    inline void setSolidPicture(Picture p) { solidPicture = p; }
    inline Picture getAlphaPicture(void) { return alphaPicture; }
    inline Picture getSolidPicture(void) { return solidPicture; }
#endif // RENDER

#ifdef PIXMAP
    inline void setPixmap(Imlib_Image p) { pixmap = p; }
    inline Imlib_Image getPixmap(void) { return pixmap; }
    inline void setContext(Imlib_Context *context) {
      imlib_context = context;
    }
    inline Imlib_Context *getContext(void) { return imlib_context; }
#endif // PIXMAP

};

// bevel options
#define WaImage_Flat		   (1L <<  1)
#define WaImage_Sunken		   (1L <<  2)
#define WaImage_Raised		   (1L <<  3)

// textures
#define WaImage_Solid		   (1L <<  4)
#define WaImage_Gradient	   (1L <<  5)

// gradients
#define WaImage_Horizontal	   (1L <<  6)
#define WaImage_Vertical	   (1L <<  7)
#define WaImage_Diagonal	   (1L <<  8)
#define WaImage_CrossDiagonal  (1L <<  9)
#define WaImage_Rectangle	   (1L << 10)
#define WaImage_Pyramid		   (1L << 11)
#define WaImage_PipeCross	   (1L << 12)
#define WaImage_Elliptic	   (1L << 13)

// bevel types
#define WaImage_Bevel1		   (1L << 14)
#define WaImage_Bevel2		   (1L << 15)

// inverted image
#define WaImage_Invert		   (1L << 16)

// parent relative image
#define WaImage_ParentRelative (1L << 17)

#ifdef    INTERLACE
// fake interlaced image
#  define WaImage_Interlaced   (1L << 18)
#endif // INTERLACE

#ifdef PIXMAP
// pixmap image
#define WaImage_Pixmap		   (1L << 19)

// scaling methods
#define WaImage_Tile		   (1L << 20)
#define WaImage_Scale	       (1L << 21)
#define WaImage_Stretch		   (1L << 22)
#endif // PIXMAP


template <typename Z> inline Z wamin(Z a, Z b) { return ((a < b) ? a : b); }
template <typename Z> inline Z wamax(Z a, Z b) { return ((a > b) ? a : b); }

class WaImage {
private:
    WaImageControl *control;

#ifdef    INTERLACE
    bool interlaced;
#endif // INTERLACE

    XColor *colors;

    WaColor *from, *to;
    int red_offset, green_offset, blue_offset, red_bits, green_bits, blue_bits,
        ncolors, cpc, cpccpc;
    unsigned char *red, *green, *blue, *red_table, *green_table, *blue_table;
    unsigned int width, height, *xtable, *ytable;


protected:
    Pixmap renderPixmap(void);

    XImage *renderXImage(void);

    void invert(void);
    void bevel1(void);
    void bevel2(void);
    void dgradient(void);
    void egradient(void);
    void hgradient(void);
    void pgradient(void);
    void rgradient(void);
    void vgradient(void);
    void cdgradient(void);
    void pcgradient(void);

public:
    WaImage(WaImageControl *, unsigned int, unsigned int);
    ~WaImage(void);

    Pixmap render(WaTexture *);
    Pixmap render_solid(WaTexture *);
    Pixmap render_gradient(WaTexture *);
    Display *display;
    unsigned int bpp;

#ifdef PIXMAP
    Pixmap render_pixmap(WaTexture *);
#endif // PIXMAP
    
};

#include "Screen.hh"

class WaImageControl {
private:
    bool dither;
    Display *display;
    WaScreen *wascreen;
    Visual *visual;
    Colormap colormap;
    
    
    XColor *colors;
    Window window;
    int colors_per_channel, ncolors, screen_number, screen_depth,
        bits_per_pixel, red_offset, green_offset, blue_offset,
        red_bits, green_bits, blue_bits;
    unsigned char red_color_table[256], green_color_table[256],
        blue_color_table[256];
    unsigned int *grad_xbuffer, *grad_ybuffer, grad_buffer_width,
        grad_buffer_height;
    unsigned long *sqrt_table, cache_max;
    
    typedef struct Cache {
        Pixmap pixmap;

        unsigned int count, width, height;
        unsigned long pixel1, pixel2, texture;
    } Cache;
    
    list<Cache *> *cache;
    
protected:
    Pixmap searchCache(unsigned int, unsigned int, unsigned long, WaColor *,
                       WaColor *);
    
public:
    WaImageControl(Display *, WaScreen *, bool = false, int = 4,
                   unsigned long = 200l);
    virtual ~WaImageControl(void);
    
    inline Display *getDisplay(void) { return display; }
    inline const bool &doDither(void) { return dither; }
    inline int getScreen(void) { return screen_number; }
    inline WaScreen *getWaScreen(void) { return wascreen; }
    inline Visual *getVisual(void) { return visual; }
    inline Colormap getColormap(void) { return colormap; }
    inline const Window &getDrawable(void) const { return window; }
    inline const int &getBitsPerPixel(void) const { return bits_per_pixel; }
    inline const int &getDepth(void) const { return screen_depth; }
    inline const int &getColorsPerChannel(void) const
        { return colors_per_channel; }
    unsigned long getColor(const char *);
    unsigned long getColor(const char *, unsigned short *, unsigned short *,
                           unsigned short *);
    unsigned long getSqrt(unsigned int);
    Pixmap renderImage(unsigned int, unsigned int, WaTexture *,
                       Pixmap = None, unsigned int = 0, unsigned int = 0,
                       Pixmap = None);
    void installRootColormap(void);
    void removeImage(Pixmap);
    void getColorTables(unsigned char **, unsigned char **, unsigned char **,
                        int *, int *, int *, int *, int *, int *);
    void getXColorTable(XColor **, int *);
    void getGradientBuffers(unsigned int, unsigned int,
                            unsigned int **, unsigned int **);
    void setDither(bool d) { dither = d; }
    void setColorsPerChannel(int);
    void parseTexture(WaTexture *, char *);
    void parseColor(WaColor *, char * = 0);
    
    virtual void timeout(void);
    
#ifdef RENDER
    Pixmap xrender(Pixmap, unsigned int, unsigned int, WaTexture *,
                   Pixmap = None, unsigned int = 0, unsigned int = 0,
                   Pixmap = None);
    void setXRootPMapId(bool);
#endif // RENDER
    
};

#endif // __Image_hh
