using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Collections.Generic;
using System.Xml;

namespace CombatPrototype.Core.XMLProperties
{
    public static class XMLPropNames
    {
        static public string strPathFindingProperties = "PathFindingProperties";
        static public string strGridWidth = "GridWidth";
        static public string strGridHeight = "GridHeight";
        static public string strDistanceBetweenNoes = "DistanceBetweenNodes";
        static public string strDrawGrid = "DrawGrid";
        static public string strDrawHealth = "DrawHealth";
        static public string strDrawStunStatus = "DrawStunStatus";
        static public string strDrawKnockBackStatus = "DrawKnockBackStatus";
        static public string strDrawGrandSlamCircles = "DrawGrandSlamCircles";
        static public string strGame = "Game";
        static public string strGlobalProperties = "GlobalProperties";
        static public string strName = "Name";
        static public string strDebugProperties = "DebugProperties";
        static public string strTrue = "true";
        static public string strFalse = "false";
        static public string strAgentProperties = "AgentProperties";
        static public string strAbility = "Ability";
        static public string strRadius = "Radius";
        static public string strDamage = "Damage";
        static public string strStunDuration = "StunDuration";
        static public string strGrandSlam = "GrandSlam";
        static public string strSuperKnockBack = "SuperKnockBack";
        static public string strSpeed = "Speed";
        static public string strFastCombat = "FastCombat";
        static public string strDrawAgentTargetLines = "DrawAgentTargetLines";
        static public string strDrawAgentStates = "DrawAgentStates";
        static public string strDegrees = "Degrees";
        static public string strKnockBackSpeed = "KnockBackSpeed";
        static public string strDuration = "Duration";
        static public string strRange = "Range";
        static public string strRoar = "Roar";
        static public string strInitialization = "Initialization";
        static public string strRandomSkirmishes = "RandomSkirmishes";
        static public string strMaxAgentsPerTeam = "MaxAgentsPerTeam";
        static public string strMinAgentsPerTeam = "MinAgentsPerTeam";
        static public string strMaxPlacementWidth = "MaxPlacementWidth";
        static public string strMaxPlacementHeight = "MaxPlacementHeight";
        static public string strRandom = "Random";
        static public string strOpposingSides = "OpposingSides";
        static public string strStar = "Star";
        static public string strSetupOption = "SetupOption";
        static public string strVariableSize = "VariableSize";
        static public string strTornado = "Tornado";
    }
    public static class GlobalProperties
    {
        public static bool mDrawGrid;
        public static bool mDrawHealth;
        public static bool mDrawStunStatus;
        public static bool mDrawKnockBackStatus;
        public static bool mDrawGrandSlamCircles;
        public static bool mDrawAgentTargetLines;
        public static bool mDrawAgentStates;
        public static int mGridWidth;
        public static int mGridHeight;
        public static int mDistanceBetweenNodes;
        public static bool mFastCombat;
        public static void ReadDebugProperties(ref XmlTextReader reader)
        {
            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    if (reader.Name == XMLPropNames.strDrawGrid)
                        mDrawGrid = reader.ReadElementContentAsBoolean();
                    else if (reader.Name == XMLPropNames.strDrawHealth)
                        mDrawHealth = reader.ReadElementContentAsBoolean();
                    else if (reader.Name == XMLPropNames.strDrawStunStatus)
                        mDrawStunStatus = reader.ReadElementContentAsBoolean();
                    else if (reader.Name == XMLPropNames.strDrawKnockBackStatus)
                        mDrawKnockBackStatus = reader.ReadElementContentAsBoolean();
                    else if (reader.Name == XMLPropNames.strDrawGrandSlamCircles)
                        mDrawGrandSlamCircles = reader.ReadElementContentAsBoolean();
                    else if (reader.Name == XMLPropNames.strFastCombat)
                        mFastCombat = reader.ReadElementContentAsBoolean();
                    else if (reader.Name == XMLPropNames.strDrawAgentTargetLines)
                        mDrawAgentTargetLines = reader.ReadElementContentAsBoolean();
                    else if (reader.Name == XMLPropNames.strDrawAgentStates)
                        mDrawAgentStates = reader.ReadElementContentAsBoolean();
                }
                else if (reader.NodeType == XmlNodeType.EndElement &&
                         reader.Name == XMLPropNames.strDebugProperties)
                    break;
            }
        }
        public static void ReadPathFindingProperties(ref XmlTextReader reader)
        {
            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    if (reader.Name == XMLPropNames.strGridHeight)
                        mGridHeight = reader.ReadElementContentAsInt();
                    else if (reader.Name == XMLPropNames.strGridWidth)
                        mGridWidth = reader.ReadElementContentAsInt();
                    else if (reader.Name == XMLPropNames.strDistanceBetweenNoes)
                        mDistanceBetweenNodes = reader.ReadElementContentAsInt();
                }
                else if (reader.NodeType == XmlNodeType.EndElement &&
                         reader.Name == XMLPropNames.strPathFindingProperties)
                    break;
            }
        }
        public static void ReadFromXML(ref XmlTextReader reader)
        {
            while (reader.Read())
            {
                if (reader.Name == XMLPropNames.strPathFindingProperties)
                    ReadPathFindingProperties(ref reader);
                else if(reader.Name == XMLPropNames.strDebugProperties)
                    ReadDebugProperties(ref reader);
                else if (reader.NodeType == XmlNodeType.EndElement &&
                         reader.Name == XMLPropNames.strGlobalProperties)
                    break;
            }
        }
    }
    public static class AgentProperties
    {
        public static bool mVariableSize;
        public static float mGrandSlamRadius;
        public static int mGrandSlamDamage;
        public static int mGrandSlamStunDuration;
        public static float mGrandSlamKnockBackSpeed;
        public static float mSuperKnockBackSpeed;
        public static int mSuperKnockBackDamage;
        public static int mRoarDegrees;
        public static float mRoarKnockBackSpeed;
        public static int mRoarDuration;
        public static int mRoarStunDuration;
        public static int mRoarRange;
        public static string mSetupOption;
        public static int mMaxAgentsPerTeam;
        public static int mMinAgentsPerTeam;
        public static int mMaxPlacementWidth;
        public static int mMaxPlacementHeight;
        public static float mTornadoSpeed;
        public static int mTornadoDuration;
        public static int mTornadoDegrees;
        public static int mTornadoDamage;
        public static int mTornadoRange;

        public static void ReadFromXML(ref XmlTextReader reader)
        {
            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    if (reader.Name == XMLPropNames.strAbility)
                    {
                        if (XMLPropNames.strGrandSlam == reader.GetAttribute(XMLPropNames.strName))
                            ReadGrandSlamAbility(ref reader);
                        else if (XMLPropNames.strSuperKnockBack == reader.GetAttribute(XMLPropNames.strName))
                            ReadSuperKnockBackAbility(ref reader);
                        else if (XMLPropNames.strRoar == reader.GetAttribute(XMLPropNames.strName))
                            ReadRoarAbility(ref reader);
                        else if (XMLPropNames.strTornado == reader.GetAttribute(XMLPropNames.strName))
                            ReadTornadoAbility(ref reader);
                    }
                    else if (reader.Name == XMLPropNames.strInitialization)
                        ReadInitialization(ref reader);
                    else if (reader.Name == XMLPropNames.strVariableSize)
                        mVariableSize = reader.ReadElementContentAsBoolean();
                }
                else if (reader.NodeType == XmlNodeType.EndElement &&
                         reader.Name == XMLPropNames.strAgentProperties)
                    break;
            }
        }
        public static void ReadStar(ref XmlTextReader reader)
        {
        }
        public static void ReadOpposingSides(ref XmlTextReader reader)
        {
        }
        public static void ReadRandom(ref XmlTextReader reader)
        {
            while (reader.Read())
            {
                if (reader.Name == XMLPropNames.strMaxAgentsPerTeam)
                    mMaxAgentsPerTeam = reader.ReadElementContentAsInt();
                else if (reader.Name == XMLPropNames.strMinAgentsPerTeam)
                    mMinAgentsPerTeam = reader.ReadElementContentAsInt();
                else if (reader.Name == XMLPropNames.strMaxPlacementWidth)
                    mMaxPlacementWidth = reader.ReadElementContentAsInt();
                else if (reader.Name == XMLPropNames.strMaxPlacementHeight)
                    mMaxPlacementHeight = reader.ReadElementContentAsInt();
                else if (reader.NodeType == XmlNodeType.EndElement &&
                         reader.Name == XMLPropNames.strRandom)
                    break;
            }                    
        }
        public static void ReadRandomSkirmishes(ref XmlTextReader reader)
        {   
        }
        public static void ReadInitialization(ref XmlTextReader reader)
        {
            while (reader.Read())
            {
                if (reader.Name == XMLPropNames.strSetupOption)
                    mSetupOption = reader.ReadElementContentAsString();
                else if (reader.Name == XMLPropNames.strRandom)
                    ReadRandom(ref reader);
                else if (reader.Name == XMLPropNames.strRandomSkirmishes)
                    ReadRandomSkirmishes(ref reader);
                else if (reader.Name == XMLPropNames.strOpposingSides)
                    ReadOpposingSides(ref reader);
                else if (reader.Name == XMLPropNames.strStar)
                    ReadStar(ref reader);
                else if (reader.NodeType == XmlNodeType.EndElement &&
                         reader.Name == XMLPropNames.strInitialization)
                    break;
            }                    
        }
        public static void ReadRoarAbility(ref XmlTextReader reader)
        {
            while (reader.Read())
            {
                if (reader.Name == XMLPropNames.strDegrees)
                    mRoarDegrees = reader.ReadElementContentAsInt();
                else if (reader.Name == XMLPropNames.strKnockBackSpeed)
                    mRoarKnockBackSpeed = reader.ReadElementContentAsFloat();
                else if (reader.Name == XMLPropNames.strDuration)
                    mRoarDuration = reader.ReadElementContentAsInt();
                else if (reader.Name == XMLPropNames.strStunDuration)
                    mRoarStunDuration = reader.ReadElementContentAsInt();
                else if (reader.Name == XMLPropNames.strRange)
                    mRoarRange = reader.ReadElementContentAsInt();
                else if (reader.NodeType == XmlNodeType.EndElement &&
                         reader.Name == XMLPropNames.strAbility)
                    break;
            }            
        }
        public static void ReadSuperKnockBackAbility(ref XmlTextReader reader)
        {
            while (reader.Read())
            {
                if (reader.Name == XMLPropNames.strSpeed)
                    mSuperKnockBackSpeed = reader.ReadElementContentAsFloat();
                else if (reader.Name == XMLPropNames.strDamage)
                    mSuperKnockBackDamage = reader.ReadElementContentAsInt();
                else if (reader.NodeType == XmlNodeType.EndElement &&
                         reader.Name == XMLPropNames.strAbility)
                    break;
            }
        }
        public static void ReadGrandSlamAbility(ref XmlTextReader reader)
        {            
            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    if (reader.Name == XMLPropNames.strRadius)
                        mGrandSlamRadius = reader.ReadElementContentAsFloat();
                    else if (reader.Name == XMLPropNames.strDamage)
                        mGrandSlamDamage = reader.ReadElementContentAsInt();
                    else if (reader.Name == XMLPropNames.strStunDuration)
                        mGrandSlamStunDuration = reader.ReadElementContentAsInt();
                    else if (reader.Name == XMLPropNames.strKnockBackSpeed)
                        mGrandSlamKnockBackSpeed = reader.ReadElementContentAsFloat();
                }
                else if (reader.NodeType == XmlNodeType.EndElement &&
                         reader.Name == XMLPropNames.strAbility)
                    break;
            }
        }
        public static void ReadTornadoAbility(ref XmlTextReader reader)
        {
            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    if (reader.Name == XMLPropNames.strSpeed)
                        mTornadoSpeed = reader.ReadElementContentAsFloat();
                    else if (reader.Name == XMLPropNames.strDuration)
                        mTornadoDuration = reader.ReadElementContentAsInt();
                    else if (reader.Name == XMLPropNames.strDegrees)
                        mTornadoDegrees = reader.ReadElementContentAsInt();
                    else if (reader.Name == XMLPropNames.strDamage)
                        mTornadoDamage = reader.ReadElementContentAsInt();
                    else if (reader.Name == XMLPropNames.strRange)
                        mTornadoRange = reader.ReadElementContentAsInt();
                }
                else if (reader.NodeType == XmlNodeType.EndElement &&
                         reader.Name == XMLPropNames.strAbility)
                    break;
            }
        }
    }
    static public class XMLPropertyReader
    {
        static public void ReadXML()
        {
            FileStream fs = new FileStream("../../../properties.xml", FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
            XmlTextReader reader = new XmlTextReader(fs);
            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    if (reader.Name == XMLPropNames.strGlobalProperties)
                        GlobalProperties.ReadFromXML(ref reader);
                    else if (reader.Name == XMLPropNames.strAgentProperties)
                        AgentProperties.ReadFromXML(ref reader);
                }
            }
        }
        static public void WriteXML()
        {
            FileStream fs = new FileStream("../../../properties.xml", FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
            XmlTextWriter writer = new XmlTextWriter(fs, null);
            writer.Formatting = Formatting.Indented;
            writer.WriteStartDocument();
            writer.WriteComment("Combat Prototype configurable properties");
            writer.WriteComment("Created On: " + DateTime.Now.ToString("dd-MMM-yyyy"));
            writer.WriteComment("===============================");
            writer.WriteStartElement(XMLPropNames.strGame);
            writer.WriteAttributeString(XMLPropNames.strName, "Combat Prototype");
            {
                writer.WriteStartElement(XMLPropNames.strGlobalProperties);
                {
                    writer.WriteStartElement(XMLPropNames.strDebugProperties);
                    {
                        writer.WriteElementString(XMLPropNames.strDrawGrid, XMLPropNames.strTrue);
                        writer.WriteElementString(XMLPropNames.strDrawHealth, XMLPropNames.strTrue);
                        writer.WriteElementString(XMLPropNames.strDrawGrandSlamCircles, XMLPropNames.strTrue);
                        writer.WriteElementString(XMLPropNames.strDrawStunStatus, XMLPropNames.strTrue);
                        writer.WriteElementString(XMLPropNames.strDrawKnockBackStatus, XMLPropNames.strTrue);
                    }
                    writer.WriteEndElement();
                }
                writer.WriteEndElement();
                writer.WriteStartElement(XMLPropNames.strAgentProperties);
                {
                    writer.WriteStartElement(XMLPropNames.strAbility);
                    writer.WriteAttributeString(XMLPropNames.strName, XMLPropNames.strGrandSlam);
                    {
                        writer.WriteElementString(XMLPropNames.strRadius, "14");
                        writer.WriteElementString(XMLPropNames.strDamage, "2");
                        writer.WriteElementString(XMLPropNames.strStunDuration, "40");
                    }
                    writer.WriteEndElement();
                    writer.WriteStartElement(XMLPropNames.strAbility);
                    writer.WriteAttributeString(XMLPropNames.strName, XMLPropNames.strSuperKnockBack);
                    {
                    }
                    writer.WriteEndElement();
                    writer.WriteStartElement(XMLPropNames.strAbility);
                    writer.WriteAttributeString(XMLPropNames.strName, "CircleKick");
                    { }
                    writer.WriteEndElement();
                }
                writer.WriteEndElement();
            }
            writer.WriteEndElement();
            writer.WriteEndDocument();
            writer.Flush();
        }
    }
}
