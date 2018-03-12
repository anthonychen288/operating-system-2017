LIBS := $(shell pkg-config --libs opencv)

CXX := mpic++
CXX_FLAGS := -Wall -Wextra -pedantic -std=c++11 -O2 -flto

TST := test
ROTA := rotate
NEGA := negative
BLR := blur
CR := crotate
CN := cnegative
CB := cblur

all:
	$(CXX) $(CXX_FLAGS) $(TST).cpp $(LIBS) -o $(TST)
	$(CXX) $(CXX_FLAGS) $(ROTA).cpp $(LIBS) -o $(ROTA)
	$(CXX) $(CXX_FLAGS) $(NEGA).cpp $(LIBS) -o $(NEGA)
	$(CXX) $(CXX_FLAGS) $(BLR).cpp $(LIBS) -o $(BLR)
	$(CXX) $(CXX_FLAGS) $(CR).cpp $(LIBS) -o $(CR)
	$(CXX) $(CXX_FLAGS) $(CN).cpp $(LIBS) -o $(CN)
	$(CXX) $(CXX_FLAGS) $(CB).cpp $(LIBS) -o $(CB)
clean:
	rm -f *blur *rotate *negative
