/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Ume� University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#include "openalpp/sounddata.h"

using namespace openalpp;

SoundData::SoundBuffer::SoundBuffer() throw (NameError) {
  alGenBuffers(1,&buffername_);
  if(alGetError()!=AL_FALSE)
    throw NameError("Error generating buffer name");
  refcount_=1;
}

SoundData::SoundBuffer::~SoundBuffer() {
  alDeleteBuffers(1,&buffername_);
}

SoundData::SoundBuffer *SoundData::SoundBuffer::reference() {
  refcount_++;
  return this;
}

void SoundData::SoundBuffer::deReference() throw (FatalError) {
  refcount_--;
  if(!refcount_)
    delete this;
  else if(refcount_<0)
    throw FatalError("SoundBuffer dereferenced too many times!");
}



SoundData::SoundData() throw (NameError,InitError) : AudioBase() {
  buffer_=new SoundBuffer();
  buffername_=buffer_->getName();
}

SoundData::SoundData(const SoundData &sounddata)
  : AudioBase(/*(const AudioBase &)sounddata*/) {
  buffer_=sounddata.buffer_->reference();
  buffername_=buffer_->getName();
}

SoundData::~SoundData() {
  buffer_->deReference();
}

ALuint SoundData::getAlBuffer() const {
  return buffername_;
}

SoundData &SoundData::operator=(const SoundData &sounddata) {
  if(this!=&sounddata) {
    buffer_->deReference();
    buffer_=sounddata.buffer_->reference();
    buffername_=buffer_->getName();
  }
  return *this;
}

