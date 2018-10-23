
namespace iwmath {
	template<typename ResultT, typename NumberTs>
	ResultT sum(NumberTs numbers...) {
		ResultT result = 0;
		for (auto number : numbers) {
			result += number;
		}

		return result;
	}
}