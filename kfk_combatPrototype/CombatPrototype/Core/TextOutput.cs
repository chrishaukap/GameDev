using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Collections.Generic;

namespace CombatPrototype.Core.TextRendering
{
    struct textNode
    {
        public int X, Y;
        public string Text;
        public Color FontColor;
        public float Rotation;
    }
    public class TextOutput : DrawableGameComponent
    {
        protected bool mApplyCameraTransformation;
        private ContentManager m_ContentManager;
        private Game1 m_Game;
        private List<textNode> m_List = new List<textNode>();
        private SpriteBatch m_SpriteBatch;
        private SpriteFont m_SpriteFont;
        public Color FontColor
        {
            get { return m_FontColor; }
            set { m_FontColor = value; }
        }
        private Color m_FontColor = Color.AntiqueWhite;

        public TextOutput(Game1 game)
            : base(game)
        {
            mApplyCameraTransformation = true;
            m_Game = game;
        }
        public void WriteAt(int x, int y, string str)
        {
            WriteAt(x, y, str, FontColor);
        }

        public void WriteAt(int x, int y, string str, Color color)
        {
            textNode n = new textNode();
            n.X = x;
            n.Y = y;
            n.Text = str;
            n.FontColor = color;
            m_List.Add(n);
        }

        public override void Draw(GameTime gameTime)
        {
            base.Draw(gameTime);
            if (mApplyCameraTransformation)
                m_SpriteBatch.Begin(SpriteBlendMode.AlphaBlend,
                                    SpriteSortMode.FrontToBack,
                                    SaveStateMode.SaveState,
                                    m_Game.Camera.ViewTransformationMatrix());
            else
                m_SpriteBatch.Begin();

            foreach (textNode n in m_List)
            {
                m_SpriteBatch.DrawString(m_SpriteFont, n.Text, new Vector2(n.X, n.Y), n.FontColor,
                    n.Rotation, new Vector2(0, 0), 1.0f, SpriteEffects.None, 0.5f);
            }
            m_SpriteBatch.End();
            m_List.Clear();
        }

        protected override void LoadGraphicsContent(bool loadAllContent)
        {
            base.LoadGraphicsContent(loadAllContent);
            m_SpriteBatch = new SpriteBatch(this.GraphicsDevice);
            m_ContentManager = new ContentManager(m_Game.Services);
            m_SpriteFont = m_ContentManager.Load<SpriteFont>("Content/Arial");
        }
    }
    public class StaticTextOutput : TextOutput 
    {
        public StaticTextOutput(Game1 game)
                : base(game)
            {
                mApplyCameraTransformation = false;
            }
    }
}
