using System;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using CombatPrototype.Core.XMLProperties;
using CombatPrototype.Core.Pathfinding;

namespace CombatPrototype.GameWorld
{
    enum CombatState
    {
        Idle,
        Dodging,
        Attacking,
        KnockedBack,
        Casting
    }
    enum AttackingState
    {
        StrikingIn=1,
        StrikingOut,
        GrandSlam,
        Roar,
        Tornado,
        SuperKnockBack
    }
    #region Property Class Definitions
    class Roar
    {
        public Roar(int degrees,
                    float knockBackSpeed,
                    int duration,
                    int stunDuration,
                    int range)
        {
            mDegrees = degrees;
            mKnockBackSpeed = knockBackSpeed;
            mDuration = duration;
            mStunDuration = stunDuration;
            mRange = range;
        }
        public int mDegrees;
        public float mKnockBackSpeed;
        public int mDuration;
        public int mStunDuration;
        public int mRange;
    }
    class GrandSlam
    {
        public GrandSlam(float radius,
                         int damage,
                         int stunDuration,
                         float knockBackSpeed)
        {
            mRadius = radius;
            mDamage = damage;
            mStunDuration = stunDuration;
            mKnockBackSpeed = knockBackSpeed;
        }
        public float mRadius;
        public int mDamage;
        public int mStunDuration;
        public float mKnockBackSpeed;
    }
    class SuperKnockBack
    {
        public SuperKnockBack(float speed,
                              int damage)
        {
            mSpeed = speed;
            mDamage = damage;
        }
        public float mSpeed;
        public int mDamage;
    }
    class Tornado
    {
        public Tornado(float speed,
                       int duration,
                       int degrees,
                       int damage,
                       int range)
        {
            mRange = range;
            mSpeed = speed;
            mDuration = duration;
            mDegrees = degrees;
            mDamage = damage;
        }
        public int mRange;
        public float mSpeed;
        public int mDuration;
        public int mDegrees;
        public int mDamage;
    }
    class Properties
    {
        public Properties(Tornado tornado, SuperKnockBack superKnockBack,
                          GrandSlam grandSlam, Roar roar)
        {
            mTornado = tornado;
            mSuperKnockBack = superKnockBack;
            mGrandSlam = grandSlam;
            mRoar = roar;
        }
        public Tornado mTornado;
        public SuperKnockBack mSuperKnockBack;
        public GrandSlam mGrandSlam;
        public Roar mRoar;
    }

    #endregion

    class Agent
    {
        #region member variables
        private Properties mProperties;
        private Cone mSpellCone;
        protected Texture2D mTexture;
        protected float mScale;
        protected GameWorldCore mGameWorld;
        //protected PathNode mCurrentNode;
        public Agent mCurrentTarget;
        private Vector2 mCurrentDodgeTargetPos;
        private Vector2 mTargetKnockBackPosition;
        private bool mStunned;
        private float mAttackSpeed;
        private float mAttackTimer;
        private float mMoveSpeed;
        private int mCurrentStunDuration;
        private int mCurrentStunTime;
        private int mKnockBackTimer2;
        private int mKnockBackTimer1;
        private float mCurrentKnockBackSpeed;
        private int mDodgeTimer;
        public int mDodgeTime;
        private float mDodgeSpeed;
        private int mKnockBackTime;
        private int mHealth;
        protected Vector2 mCurrentPos;
        protected Vector2 mStrikePos;
        protected float mCurrentRot;
        protected bool mInCombat;
        public CombatState mState;
        public AttackingState mAttackingState;
        public int mNumTimesKnockedBack;
        #endregion
        
