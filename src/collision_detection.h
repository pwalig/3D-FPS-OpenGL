#pragma once
#include "physics.h"
#include "event_subscription_list.h"

#define COLLIDERS_NONE 0
#define COLLIDERS_AABB 1
#define COLLIDERS_SPHERE 2
#define COLLIDERS_PLANE 3
#define COLLIDERS_CUBE 4
#define COLLIDERS_CAPSULE 5
#define COLLIDERS_AMOUNT 6

namespace physics {
	class collider;
	extern std::vector<collider*> all_colliders;

	struct collision_info {
		bool collision;
		glm::vec3 collision_point;
		glm::vec3 normal;
		collider* other;
	};


	class collider {
	private:
		std::vector<collider*> _collided_last_frame;
		std::vector<collider*> _collided_this_frame;
	public:
		physics::rigidbody* rigidbody;
		engine::event_subscription_list<physics::collision_info> on_collision_enter;
		engine::event_subscription_list<physics::collision_info> on_collision_stay;
		engine::event_subscription_list<> on_collision_exit;

		collider();
		collider(physics::rigidbody* const rb);
		void collision_notify(const physics::collision_info& ci); // call when collision has been detected
		void swap_collider_buffers(); // call after all collision checks

		virtual int get_type();
	};
	namespace colliders {
		class aabb : public collider {
		public:
			glm::vec3& position;
			glm::vec3 size;
			aabb(glm::vec3& position_, const glm::vec3& size_);

			int get_type() override;
		};

		class sphere : public collider {
		public:
			glm::vec3& position;
			float radius;
			sphere(glm::vec3& position_, const float& radius_);

			int get_type() override;
		};

		class plane : public collider {
		public:
			glm::vec3& position;
			glm::quat& rotation;
			glm::vec2 size;
			plane(glm::vec3& position_, glm::quat& rotation_, const glm::vec2& size_);

			int get_type() override;
		};
	}

	void detect_collisions();

	template <typename T, typename U>
	void check_collision(collider* c1, collider* c2);

	collision_info get_collision_info(const colliders::aabb& a, const colliders::aabb& b);
	collision_info get_collision_info(const colliders::sphere& a, const colliders::sphere& b);
	collision_info get_collision_info(const colliders::plane& a, const colliders::plane& b);

	collision_info get_collision_info(const colliders::sphere& s, const colliders::plane& p);
	collision_info get_collision_info(const colliders::plane& p, const colliders::sphere& s);


	template<typename T, typename U>
	void check_collision(collider* c1, collider* c2)
	{
		if (T* a = dynamic_cast<T*>(c1))
		{
			if (U* b = dynamic_cast<U*>(c2))
			{
				collision_info ci1 = get_collision_info(*a, *b);
				if (ci1.collision) {
					ci1.other = b;
					c1->collision_notify(ci1);
					collision_info ci2;
					ci2.other = a;
					ci2.normal = -ci1.normal;
					c2->collision_notify(ci2);
				}
			}
			else printf("Could not cast c2 to apropriate collider type.\n");
		}
		else printf("Could not cast c1 to apropriate collider type.\n");
	}
}