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
	
	auto box = a.rb1;
	auto sphere = a.rb2;

	glm::vec3 s_centre = sphere.getCentroid();
	
	glm::vec3 rel_s_centre = box.getCollider()->toBoxCoordinates(s_centre);

	glm::vec3 closest_pt; // 
	
	// Clamp the transformed coordinates by half-edges of the box
	float dist = rel_s_centre.x;
	if (dist > 0.5) dist = 0.5;
	if (dist < -0.5) dist = -0.5;
	closest_pt.x = dist;

	dist = rel_s_centre.y;
	if (dist > 0.5) dist = 0.5;
	if (dist < -0.5) dist = -0.5;
	closest_pt.y = dist;

	dist = rel_s_centre.z;
	if (dist > 0.5) dist = 0.5;
	if (dist < -0.5) dist = -0.5;
	closest_pt.z = dist;
	
	// Check to see if we’re in contact.
	dist = (closestPt - rel_s_centre).squareMagnitude();
	return (dist > sphere.radius * sphere.radius);

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