        public Agent(GameWorldCore gwc, Vector2 pos, float size)
        {
            #region init properties from properties.xml
            mProperties = 
                new Properties( 
                    new Tornado(
                        Core.XMLProperties.AgentProperties.mTornadoSpeed,
                        Core.XMLProperties.AgentProperties.mTornadoDuration,
                        Core.XMLProperties.AgentProperties.mTornadoDegrees,
                        Core.XMLProperties.AgentProperties.mTornadoDamage,
                        Core.XMLProperties.AgentProperties.mTornadoRange),
                    new SuperKnockBack(
                        Core.XMLProperties.AgentProperties.mSuperKnockBackSpeed,
                        Core.XMLProperties.AgentProperties.mSuperKnockBackDamage),
                    new GrandSlam(
                        Core.XMLProperties.AgentProperties.mGrandSlamRadius,
                        Core.XMLProperties.AgentProperties.mGrandSlamDamage,
                        Core.XMLProperties.AgentProperties.mGrandSlamStunDuration,
                        Core.XMLProperties.AgentProperties.mGrandSlamKnockBackSpeed),
                    new Roar(
                        Core.XMLProperties.AgentProperties.mRoarDegrees,
                        Core.XMLProperties.AgentProperties.mRoarKnockBackSpeed,
                        Core.XMLProperties.AgentProperties.mRoarDuration,
                        Core.XMLProperties.AgentProperties.mRoarStunDuration,
                        Core.XMLProperties.AgentProperties.mRoarRange));
            #endregion
            
            mScale = size;
            mGameWorld = gwc;
            mTexture = mGameWorld.mContent.Load<Texture2D>("agent");

            // Knockback properties
            mCurrentKnockBackSpeed = 0;
            mKnockBackTimer2 = 0;
            mKnockBackTimer1 = 0;
            
            mKnockBackTime = 0;
            mMoveSpeed = 2.0f;
            mDodgeSpeed = 2.0f;
            mCurrentPos = pos;
            mCurrentStunTime = 0;
            mCurrentStunDuration = 0;
            mStunned = false;

            Health = (int)(5 * (2*mScale));
            mAttackTimer = 0;
            mAttackSpeed = 1;

            mDodgeTimer = 100;
            mDodgeTime = 0;

            mState = CombatState.Idle;
        }

        #region PropertyAccessors
        public Roar RoarProperties { get { return mProperties.mRoar; } }
        public GrandSlam GrandSlamProperties { get { return mProperties.mGrandSlam; } }
        public SuperKnockBack SuperKnockBackProperties { get { return mProperties.mSuperKnockBack; } }
        public Tornado TornadoProperties { get { return mProperties.mTornado; } }
        public float CurrentRotation { get { return mCurrentRot; } }
        public Vector2 GetTargetKnockBackPosition { get { return mTargetKnockBackPosition; } }
        public int Size { get { return (int)(mTexture.Height * mScale); } }
        public int Health
        { 
            get { return mHealth; }
            set { mHealth = value; }
        }
        public Vector2 CurrentPosition { get { return mCurrentPos; } }
        public virtual bool IsEnemy() { return false; }
        #endregion

        #region State Accessors
        public bool IsCasting() { return mState == CombatState.Casting; }
        public bool IsDodging() { return mState == CombatState.Dodging; }
        public bool IsStunned() { return mStunned; }
        private bool IsTargetDodging() { Debug.Assert(HasTarget()); return mCurrentTarget.IsDodging(); }
        private bool IsTargetAlive() { Debug.Assert(HasTarget()); return mCurrentTarget.Health > 0; }
        public bool HasTarget() { return mCurrentTarget != null; }
        private bool IsTargetAttacking() { Debug.Assert(HasTarget()); return mCurrentTarget.IsAttacking(); }
        public bool IsIdle() { return mState == CombatState.Idle; }
        public bool IsAttacking() { return mState == CombatState.Attacking; }
        public bool IsKnockedBack() { return mState == CombatState.KnockedBack; }
        public bool InCombat() { return mInCombat; }
       
        #endregion

        #region State Processing
        public  void UpdateKnockBack()
        {
            if (IsKnockedBack())
            {
                ++mKnockBackTime;
                float speed = mCurrentKnockBackSpeed;
                if (mKnockBackTime >= mKnockBackTimer2)
                {
                    mKnockBackTime = 0;
                    mState = CombatState.Idle;
                    return;
                }
                else if (mKnockBackTime >= mKnockBackTimer1)
                    speed = mCurrentKnockBackSpeed * 0.40f;
                MoveToPosition(mTargetKnockBackPosition, speed);
            }
        }
        public virtual void Update()
        {
            if (IsCasting())
            {
                Debug.Assert(mSpellCone != null);
                if (mSpellCone.HasExpired())
                    mState = CombatState.Idle;
                else
                    return;
            }

            if (HasTarget() && !IsTargetAlive())
                mCurrentTarget = null;
            if (!HasTarget())
                mInCombat = false;

            UpdateDodgeTimer();
            if (IsStunned())
                if (++mCurrentStunTime > mCurrentStunDuration)
                {
                    mState = CombatState.Idle;
                    mStunned = false;
                }

            UpdateKnockBack();

            if (mInCombat)
            {
                Debug.Assert(HasTarget());

                CombatUpdate();
            }
            else if(!IsStunned())
            {
                mCurrentTarget = FindNearestEnemy();
                if (!HasTarget())
                    return;

                PathfindingUpdate();
            }
        }

