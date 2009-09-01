using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using System.Linq;
using System.Text;
using CombatPrototype.GameWorld;
using CombatPrototype.Core.XMLProperties;
namespace CombatPrototype.Core.Pathfinding
{
    class Pathfinder
    {
        List<PathNode> mPathNodes;
        public const int boundary = 20;

        GraphicsDeviceManager mGraphics;
        GameWorldCore mGameWorld;

        public Pathfinder(GraphicsDeviceManager g, GameWorldCore gwc)
        {
            mPathNodes = new List<PathNode>();
            mGraphics = g;
            mGameWorld = gwc;
            GenerateMap();
        }

        public void GenerateMap()
        {
            int width = Core.XMLProperties.GlobalProperties.mGridWidth;
            int height = Core.XMLProperties.GlobalProperties.mGridHeight;
            int distance = Core.XMLProperties.GlobalProperties.mDistanceBetweenNodes;
            for (int i = 0; i <= width; i += distance)
            {
                for (int j = 0; j <= height; j += distance)
                {
                    PathNode node = new PathNode(mGameWorld.Camera, mGraphics, i, j);
                    mPathNodes.Add(node);
                }
            }

            int distance2 = System.Convert.ToInt32(distance * 1.5);
            foreach (PathNode p1 in mPathNodes)
            {
                foreach (PathNode p2 in mPathNodes)
                {
                    if (p1 != p2 && Vector2.Distance(p1.mPosition, p2.mPosition) < distance2)
                    {
                        p1.mNeighbours.Add(p2);
                    }
                }
            }
        }

        private void ResetPathNodes()
        {
            foreach (PathNode node in mPathNodes)
            {
                node.mCurrentCost = 0;
                node.mCurrentHeuristic = 999;
                node.mCurrentParent = null;
            }
        }

        public List<Vector2> FindPath(Vector2 startPos, Vector2 endPos, int agentSize)
        {
            PathNode startNode = FindNearestNode(startPos);
            PathNode endNode = FindNearestNode(endPos);

            List<Vector2> path = GeneratePath(startNode, endNode);
            //OptimisePath(path, agentSize);

            startNode.mActive = false;
            startNode.marked = true;

            return path;
        }

        public List<Vector2> GeneratePath(PathNode startNode, PathNode endNode)
        {
            ResetPathNodes();

            List<Vector2> path = new List<Vector2>();

            if (startNode == null || endNode == null)
                return path;

            List<PathNode> closedList = new List<PathNode>();
            List<PathNode> openList = new List<PathNode>();

            float dist = Vector2.Distance(startNode.mPosition, endNode.mPosition);
            openList.Add(startNode);

            while (openList.Count > 0)
            {
                PathNode current = openList[0];
                openList.RemoveAt(0);

                if (current == endNode)
                {
                    while (current.mCurrentParent != null)
                    {
                        path.Add(current.mPosition);
                        current = current.mCurrentParent;
                    }
                    break;
                }

                foreach (PathNode neighbour in current.mNeighbours)
                {
                    float distFromTarget = Vector2.Distance(current.mPosition, endNode.mPosition);
                    float distFromSource = current.mCurrentCost + Vector2.Distance(current.mPosition, neighbour.mPosition);
                    float heuristic = current.mCurrentCost + distFromTarget;

                    int idxOpen = openList.IndexOf(neighbour);
                    if (idxOpen != -1 && openList[idxOpen].mCurrentCost < distFromSource)
                        continue;

                    int idxClosed = closedList.IndexOf(neighbour);
                    if (idxClosed != -1 && closedList[idxClosed].mCurrentCost < distFromSource)
                        continue;

                    if (idxOpen != -1)
                        openList.RemoveAt(idxOpen);

                    if (idxClosed != -1)
                        closedList.RemoveAt(idxClosed);

                    neighbour.mCurrentParent = current;
                    neighbour.mCurrentCost = distFromSource;
                    neighbour.mCurrentHeuristic = heuristic;

                    int insIdx = -1;
                    for (int i = 0; i < openList.Count; i++)
                    {
                        if (heuristic < openList[i].mCurrentHeuristic)
                        {
                            insIdx = i;
                            break;
                        }
                    }

                    if (insIdx == -1)
                    {
                        openList.Add(neighbour);
                    }
                    else
                        openList.Insert(insIdx, neighbour);
                }

                closedList.Add(current);
            }

            return path;
        }

        private void OptimisePath(List<Vector2> path, int agentSize)
        {
            int idx = 0;
            while (idx < path.Count - 2)
            {
                Vector3 nextPos = new Vector3(path[idx + 2].X, path[idx + 2].Y, 0);
                Vector3 currPos = new Vector3(path[idx].X, path[idx].Y, 0);

                Vector3 dir = currPos - nextPos;
                dir.Normalize();

                Ray ray = new Ray(nextPos, dir);

                bool collision = false;

                if (collision)
                    idx++;
                else
                    path.RemoveAt(idx + 1);
            }
        }

        public PathNode FindNearestNode(Vector2 pos)
        {
            float shortestDist = -1;
            PathNode nearest = null;

            float shortestDistFromMe = -1;

            foreach(PathNode node in mPathNodes)
            {
                float dist = Vector2.Distance(pos, node.mPosition);
                if (node.mActive )
                {
                    if ( shortestDist == -1 || dist < shortestDist )
                    {
                        nearest = node;
                        shortestDist = dist;

                        if(shortestDistFromMe == -1)
                            shortestDistFromMe = Vector2.Distance(pos, node.mPosition);
                    }
                    else if (dist == shortestDist)
                    {
                        dist = Vector2.Distance(pos, node.mPosition);
                        if (dist < shortestDistFromMe)
                        {
                            nearest = node;
                            shortestDistFromMe = dist;
                        }
                    }
                    
                }
            }

            return nearest;
        }

        public void Draw()
        {
            foreach (PathNode p in mPathNodes)
            {
                p.Draw();
            }
        }
    }
}