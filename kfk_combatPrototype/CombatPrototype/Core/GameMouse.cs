using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework;
using System.Linq;
using System.Text;

namespace CombatPrototype.Core
{
    public class GameMouse
    {
        private Texture2D mMousePointer;
        private SpriteBatch mSpriteBatch;
        private GraphicsDeviceManager mGraphics;
        private ContentManager mContent;
        private bool mLeftIsPressed;
        private bool mRightIsPressed;
        private bool mLeftPressedThisFrame;
        private bool mRightPressedThisFrame;
        private int mScrollValue;
        private bool mScrolledUpThisFrame;
        private bool mScrolledDownThisFrame;

        public GameMouse(GraphicsDeviceManager g, ContentManager c)
        {
            mScrollValue = 0;
            mScrolledUpThisFrame = false;
            mScrolledDownThisFrame = false;

            mGraphics = g;
            mContent = c;

            mSpriteBatch = new SpriteBatch(mGraphics.GraphicsDevice);
            mMousePointer = mContent.Load<Texture2D>("Cursor");

            mLeftIsPressed = mRightIsPressed = mLeftPressedThisFrame = mRightPressedThisFrame = false;
        }

        public void Update()
        {
            CheckLeftButtonPressed();
            CheckRightButtonPressed();
            CheckScroll();
        }
        public void CheckScroll()
        {
            mScrolledUpThisFrame = false;
            mScrolledDownThisFrame = false;
            int newScrollValue = Mouse.GetState().ScrollWheelValue;
            if (newScrollValue > mScrollValue)
            {
                mScrollValue = newScrollValue;
                mScrolledUpThisFrame = true;
            }
            else if (newScrollValue < mScrollValue)
            {
                mScrollValue = newScrollValue;
                mScrolledDownThisFrame = true;
            }
        }
        public void CheckLeftButtonPressed()
        {
            mLeftPressedThisFrame = false;
            if (Mouse.GetState().LeftButton == ButtonState.Pressed)
                mLeftIsPressed = true;

            if (mLeftIsPressed && Mouse.GetState().LeftButton == ButtonState.Released)
            {
                mLeftPressedThisFrame = true;
                mLeftIsPressed = false;
            }
        }

        public void CheckRightButtonPressed()
        {
            mRightPressedThisFrame = false;

            if (Mouse.GetState().RightButton == ButtonState.Pressed)
                mRightIsPressed = true;

            if (mRightIsPressed && Mouse.GetState().RightButton == ButtonState.Released)
            {
                mRightPressedThisFrame = true;
                mRightIsPressed = false;
            }
        }

        public bool ScrolledUpThisFrame { get { return mScrolledUpThisFrame; } }
        public bool ScrolledDownThisFrame { get { return mScrolledDownThisFrame; } }
        public bool PressedThisFrameLeft { get{ return mLeftPressedThisFrame; } }
        public bool PressedThisFrameRight { get { return mRightPressedThisFrame; } }
        public Vector2 CurrentMousePos() { return new Vector2(Mouse.GetState().X, Mouse.GetState().Y); }

        public void Draw()
        {
            Vector2 mousePos = new Vector2(Mouse.GetState().X - mMousePointer.Width / 2, Mouse.GetState().Y - mMousePointer.Height / 2);

            mSpriteBatch.Begin();
            mSpriteBatch.Draw(mMousePointer, mousePos, Color.White);
            mSpriteBatch.End();
        }
    }
}
