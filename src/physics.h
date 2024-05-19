#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "rigidbody.h"
#include "event_subscription_list.h"
#include "game_script.h"

#define COLLIDERS_NONE 0
#define COLLIDERS_AABB 1
#define COLLIDERS_SPHERE 2
#define COLLIDERS_PLANE 3
#define COLLIDERS_CUBE 4
#define COLLIDERS_CAPSULE 5
#define COLLIDERS_AMOUNT 6

#define RAY_INTERSECT_NONE 0
#define RAY_INTERSECT_EXIT_ONLY 1
#define RAY_INTERSECT_ALL 2

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

	struct ray {
		glm::vec3 origin;
		glm::vec3 direction;

		ray(const glm::vec3& origin_, const glm::vec3& direction_);
	};

	struct ray_intersection_info {
		int intersect = RAY_INTERSECT_NONE;
		glm::vec3 enter;
		glm::vec3 exit;
		collider* col;
		float distance = 0.0f;
	};

	class collider {
	private:
		std::vector<collider*> _collided_last_frame;
		std::vector<collider*> _collided_this_frame;
	public:
		scripts_system::script* owner;
		physics::rigidbody* rigidbody;
		engine::event_subscription_list<physics::collision_info> on_collision_enter;
		engine::event_subscription_list<physics::collision_info> on_collision_stay;
		engine::event_subscription_list<> on_collision_exit;

		collider(scripts_system::script* const owner_ = nullptr);
		collider(physics::rigidbody* const rb, scripts_system::script* const owner_ = nullptr);
		void collision_notify(const physics::collision_info& ci); // call when collision has been detected
		bool in_collided_last_frame(const physics::collider* const col) const; // checks if col is in collided last frame
		void swap_collider_buffers(); // call after all collision checks

		virtual ray_intersection_info get_ray_intersection_info(const ray& r);
		virtual int get_type();
		virtual void adjust_position(const glm::vec3& collision_point);
		virtual ~collider();
	};
	namespace colliders {
		class aabb : public collider {
		public:
			glm::vec3& position;
			glm::vec3 size;
			aabb(physics::rigidbody* const rb, const glm::vec3& size_ = glm::vec3(1.0f));
			aabb(physics::rigidbody* const rb, scripts_system::script* const owner_, const glm::vec3& size_ = glm::vec3(1.0f));
			aabb(glm::vec3& position_, const glm::vec3& size_ = glm::vec3(1.0f));
			aabb(glm::vec3& position_, scripts_system::script* const owner_, const glm::vec3& size_ = glm::vec3(1.0f));

			ray_intersection_info get_ray_intersection_info(const ray& r) override;
			int get_type() override;
			void adjust_position(const glm::vec3& collision_point) override;
		};

		class sphere : public collider {
		public:
			glm::vec3& position;
			float radius;
			sphere(physics::rigidbody* rb, const float& radius_ = 1.0f);
			sphere(physics::rigidbody* rb, scripts_system::script* const owner_, const float& radius_ = 1.0f);
			sphere(glm::vec3& position_, const float& radius_ = 1.0f);
			sphere(glm::vec3& position_, scripts_system::script* const owner_, const float& radius_ = 1.0f);

			ray_intersection_info get_ray_intersection_info(const ray& r) override;
			int get_type() override;
			void adjust_position(const glm::vec3& collision_point) override;
		};

		class plane : public collider {
		public:
			glm::vec3& position;
			glm::quat& rotation;
			glm::vec3 size;
			plane(physics::rigidbody* rb, const glm::vec3& size_ = glm::vec3(1.0f)); // y component is thickness
			plane(physics::rigidbody* rb, scripts_system::script* const owner_, const glm::vec3& size_ = glm::vec3(1.0f)); // y component is thickness
			plane(glm::vec3& position_, glm::quat& rotation_, const glm::vec3& size_ = glm::vec3(1.0f)); // y component is thickness
			plane(glm::vec3& position_, scripts_system::script* const owner_, glm::quat& rotation_, const glm::vec3& size_ = glm::vec3(1.0f)); // y component is thickness

			ray_intersection_info get_ray_intersection_info(const ray& r) override;
			int get_type() override;
		};
	}

	void collide(rigidbody* rb1, rigidbody* rb2, const physics::collision_info& ci);

	void run();
	std::vector<ray_intersection_info> ray_cast_all(const ray& r, const bool& intersect_all_only = true, const bool& sort = true);
	ray_intersection_info ray_cast(const ray& r, const bool& intersect_all_only = true);

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
				// check for collision
				collision_info ci1 = get_collision_info(*a, *b);
				if (ci1.collision) {

					// fill collision info
					ci1.other = b;
					ci1.enter_stay = !c1->in_collided_last_frame(c2);
					collision_info ci2(ci1);
					ci2.other = a;
					ci2.normal = -ci1.normal;
					ci2.enter_stay = !c2->in_collided_last_frame(c1);

					// look for rigidbodies
					if (c1->rigidbody != nullptr && c2->rigidbody != nullptr) {
						c1->adjust_position(ci1.collision_point);
						c2->adjust_position(ci2.collision_point);
						physics::collide(c1->rigidbody, c2->rigidbody, ci1);
					}

					// call subscribed events
					c1->collision_notify(ci1);
					c2->collision_notify(ci2);
				}
			}
			else printf("Could not cast c2 to apropriate collider type.\n");
		}
		else printf("Could not cast c1 to apropriate collider type.\n");
	}
}

