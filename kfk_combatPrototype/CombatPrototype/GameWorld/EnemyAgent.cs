using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;

namespace CombatPrototype.GameWorld
{
    class EnemyAgent : Agent
    {
        public EnemyAgent(GameWorldCore gwc, Vector2 pos, float size) : 
            base(gwc, pos, size)
        {
            mScale = size;
            mTexture = mGameWorld.mContent.Load<Texture2D>("agentbuilding");
        }

        public override bool IsEnemy()
        {
            return true;
        }
    }
}
