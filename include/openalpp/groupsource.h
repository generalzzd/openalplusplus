/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Ume� University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
 *              CommonC++ (http://cplusplus.sourceforge.net/)
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

#ifndef GROUPSOURCE_H_INCLUDED_C427B440
#define GROUPSOURCE_H_INCLUDED_C427B440

#include "openalpp/source.h"
#include "openalpp/audioconvert.h"
#include <vector>
#include <math.h>
#include <stdlib.h>

namespace openalpp {

/**
 * Class for group sources.
 * Used for mixing together several sources _before_ they are played. This can
 * be used to play more sounds with less processing power. Of course the
 * problem is that the sources cannot be moved separately.
 */
class GroupSource : public SourceBase {
  /**
   * Flag for whether the sources have been mixed yet.
   */
  bool mixed_;

  /**
   * Vector containing the sources in the group.
   */
  std::vector<Source *> sources_;

  /**
   * OpenAL buffer for the group source.
   */
  ALuint buffer_;

  typedef enum Speaker {Left,Right};

  /**
   * Handles distance attenuation and directional sources.
   * @param source is the source to filter.
   * @return the gain.
   */
  ALfloat GroupSource::FilterDistance(ALuint source,Speaker speaker);

  /**
   * Reverb filter.
   * @param source is (a pointer to) the source.
   * @param buffer is (a pointer to) the buffer (=memory area).
   * @param size is the size of the buffer.
   * @param frequency is the freguency of the sound.
   * @return new pointer to buffer.
   */
  ALshort *GroupSource::FilterReverb(Source *source,ALshort *buffer,
				     ALsizei &size,unsigned int frequency);

  /**
   * Apply filters.
   * @param source is (a pointer to) the source.
   * @param buffer is the buffer containing the sound.
   * @param size is the size of the buffer.
   * @param frequency is the frequency of the sound.
   * @return (new) pointer to buffer.
   */
  ALshort *ApplyFilters(Source *source,ALshort *buffer,ALsizei &size,
			unsigned int frequency);
 public:
  /**
   * Constructor.
   * Creates the group source at the specified position.
   * @param x x coordinate.
   * @param y y coordinate.
   * @param z z coordinate.
   */
  GroupSource(float x = 0.0, float y = 0.0, float z = 0.0) throw (NameError);

  /**
   * Same as SourceBase::Play, except that this mixes the sources in the
   * group if it haven't been done yet.
   */
  void Play() throw (InitError,FileError);

  /**
   * Mix all added sources into one. This function will be called by
   * Play(), if sources have been included since the last time MixSamples()
   * was called, so if you want the source to start playing as fast as
   * possible after the Play()-call, MixSources() should be called
   * separately
   * @param frequency is the frequency that will be used when mixing.
   */
  void MixSources(unsigned int frequency=22050) 
    throw (InitError,FileError,FatalError,ValueError,MemoryError);

  /**
   * Includes a source in the group.
   * Returns an identifier that can be used as an argument to ExcludeSource(). 
   * This identifier is also the OpenAL name for the included source.
   * @param source is (a pointer to) the source to include.
   * @return identifier for the source.
   */
  ALuint IncludeSource(Source *source) throw (ValueError);

  /**
   * Removes a source from the group.
   * This will of course require the remaining sources to be mixed again.
   * @param source is the source to exclude.
   */
  void ExcludeSource(const Source &source) throw (NameError);

  /**
   * Removes a source from the group.
   * This will of course require the remaining sources to be mixed again.
   * @param source is the identifier of the source to exclude.
   */
  void ExcludeSource(ALuint source) throw (NameError);

  /**
   * Copy constructor.
   */
  GroupSource(const GroupSource &groupsource);

  /**
   * Destructor.
   */
  ~GroupSource();

  /**
   * Assignment operator.
   */
  GroupSource &operator=(const GroupSource &groupsource);
};

}

#endif /* GROUPSOURCE_H_INCLUDED_C427B440 */