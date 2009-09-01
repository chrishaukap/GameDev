using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using CombatPrototype.Core.XMLProperties;

namespace CombatPrototype.GameWorld
{
    class Crater
    {
        Core.PrimitiveBatch mPrimitiveBatch;
        GameWorldCore mGameWorldCore;
        public float mRadius = 50.0f;
        public float mLargestRadius = 100.0f;
        public int mTimer = 100;
        public Vector2 mPosition;
        private const float mGrowthRate = 0.25f;        
        public Crater(GameWorldCore gwc, Vector2 pos, float radius)
        {
            mRadius = radius;
            mGameWorldCore = gwc;
            mPosition = pos;
            mPrimitiveBatch = new Core.PrimitiveBatch(gwc.Camera, mGameWorldCore.Graphics.GraphicsDevice);
        }

        public void Update()
        {
            --mTimer;
            //mRadius += mGrowthRate;
        }
        public bool HasExpired()
        { 
            if(mTimer <= 0) //crater.mRadius >= crater.mLargestRadius
                return true;
            return false;           
        }
        public void Draw()
        {
            if(Core.XMLProperties.GlobalProperties.mDrawGrandSlamCircles)
            {
                List<Vector2> vertexList = new List<Vector2>();
                int num_verts = 30;
                for (int i = 0; i < num_verts; i++)
                {
                    double angle = 3.14159 * 2 / num_verts * i;
                    Vector2 vec = new Vector2((float)Math.Sin(angle),
                                              (float)Math.Cos(angle));
                    vec *= mRadius;
                    vec += mPosition;
                    vertexList.Add(vec);
                }

                mPrimitiveBatch.Begin(PrimitiveType.LineList);
                for (int i = 0; i < num_verts-1; i++)
                {                       
                    mPrimitiveBatch.AddVertex(vertexList[i], Color.Blue);
                    mPrimitiveBatch.AddVertex(vertexList[i+1], Color.Blue);
                }
                mPrimitiveBatch.AddVertex(vertexList[0], Color.Blue);
                mPrimitiveBatch.AddVertex(vertexList[num_verts-1], Color.Blue);
                mPrimitiveBatch.End();
            }
        }
    }
}
