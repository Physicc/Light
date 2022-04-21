#include "gtest/gtest.h"

#include "light.hpp"

RESOURCE_CLASS(CustomResource)
{
public:
	CustomResource(int id) : m_value(id) {}
	int getValue() const { return m_value; }
private:
	int m_value;
};

RESOURCE_CLASS(CustomResource2)
{
public:
	CustomResource2(int id) : m_value(id) {}
	int getValue() const { return m_value; }
private:
	int m_value;
};

TEST(Resource, ResourceTypeDerived)
{
	CustomResource resource(1), resource2(2);

	CustomResource2 resource3(3), resource4(4);

	EXPECT_EQ(resource.getType(), resource2.getType());
	EXPECT_EQ(resource3.getType(), resource4.getType());
	EXPECT_NE(resource.getType(), resource3.getType());
}

TEST(Resource, ResourceTypeBase)
{
	std::array<Light::ResourceBase*, 4> resources = {
		new CustomResource(1),
		new CustomResource2(2),
		new CustomResource(3),
		new CustomResource2(4)
	};

	EXPECT_EQ(resources[0]->getType(), resources[2]->getType());
	EXPECT_EQ(resources[1]->getType(), resources[3]->getType());
	EXPECT_NE(resources[0]->getType(), resources[1]->getType());
}

TEST(Resource, ResourceCacheFetch)
{
	Light::ResourceCache<CustomResource> cache;

	int id = 1;
	cache.get("myResource", [=]() { return std::make_unique<CustomResource>(id); });

	auto res = cache.get("myResource");

	EXPECT_EQ(1, res->getValue());
}

TEST(Resource, ResourceCacheCaching)
{
	Light::ResourceCache<CustomResource> cache;

	auto res = cache.get("myResource", [](int id) { return std::make_unique<CustomResource>(id); }, 1);

	auto res2 = cache.get("myResource");

	EXPECT_EQ(res.get(), res2.get());
}

TEST(Resource, ResourceCacheResourceNotFound)
{
	Light::ResourceCache<CustomResource> cache;

	auto res = cache.get("myResource");

	EXPECT_EQ(nullptr, res);
}

TEST(Resource, ResourceManagerFetch)
{
	Light::ResourceManager manager;

	manager.get<CustomResource>("myResource", [](int id) { return std::make_unique<CustomResource>(id); }, 1);
	manager.get<CustomResource2>("myResource2", [](int id) { return std::make_unique<CustomResource2>(id); }, 2);

	auto res = manager.get<CustomResource>("myResource");
	auto res2 = manager.get<CustomResource2>("myResource2");

	EXPECT_EQ(1, res->getValue());
	EXPECT_EQ(2, res2->getValue());
}
