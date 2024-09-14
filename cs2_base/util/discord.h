#pragma once

#include "memmanager.h"
#include <iostream>
#include <string>

namespace discord {

    typedef struct _DHEADER
	{
		uint32_t Size; // size of the mapped buffer
		uint64_t Sequence; // frame sequence number, increase after present
		uint32_t Width;
		uint32_t Height;
		uint8_t Data[1];

	} DHEADER, *PDHEADER;

    typedef struct _DISCORDCONTROLLER {
        MemoryManager MM;
        PDHEADER framebuffer;
    } DiscordController, *PDC;

    inline bool connect(DiscordController& DC) {
        std::string filename = "DiscordOverlay_Framebuffer_Memory_" + std::to_string(DC.MM.pID);
        DC.MM.hProc = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, filename.c_str());
        
        if (!DC.MM.hProc || DC.MM.hProc == INVALID_HANDLE_VALUE) return false;

        DC.framebuffer  = static_cast<PDHEADER>(MapViewOfFile(DC.MM.hProc, FILE_MAP_ALL_ACCESS, 0, 0, 0));

        return DC.framebuffer != NULL;
    }

    inline void disconnect(DiscordController& DC) {
        UnmapViewOfFile(DC.framebuffer);
        DC.framebuffer = nullptr;

        CloseHandle(DC.MM.hProc);
        DC.MM.hProc = nullptr;
    }

    inline void sendFrame(DiscordController& DC, uint32_t width, uint32_t height, void* frame, size_t size) {
        
        DC.framebuffer->Width = width;
        DC.framebuffer->Height = height;

        memcpy(DC.framebuffer->Data, frame, size);

        DC.framebuffer->Sequence++;
    }


    namespace draw {

        typedef struct _FRAME {
            uint32_t width;
            uint32_t height;
            size_t size;
            void* buffer;
        } Frame, * PFrame;

        typedef struct _PIXEL {
            BYTE B, G, R, A;
        } Pixel, * PPixel;

        typedef struct _POINT {
            int32_t x, y;
        } Point;

        inline Frame createFrame(uint32_t width, uint32_t height) {
            Frame ret;

            ret.width = width;
            ret.height = height;
            ret.size = width * height * 4;

            ret.buffer = malloc(ret.size);
            memset(ret.buffer, 0, ret.size);

            return ret;
        }

        inline void clearFrame(Frame& frame) {
            memset(frame.buffer, 0, frame.size);
        }

        inline void _setPixel(Frame& frame, Point p1, Pixel pixel) {
            if (frame.width > p1.x && frame.height > p1.y) {
                if (p1.x < 0 || p1.y < 0) return;//changed point x,y from uint to int
                PPixel buffer = static_cast<PPixel>(frame.buffer);
                buffer[p1.y * frame.width + p1.x] = pixel;
            }
        }

        inline void iLine(Frame& frame, Point p1, Point p2, Pixel color) {
            Point start = p1.x > p2.x ? p2 : p1;
            Point end = p1.x > p2.x ? p1 : p2;
            int dx = end.x - start.x;
            int dy = end.y - start.y;
            int sy = dy >= 0 ? 1 : -1;
            int D = 2*dy*sy - dx;
            while (start.x <= end.x) {
                _setPixel(frame, start, color);
                if (D > 0) {
                    start.y += sy;
                    D -= 2 * dx;
                }
                D += 2 * dy*sy;
                start.x += 1;
            }
        }

        inline void hLine(Frame& frame, Point p1, Point p2, Pixel color) {
            int dx = p1.x > p2.x ? -1 : 1;

            while (p1.x != p2.x) {
                _setPixel(frame, p1, color);
                p1.x += dx;
            }
        }

        inline void vLine(Frame& frame, Point p1, Point p2, Pixel color) {
            int dy = p1.y > p2.y ? -1 : 1;

            while (p1.y != p2.y) {
                _setPixel(frame, p1, color);
                p1.y += dy;
            }
        }

        inline void Rect(Frame& frame, Point max, Point min, Pixel color) {
            Point p4 = { min.x, max.y };
            Point p2 = { max.x, min.y };
            hLine(frame, min, p2, color);
            vLine(frame, p2, max, color);
            hLine(frame, max, p4, color);
            vLine(frame, p4, min, color);
        }

        inline void fillRect(Frame& frame, Point max, Point min, Pixel color) {
            for (int i = min.x + 1; i < max.x - 1; i++) {
                for (int j = min.y + 1; j < max.y - 1; j++) {
                    _setPixel(frame, Point(i, j), color);
                }
            }
        }

        inline void _CircunferenceAux(Frame& frame, int xc, int yc, int x, int y, Pixel color) {
            _setPixel(frame, Point(xc + x, yc + y), color);
            _setPixel(frame, Point(xc - x, yc + y), color);
            _setPixel(frame, Point(xc + x, yc - y), color);
            _setPixel(frame, Point(xc - x, yc - y), color);
            _setPixel(frame, Point(xc + y, yc + x), color);
            _setPixel(frame, Point(xc - y, yc + x), color);
            _setPixel(frame, Point(xc + y, yc - x), color);
            _setPixel(frame, Point(xc - y, yc - x), color);
        }

        inline void Circle(Frame& frame, Point center, uint32_t radius, Pixel color) {
            //bresenham circle drawing algorithm
            int p = 1 - radius;
            int x = 0, y = radius;

            _CircunferenceAux(frame, center.x, center.y, x, y, color);

            while (x < y) {
                x++;
                if (p < 0) p += 2 * x + 1;
                else {
                    y--;
                    p += 2 * (x - y) + 1;
                }
                _CircunferenceAux(frame, center.x, center.y, x, y, color);
            }
        }   
    }

}