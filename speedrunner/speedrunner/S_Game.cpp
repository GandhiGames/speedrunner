#include "S_Game.h"
#include "StateManager.h"

S_Game::S_Game(StateManager* stateManager) : Scene(stateManager)
{

}

S_Game::~S_Game()
{
}

void S_Game::OnCreate()
{
	sf::Vector2u size = m_stateManager->m_context->m_window->getSize();
	m_view.setSize((float)size.x, (float)size.y);
	m_view.setCenter(size.x / 2.f, size.y / 2.f);
	//TODO: move to camera component.
	m_view.zoom(.8f);
	m_stateManager->m_context->m_window->setView(m_view);

	//TODO: move this a text/data file.
	const int animationSpeed = 9;
	const int swingAnimationSpeed = 12;
	const int startFrame = 1;
	const int endFrame = 9;
	const int spriteSize = 64;

	/*******************
	 Player setup start.
	********************/
	{
		m_player = std::make_shared<Object>(*m_stateManager->m_context);

		m_player->m_transform->SetPosition(sf::Vector2f(0.f, 0.f));
		m_player->GetComponent<C_Tag>()->Set(PLAYER_TAG);

		m_player->AddComponent<C_Velocity>();

		auto controller = m_player->AddComponent<C_KeyboardController>();
		controller->SetMovementSpeed(120);

		auto playerSprite = m_player->AddComponent<C_AnimatedSprite>();

		//TODO: need to setup animations.


		playerSprite->SetSortOrder(1000);

		m_player->AddComponent<C_DirectionalAnimation>();

		auto collider1 = m_player->AddComponent<C_BoxCollider>();
		//TODO: need to setup player collider size
		collider1->SetCollidable(sf::FloatRect());
		collider1->SetLayer(CollisionLayer::Player);

		//m_player->AddComponent<C_CollidableTest>();
		//m_player->AddComponent<C_RaycastTest>();

		m_player->AddComponent<C_Camera>();
		m_player->AddComponent<C_MeleeAttack>();
		m_player->AddComponent<C_Direction>();

		Object::Add(m_player);

		auto label = m_player->AddComponent<C_DebugPositionLabel>();
		label->SetOffset(sf::Vector2f(0.f, 0.f));
		label->SetSortOrder(1010);
	}
	/*******************
	 Player setup end.
	********************/

	auto context = m_stateManager->m_context;
	context->m_player = m_player;
	context->m_textureManager = &m_textureManager;

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
	auto window = m_stateManager->m_context->m_window;

	Object::DrawAll(*window, deltaTime);

	Debug::Draw(*window);

	m_fps.Draw(*window, deltaTime);
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