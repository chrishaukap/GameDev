using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;

namespace CombatPrototype.GameWorld
{
    class Cone
    {
        public enum DirectionOfPull
        {
            Forward =1,
            Backward
        }
        private Core.PrimitiveBatch mPrimitiveBatch;
        private GameWorldCore mGameWorldCore;
        private int mTimer = 100;
        private int mRange;
        private int mDegrees;
        private Agent mCastingAgent;
        private Vector2 mPosition;
        private float mOrientation;
        private DirectionOfPull mForceDirection;
        public Cone(GameWorldCore gwc, Agent castingAgent,
                    DirectionOfPull forceDirection )            
        {
            mCastingAgent = castingAgent;
            mPosition = castingAgent.CurrentPosition;
            mOrientation = castingAgent.CurrentRotation;
            mTimer = castingAgent.RoarProperties.mDuration;
            mGameWorldCore = gwc;
            mRange = castingAgent.RoarProperties.mRange;
            mDegrees = castingAgent.RoarProperties.mDegrees;
            mForceDirection = forceDirection;
            mPrimitiveBatch = new Core.PrimitiveBatch(gwc.Camera, mGameWorldCore.Graphics.GraphicsDevice);
        }
        public bool IsAgentWithin(Agent agent)
        {
            float roarArcRadians = MathHelper.ToRadians(mDegrees);
            float minAngle = mOrientation - roarArcRadians / 2;
            float maxAngle = mOrientation + roarArcRadians / 2;

            float agentsRotation =
                (float)System.Math.Atan2(mPosition.Y - agent.CurrentPosition.Y,
                                         mPosition.X - agent.CurrentPosition.X);
            if (agentsRotation <= 0)
                agentsRotation += MathHelper.TwoPi;

            if (agentsRotation >= minAngle && agentsRotation <= maxAngle)
                return true;
            return false;
        }
        public void Update()
        {
            --mTimer;
            List<Agent> agents = mGameWorldCore.GetAgentsWithinDistance(mPosition, mRange);
            foreach (Agent agent in agents)
                if(IsAgentWithin(agent))
                    agent.KnockBack(mPosition, Core.XMLProperties.AgentProperties.mRoarKnockBackSpeed);
        }
        public bool HasExpired()
        { 
            if(mTimer <= 0)
                return true;
            if (mCastingAgent == null || mCastingAgent.Health <= 0)
                return true;
            return false;           
        }
        public void Draw()
        {
            List<Vector2> vertexList = new List<Vector2>();
            vertexList.Add(mPosition);
            float maxAngle = MathHelper.ToRadians(mDegrees);
                         
            float angle = 0;
            while( angle < maxAngle )
            {
                float workingAngle = angle + mOrientation + MathHelper.Pi - (maxAngle / 2);
                Vector2 vec = new Vector2((float)Math.Cos(workingAngle), (float)Math.Sin(workingAngle));
                vec *= mRange;
                vec += mPosition;
                vertexList.Add(vec);
                angle += (float)MathHelper.Pi / 16;
            }

            Color color = Color.Blue;
            if (mForceDirection == DirectionOfPull.Backward)
                color = Color.Green;

            mPrimitiveBatch.Begin(PrimitiveType.LineList);
            for (int i = 0; i < vertexList.Count - 1; i++)
            {
                mPrimitiveBatch.AddVertex(vertexList[i], color);
                mPrimitiveBatch.AddVertex(vertexList[i + 1], color);
            }
            mPrimitiveBatch.AddVertex(vertexList[0], color);
            mPrimitiveBatch.AddVertex(vertexList[vertexList.Count - 1], color);
            mPrimitiveBatch.End();
        }
    }
}
