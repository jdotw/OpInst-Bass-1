/*
 * sd.c
 *
 *  Created on: 5 Oct. 2021
 *      Author: jwilson
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "fatfs.h"
#include "cJSON.h"

void sd_init()
{
}

char *_sd_test_json(void)
{
  const unsigned int resolution_numbers[3][2] = {
      {1280, 720},
      {1920, 1080},
      {3840, 2160}};
  char *string = NULL;
  cJSON *resolutions = NULL;
  size_t index = 0;

  cJSON *monitor = cJSON_CreateObject();

  if (cJSON_AddStringToObject(monitor, "name", "Awesome 4K") == NULL)
  {
    goto end;
  }

  resolutions = cJSON_AddArrayToObject(monitor, "resolutions");
  if (resolutions == NULL)
  {
    goto end;
  }

  for (index = 0; index < (sizeof(resolution_numbers) / (2 * sizeof(int))); ++index)
  {
    cJSON *resolution = cJSON_CreateObject();

    if (cJSON_AddNumberToObject(resolution, "width", resolution_numbers[index][0]) == NULL)
    {
      goto end;
    }

    if (cJSON_AddNumberToObject(resolution, "height", resolution_numbers[index][1]) == NULL)
    {
      goto end;
    }

    cJSON_AddItemToArray(resolutions, resolution);
  }

  string = cJSON_Print(monitor);
  if (string == NULL)
  {
    fprintf(stderr, "Failed to print monitor.\n");
  }

end:
  cJSON_Delete(monitor);
  return string;
}

void _sd_test_json_parse(char *buf)
{
  const char *parse_end = 0;
  cJSON *parsed = cJSON_ParseWithOpts(buf, &parse_end, true);
  if (!parsed)
  {
    Error_Handler();
  }
}

bool sd_mkdir(char *path)
{
  if (f_mount(&SDFatFS, (TCHAR const *)SDPath, 0) == FR_OK)
  {
    f_mkdir(path);
  }
  return false;
}

bool sd_write(char *filename, char *content, uint32_t content_len)
{
  uint32_t wbytes;
  if (f_mount(&SDFatFS, (TCHAR const *)SDPath, 0) == FR_OK)
  {
    if (f_open(&SDFile, filename, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
    {
      if (f_write(&SDFile, content, content_len, (void *)&wbytes) == FR_OK)
      {
        f_close(&SDFile);
        return true;
      }
    }
  }
  return false;
}

bool sd_read(char *filename, char *buf, size_t len, size_t *num_read)
{
  if (f_mount(&SDFatFS, (TCHAR const *)SDPath, 0) == FR_OK)
  {
    if (f_open(&SDFile, filename, FA_READ) == FR_OK)
    {
      if (f_read(&SDFile, buf, len, num_read) == FR_OK)
      {
        f_close(&SDFile);
        return true;
      }
    }
  }
  return false;
}

void sd_test(void)
{
  uint32_t wbytes; /* File write counts */
  char *wtext = _sd_test_json();
  uint32_t wtext_len = strlen((const char *)wtext);
  if (f_mount(&SDFatFS, (TCHAR const *)SDPath, 0) == FR_OK)
  {
    if (f_open(&SDFile, "BASS-1.TXT", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
    {
      if (f_write(&SDFile, wtext, wtext_len, (void *)&wbytes) == FR_OK)
      {
        f_close(&SDFile);
      }
    }
    //    if(f_open(&SDFile, "BASS-1.TXT", FA_READ) == FR_OK) {
    //      uint8_t buf[1024] = {0};
    //      unsigned int num_read = 0;
    //      if (f_read(&SDFile, buf, 1023, &num_read) == FR_OK) {
    //        _sd_test_json_parse(buf);
    //      }
    //      f_close(&SDFile);
    //
    //    }
  }
}
