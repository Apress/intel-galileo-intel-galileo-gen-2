#!/usr/bin/env python
# Software License Agreement (BSD License)
#
# Copyright (c) 2012, Philipp Wagner
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of the author nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Manoel Ramon 06/11/2014- changed the code to support images used 
#                          as example of emotion classification
#
import sys, math, Image

def Distance(p1,p2):
  dx = p2[0] - p1[0]
  dy = p2[1] - p1[1]
  return math.sqrt(dx*dx+dy*dy)

def ScaleRotateTranslate(image, angle, center = None, new_center = None, scale = None, resample=Image.BICUBIC):
  if (scale is None) and (center is None):
    return image.rotate(angle=angle, resample=resample)
  nx,ny = x,y = center
  sx=sy=1.0
  if new_center:
    (nx,ny) = new_center
  if scale:
    (sx,sy) = (scale, scale)
  cosine = math.cos(angle)
  sine = math.sin(angle)
  a = cosine/sx
  b = sine/sx
  c = x-nx*a-ny*b
  d = -sine/sy
  e = cosine/sy
  f = y-nx*d-ny*e
  return image.transform(image.size, Image.AFFINE, (a,b,c,d,e,f), resample=resample)

def CropFace(image, eye_left=(0,0), eye_right=(0,0), offset_pct=(0.2,0.2), dest_sz = (70,70)):
  # calculate offsets in original image
  offset_h = math.floor(float(offset_pct[0])*dest_sz[0])
  offset_v = math.floor(float(offset_pct[1])*dest_sz[1])
  # get the direction
  eye_direction = (eye_right[0] - eye_left[0], eye_right[1] - eye_left[1])
  # calc rotation angle in radians
  rotation = -math.atan2(float(eye_direction[1]),float(eye_direction[0]))
  # distance between them
  dist = Distance(eye_left, eye_right)
  # calculate the reference eye-width
  reference = dest_sz[0] - 2.0*offset_h
  # scale factor
  scale = float(dist)/float(reference)
  # rotate original around the left eye
  image = ScaleRotateTranslate(image, center=eye_left, angle=rotation)
  # crop the rotated image
  crop_xy = (eye_left[0] - scale*offset_h, eye_left[1] - scale*offset_v)
  crop_size = (dest_sz[0]*scale, dest_sz[1]*scale)
  image = image.crop((int(crop_xy[0]), int(crop_xy[1]), int(crop_xy[0]+crop_size[0]), int(crop_xy[1]+crop_size[1])))
  # resize it
  image = image.resize(dest_sz, Image.ANTIALIAS)
  return image

if __name__ == "__main__":

#Serious_01.jpg
#left  -> 528, 423
#right -> 770, 431

  image =  Image.open("serious_01.jpg")
  CropFace(image, eye_left=(528,423), eye_right=(770,431), offset_pct=(0.2,0.2)).save("serious01_20_20_70_70.jpg")


#Serious_02.jpg
#left  -> 522,412
#right -> 758, 415

  image =  Image.open("serious_02.jpg")
  CropFace(image, eye_left=(522,412), eye_right=(758,415), offset_pct=(0.2,0.2)).save("serious02_20_20_70_70.jpg")


#Serious_03.jpg
#left  -> 518, 423
#right -> 754, 425

  image =  Image.open("serious_03.jpg")
  CropFace(image, eye_left=(518,423), eye_right=(754,425), offset_pct=(0.2,0.2)).save("serious03_20_20_70_70.jpg")

#Smile_01.jpg
#left  -> 516, 377
#right -> 753, 379

  image =  Image.open("smile_01.jpg")
  CropFace(image, eye_left=(516,377), eye_right=(753,379), offset_pct=(0.2,0.2)).save("smile01_20_20_70_70.jpg")


#Smile_02.jpg
#left  -> 533, 374
#right -> 763, 380

  image =  Image.open("smile_02.jpg")
  CropFace(image, eye_left=(533,374), eye_right=(763,380), offset_pct=(0.2,0.2)).save("smile02_20_20_70_70.jpg")


#Smile_03.jpg
#left  -> 518, 379
#right -> 749, 381

  image =  Image.open("smile_03.jpg")
  CropFace(image, eye_left=(518,379), eye_right=(749,381), offset_pct=(0.2,0.2)).save("smile03_20_20_70_70.jpg")

#surprised_01.jpg
#left  -> 516,356
#right -> 754,355

  image =  Image.open("surprised_01.jpg")
  CropFace(image, eye_left=(516,356), eye_right=(754,355), offset_pct=(0.2,0.2)).save("surprised01_20_20_70_70.jpg")


#surprised_02.jpg
#left  -> 548, 364
#right -> 793, 364

  image =  Image.open("surprised_02.jpg")
  CropFace(image, eye_left=(548,364), eye_right=(793,364), offset_pct=(0.2,0.2)).save("surprised02_20_20_70_70.jpg")

#surprised_03.jpg
#left  -> 528, 377
#right -> 770, 378

  image =  Image.open("surprised_03.jpg")
  CropFace(image, eye_left=(528,377), eye_right=(770,378), offset_pct=(0.2,0.2)).save("surprised03_20_20_70_70.jpg")


