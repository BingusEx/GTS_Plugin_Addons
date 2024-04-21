#include "magic/effects/Poisons/Poison_Of_Shrinking.hpp"
#include "managers/ShrinkToNothingManager.hpp"
#include "magic/effects/common.hpp"
#include "managers/GtsManager.hpp"
#include "utils/actorUtils.hpp"
#include "managers/Rumble.hpp"
#include "data/runtime.hpp"
#include "magic/magic.hpp"
#include "scale/scale.hpp"
#include "events.hpp"
#include "timer.hpp"


namespace Gts {
	std::string Shrink_Poison::GetName() {
		return "Shrink_Poison";
	}

	void Shrink_Poison::OnStart() {
		auto caster = GetCaster();
		if (!caster) {
			return;
		}
		auto target = GetTarget();
		if (!target) {
			return;
		}

		Rumbling::Once("Shrink_Poison", target, 2.0, 0.05);

		float Volume = std::clamp(get_visual_scale(target) * 0.10f, 0.10f, 1.0f);
		Runtime::PlaySound("shrinkSound", target, Volume, 1.0);
	}

	void Shrink_Poison::OnUpdate() {
		const float BASE_POWER = 0.004000;

		auto caster = GetCaster();
		if (!caster) {
			return;
		}
		auto target = GetTarget();
		if (!target) {
			return;
		}

		float AlchemyLevel = std::clamp(caster->AsActorValueOwner()->GetActorValue(ActorValue::kAlchemy)/100.0f + 1.0f, 1.0f, 2.0f);
		Rumbling::Once("Shrink_Poison", target, 0.4, 0.05);
		float powercap = std::clamp(get_visual_scale(target), 0.85f, 1.10f);
		float Power = BASE_POWER * powercap * AlchemyLevel;

		ShrinkActor(target, Power, 0.0);
		if (get_visual_scale(target) < 0.08/GetSizeFromBoundingBox(target) && ShrinkToNothingManager::CanShrink(caster, target)) {
			PrintDeathSource(caster, target, DamageSource::Explode);
			ShrinkToNothingManager::Shrink(caster, target);
		}
	}

	void Shrink_Poison::OnFinish() {
	}
}