#include "Helpers/Type/Matchers/container.hpp"
#include <IR/ir.hpp>
#include <ctre.hpp>
#include <optional>
#include <string_view>

namespace Helpers::Type::Matchers {

static constexpr auto vectorPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::vector<.*?>"};

constexpr auto matchVector(std::string_view sv) noexcept {
	return ctre::match<vectorPattern>(sv);
}

static constexpr auto stackPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::stack<.*?>"};

constexpr auto matchStack(std::string_view sv) noexcept {
	return ctre::match<stackPattern>(sv);
}

static constexpr auto queuePattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::queue<.*?>"};

constexpr auto matchQueue(std::string_view sv) noexcept {
	return ctre::match<queuePattern>(sv);
}

static constexpr auto priorityQueuePattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::priority_queue<.*?>"};

constexpr auto matchPriorityQueue(std::string_view sv) noexcept {
	return ctre::match<priorityQueuePattern>(sv);
}

static constexpr auto dequePattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::deque<.*?>"};

constexpr auto matchDeque(std::string_view sv) noexcept {
	return ctre::match<dequePattern>(sv);
}

static constexpr auto listPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::list<.*?>"};

constexpr auto matchList(std::string_view sv) noexcept {
	return ctre::match<listPattern>(sv);
}

static constexpr auto forwardListPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::forward_list<.*?>"};

constexpr auto matchForwardList(std::string_view sv) noexcept {
	return ctre::match<forwardListPattern>(sv);
}

static constexpr auto arrayPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::array<.*?>"};

constexpr auto matchArray(std::string_view sv) noexcept {
	return ctre::match<arrayPattern>(sv);
}

static constexpr auto mapPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::map<.*?>"};

constexpr auto matchMap(std::string_view sv) noexcept {
	return ctre::match<mapPattern>(sv);
}

static constexpr auto unorderedMapPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::unordered_map<.*?>"};

constexpr auto matchUnorderedMap(std::string_view sv) noexcept {
	return ctre::match<unorderedMapPattern>(sv);
}

static constexpr auto unorderedMultimapPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::unordered_multimap<.*?>"};

constexpr auto matchUnorderedMultimap(std::string_view sv) noexcept {
	return ctre::match<unorderedMultimapPattern>(sv);
}

static constexpr auto multimapPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::multimap<.*?>"};

constexpr auto matchMultimap(std::string_view sv) noexcept {
	return ctre::match<multimapPattern>(sv);
}

static constexpr auto multisetPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::multiset<.*?>"};

constexpr auto matchMultiset(std::string_view sv) noexcept {
	return ctre::match<multisetPattern>(sv);
}

static constexpr auto setPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::set<.*?>"};

constexpr auto matchSet(std::string_view sv) noexcept {
	return ctre::match<setPattern>(sv);
}

static constexpr auto unorderedSetPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::unordered_set<.*?>"};

constexpr auto matchUnorderedSet(std::string_view sv) noexcept {
	return ctre::match<unorderedSetPattern>(sv);
}

static constexpr auto unorderedMultisetPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::unordered_multiset<.*?>"};

constexpr auto matchUnorderedMultiset(std::string_view sv) noexcept {
	return ctre::match<unorderedMultisetPattern>(sv);
}

static constexpr auto optionalPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::optional<.*?>"};

constexpr auto matchOptional(std::string_view sv) noexcept {
	return ctre::match<optionalPattern>(sv);
}

static constexpr auto variantPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::variant<.*?>"};

constexpr auto matchVariant(std::string_view sv) noexcept {
	return ctre::match<variantPattern>(sv);
}

static constexpr auto allocatorPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::allocator<.*?>"};

constexpr auto matchAllocator(std::string_view sv) noexcept {
	return ctre::match<allocatorPattern>(sv);
}

static constexpr auto pairPattern =
    ctll::fixed_string {"struct std(::__1)?::pair<.*?>"};

constexpr auto matchPair(std::string_view sv) noexcept {
	return ctre::match<pairPattern>(sv);
}

static constexpr auto tuplePattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::tuple<.*?>"};

constexpr auto matchTuple(std::string_view sv) noexcept {
	return ctre::match<tuplePattern>(sv);
}

static constexpr auto greaterPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::greater<.*?>"};

constexpr auto matchGreater(std::string_view sv) noexcept {
	return ctre::match<greaterPattern>(sv);
}

static constexpr auto lessPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::less<.*?>"};

constexpr auto matchLess(std::string_view sv) noexcept {
	return ctre::match<lessPattern>(sv);
}

static constexpr auto hashPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::hash<.*?>"};

constexpr auto matchHash(std::string_view sv) noexcept {
	return ctre::match<hashPattern>(sv);
}

static constexpr auto equalToPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::equal_to<.*?>"};

constexpr auto matchEqualTo(std::string_view sv) noexcept {
	return ctre::match<equalToPattern>(sv);
}

static constexpr auto sharedPtrPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::shared_ptr<.*?>"};

constexpr auto matchSharedPtr(std::string_view sv) noexcept {
	return ctre::match<sharedPtrPattern>(sv);
}

static constexpr auto uniquePtrPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::unique_ptr<.*?>"};

constexpr auto matchUniquePtr(std::string_view sv) noexcept {
	return ctre::match<uniquePtrPattern>(sv);
}

static constexpr auto valarrayPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::valarray<.*?>"};

constexpr auto matchValarray(std::string_view sv) noexcept {
	return ctre::match<valarrayPattern>(sv);
}

std::optional<IR::ContainerType> getContainerType(std::string_view type) {
	using IR::ContainerType;
	if (matchVector(type)) {
		return ContainerType::Vector;
	} else if (matchAllocator(type)) {
		return ContainerType::Allocator;
	} else if (matchArray(type)) {
		return ContainerType::Array;
	} else if (matchGreater(type)) {
		return ContainerType::Greater;
	} else if (matchDeque(type)) {
		return ContainerType::Deque;
	} else if (matchLess(type)) {
		return ContainerType::Less;
	} else if (matchSet(type)) {
		return ContainerType::Set;
	} else if (matchMap(type)) {
		return ContainerType::Map;
	} else if (matchPair(type)) {
		return ContainerType::Pair;
	} else if (matchEqualTo(type)) {
		return ContainerType::EqualTo;
	} else if (matchQueue(type)) {
		return ContainerType::Queue;
	} else if (matchUniquePtr(type)) {
		return ContainerType::UniquePtr;
	} else if (matchSharedPtr(type)) {
		return ContainerType::SharedPtr;
	} else if (matchOptional(type)) {
		return ContainerType::Optional;
	} else if (matchVariant(type)) {
		return ContainerType::Variant;
	} else if (matchStack(type)) {
		return ContainerType::Stack;
	} else if (matchHash(type)) {
		return ContainerType::Hash;
	} else if (matchUnorderedMap(type)) {
		return ContainerType::UnorderedMap;
	} else if (matchUnorderedSet(type)) {
		return ContainerType::UnorderedSet;
	} else if (matchTuple(type)) {
		return ContainerType::Tuple;
	} else if (matchMultiset(type)) {
		return ContainerType::MultiSet;
	} else if (matchMultimap(type)) {
		return ContainerType::MultiMap;
	} else if (matchList(type)) {
		return ContainerType::List;
	} else if (matchPriorityQueue(type)) {
		return ContainerType::PriorityQueue;
	} else if (matchUnorderedMultimap(type)) {
		return ContainerType::UnorderedMultiMap;
	} else if (matchUnorderedMultiset(type)) {
		return ContainerType::UnorderedMultiSet;
	} else if (matchForwardList(type)) {
		return ContainerType::ForwardList;
	} else if (matchValarray(type)) {
		return ContainerType::Valarray;
	}
	return {};
}
}    // namespace Helpers::Type::Matchers
