/******************************************************************************/
/*!
    *This level contains basic features of this engine.
    How to control the main camera, how to set, control objects
    and basic procedure of this engine(init - update - close).

    *Instuctions
    1. Camera panning - Arrow keys
    2. Camera zooming - W(Zooming out), S(Zooming in)
    3. Camera rotating - Q(Counter-Clockwise), E(Clockwise)
    4. Set animation on specific frame - Number keys (1-6)
    5. Stop the animation- Space
    6. Rewind the animation - Back space
    7. Play the animation from start - ENTER
    8. Move to next state - C
*/
/******************************************************************************/

#include "CommonLevel.h"

//to check time
double timer = 0;
b2Vec2 playerVelocity(15, -19);
float distance_ObjectforRope = 650.0f;
float basicDistance_ObjectforRope = -70.0f;
b2Vec3 rope_position;  //animation.transform.position; //+ objectForRope->transform.position);



const  SDL_Color SKY_BLUE = { 64, 64, 255, 255 };
//float currentPlayerLoc;

void Level1::Initialize()
{
//    float Tile1Loc =-400;
  /*  float Tile2Loc;
    float Tile3Loc;
    float Tile4Loc;
    float Tile5Loc;*/
    m_backgroundColor = SKY_BLUE;

   /* m_backgroundColor.r = 255, m_backgroundColor.g = 64,
        m_backgroundColor.b = 0;*/
    // Set Camera
    // You don't need to do this anymore
    //camera.Initialize(int(State::m_width), int(State::m_height));
    // Set position of the camera
    camera.position.Set(0, 0, 0);

    // Set main font
    // Load the font file to use as a main font
    // and set the default size of it
    mainFont = TTF_OpenFont("font/Default.ttf", 48);

    
    //현재 진행되고 있는 레벨에서 모든물체에 중력을 적용 -> 즉 static이 아닌이상 밑으로 떨어질것임

    b2Vec2 gravity(0.f, -9.8f);
    SetPhysicsWorld(gravity);
    
    animation = new Object();
    animation->SetName("Walking boy");
    animation->transform.position.Set(-480.f, -70.f, 0.f);
    animation->transform.SetScale(90, 150);
    //animation.transform.rotation = 45.F;
    animation->sprite.LoadImage("texture/animation.png", State::m_renderer);
    // Set toggle true so animation activates 
    animation->sprite.activeAnimation = true;
    // Set the number of frames to update
    animation->sprite.SetFrame(8);
    // Set the speed of frame updating
    animation->sprite.SetSpeed(20.f);
    
    animation->physics.bodyType = Physics::DYNAMIC;
    // Set body shape
    // Set density
    animation->physics.SetDensity(10.F);
    // Set restitution -> 튕겨 나가는거
    //animation.physics.SetRestitution(.75f);

    // Generate rigidbody passing world and transform
    animation->physics.GenerateBody(GetPhysicsWorld(), &animation->transform);



    basicDistance_ObjectforRope = -70.0f;
    objectForRope = new Object();
    objectForRope->SetName("objectforRope");
    objectForRope->transform.position.Set(basicDistance_ObjectforRope, 120.0f, 0.0f);
    objectForRope->transform.SetScale(50, 50);
    objectForRope->sprite.LoadImage("texture/TouchPoint.png", m_renderer);
    objectForRope->physics.bodyType = Physics::STATIC;
    objectForRope->physics.GenerateBody(GetPhysicsWorld(), &objectForRope->transform);


    basicTiles();
    AddObject(animation);
    AddObject(objectForRope);
    AddObject(&newTilePoint);

}


