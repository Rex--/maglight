#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	usage(void);
char*	append_filename(char *path, char *filename);
int	get_max_brightness(char *path);
int	get_brightness(char *path);
void	print_brightness(char *path);
void	set_brightness(char *path, char *brightness);
void	set_percentage(char *path, char *percentage);
void	inc_percentage(char *path, char *percentage);
void	dec_percentage(char *path, char *percentage);


int main(int argc, char *argv[]) {
	char *disp_path = "/sys/class/backlight/intel_backlight/";
	char *kbd_path = "/sys/class/leds/smc::kbd_backlight/";
	char *path;

	if (argc < 2)
		return usage();

	if (argv[1][0] == 'k') {
		path = kbd_path;
		//printf("Keyboard Backlight - ");
	}
	else if (argv[1][0] == 'd') {
		path = disp_path;
		//printf("Display Backlight - ");
	}
	else
		return usage();
	
	if (argc < 3) {
		print_brightness(path);
		return (0);
	}

	switch (argv[2][0]) {
		case 'g':
			print_brightness(path);
			break;
		case 's':
			set_brightness(path, argv[3]);
			break;
		default:
			usage();
	}

	return (0);
}

char* append_filename(char *path, char *filename) {
	char *filepath = malloc(strlen(path) + strlen(filename) + 1);
	strcpy(filepath, path);
	return strcat(filepath, filename);
}

int get_max_brightness(char *path) {
	FILE *f;
	char buff[255];
	char *filepath;
	
	filepath = append_filename(path, "max_brightness");
	f = fopen(filepath, "r");
	fgets(buff, 255, f);
	fclose(f);

	return (atoi(buff));
}

int get_brightness(char *path) {
	FILE *f;
	char buff[255];
	char *filepath;
	
	filepath = append_filename(path, "brightness");
	f = fopen(filepath, "r");
	fgets(buff, 255, f);
	fclose(f);

	return (atoi(buff));
}

void print_brightness(char *path) {
	int brightness = get_brightness(path);
	int max_brightness = get_max_brightness(path);
	float percentage = (float)brightness / max_brightness * 100;

	printf("%i/%i - %.0f%%\n", brightness, max_brightness, percentage);
}

void set_brightness(char *path, char *brightness) {
	FILE *f;
	char *filepath;

	filepath = append_filename(path, "brightness");
	f = fopen(filepath, "w");
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
+ [p]ercent <num>\tSet a device's brightness to percentage\n\
+ [i]nc <per>\t\tIncrease a device's brightness level by percentage\n\
+ [d]ec <per>\t\tDecrease a device's brightness level by percentage\n"
	);
	return (0);
}
