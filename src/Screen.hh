/**
 * Meawm - Simple and configurable window manager
 *
 * @file   Screen.hh
 * @author David Reveman <david@meawm.org>
 * @date   25-Jul-2001 23:25:51
 *
 * @brief Definition of WaScreen and ScreenEdge classes
 *
 * Function declarations and variable definitions for WaScreen and
 * ScreeeEdge classes.
 *
 * Copyright (C) David Reveman. All rights reserved.
 *
 * @author Pierre Innocent <dev@breezeos.com>
 * Copyright (C) Tsert.Inc, All rights reserved.
 *
 */

#ifndef __Screen_hh
#define __Screen_hh

extern "C" {
#include <X11/Xlib.h>

#ifdef    RENDER
#  include <X11/extensions/Xrender.h>
#endif // RENDER

#ifdef    XINERAMA
#  include <X11/extensions/Xinerama.h>
#endif // XINERAMA

#ifdef    XFT
#  include <X11/Xft/Xft.h>
#endif // XFT
}

class WaScreen;
class ScreenEdge;

typedef struct _WaAction WaAction;
typedef void (WaScreen::*RootActionFn)(XEvent *, WaAction *);

typedef struct {
    Window window;
    int left;
    int right;
    int top;
    int bottom;
} WMstrut;

#include "Image.hh"
#include "Font.hh"
#include "Resources.hh"
#include "Dockapp.hh"
#include "Net.hh"
#include "Menu.hh"

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Workarea;

class Desktop {
public:
    inline Desktop(int _number, int w, int h) {
        number = _number;
        workarea.x = workarea.y = 0;
        workarea.width = w;
        workarea.height = h;
    }
    unsigned int number;
    Workarea workarea;
};

class SystrayWindow : public WindowObject {
public:
    inline SystrayWindow(Window id, WaScreen *_ws) :
        WindowObject(id, SystrayType) {
        ws = _ws;
    }
    
    WaScreen *ws;
};

class MReq {
public:
    inline MReq(Window m, WaWindow *w, int t) { mid = m; win = w; type = t; }

    Window mid;
    WaWindow *win;
    int type;
};

#define WestDirection  1
#define EastDirection  2
#define NorthDirection 3
#define SouthDirection 4

typedef struct {
    WaColor l_text_focus, l_text_focus_s, l_text_unfocus, l_text_unfocus_s,
        border_color, outline_color;
    WaTexture t_focus, t_unfocus, l_focus, l_unfocus, h_focus, h_unfocus,
        g_focus, g_unfocus;
    WaFont wa_font, wa_font_u;    
    
    int justify, y_pos;
    unsigned int handle_width, border_width, title_height;

    list<ButtonStyle *> buttonstyles;
    list<DockStyle *> dockstyles;
    int b_num;
} WindowStyle;

typedef struct {
    WaColor f_text, f_hilite_text, t_text, f_text_s, f_hilite_text_s, t_text_s,
        border_color;
    WaTexture back_frame, title, hilite;
    WaFont wa_f_font, wa_fh_font, wa_t_font, wa_b_font, wa_bh_font,
        wa_ct_font, wa_cth_font, wa_cf_font, wa_cfh_font;
    char *bullet, *checkbox_true, *checkbox_false;    
    
    int f_justify, t_justify, f_y_pos, t_y_pos, b_y_pos, ct_y_pos, cf_y_pos;
    unsigned int border_width, title_height, item_height;
} MenuStyle;

typedef struct {
    char *style_file, *menu_file, *action_file;
    unsigned int virtual_x;
    unsigned int virtual_y;
    unsigned int desktops;
    int colors_per_channel, menu_stacking;
    long unsigned int cache_max;
    bool image_dither, transient_above, db, revert_to_window;

#ifdef RENDER
    bool lazy_trans;
#endif // RENDER
    
    list<WaAction *> frameacts, awinacts, pwinacts, titleacts, labelacts,
        handleacts, rgacts, lgacts, rootacts, weacts, eeacts, neacts,
        seacts, mtacts, miacts, msacts, mcbacts;
    list<WaAction *> **bacts;

    list<WaActionExtList *> ext_frameacts, ext_awinacts, ext_pwinacts,
        ext_titleacts, ext_labelacts, ext_handleacts, ext_rgacts, ext_lgacts;
    list<WaActionExtList *> **ext_bacts;
} ScreenConfig;

class WaScreen : public WindowObject {
public:
    WaScreen(Display *, int, Meawm *);
    virtual ~WaScreen(void);

    void RaiseWindow(Window);
    void LowerWindow(Window);
    void RestackWindows(Window);
    void UpdateCheckboxes(int);
    WaMenu *GetMenuNamed(char *);
    WaMenu *CreateDynamicMenu(char *);
    void MoveViewportTo(int, int);
    void MoveViewport(int);
    void ScrollViewport(int, bool, WaAction *);
    void MenuMap(XEvent *, WaAction *, bool);
    void MenuRemap(XEvent *, WaAction *, bool);
    void MenuUnmap(XEvent *, WaAction *, bool);
    void UpdateWorkarea(void);
    void GetWorkareaSize(int *, int *, int *, int *);
    void AddDockapp(Window window);
    void GoToDesktop(unsigned int);
    WaWindow *RegexMatchWindow(char *, WaWindow * = NULL);
    void SmartName(WaWindow *);
    void SmartNameRemove(WaWindow *);

#ifdef RANDR
    void RRUpdate(void);
#endif // RANDR
    