void Level1::Update(float dt)
{
   // TransformObjects(dt);
    ControlCamera(dt);
    MakeNewTiles(dt);
    makeNewRope();


    if (rope_joint != nullptr)
        std::cout << "hi" << std::endl;
    
    // Move to Level2 by pressing 'C' key
   /* if (m_input->IsTriggered(SDL_SCANCODE_C))
        m_game->Change(LV_Level2);
*/
    if (m_input->IsTriggered(SDL_SCANCODE_R)) {
 
        RemoveObject(animation);
        delete animation;
        animation = nullptr;
       /* RemoveObject(newTile);
        delete []newTile;
        rope = &tile1;
        */
        RemoveObject(objectForRope);
        delete objectForRope;
        objectForRope = nullptr;
        Initialize();
    }

    // Must be one of the last functions called at the end of State Update
    if (rope_joint != nullptr)
    {
        if (m_input->IsPressed(SDL_SCANCODE_UP))
        {
            rope_joint->SetLength(rope_joint->GetLength() - 0.3f);
        }

        if (m_input->IsPressed(SDL_SCANCODE_DOWN))
        {
            rope_joint->SetLength(rope_joint->GetLength() + 0.3f);
        }

    }

    b2Vec3 angle_vector = objectForRope->transform.position - animation->transform.position;
    float angle = atan2(angle_vector.y, angle_vector.x);

    //for checking the time
   // if(rope )
    timer += 0.01*playerVelocity.x;
    std::cout <<timer  << std::endl;
  
    if (rope != nullptr && rope_joint_def.collideConnected == true) {
         rope_position = animation->transform.position + objectForRope->transform.position;
         rope->transform.position.Set(rope_position.x/2.0f , rope_position.y/2.0f , rope_position.z);
        rope->transform.rotation = (180.0f*angle) / 3.14f;
    }

    //Rope has created but no jointed
    if (rope != nullptr && timer > 0.04f*playerVelocity.x && rope_joint_def.collideConnected == false) {
        RemoveObject(rope);
       // timer = 0;
        delete rope;
        rope = nullptr;
    }
    //Rope has created and jointed
    else if (rope != nullptr && timer > 0.4*playerVelocity.x && rope_joint_def.collideConnected == true) {
       // timer = 0;
           // rope_position = animation->transform.position + objectForRope->transform.position;
           // rope->transform.position.Set(rope_position.x , rope_position.y , rope_position.z);
           // rope->transform.rotation = (180.0f*angle) / 3.14f;
        
           RemoveObject(rope);
           delete rope;
           rope = nullptr;
           rope_joint_def.collideConnected = false;
           GetPhysicsWorld()->DestroyJoint(rope_joint);

            RemoveObject(objectForRope);
           delete objectForRope;
           objectForRope = nullptr;

           basicDistance_ObjectforRope += distance_ObjectforRope;
           objectForRope = new Object();
           objectForRope->SetName("objectforRope");
           objectForRope->transform.position.Set( basicDistance_ObjectforRope, 120.0f, 0.0f);
           objectForRope->transform.SetScale(50, 50);
           objectForRope->sprite.LoadImage("texture/TouchPoint.png", m_renderer);
           objectForRope->physics.bodyType = Physics::STATIC;
           objectForRope->physics.GenerateBody(GetPhysicsWorld(), &objectForRope->transform);

           AddObject(objectForRope);
      }
    UpdatePhysics(dt);
    Render(dt);
}

void Level1::Close()
{
    // Wrap up state
    ClearBaseState();
}

void Level1::basicTiles()
{
    //가로를 거의 다채우는 크기 1270
    newTile = new Object[3];

    newTile[0].SetName("Tile1");
    newTile[0].transform.position.Set(-400.0f, -250.0f, 0.0f);
    newTile[0].transform.SetScale(500, 200);
    newTile[0].sprite.color = SDL_Color{ 0, 255, 255, 255 };
    newTile[0].sprite.LoadImage("texture/rect.png", m_renderer);
    newTile[0].sprite.color.r = newTile[0].sprite.color.g
    = newTile[0].sprite.color.b = 0X88;
    newTile[0].physics.bodyType = Physics::STATIC;
    newTile[0].physics.GenerateBody(GetPhysicsWorld(), &newTile[0].transform);//tile1[0].transform);

    newTile[1].SetName("Tile2");
    newTile[1].transform.position.Set(  250.0f, -250.0f, 0.0f);
    newTile[1].transform.SetScale(500, 200);
    newTile[1].sprite.color = SDL_Color{ 0, 255, 255, 255 };
    newTile[1].sprite.LoadImage("texture/rect.png", m_renderer);
    newTile[1].sprite.color.r = newTile[1].sprite.color.g
        = newTile[1].sprite.color.b = 0X88;
    newTile[1].physics.bodyType = Physics::STATIC;
    newTile[1].physics.GenerateBody(GetPhysicsWorld(), &newTile[1].transform);//tile1[0].transform);
    
    AddObject(&newTile[0]);
    AddObject(&newTile[1]);
}

