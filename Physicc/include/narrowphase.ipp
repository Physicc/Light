template <>
Contact checkCollision<Sphere, Sphere>(Broadphase::PotentialContact a)
{
	// returns true if the spheres in `a` are colliding. Otherwise
	// returns false. Also writes the data to the public collision array.

	auto sphere1 = a.rb1;
	auto sphere2 = a.rb2;
	glm::vec3 centre1 = sphere1.getCentroid();
	glm::vec3 centre2 = sphere2.getCentroid();
	float radius1 = sphere1.getCollider()->getRadius();
	float radius2 = sphere2.getCollider()->getRadius();

	glm::vec3 centreLine = centre1 - centre2;
	float centreLineLength = centreLine.magnitude();

	if (centreLineLength <= radius1 + radius2)
	{	
		return Contact(rb1,
						rb2,
						(centre1+centre2)/2, // contact point
						centreLine/centreLineLength, // contact normal
						radius1 + radius2 - centreLineLength // penetration
						);
	}


}

template <>
Contact checkCollision<Box, Sphere>(Broadphase::PotentialContact a)
{
	// PotentialContact contains two pointers to rigid bodies.
	// For BoxSphere, the first is a box, the second a sphere.
	// Returns true if the box and the sphere in `a` are colliding.
	// Otherwise returns false. Also writes the data to the public collision array.
	
	auto box = a.rb1;
	auto sphere = a.rb2;

	glm::vec3 sphereCentre = sphere.getCentroid();
	
	glm::vec3 relativeSphereCentre = box.getCollider()->toBoxCoordinates(sphereCentre);

	glm::vec3 closestPt; // 
	
	// Clamp the transformed coordinates by half-edges of the box
	float dist = relativeSphereCentre.x;
	if (dist > 0.5) dist = 0.5;
	if (dist < -0.5) dist = -0.5;
	closestPt.x = dist;

	dist = relativeSphereCentre.y;
	if (dist > 0.5) dist = 0.5;
	if (dist < -0.5) dist = -0.5;
	closestPt.y = dist;

	dist = relativeSphereCentre.z;
	if (dist > 0.5) dist = 0.5;
	if (dist < -0.5) dist = -0.5;
	closestPt.z = dist;
	
	// Check to see if we’re in contact.
	dist = (closestPt - relativeSphereCentre).squareMagnitude();
	return (dist > sphere.radius * sphere.radius);

}

template <>
Contact checkCollision<Sphere, Box>(Broadphase::PotentialContact a)
{
	return checkCollision<Box, Sphere>(a);
}

template <>
Contact checkCollision<Box, Box>(Broadphase::PotentialContact a)
{
	// returns true if boxes the in `a` are colliding. Otherwise
	// returns false. Also writes the data to the public collision array.
}
