template <>
bool checkCollision<Sphere, Sphere>(Broadphase::PotentialContact a)
{
	// returns true if the spheres in `a` are colliding. Otherwise
	// returns false. Also writes the data to the public collision array.

	auto sphere1 = a.rb1;
	auto sphere2 = a.rb2;
	glm::vec3 centre1 = sphere1.getCentroid();
	glm::vec3 centre2 = sphere2.getCentroid();
	float radius1 = sphere1.getCollider()->getRadius();
	float radius2 = sphere2.getCollider()->getRadius();

	return (glm::distance(centre1, centre2) <= radius1 + radius2);
}

template <>
bool checkCollision<Box, Sphere>(Broadphase::PotentialContact a)
{
	// PotentialContact contains two pointers to rigid bodies.
	// For BoxSphere, the first is a box, the second a sphere.
	// Returns true if the box and the sphere in `a` are colliding.
	// Otherwise returns false. Also writes the data to the public collision array.
}

template <>
bool checkCollision<Sphere, Box>(Broadphase::PotentialContact a)
{
	return checkCollision<Box, Sphere>(Broadphase::PotentialContact(a.second, a.first));
}

template <>
bool checkCollision<Box, Box>(Broadphase::PotentialContact a)
{
	// returns true if boxes the in `a` are colliding. Otherwise
	// returns false. Also writes the data to the public collision array.
}