void Level1::makeNewRope()
{

    if (m_input->IsTriggered(SDL_SCANCODE_SPACE) && rope_joint_def.collideConnected != true){
         rope = new Object;

         rope->SetName("Rope");
         //If player is out of range to the objectForRope , the player only make rope and delete without joint.
         if (objectForRope->transform.position.x - animation->transform.position.x > 200.0f) {
             rope_position =animation->transform.position;
             rope->transform.position.Set(rope_position.x + 85, rope_position.y + 85, rope_position.z);
             rope->transform.SetScale(250, 5);
             rope->transform.rotation = 45.0f;
             rope->sprite.LoadImage("texture/rect.png", m_renderer);
             //newTile[0].sprite.color = SDL_Color{ 0, 255, 255, 255 };
             rope->sprite.color.r = rope->sprite.color.g = rope->sprite.color.b = 0X58;
       

             AddObject(rope);
             timer = 0;
         }
         else if (objectForRope->transform.position.x - animation->transform.position.x < 200.0f)
         {
             rope_position = animation->transform.position + objectForRope->transform.position;
             float ropeX = objectForRope->transform.position.x - animation->transform.position.x;
             float ropeY = objectForRope->transform.position.y - animation->transform.position.y;

             float ropeSize = sqrt((ropeX*ropeX) + (ropeY*ropeY));
            
             rope->transform.position.Set(rope_position.x/2.0f, rope_position.y/2.0f, rope_position.z);
             rope->transform.SetScale(ropeSize, 5);

             b2Vec3 angle_vector = objectForRope->transform.position - animation->transform.position;
             float angle = atan2(angle_vector.y, angle_vector.x);

             rope->transform.rotation = (180.0f*angle) / 3.14f;

             rope->sprite.LoadImage("texture/rect.png", m_renderer);
             rope->sprite.color.r = rope->sprite.color.g = rope->sprite.color.b = 0X58;

             rope_joint_def.bodyA = objectForRope->physics.GetBody();
             rope_joint_def.bodyB = animation->physics.GetBody(); //objectForRope.physics.GetBody()
             rope_joint_def.length = 10.0f;

             rope_joint_def.collideConnected = true;
             rope_joint = (b2DistanceJoint*)GetPhysicsWorld()->CreateJoint(&rope_joint_def);
             AddObject(rope);
             timer = 0;

         }
        //b2Vec2 temp = set_anchor; //- object->customPhysics.Getb2Body()->GetTransform().p
      //  rope->transform.rotation
        //rope->physics.bodyType = Physics::STATIC;
       // newTile->sprite.color = Level5Color::BLACK;
       // rope->sprite.LoadImage("texture/rect.png", m_renderer);

        //rope->physics.GenerateBody(GetPhysicsWorld(), &rope->transform);

        //AddObject(rope);

    }

}
void Level1::MakeNewTiles(float dt)
{
    //고정후 터치해도 지나갈수있게
    newTilePoint.physics.ActiveGhostCollision(true);
    dt = 0;
   // if()
    if (animation->physics.HasBody())
    {
        if(animation->physics.IsColliding())
        {
            if(animation->physics.IsCollidingWith(&newTilePoint))
            {
                newTile = new Object;

                newTile->SetName("new");

                newTile->transform.SetScale(350, 32);
                b2Vec2 scaleSize = newTile->transform.GetScale();

                newTile->transform.position.Set( newTilePoint.transform.position.x + scaleSize.x, -150.f, 0.f);
                newTile->physics.bodyType = Physics::STATIC;
                // newTile->sprite.color = Level5Color::BLACK;
                newTile->sprite.LoadImage("texture/rect.png", m_renderer);

                newTile->physics.GenerateBody(GetPhysicsWorld(), &newTile->transform);
              
                AddObject(newTile);
            }
        }
    }
}

