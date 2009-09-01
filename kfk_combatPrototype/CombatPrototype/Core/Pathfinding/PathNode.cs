using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework.Graphics;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using CombatPrototype.Core.XMLProperties;
namespace CombatPrototype.Core.Pathfinding
{
    class PathNode
    {
        public Vector2 mPosition;
        public List<PathNode> mNeighbours;
        public bool mActive;
        public bool marked;

        public float mCurrentCost;
        public float mCurrentHeuristic;
        public PathNode mCurrentParent;

        PrimitiveBatch mPrimitiveBatch;
        GraphicsDeviceManager mGraphics;

        public PathNode(XNA2DCamera camera, GraphicsDeviceManager g, int x, int y)
        {
            mPosition.X = x;
            mPosition.Y = y;
            mNeighbours = new List<PathNode>();
            mActive = true;
            marked = false;

            mGraphics = g;
            mPrimitiveBatch = new PrimitiveBatch(camera, mGraphics.GraphicsDevice);
        }

        public void DisableNode()
        {
            mActive = false;

            foreach (PathNode n in mNeighbours)
                n.mNeighbours.Remove(this);

            mNeighbours.Clear();
        }

        public void Draw()
        {
            if (mNeighbours.Count >= 1)
            {
                if (Core.XMLProperties.GlobalProperties.mDrawGrid)
                {
                    mPrimitiveBatch.Begin(PrimitiveType.LineList);

                    foreach (PathNode p in mNeighbours)
                    {
                        mPrimitiveBatch.AddVertex(mPosition, Color.Red);
                        mPrimitiveBatch.AddVertex(p.mPosition, Color.Red);
                    }

                    mPrimitiveBatch.End();
                }

            }
        }
    }
}