#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const int SVG_SIZE = 500;

const double ORIGIN_X = SVG_SIZE / 2;

const double ORIGIN_Y = SVG_SIZE / 2;

const double CIRCLE_RADIUS = SVG_SIZE / 2 * 0.96;

const double DIVISION_LENGTH = CIRCLE_RADIUS * 0.15;

const double HOUR_HAND_LENGTH = CIRCLE_RADIUS * 0.6;
const double MINUTE_HAND_LENGTH = CIRCLE_RADIUS * 0.8;
const double SECOND_HAND_LENGTH = CIRCLE_RADIUS * 0.9;

void draw_line_from_origin(FILE *fp, double angle, double length, int stroke_width, char *color) {
    fprintf(fp, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\" stroke-width=\"%d\" />\n",
            ORIGIN_X,
            ORIGIN_Y,
            ORIGIN_X + cos(angle) * length,
            ORIGIN_Y - sin(angle) * length,
            color,
            stroke_width);
}

void draw_clock_dial(FILE *fp) {
    fprintf(fp, "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"black\" stroke-width=\"2\" fill=\"white\" />\n",
            ORIGIN_X, ORIGIN_Y, CIRCLE_RADIUS); // draw outer circle

    const double DIVISION_PERIOD = 2 * M_PI / 60;

    for (int i = 0; i <= 60; i++) {
        double angle = DIVISION_PERIOD * i;
        double length = DIVISION_LENGTH * (i % 5 == 0 ? 1.5 : 1);
        int stroke_width = i % 5 == 0 ? 2 : 1;

        fprintf(fp, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"black\" stroke-width=\"%d\" />\n",
                ORIGIN_X + cos(angle) * (CIRCLE_RADIUS - length),
                ORIGIN_Y - sin(angle) * (CIRCLE_RADIUS - length),
                ORIGIN_X + cos(angle) * CIRCLE_RADIUS,
                ORIGIN_Y - sin(angle) * CIRCLE_RADIUS,
                stroke_width);
    }
}

void draw_clock_state(FILE *fp, int hour, int minute, int second) {
    // draw second hand
    double angle = M_PI / 2 - second * 2 * M_PI / 60;
    draw_line_from_origin(fp, angle, SECOND_HAND_LENGTH, 3, "red");
    draw_line_from_origin(fp, angle + M_PI, SECOND_HAND_LENGTH * 0.2, 3, "red");

    // draw minute hand
    double precise_minute = ((double)minute + (double)second / 60);
    angle = M_PI / 2 - precise_minute * 2 * M_PI / 60;
    draw_line_from_origin(fp, angle, MINUTE_HAND_LENGTH, 4, "black");
    draw_line_from_origin(fp, angle + M_PI, MINUTE_HAND_LENGTH * 0.2, 4, "black");

    // draw hour hand
    double precise_hour = ((double)hour + precise_minute / 60);
    angle = M_PI / 2 - precise_hour * 2 * M_PI / 12;
    draw_line_from_origin(fp, angle, HOUR_HAND_LENGTH, 5, "black");
    draw_line_from_origin(fp, angle + M_PI, HOUR_HAND_LENGTH * 0.2, 5, "black");
}

int main(void) {
    int hour, minute, second;

    printf("What's the time? (format: hh mm ss):\n");
    scanf("%d %d %d", &hour, &minute, &second);
    hour = hour % 12;
    minute = minute % 60;
    second = second % 60;

    FILE *fp;
    fp = fopen("./clock.svg", "w+");

    fprintf(fp, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n",
            SVG_SIZE, SVG_SIZE); // start

    draw_clock_dial(fp);

    draw_clock_state(fp, hour, minute, second);

    fprintf(fp, "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"black\" fill=\"black\" />\n",
            ORIGIN_X, ORIGIN_Y, 4.0); // draw outer circle

    fprintf(fp, "</svg>\n"); // end

    fclose(fp);

    return 0;
}
