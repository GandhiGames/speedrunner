#include "S_Game.h"
#include "StateManager.h"

S_Game::S_Game(StateManager* stateManager) : Scene(stateManager),
m_map(*stateManager->m_context)
{

}

S_Game::~S_Game()
{
}

void S_Game::OnCreate()
{
	auto context = m_stateManager->m_context;

	context->m_textureManager = &m_textureManager;

	sf::Vector2u size = context->m_window->getSize();
	m_view.setSize((float)size.x, (float)size.y);
	m_view.setCenter(size.x / 2.f, size.y / 2.f);
	//TODO: move to camera component.
	m_view.zoom(.6f);
	context->m_window->setView(m_view);

	m_player = std::make_shared<Object>(*context);
	Object::Add(m_player);

	context->m_player = m_player;

	m_player->GetComponent<C_Tag>()->Set(PLAYER_TAG);
	m_player->AddComponent<C_Camera>();
	m_player->AddComponent<C_Velocity>();
	m_player->AddComponent<C_KeyboardController>();
	m_player->AddComponent<C_Direction>();
	auto collider1 = m_player->AddComponent<C_BoxCollider>();
	collider1->SetCollidable(sf::FloatRect(0, 0, 25, 40)); 	//TODO: need to setup player collider size
	collider1->SetLayer(CollisionLayer::Player);
	m_player->AddComponent<C_MapCollision>();
	m_player->AddComponent<C_DebugDrawCollider>();
	m_player->AddComponent<C_DebugDrawMapCollisions>();

	auto animation = m_player->AddComponent<C_AnimatedSprite>();
	
	std::map<ANIMATION_STATE, std::shared_ptr<AnimationGroup>> playerAnimations = 
		AnimationFactory::FromFile(*context, resourcePath() + "data/spritesheets/ghouls_n_ghosts_character.xml");
	
	for (auto& a : playerAnimations)
	{
		animation->AddAnimation(a.first, a.second);
	}
	animation->SetAnimationState(ANIMATION_STATE::IDLE);


	m_player->AddComponent<C_MovementAnimation>();
	m_player->AddComponent<C_MeleeAttack>();

	//TODO: change from relative path. Make sure you do this for all paths.
	//m_map.LoadTiles("../resources/data/test_tileset.data",
	//	"../resources/spritesheets/test_tileset.png");
	m_map.LoadMap(resourcePath() + "data/maps/boss_1/", "boss_1_map.tmx");

	const sf::Vector2f& mapPos = m_map.GetStartPosition();

	m_view.setCenter(mapPos);

	context->m_map = &m_map;

	m_player->m_transform->SetPosition(mapPos);

	Raycast::Initialise(context);
	Debug::Initialise(*context);
	Input::Initialise();
}

void S_Game::OnDestroy()
{

}

void S_Game::Activate() {}

void S_Game::Deactivate() {}

void S_Game::Update(float deltaTime)
{
	Input::EarlyUpdate();

	m_collisions.Resolve();

	Object::UpdateAll(deltaTime);

	if (Input::IsKeyPressed(Input::KEY::KEY_ESC))
	{
		m_stateManager->m_context->m_window->close();
	}
}

void S_Game::Draw(float deltaTime)
{
	sf::RenderWindow& window =* m_stateManager->m_context->m_window;

	m_map.Draw(window);

	Object::DrawAll(window, deltaTime);

	Debug::Draw(window);

	m_fps.Draw(window, deltaTime);
}

void S_Game::LateUpdate(float deltaTime)
{
	Object::LateUpdateAll(deltaTime);

	bool removed = Object::ProcessRemovals();

	if (removed)
	{
		m_collisions.ProcessRemovals();
	}

	// has to be processed before Object::ProcessNewObjects.
	m_collisions.ProcessNewObjects();

	Object::ProcessNewObjects();

	Input::LateUpdate();
}
