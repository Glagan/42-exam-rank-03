ulong ft_strlen(long param_1)
{
  uint i;
  
  i = 0;
  while (*(char *)(param_1 + (int)i) != '\0') {
    i = i + 1;
  }
  return (ulong)i;
}



// rectangle: [x, y, width, height]

ulong is_in_rectangle(float x,float y,float *rectangle)
{
  uint ret;
  
  if ((((x < rectangle[1]) || (rectangle[1] + rectangle[3] < x)) || (y < rectangle[2])) ||
     (rectangle[2] + rectangle[4] < y)) {
    ret = 0;
  }
  else {
    if (((x - rectangle[1] < 1.00000000) || ((rectangle[1] + rectangle[3]) - x < 1.00000000)) ||
       ((y - rectangle[2] < 1.00000000 || ((rectangle[2] + rectangle[4]) - y < 1.00000000)))) {
      ret = 2;
    }
    else {
      ret = 1;
    }
  }
  return (ulong)ret;
}



ulong get_info(FILE *param_1,int *drawing)
{
  int iVar1;
  void *tmp;
  int i;
  undefined background;
  int *_drawing;
  FILE *file;
  uint ret;
  
  _drawing = drawing;
  file = param_1;
  iVar1 = fscanf(param_1,"%d %d %c\n",drawing,drawing + 1,&background);
  if (iVar1 == 3) {
    if ((((*_drawing < 1) || (300 < *_drawing)) || (_drawing[1] < 1)) || (300 < _drawing[1])) {
      ret = 1;
    }
    else {
      tmp = malloc((long)*_drawing * (long)_drawing[1]);
      *(void **)(_drawing + 2) = tmp;
      if (*(long *)(_drawing + 2) == 0) {
        ret = 1;
      }
      else {
        i = 0;
        while (i < *_drawing * _drawing[1]) {
          *(undefined *)(*(long *)(_drawing + 2) + (long)i) = background;
          i = i + 1;
        }
        ret = 0;
      }
    }
  }
  else {
    ret = 1;
  }
  return (ulong)ret;
}



void execute_one(float *shape,int *drawing,int x,int y)

{
  int is_in;
  
  is_in = is_in_rectangle((float)x,(float)y,shape);
  if ((is_in == 2) || ((is_in == 1 && (*(char *)shape == 'R')))) {
    *(undefined *)(*(long *)(drawing + 2) + (long)(x + y * *drawing)) = *(undefined *)(shape + 5);
  }
  return;
}



ulong apply_op(char *shape,int *drawing)

{
  uint j;
  uint i;
  uint ret;
  
  if (((*(float *)(shape + 0xc) <= 0.00000000) || (*(float *)(shape + 0x10) <= 0.00000000)) ||
     ((*shape != 'R' && (*shape != 'r')))) {
    ret = 1;
  }
  else {
    i = 0;
    while ((int)i < *drawing) {
      j = 0;
      while ((int)j < drawing[1]) {
        execute_one(shape,drawing,(ulong)i,(ulong)j);
        j = j + 1;
      }
      i = i + 1;
    }
    ret = 0;
  }
  return (ulong)ret;
}



ulong print_info(uint *param_1)

{
  uint i;
  
  i = 0;
  while ((int)i < (int)param_1[1]) {
    printf("%.*s\n",(ulong)*param_1,*(long *)(param_1 + 2) + (long)(int)(i * *param_1));
    i = i + 1;
  }
  return (ulong)i;
}



ulong execute(FILE *param_1)

{
  int valid_zone;
  int fscanf_ret;
  char type;
  float x;
  float y;
  float width;
  float height;
  char color;
  undefined auStack40 [16];
  FILE *file;
  uint ret;
  
  file = param_1;
  valid_zone = get_info(param_1,auStack40);
  if (valid_zone == 0) {
    fscanf_ret = fscanf(file,"%c %f %f %f %f %c\n",&type,&x,&y,&width,&height,&color);
    while (fscanf_ret == 6) {
      valid_zone = apply_op(&type,auStack40);
      if (valid_zone != 0) {
        ret = 1;
        goto LAB_100000e2a;
      }
      fscanf_ret = fscanf(file,"%c %f %f %f %f %c\n",&type,&x,&y,&width,&height,&color);
    }
    if (fscanf_ret == -1) {
      print_info(auStack40);
      ret = 0;
    }
    else {
      ret = 1;
    }
  }
  else {
    ret = 1;
  }
LAB_100000e2a:
  return (ulong)ret;
}



ulong main(int param_1,long param_2)

{
  int  i;
  FILE *file;
  uint local_c;
  
  local_c = 0;
  if (param_1 == 2) {
    file = fopen(*(char **)(param_2 + 8),"r");
    if (file != (FILE *)0x0) {
      i = execute(file);
      if (i == 0) goto LAB_100000ee9;
    }
    i = ft_strlen("Error: Operation file corrupted\n");
    write(1,"Error: Operation file corrupted\n",(long)i);
  }
  else {
    i = ft_strlen("Error: argument\n");
    write(1,"Error: argument\n",(long)i);
    local_c = 1;
  }
LAB_100000ee9:
  return (ulong)local_c;
}
