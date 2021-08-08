#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H

#include "ozz/animation/runtime/animation.h"
#include "ozz/animation/runtime/local_to_model_job.h"
#include "ozz/animation/runtime/sampling_job.h"
#include "ozz/animation/runtime/skeleton.h"
#include "ozz/base/log.h"
#include "ozz/base/maths/simd_math.h"
#include "ozz/base/maths/soa_transform.h"
#include "ozz/base/maths/vec_float.h"
#include "ozz/options/options.h"

namespace engine
{

	void testOzz();

	class Animator
	{
		friend void testOzz();
	public:

	private:
		ozz::animation::Skeleton m_skeleton;
	};
}

#endif // !ANIMATEDMODEL_H
