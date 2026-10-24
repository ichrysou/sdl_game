#pragma once

#include "AnimationComponent.h"
#include "ColliderComponent.h"
#include "DeadComponent.h"
#include "TransformComponent.h"
#include "../game.h"
#include <cmath>

class ThunderMeleeComponent : public Component
{
public:
    ThunderMeleeComponent(Manager &manager, TransformComponent &owner, Vector2D direction,
                          Vector2D originOffset)
        : m_manager(manager), m_owner(owner), m_direction(direction),
          m_originOffset(originOffset)
    {
    }

    void update() override
    {
        constexpr float frameSize = 64.0f;
        m_origin = Vector2D(
            m_owner.position.x + m_owner.width * m_owner.scale / 2.0f,
            m_owner.position.y + m_owner.height * m_owner.scale / 2.0f);
        m_origin.x += m_originOffset.x;
        m_origin.y += m_originOffset.y;
        auto &attackTransform = entity->getComponent<TransformComponent>();

        auto animation = entity->getComponent<AnimationComponent>().getActive();
        if (animation->isDone())
        {
            entity->destroy();
            return;
        }

        const float maximumReach = animation->m_frame_w * entity->getComponent<TransformComponent>().scale;
        const float expansion = animation->m_num_frames > 1
                                    ? static_cast<float>(animation->getCurrentFrame()) / (animation->m_num_frames - 1)
                                    : 1.0f;
        const float reach = maximumReach * expansion;
        const float collisionReach = reach + maximumReach * expansion;
        m_lightPosition = Vector2D(
            m_origin.x + m_direction.x * collisionReach,
            m_origin.y + m_direction.y * collisionReach);
        const float angle = std::atan2(m_direction.y, m_direction.x);
        const float anchorX = -frameSize / 2.0f;
        const float anchorY = 30.0f - frameSize / 2.0f;
        const float rotatedAnchorX =
            std::cos(angle) * anchorX - std::sin(angle) * anchorY;
        const float rotatedAnchorY =
            std::sin(angle) * anchorX + std::cos(angle) * anchorY;
        const float boltStartX = m_origin.x + m_direction.x * reach;
        const float boltStartY = m_origin.y + m_direction.y * reach;
        attackTransform.position = Vector2D(
            boltStartX - rotatedAnchorX - frameSize / 2.0f,
            boltStartY - rotatedAnchorY - frameSize / 2.0f);

        const float halfWidth = animation->m_frame_h *
                                entity->getComponent<TransformComponent>().scale * 0.25f;

        for (auto *enemy : m_manager.getGroup(Game::groupEnemies))
        {
            if (enemy->hasComponent<DeadComponent>() ||
                !enemy->hasComponent<ColliderComponent>())
                continue;

            const auto &collider = enemy->getComponent<ColliderComponent>().collider;
            const float enemyX = collider.x + collider.w / 2.0f - m_origin.x;
            const float enemyY = collider.y + collider.h / 2.0f - m_origin.y;
            const float along = enemyX * m_direction.x + enemyY * m_direction.y;
            const float across = std::abs(enemyX * m_direction.y - enemyY * m_direction.x);
            const float enemyHalfLength =
                (std::abs(m_direction.x) * collider.w + std::abs(m_direction.y) * collider.h) / 2.0f;
            const float enemyHalfWidth =
                (std::abs(m_direction.y) * collider.w + std::abs(m_direction.x) * collider.h) / 2.0f;

            if (along + enemyHalfLength < 0.0f ||
                along - enemyHalfLength > collisionReach ||
                across > halfWidth + enemyHalfWidth)
                continue;

            enemy->addComponent<DeadComponent>();
            enemy->getComponent<AnimationComponent>().setActive("die");
            enemy->getComponent<TransformComponent>().velocity.Zero();
        }
    }

    Vector2D getLightPosition() const
    {
        return m_lightPosition;
    }

private:
    Manager &m_manager;
    TransformComponent &m_owner;
    Vector2D m_origin;
    Vector2D m_direction;
    Vector2D m_originOffset;
    Vector2D m_lightPosition;
};
