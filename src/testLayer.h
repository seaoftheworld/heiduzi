#include "04_api/layers.h"  // Baseclass' header file

// #include "room.h"             // Game logic
// using namespace SmallHazel;

class test_gameLayer : public gameLayer {
public:
    test_gameLayer() {}
    virtual ~test_gameLayer() {}

    void attach() override;
    void dettach() override;
    void update(float deltaTime, gameInputStatus input) override;
    void render() override;

private:
    // room *m_room = NULL;

    void attach00();
    void attach01();
    void attach02();

    void dettach02();
    void dettach01();
    void dettach00();
};
