/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "blackrock_depths.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_SHADOWWORDPAIN                                   = 10894,
    SPELL_MANABURN                                         = 10876,
    SPELL_PSYCHICSCREAM                                    = 8122,
    SPELL_SHADOWSHIELD                                     = 22417
};

enum Events
{
    EVENT_SHADOW_WORD_PAIN                                 = 1,
    EVENT_MANABURN                                         = 2,
    EVENT_PSYCHIC_SCREAM                                   = 3,
    EVENT_SHADOWSHIELD                                     = 4
};

class boss_high_interrogator_gerstahn : public CreatureScript
{
    public:
        boss_high_interrogator_gerstahn() : CreatureScript("boss_high_interrogator_gerstahn") { }

        struct boss_high_interrogator_gerstahnAI : public BossAI
        {
            boss_high_interrogator_gerstahnAI(Creature* creature) : BossAI(creature, BOSS_HIGH_INTERROGATOR_GERSTAHN) { }

            void JustEngagedWith(Unit* who) override
            {
                _JustEngagedWith(who);
                events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, 4s);
                events.ScheduleEvent(EVENT_MANABURN, 14s);
                events.ScheduleEvent(EVENT_PSYCHIC_SCREAM, 32s);
                events.ScheduleEvent(EVENT_SHADOWSHIELD, 8s);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOW_WORD_PAIN:
                            if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, 100.0f, true))
                                DoCast(target, SPELL_SHADOWWORDPAIN);
                            events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, 7s);
                            break;
                        case EVENT_PSYCHIC_SCREAM:
                            DoCastVictim(SPELL_PSYCHICSCREAM);
                            events.ScheduleEvent(EVENT_PSYCHIC_SCREAM, 30s);
                            break;
                        case EVENT_MANABURN:
                            if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, 100.0f, true))
                                DoCast(target, SPELL_MANABURN);
                            events.ScheduleEvent(EVENT_MANABURN, 10s);
                            break;
                        case EVENT_SHADOWSHIELD:
                            DoCast(me, SPELL_SHADOWSHIELD);
                            events.ScheduleEvent(EVENT_SHADOWSHIELD, 25s);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetBlackrockDepthsAI<boss_high_interrogator_gerstahnAI>(creature);
        }
};

void AddSC_boss_high_interrogator_gerstahn()
{
    new boss_high_interrogator_gerstahn();
}