void Level1::ControlCamera(float dt)
{
    // Set the camera's speed
    static float speed = 100.f * dt;
    static bool toggle = true;

    
     //camera.position.x += speed;

     //플레이어 컨트롤을 플레이어의 포지션으로 했을때 문제가 되는가?
     
     camera.position = b2Vec3(animation->transform.position.x + 350, animation->transform.position.y, animation->transform.position.z);
     animation->physics.SetVelocity(playerVelocity);
    

    // animation->transform.position.x += speed;
    // Panning by pressing arrow keys
    if (m_input->IsPressed(SDL_SCANCODE_RIGHT))
        animation->transform.position.x += speed*2;
    if (m_input->IsPressed(SDL_SCANCODE_LEFT))
        animation->transform.position.x -= speed*2;

    if (m_input->IsPressed(SDL_SCANCODE_UP) && animation->physics.IsCollidingWith(&tile1)) {

       // animation.physics.GenerateBody(GetPhysicsWorld(), &animation.transform);
        animation->transform.position.y += 45;
    }

    if (m_input->IsPressed(SDL_SCANCODE_UP) && animation->physics.IsCollidingWith(newTile) ){

        // animation.physics.GenerateBody(GetPhysicsWorld(), &animation.transform);
            animation->transform.position.y++;
    }
    if (m_input->IsTriggered(SDL_SCANCODE_P)) {
        newTilePoint.physics.ActiveGhostCollision(toggle);
        toggle = !toggle;
    }

    if(m_input->IsTriggered(SDL_SCANCODE_K))//animation.transform.position.x >= currentPlayerLoc +200)
    {
        newTile = new Object;

        newTile->SetName("new");

        newTile->transform.position.Set(animation->transform.position.x+40, -150.f, 0.f);
        newTile->transform.SetScale(400, 32);
        newTile->physics.bodyType = Physics::STATIC;
       // newTile->sprite.color = Level5Color::BLACK;
        newTile->sprite.LoadImage("texture/rect.png", m_renderer);

        newTile->physics.GenerateBody(GetPhysicsWorld(), &newTile->transform);


        AddObject(newTile);

        //float scaleX = TURRET_WIDTH;
        //float scaleY = TURRET_HEIGHT;
        //newTurret->transform.SetScale(scaleX, scaleY);

        //newTurret->sprite.color = Level5Color::BLACK;
        //newTurret->sprite.LoadImage("texture/rect.png", m_renderer);

        //AddObject(newTurret);
        
    }

    //if (m_input->IsPressed(SDL_SCANCODE_DOWN))
    //    camera.position.y += speed;
    //if (m_input->IsPressed(SDL_SCANCODE_UP))
    //    camera.position.y -= speed;

    ////Zooming by pressing arrow W and S key
    //if (m_input->IsPressed(SDL_SCANCODE_W))
    //    camera.position.z -= speed;
    //if (m_input->IsPressed(SDL_SCANCODE_S))
    //    camera.position.z += speed;

    ////Rotating by pressing arrow Q and E key
    //if (m_input->IsPressed(SDL_SCANCODE_Q))
    //    camera.rotation -= speed;
    //if (m_input->IsPressed(SDL_SCANCODE_E))
    //    camera.rotation += speed;
}


//Object& Level1::BuildAndRegisterWall(Object &wall, const char *name, float positionX, float positionY, float scaleX, float scaleY)
//{
//    const float POSITION_Z = 0.f;
//
//    wall.SetName(name);
//
//    wall.transform.position.Set(positionX, positionY, POSITION_Z);
//    wall.transform.SetScale(scaleX, scaleY);
//
//    wall.sprite.color = Level4Color::GREY;
//    wall.sprite.LoadImage("texture/rect.png", m_renderer);
//
//    wall.physics.bodyType = Physics::STATIC;
//    wall.physics.GenerateBody(GetPhysicsWorld(), &wall.transform);
//
//    AddObject(&wall);
//
//    return wall;
//}

