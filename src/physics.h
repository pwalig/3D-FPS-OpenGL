#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "rigidbody.h"
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
	struct collision_info;

	const glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
	extern std::vector<collider*> all_colliders;
	extern std::vector<rigidbody*> rigidbodies;

	struct collision_info {
		bool collision;
		glm::vec3 collision_point;
		glm::vec3 normal;
		collider* other;
		bool enter_stay; // enter -> true; stay -> false
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
		void collision_notify(physics::collision_info& ci); // call when collision has been detected
		void swap_collider_buffers(); // call after all collision checks

		virtual int get_type();
		virtual void adjust_position(const glm::vec3& collision_point);
	};
	namespace colliders {
		class aabb : public collider {
		public:
			glm::vec3& position;
			glm::vec3 size;
			aabb(physics::rigidbody* rb, const glm::vec3& size_);
			aabb(glm::vec3& position_, const glm::vec3& size_);

			int get_type() override;
			void adjust_position(const glm::vec3& collision_point) override;
		};

		class sphere : public collider {
		public:
			glm::vec3& position;
			float radius;
			sphere(physics::rigidbody* rb, const float& radius_);
			sphere(glm::vec3& position_, const float& radius_);

			int get_type() override;
			void adjust_position(const glm::vec3& collision_point) override;
		};

		class plane : public collider {
		public:
			glm::vec3& position;
			glm::quat& rotation;
			glm::vec2 size;
			plane(physics::rigidbody* rb, const glm::vec2& size_);
			plane(glm::vec3& position_, glm::quat& rotation_, const glm::vec2& size_);

			int get_type() override;
		};
	}

	void collide(rigidbody* rb, const physics::collision_info& ci);
	void collide(rigidbody* rb1, rigidbody* rb2, const physics::collision_info& ci);

	void run();

	template <typename T, typename U>
	void check_collision(collider* c1, collider* c2);

	collision_info get_collision_info(const colliders::aabb& a, const colliders::aabb& b);
	collision_info get_collision_info(const colliders::sphere& a, const colliders::sphere& b);
	collision_info get_collision_info(const colliders::plane& a, const colliders::plane& b);

	collision_info get_collision_info(const colliders::sphere& s, const colliders::plane& p);
	collision_info get_collision_info(const colliders::plane& p, const colliders::sphere& s);


	template<typename T, typename U>
	inline void check_collision(collider* c1, collider* c2)
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
					ci2.collision = ci1.collision;
					ci2.other = a;
					ci2.normal = -ci1.normal;
					c2->collision_notify(ci2);
					if (c2->rigidbody != nullptr && c2->rigidbody->dynamic) c2->adjust_position(ci2.collision_point);
					if (c1->rigidbody != nullptr) {
						if (c1->rigidbody->dynamic) c1->adjust_position(ci1.collision_point);
						if (c2->rigidbody != nullptr) physics::collide(c1->rigidbody, c2->rigidbody, ci1);
						else physics::collide(c1->rigidbody, ci1);
					}
					else if (c2->rigidbody != nullptr) physics::collide(c2->rigidbody, ci2);
				}
			}
			else printf("Could not cast c2 to apropriate collider type.\n");
		}
		else printf("Could not cast c1 to apropriate collider type.\n");
	}
}