        protected void MoveCloserToTargetAndRotate()
        {
            MoveToPosition(mCurrentTarget.CurrentPosition, mMoveSpeed);
            RotateToFace(mCurrentTarget.CurrentPosition);            
        }
        protected void PathfindingUpdate()
        {
            if (!IsWithinRangeOfTarget(mCurrentTarget.CurrentPosition, Size * 2))
                MoveCloserToTargetAndRotate();
            else
            {
                mState = CombatState.Idle;
                mInCombat = true;
            }
        }
        private bool IsWithinRangeOfTarget(Vector2 target, int range)
        {
            return Vector2.Distance(target, mCurrentPos) <= range;
        }
        private void MoveToPosition(Vector2 pos, float speed)
        {                
            Vector2 targetVel = new Vector2(pos.X - mCurrentPos.X, pos.Y - mCurrentPos.Y);
            targetVel /= 10;

            Vector2 dispersionVel = GetDispersionVector();
            targetVel += dispersionVel;

            Vector2 vel = (targetVel / targetVel.Length()) * speed;
            mCurrentPos = mCurrentPos + vel;
        }
        private Vector2 GetDispersionVector()
        {
            Vector2 vec = Vector2.Zero;
            foreach (Agent Agent in mGameWorld.mAgents)
            {
                // HAUKAP - removed the strike position distance calculation because it was causing jittering
                float dist = Vector2.Distance(mCurrentPos, Agent.mCurrentPos);
                //float dist2 = Vector2.Distance(mCurrentPos, Agent.mStrikePos);

                if (Agent != this)
                {
                    if (dist < Size)
                        vec -= (Agent.CurrentPosition - mCurrentPos);

                    //if (dist2 < Height)
                    //    vec -= (Agent.mStrikePos - mCurrentPos);
                }
            }

            Vector2 perpVec = new Vector2(vec.Y, -vec.X);
            vec += perpVec;

            return vec;
        }
        
        #endregion

        #region Combat Processing

