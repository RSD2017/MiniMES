
SET(SOURCE_CPP
	main.cpp 
	Order.cpp
	OrderManager.cpp
	PartManager.cpp
	SocketCommunicator.cpp
	)
	
set(SOURCE_HPP
	Order.hpp
	OrderManager.hpp
	PartManager.hpp
	SocketCommunicator.hpp
	)

ADD_EXECUTABLE( MiniMES ${SOURCE_CPP} ${SOURCE_HPP})
MESSAGE("BOOST:"${Boost_LIBRARIES})
target_link_libraries(MiniMES ${Boost_LIBRARIES})