    inline void MenuMap(XEvent *e, WaAction *ac) {
        MenuMap(e, ac, false);
    }
    inline void MenuMapFocused(XEvent *e, WaAction *ac) {
        MenuMap(e, ac, true);
    }
    inline void MenuRemap(XEvent *e, WaAction *ac) {
        MenuRemap(e, ac, false);
    }
    inline void MenuRemapFocused(XEvent *e, WaAction *ac) {
        MenuRemap(e, ac, true);
    }
    void ViewportMove(XEvent *, WaAction *);
    void EndMoveResize(XEvent *, WaAction *);
    void Focus(XEvent *, WaAction *);
    inline void MenuUnmap(XEvent *e, WaAction *wa) {
        MenuUnmap(e, wa, false);
    }
    inline void MenuUnmapFocus(XEvent *e, WaAction *wa) {
        MenuUnmap(e, wa, true);
    }

    void Carrousel(unsigned int);
    void Carrousel(XEvent*, WaAction*);

    void Showing(unsigned int);
    void Showing(XEvent*, WaAction*);

    void Compositing(unsigned int);
    void Compositing(XEvent*, WaAction*);

    void Restart(XEvent*, WaAction*);
    void Exit(XEvent*, WaAction*);

    void TaskSwitcher(XEvent *, WaAction *);
    void PreviousTask(XEvent *, WaAction *);
    void NextTask(XEvent *, WaAction *);
    void PointerRelativeWarp(XEvent *, WaAction *);
    void PointerFixedWarp(XEvent *, WaAction *);
    void ViewportRelativeMove(XEvent *, WaAction *);
    void ViewportFixedMove(XEvent *, WaAction *);
    void GoToDesktop(XEvent *, WaAction *);
    void NextDesktop(XEvent *, WaAction *);
    void PreviousDesktop(XEvent *, WaAction *);

    inline void MoveViewportLeft(XEvent *, WaAction *) {
        MoveViewport(WestDirection);
    }
    inline void MoveViewportRight(XEvent *, WaAction *) {
        MoveViewport(EastDirection);
    }
    inline void MoveViewportUp(XEvent *, WaAction *) {
        MoveViewport(NorthDirection);
    }
    inline void MoveViewportDown(XEvent *, WaAction *) {
        MoveViewport(SouthDirection);
    }
    inline void Nop(XEvent *, WaAction *) {}
    
    void EvAct(XEvent *, EventDetail *, list<WaAction *> *);

    Display *display;
    int screen_number, screen_depth, width, height, v_x, v_y, v_xmax, v_ymax;
    Colormap colormap;
    Visual *visual;
    Meawm *meawm;
    NetHandler *net;
    ResourceHandler *rh;
    WaImageControl *ic;
    WindowStyle wstyle;
    MenuStyle mstyle;
    ScreenConfig config;
    WaFont default_font;
    XFontStruct *def_font;
    WindowMenu *window_menu;

    Pixmap fgrip, ugrip;
    Display *pdisplay;
    
#ifdef RENDER
    bool render_extension;
    Pixmap xrootpmap_id;
#endif // RENDER

#ifdef PIXMAP
    Imlib_Context imlib_context;
#endif // PIXMAP

    unsigned long fbutton_pixel, ubutton_pixel, pbutton_pixel;
    unsigned long fgrip_pixel, ugrip_pixel;
    char displaystring[1024];

    ScreenEdge *west, *east, *north, *south;
    Window wm_check;

    bool focus, shutdown;
	bool carrousel_desktop;
	bool showing_desktop;
	bool compositing_on;

    list<Desktop*> desktop_list;
    Desktop *current_desktop;

    list<Window> aot_stacking_list, stacking_list, aab_stacking_list;
    list<WaWindow *> wawindow_list;
    list<WaWindow *> wawindow_list_map_order;
    list<WaMenu *> wamenu_list;
    list<WMstrut *> strut_list;
    list<DockappHandler *> docks;
    list<Window> systray_window_list;
    list<MReq *> mreqs;

private:
    void CreateVerticalEdges(void);
    void CreateHorizontalEdges(void);
    void CreateColors(void);
    void CreateFonts(void);
    void RenderCommonImages(void);

#ifdef XFT
    void CreateXftColor(WaColor *, XftColor *);
#endif // XFT

    int move;
};

class ScreenEdge : public WindowObject {
public:
    ScreenEdge(WaScreen *, int, int, int, int, int);
    virtual ~ScreenEdge(void);

    void SetActionlist(list<WaAction *> *);
    
    WaScreen *wa;
};

#endif // __Screen_hh