        private void ExecuteRoar()
        {
            List<Agent> agents = FindEnemiesWithinDistance(RoarProperties.mRange);
            if (agents.Count > 0)
                mSpellCone = mGameWorld.AddRoarCone(this);
            foreach (Agent agent in agents)
            {
                Debug.Assert(this != agent);
                if (mSpellCone.IsAgentWithin(agent))
                {
                    agent.CheckDodge(CurrentPosition);
                    if (!agent.IsDodging())
                        agent.Stun(RoarProperties.mStunDuration);
                }
            }

            mCurrentTarget = null;
            mState = CombatState.Casting;
        }
        private void ExecuteTornado()
        {
            List<Agent> agents = FindEnemiesWithinDistance(RoarProperties.mRange);
            if (agents.Count > 0)
                mSpellCone = mGameWorld.AddTornadoCone(this);
            foreach (Agent agent in agents)
            {
                Debug.Assert(this != agent);
                if (mSpellCone.IsAgentWithin(agent))
                {
                    agent.CheckDodge(CurrentPosition);
                    if (!agent.IsDodging())
                        agent.Stun(RoarProperties.mStunDuration);
                }
            }

            mCurrentTarget = null;
            mState = CombatState.Casting;
        }
        private void CheckStriking()
        {
            switch (mAttackingState)
            {
                case AttackingState.StrikingIn:
                    ExecuteStrikeIn();
                    break;
                case AttackingState.Tornado:
                    ExecuteTornado();
                    break;
                case AttackingState.StrikingOut:
                    ExecuteStrikeOut();
                    break;
                case AttackingState.GrandSlam:
                    ExecuteGrandSlam();
                    break;
                case AttackingState.SuperKnockBack:
                    ExecuteSuperKnockBack();
                    break;
                case AttackingState.Roar:
                    ExecuteRoar();
                    break;
                default:
                    Debug.Assert(false);
                    break;
            }
        }
        private void ExecuteStrikeIn()
        {
            if (IsTargetAlive())
            {
                mStrikePos = CurrentPosition;
                if (!IsWithinRangeOfTarget(mCurrentTarget.CurrentPosition, Size))
                    MoveTowardTargetAgent();
                else
                {
                    mCurrentTarget.CheckDodge(CurrentPosition);
                    if (!IsTargetDodging())
                        ApplyDamageToTarget(1);
                    mAttackingState = AttackingState.StrikingOut;
                }
            }
            else
            {
                mAttackingState = AttackingState.StrikingOut;
            }        
        }
        private void ExecuteStrikeOut()
        {
            if (IsWithinRangeOfTarget(mCurrentTarget.CurrentPosition, Size * 2))
                MoveAwayFromTargetAgent();
            else
            {
                mStrikePos = Vector2.Zero;
                mState = CombatState.Idle;
                mInCombat = false;
            }
        }
        private void ExecuteGrandSlam()
        {
            if (IsTargetAlive())
            {
                mGameWorld.AddCrater(CurrentPosition, GrandSlamProperties.mRadius);
                List<Agent> agents = FindEnemiesWithinDistance(Core.XMLProperties.AgentProperties.mGrandSlamRadius);
                foreach (Agent agent in agents)
                {
                    Debug.Assert(this != agent);
                    agent.CheckDodge(CurrentPosition);
                    if (agent.mState != CombatState.Dodging)
                    {
                        agent.KnockBack(CurrentPosition, GrandSlamProperties.mKnockBackSpeed);
                        agent.Stun(GrandSlamProperties.mStunDuration);
                        agent.Health -= GrandSlamProperties.mDamage;
                    }
                }
            }
            else
                mInCombat = false;

            mState = CombatState.Idle;
            ResetAttackTimer(false);
        }
        private void ExecuteSuperKnockBack()
        {
            if (IsTargetAlive())
            {
                if (!IsWithinRangeOfTarget(mCurrentTarget.CurrentPosition, Size))
                    MoveTowardTargetAgent();
                else
                {
                    mCurrentTarget.CheckDodge(mCurrentPos);
                    if (!IsTargetDodging())
                    {
                        mCurrentTarget.KnockBack(CurrentPosition, SuperKnockBackProperties.mSpeed);
                        mCurrentTarget.Health -= SuperKnockBackProperties.mDamage;
                    }
                    mState = CombatState.Idle;
                    ResetAttackTimer(false);
                }
            }
            else
            {
                mInCombat = false;
                mState = CombatState.Idle;
                ResetAttackTimer(false);
            }
        }
        private AttackingState DetermineAttackType()
        {
            //if (IsEnemy())
            //    return AttackingState.Tornado;
            //else return AttackingState.StrikingIn;
            int randValue = mGameWorld.mRandom.Next(1, 1000);
            if (randValue >= 983)
                return AttackingState.GrandSlam;
            else if (randValue >= 966)
                return AttackingState.Roar;
            else if (randValue >= 950)
                return AttackingState.Tornado;
            else if (randValue >= 700)
                return AttackingState.SuperKnockBack;
            else
                return AttackingState.StrikingIn;
        }
        protected void CombatUpdate()
        {
            Debug.Assert(HasTarget());
            Debug.Assert(IsEnemy() != mCurrentTarget.IsEnemy());
            RotateToFace(mCurrentTarget.CurrentPosition);

            if (IsDodging())
            {                
                if (!IsStunned())
                {
                    MoveToPosition(mCurrentDodgeTargetPos, mDodgeSpeed);
                    if (IsWithinRangeOfTarget(mCurrentDodgeTargetPos, Size))
                        mState = CombatState.Idle;
                }
            }
            else if (IsKnockedBack())
            {
            }
            else if (IsWithinRangeOfTarget(mCurrentTarget.CurrentPosition, Size * 2))
            {
                if (IsIdle() && !IsStunned())
                {
                    mAttackTimer -= mAttackSpeed;

                    if (mAttackTimer <= 0.0f &&
                        !IsTargetAttacking())
                    {
                        ResetAttackTimer(false);
                        mState = CombatState.Attacking;
                        mAttackingState = DetermineAttackType();
                    }
                }
            }
            else
            {
                mState = CombatState.Idle;
                mInCombat = false;
            }

            if(IsAttacking())
                CheckStriking();
        }
        
