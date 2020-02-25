#include <stdio.h>
#include <stdlib.h>

int	usage(void);
int	get_max_brightness(char *path);
void	get_brightness(char *path, char *max_path);
void	set_brightness(char *path, char *brightness);
void	set_percentage(char *path, char *percentage);
void	inc_percentage(char *path, char *percentage);
void	dec_percentage(char *path, char *percentage);


int main(int argc, char *argv[]) {
	char *disp_path = "/sys/class/backlight/intel_backlight/brightness";
	char *kbd_path = "/sys/class/leds/smc::kbd_backlight/brightness";
	char *disp_max = "/sys/class/backlight/intel_backlight/max_brightness";
	char *kbd_max = "/sys/class/leds/smc::kbd_backlight/max_brightness";	
	char *path;
	char *max;

	if (argc < 2)
		return usage();

	if (argv[1][0] == 'k') {
		path = kbd_path;
		max = kbd_max;
		//printf("Keyboard Backlight - ");
	}
	else if (argv[1][0] == 'd') {
		path = disp_path;
		max = disp_max;
		//printf("Display Backlight - ");
	}
	else
		return usage();
	
	if (argc < 3) {
		get_brightness(path, max);
		return (0);
	}

	switch (argv[2][0]) {
		case 'g':
			get_brightness(path, max);
			break;
		case 's':
			set_brightness(path, argv[3]);
			break;
		default:
			usage();
	}

	return (0);
}

int get_max_brightness(char *path) {
	FILE *f;
	char buff[255];
	int max = 0;

	f = fopen(path, "r");
	fgets(buff, 255, f);
	fclose(f);
	max = atoi(buff);
	return (max);
}

void get_brightness(char *path, char *max_path) {
	FILE *f;
	char buff[255];
	int max = get_max_brightness(max_path);
	int brightness;
	float percentage;

	f = fopen(path, "r");
	fgets(buff, 255, f);
	fclose(f);

	brightness = atoi(buff);
	percentage = (float)brightness / max * 100;

	printf("%i/%i - %.0f%%\n", brightness, max, percentage);
}

void set_brightness(char *path, char *brightness) {
	FILE *f;

	f = fopen(path, "w");
	fputs(brightness, f);
	fclose(f);
}

/*void set_percentage(char *path, char *percentage) {
	FILE *f;

	f 
}*/

int usage(void) {
	printf(
"Usage: maglight [device] [command] <arguments>\n\
\n\
Devices:\n\
- [k]eyboard\n\
- [d]isplay\n\
\n\
Commands:\n\
- [g]et\t\t\tGet a device's brightness level\n\
- [s]et <num>\t\tSet a device's brightness level\n\
- [p]ercent <num>\tSet a device's brightness to percentage\n\
- [i]nc <per>\t\tIncrease a device's brightness level by percentage\n\
- [d]ec <per>\t\tDecrease a device's brightness level by percentage\n"
	);
	return (0);
}
