using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework;
using System.Linq;
using System.Text;

using CombatPrototype;
using CombatPrototype.GameWorld;
using CombatPrototype.Core;
using CombatPrototype.Core.Pathfinding;
using CombatPrototype.Core.XMLProperties;

namespace CombatPrototype.GameWorld
{
    class GameWorldCore
    {
        public SpriteBatch mSpriteBatch;
        public ContentManager mContent;
        Game1 mGame;
        public XNA2DCamera Camera { get { return mGame.Camera; } }

        public Random mRandom;

        public List<Agent> mAgents;
        private List<Crater> mCraters;
        private List<Cone> mCones;

        public Pathfinder mPathfinder;
        PrimitiveBatch mPrimitiveBatch;

        public GameWorldCore(GraphicsDeviceManager g, ContentManager c, Game1 game)
        {
            mContent = c;
            mGraphics = g;
            mGame = game;
            mPrimitiveBatch = new PrimitiveBatch(Camera, g.GraphicsDevice);
            mSpriteBatch = new SpriteBatch(mGraphics.GraphicsDevice);
            mPathfinder = new Pathfinder(g, this);
            mAgents = new List<Agent>();
            mCraters = new List<Crater>();
            mCones = new List<Cone>();
            mRandom = new Random();

            InitializeStartingAgents();
        }
        private void CreateAgent(Vector2 position, bool enemy)
        {
            float scale = 0.8f;
            if (Core.XMLProperties.AgentProperties.mVariableSize)
            {
                int rand = mRandom.Next(100);
                if (rand >= 99)
                    scale = 2.2f;
                else if (rand >= 94)
                    scale = 1.8f;
                else if (rand >= 85)
                    scale = 1.4f;
                else if (rand >= 55)
                    scale = 1.2f;
            }

            if (enemy)
                mAgents.Add(new Agent(this, position, scale));
            else
                mAgents.Add(new EnemyAgent(this, position, scale));
        }
        private void InitAGentSetup_Random()
        {
            int maxAgents = Core.XMLProperties.AgentProperties.mMaxAgentsPerTeam - Core.XMLProperties.AgentProperties.mMinAgentsPerTeam;
            int minAgents = Core.XMLProperties.AgentProperties.mMinAgentsPerTeam;
            int maxWidth = Core.XMLProperties.AgentProperties.mMaxPlacementWidth;
            int maxHeight = Core.XMLProperties.AgentProperties.mMaxPlacementHeight;
            int numAgentsPerTeam = mRandom.Next(maxAgents) + minAgents;

            for (int j = 0; j < numAgentsPerTeam; ++j)
                CreateAgent(new Vector2(mRandom.Next(maxWidth), mRandom.Next(maxHeight)), false);
            for (int j = 0; j < numAgentsPerTeam; ++j)
                CreateAgent(new Vector2(mRandom.Next(maxWidth), mRandom.Next(maxHeight)), true);
        }
        private void InitAgentSetup_RandomSkirmishes()
        {
            int numSkirmishes = mRandom.Next(10) + 5;
            for (int i = 0; i < numSkirmishes; ++i)
            {
                int goodGuyX = mRandom.Next(800);
                int goodGuyY = mRandom.Next(600);
                int badGuyX = goodGuyX + 20;
                int badGuyY = goodGuyY;
                for (int j = 0; j < 5; ++j)
                    CreateAgent(new Vector2(goodGuyX, goodGuyY), false);
                for (int j = 0; j < 5; ++j)
                    CreateAgent(new Vector2(badGuyX, badGuyY), true);
            }
        }
        private void InitAgentSetup_OpposingSides()
        {
            const int startX = 0;
            const int startY = 70;
            const int rows = 25;
            const int cols = 5;
            const int rowspace = 20;
            const int colspace = 15;
            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < cols; ++j)
                    CreateAgent(new Vector2(j * colspace, startY + i * rowspace), false);
            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < cols; ++j)
                    CreateAgent(new Vector2(700 + j * colspace, startY + i * rowspace), true);
        }
        private void InitAgentSetup_Star()
        {
            for (int i = 0; i < 40; ++i)
                CreateAgent(new Vector2(0f, 300f), false);
            for (int i = 0; i < 40; ++i)
                CreateAgent(new Vector2(400, 0f), false);
            for (int i = 0; i < 40; ++i)
                CreateAgent(new Vector2(800f, 300f), false);
            for (int i = 0; i < 40; ++i)
                CreateAgent(new Vector2(400f, 600f), false);
            for (int i = 0; i < 40; ++i)
                CreateAgent(new Vector2(260f, 400f), true);
            for (int i = 0; i < 40; ++i)
                CreateAgent(new Vector2(260f, 200f), true);
            for (int i = 0; i < 40; ++i)
                CreateAgent(new Vector2(520f, 200f), true);
            for (int i = 0; i < 40; ++i)
                CreateAgent(new Vector2(520f, 400f), true);
        }
        private void InitializeStartingAgents()
        {
            if( Core.XMLProperties.AgentProperties.mSetupOption == "Random" )
                InitAGentSetup_Random();
            else if (Core.XMLProperties.AgentProperties.mSetupOption == "RandomSkirmishes")
                InitAgentSetup_RandomSkirmishes();
            else if (Core.XMLProperties.AgentProperties.mSetupOption == "OpposingSides")
                InitAgentSetup_OpposingSides();
            else if (Core.XMLProperties.AgentProperties.mSetupOption == "Star")
                InitAgentSetup_Star();
        }
        private GraphicsDeviceManager mGraphics;  
        public GraphicsDeviceManager Graphics
        { get { return mGraphics; } }
        public void AddCrater(Vector2 pos, float radius)
        {
            mCraters.Add(new Crater(this, pos, radius));
        }
        public Cone AddRoarCone(Agent castingAgent)
        {
            Cone cone = new Cone(this, castingAgent, Cone.DirectionOfPull.Forward);
            mCones.Add(cone);
            return cone;
        }
        public Cone AddTornadoCone(Agent castingAgent)
        {
            Cone cone = new Cone(this, castingAgent, Cone.DirectionOfPull.Backward);
            mCones.Add(cone);
            return cone;
        }
        public void CheckMouseEvents()
        {
            const float ZoomAmount = 0.1f;
            if(mGame.mGameMouse.ScrolledDownThisFrame)
                Camera.Zoom = new Vector2(Camera.Zoom.X - ZoomAmount, Camera.Zoom.Y - ZoomAmount);
            else if (mGame.mGameMouse.ScrolledUpThisFrame)
                Camera.Zoom = new Vector2(Camera.Zoom.X + ZoomAmount, Camera.Zoom.Y + ZoomAmount);

            if (mGame.mGameMouse.PressedThisFrameLeft || mGame.mGameMouse.PressedThisFrameRight)
            {
                Vector2 currentMousePos = mGame.mGameMouse.CurrentMousePos();
                Vector3 translationVector = Camera.ViewTransformationMatrix().Translation;

                currentMousePos.X -= translationVector.X;
                currentMousePos.Y -= translationVector.Y;
                currentMousePos *= new Vector2( 1 / Camera.Zoom.X, 1 / Camera.Zoom.Y );

                if (mGame.mGameMouse.PressedThisFrameLeft)
                    CreateAgent(currentMousePos, false);

                if (mGame.mGameMouse.PressedThisFrameRight)
                    CreateAgent(currentMousePos, true);
            }

        }
        public void CheckKeyboardEvents()
        {
            const float ScrollAmount = 10.0f;
            if( Keyboard.GetState().IsKeyDown(Keys.Left))
                Camera.Offset = new Vector2(Camera.Offset.X - ScrollAmount, Camera.Offset.Y);
            if (Keyboard.GetState().IsKeyDown(Keys.Right))
                Camera.Offset = new Vector2(Camera.Offset.X + ScrollAmount, Camera.Offset.Y);
            if( Keyboard.GetState().IsKeyDown(Keys.Down))
                Camera.Offset = new Vector2(Camera.Offset.X, Camera.Offset.Y + ScrollAmount);
            if (Keyboard.GetState().IsKeyDown(Keys.Up))
                Camera.Offset = new Vector2(Camera.Offset.X, Camera.Offset.Y - ScrollAmount);
        }
        public void Update()
        {
            CheckKeyboardEvents();
            CheckMouseEvents();

            // process Agents
            {
                List<Agent> removeList = new List<Agent>();
                int i = 0;
                foreach (Agent agent in mAgents)
                {
                    agent.Update();
                    if (agent.Health <= 0.0f)
                        removeList.Add(agent);

                    i++;
                }

                foreach (Agent agentToRem in removeList)
                {
                    mAgents.Remove(agentToRem);

                    foreach (Agent agent in mAgents)
                    {
                        if (agent.mCurrentTarget == agent)
                            agent.mCurrentTarget = null;
                    }
                }
            }

            // Process Craters 
            {
                List<Crater> craterRemoveList = new List<Crater>();
                foreach (Crater crater in mCraters)
                {
                    if (crater.HasExpired())
                        craterRemoveList.Add(crater);
                    crater.Update();
                }

                foreach (Crater crater in craterRemoveList)
                    mCraters.Remove(crater);
            }
            // Process Cones 
            {
                List<Cone> coneRemoveList = new List<Cone>();
                foreach (Cone cone in mCones)
                {
                    if (cone.HasExpired())
                        coneRemoveList.Add(cone);
                    cone.Update();
                }

                foreach (Cone cone in coneRemoveList)
                    mCones.Remove(cone);
            }
        }

        public void Draw()
        {
            mPathfinder.Draw();
            foreach (Crater crater in mCraters)
                crater.Draw();
            foreach (Cone cone in mCones)
                cone.Draw();

            foreach (Agent agent in mAgents)
            {
                Vector2 pos = agent.CurrentPosition;
                agent.Draw();
                if(Core.XMLProperties.GlobalProperties.mDrawHealth)
                    mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y), Convert.ToString(agent.Health));
                //mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y), Convert.ToString(agent.mDodgeTime));
                //if (agent.InCombat())
                //    mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y), "C");

                //mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y), 
                //                            Convert.ToString(MathHelper.ToDegrees(agent.CurrentRotation)));
                //mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y),
                //                            Convert.ToString(agent.mNumTimesKnockedBack));                
                bool drawOrientation = false;
                if (drawOrientation)
                {
                    Vector2 vec =
                        new Vector2((float)Math.Cos(agent.CurrentRotation),
                                    (float)Math.Sin(agent.CurrentRotation));
                    vec *= -1;
                    vec *= 50.0f;
                    vec += agent.CurrentPosition;

                    mPrimitiveBatch.Begin(PrimitiveType.LineList);
                    mPrimitiveBatch.AddVertex(agent.CurrentPosition, Color.Yellow);
                    mPrimitiveBatch.AddVertex(vec, Color.Yellow);
                    mPrimitiveBatch.End();
                }

                if (Core.XMLProperties.GlobalProperties.mDrawAgentTargetLines && agent.HasTarget())
                {
                    mPrimitiveBatch.Begin(PrimitiveType.LineList);
                    mPrimitiveBatch.AddVertex(agent.CurrentPosition, Color.Black);
                    mPrimitiveBatch.AddVertex(agent.mCurrentTarget.CurrentPosition, Color.Black);
                    mPrimitiveBatch.End();
                }

                if( Core.XMLProperties.GlobalProperties.mDrawAgentStates)
                {
                    if (agent.mState == CombatState.Attacking)
                    {
                        mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y), "At");
                        if (agent.mAttackingState == AttackingState.GrandSlam)
                            mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y + 10), "GS");
                        if (agent.mAttackingState == AttackingState.SuperKnockBack)
                            mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y + 10), "SK");
                        if (agent.mAttackingState == AttackingState.StrikingIn)
                            mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y + 10), "SI");
                        if (agent.mAttackingState == AttackingState.StrikingOut)
                            mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y + 10), "SO");
                    }
                    if (agent.mState == CombatState.Dodging)
                        mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y), "Do");
                    if (agent.mState == CombatState.Idle)
                        mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y), "Id");
                    if (agent.mState == CombatState.KnockedBack)
                        mGame.TextOutputter.WriteAt((int)(pos.X), (int)(pos.Y), "Kn");
                }

                if (Core.XMLProperties.GlobalProperties.mDrawStunStatus)
                    if (agent.IsStunned())
                        mGame.TextOutputter.WriteAt((int)(pos.X - agent.Size / 2),
                                                    (int)(pos.Y - agent.Size / 2),
                                                    "S", Color.Blue);
                if (Core.XMLProperties.GlobalProperties.mDrawKnockBackStatus)
                    if (agent.IsKnockedBack())
                    {
                        Vector2 targetKnockBack = agent.GetTargetKnockBackPosition;
                        mGame.TextOutputter.WriteAt((int)targetKnockBack.X,
                                                    (int)targetKnockBack.Y, "X", Color.Red);
                        mGame.TextOutputter.WriteAt((int)(pos.X - agent.Size / 2),
                                                    (int)(pos.Y - agent.Size / 2),
                                                    "X", Color.Green);
                    }                           
            }
            mGame.StaticTextOutputter.WriteAt(5,5, "Zoom: " + Convert.ToString(Camera.Zoom.ToString()));     
        }

        public List<Agent> GetAgentsWithinDistance(Vector2 origin, float distance)
        {
            List<Agent> agents = new List<Agent>();
            foreach (Agent agent in mAgents)
            {
                float dist = Vector2.Distance(origin, agent.CurrentPosition);
                if (dist <= distance)
                    agents.Add(agent);
            }
            return agents;
        }
    }
}