        #endregion

        #region Helper Functions

        private void UpdateDodgeTimer()
        { 
            if (++mDodgeTime >= mDodgeTimer)
                mDodgeTime = 0;
        }
        public void KnockBack(Vector2 origin, float speed )
        {
            ++mNumTimesKnockedBack;
            mState = CombatState.KnockedBack;

            Vector2 knockBackVec = (mCurrentPos - origin);
            knockBackVec.Normalize();
            knockBackVec *= 9999;
            mCurrentKnockBackSpeed = speed;
            mTargetKnockBackPosition = mCurrentPos + knockBackVec;
            mKnockBackTime = 0;

            mKnockBackTimer1 = System.Convert.ToInt32(mCurrentKnockBackSpeed * 3);
            mKnockBackTimer2 = System.Convert.ToInt32(mKnockBackTimer1 * 3);
        }

        private void CheckDodge(Vector2 origin)
        {
            int rand = mGameWorld.mRandom.Next(2);
            if ( rand == 0 && mDodgeTime >= mDodgeTimer)
            {
                mDodgeTime = 0;
                Vector2 retreatDir = (mCurrentPos - origin);
                Vector2 perpVec = new Vector2(retreatDir.Y, -retreatDir.X);

                List<Vector2> retreatVecs = new List<Vector2>();

                CheckVector(retreatVecs, perpVec * 3);
                CheckVector(retreatVecs, perpVec * -3);
                CheckVector(retreatVecs, retreatDir * 3);
                CheckVector(retreatVecs, retreatDir * 3 + perpVec * 3);
                CheckVector(retreatVecs, retreatDir * 3 + perpVec * -3);

                if (retreatVecs.Count > 0)
                {
                    rand = mGameWorld.mRandom.Next(retreatVecs.Count);
                    mCurrentDodgeTargetPos = mCurrentPos + retreatVecs[rand];

                    mState = CombatState.Dodging;
                    ResetAttackTimer(true);
                }
            }
        }
        private void CheckVector(List<Vector2> retreatVecs, Vector2 vec)
        {
            Vector2 normalised = vec;
            normalised.Normalize();
            Vector3 currentPos3D = new Vector3(mCurrentPos.X + normalised.X, mCurrentPos.Y + normalised.Y, 0);
            Vector3 dir3D = new Vector3(vec.X, vec.Y, 0);

            foreach( Agent agent in mGameWorld.mAgents)
            {
                if(agent == this || agent.IsEnemy() == IsEnemy())
                    continue;

                Vector3 enemyPos3D = new Vector3(agent.mCurrentPos.X, agent.mCurrentPos.Y, 0);

                if (IsWithinRangeOfTarget(agent.CurrentPosition, Size * 2))
                {
                    Ray ray = new Ray(currentPos3D, dir3D);
                    BoundingSphere sphere = new BoundingSphere(enemyPos3D, Size * 0.75f);
                    float? intersect = ray.Intersects(sphere);

                    if (intersect != null)
                        return;
                }
            }
            
            retreatVecs.Add(vec);
        }

        private void ResetAttackTimer(bool halved)
        {
            if (Core.XMLProperties.GlobalProperties.mFastCombat)
            {
                if (halved)
                    mAttackTimer = 25;
                else
                    mAttackTimer = 50;
            }
            else
            {
                if (halved)
                    mAttackTimer = 100;
                else
                    mAttackTimer = 200;
            }
        }
        private Vector2 ApplyVelocity(Vector2 targetVel, float speed)
        {
            targetVel /= 10;
            return (targetVel / targetVel.Length()) * speed;
        }
        private void MoveTowardTargetAgent()
        {
            Debug.Assert(HasTarget());
            Vector2 targetVel = new Vector2(mCurrentTarget.mCurrentPos.X - mCurrentPos.X,
                                            mCurrentTarget.mCurrentPos.Y - mCurrentPos.Y);
            mCurrentPos += ApplyVelocity(targetVel, mMoveSpeed);
        }
        private void MoveAwayFromTargetAgent()
        {
            Vector2 targetVel = new Vector2(CurrentPosition.X - mCurrentTarget.CurrentPosition.X,
                                            CurrentPosition.Y - mCurrentTarget.CurrentPosition.Y);
            mCurrentPos += ApplyVelocity(targetVel, mMoveSpeed );
        }
        private void ApplyDamageToTarget(int damage)
        {
            Debug.Assert(HasTarget());
            mCurrentTarget.Health -= damage;
        }
        
