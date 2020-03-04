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


int main(int argc, char *argv[])
{
	char *disp_path = "/sys/class/backlight/intel_backlight/";
	char *kbd_path = "/sys/class/leds/smc::kbd_backlight/";
	char *path;

	if (argc < 2)
		return usage();

	if (argv[1][0] == 'k') {
		path = kbd_path;
	}
	else if (argv[1][0] == 'd') {
		path = disp_path;
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
		case 'p':
			set_percentage(path, argv[3]);
			break;
		case 'i':
			inc_percentage(path, argv[3]);
			break;
		case 'd':
			dec_percentage(path, argv[3]);
			break;
		default:
			usage();
	}

	return (0);
}

char* append_filename(char *path, char *filename)
{
	char *filepath = malloc(strlen(path) + strlen(filename) + 1);
	strcpy(filepath, path);
	return strcat(filepath, filename);
}

int get_max_brightness(char *path)
{
	char *filepath = append_filename(path, "max_brightness");
	FILE *f = fopen(filepath, "r");
	char buff[255];
	fgets(buff, 255, f);
	fclose(f);
	return (atoi(buff));
}

int get_brightness(char *path)
{
	char *filepath = append_filename(path, "brightness");
	FILE *f = fopen(filepath, "r");
	char buff[255];
	fgets(buff, 255, f);
	fclose(f);
	return (atoi(buff));
}

void print_brightness(char *path)
{
	int brightness = get_brightness(path);
	int max_brightness = get_max_brightness(path);
	float percentage = (float)brightness / max_brightness * 100;
	printf("%i/%i - %.0f%%\n", brightness, max_brightness, percentage);
}

void set_brightness(char *path, char *brightness)
{
	char *filepath = append_filename(path, "brightness");
	FILE *f = fopen(filepath, "w");
	fputs(brightness, f);
	fclose(f);
}

void set_percentage(char *path, char *percentage)
{
	int brightness = (atoi(percentage) * 0.01) * get_max_brightness(path);
	char number[10];
	sprintf(number, "%i", brightness);
	set_brightness(path, number);
}

void inc_percentage(char *path, char *percentage)
{
	int inc = (atoi(percentage) * 0.01) * get_max_brightness(path);
	int brightness = get_brightness(path) + inc;
	char number[10];
	sprintf(number, "%i", brightness);
	set_brightness(path, number);
}

void dec_percentage(char *path, char *percentage)
{
	int dec = (atoi(percentage) * 0.01) * get_max_brightness(path);
	int brightness = get_brightness(path) - dec;
	char number[10];
	sprintf(number, "%i", brightness);
	set_brightness(path, number);
}

int usage(void)
{
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