        private void RotateToFace(Vector2 pos)
        {
            float oldRotation = mCurrentRot;
            float newRotation = (float)System.Math.Atan2(mCurrentPos.Y - pos.Y, mCurrentPos.X - pos.X);
            if (newRotation < 0) newRotation += MathHelper.TwoPi;

            if (oldRotation > MathHelper.PiOver4 && newRotation < -MathHelper.PiOver4)
                mCurrentRot = -MathHelper.Pi + 0.1f;
            else if (newRotation > MathHelper.PiOver4 && oldRotation < -MathHelper.PiOver4)
                mCurrentRot = MathHelper.Pi - 0.1f;
            else if (oldRotation - newRotation > 0.1f)
                mCurrentRot -= 0.1f;
            else if (newRotation - oldRotation > 0.1f)
                mCurrentRot += 0.1f;
        }
        #endregion

        private void Stun(int duration) { mStunned = true; mCurrentStunTime = 0; mCurrentStunDuration = duration; }
        

        #region Pathfinding Code
        /*private void SetCurrentNode(PathNode node)
        {
            if(mCurrentNode != null)
                mCurrentNode.mActive = true;

            mCurrentNode = node;
            mCurrentNode.mActive = false;
        }*/
        /*
        private void CheckCurrentNode()
        {
            if (mCurrentNode == null)
            {
                SetCurrentNode(mGameWorld.mPathfinder.FindNearestNode(mCurrentTarget.mCurrentPos));
                return;
            }

            float oldDist = Vector2.Distance(mCurrentNode.mPosition, mCurrentTarget.CurrentPosition);
            PathNode newNode = mGameWorld.mPathfinder.FindNearestNode(mCurrentTarget.mCurrentPos);
            float newDist = Vector2.Distance(mCurrentNode.mPosition, newNode.mPosition);

            if (newDist < oldDist)
            {
                SetCurrentNode(newNode);
            }
        }*/


        #endregion
        // HAUKAP - not currently used (all pathfinding and node finding stuff not used).
        /*private void MoveToCurrentNode()
        {
            MoveToPosition(mCurrentNode.mPosition, mMoveSpeed);
        }*/

        #region Rendering
        public void Draw()
        {
            Vector2 origin = new Vector2(mTexture.Width / 2, mTexture.Height / 2);

            mGameWorld.mSpriteBatch.Begin(SpriteBlendMode.AlphaBlend, 
                                          SpriteSortMode.Deferred, 
                                          SaveStateMode.SaveState,
                                          mGameWorld.Camera.ViewTransformationMatrix());
            mGameWorld.mSpriteBatch.Draw(mTexture, mCurrentPos, null, 
                                         Color.White, mCurrentRot, origin,
                                         mScale, SpriteEffects.None, 0.0f);
            mGameWorld.mSpriteBatch.End();
        }
        #endregion
        

        #region World Querying Methods
        protected Agent FindNearestEnemy()
        {
            Agent closestAgent = null;
            float closestDist = 0;
            foreach (Agent agent in mGameWorld.mAgents)
            {
                if( agent != this && agent.IsEnemy() != IsEnemy())
                {
                    float dist = Vector2.Distance(mCurrentPos, agent.mCurrentPos);

                    if (closestAgent == null || dist < closestDist)
                    {
                        closestDist = dist;
                        closestAgent = agent;
                    }
                }
            }

            return closestAgent;
        }
        protected List<Agent> FindEnemiesWithinDistance(float distance)
        {
            List<Agent> possibleAgents = mGameWorld.GetAgentsWithinDistance(CurrentPosition, distance);
            List<Agent> retAgents = new List<Agent>();
            foreach (Agent agent in possibleAgents)
            {
                if (agent != this && agent.IsEnemy() != IsEnemy())
                    retAgents.Add(agent);
            }
            return retAgents;
        }
        #endregion
    }
}
