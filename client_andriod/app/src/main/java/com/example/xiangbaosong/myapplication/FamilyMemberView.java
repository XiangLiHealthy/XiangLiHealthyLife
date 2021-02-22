package com.example.xiangbaosong.myapplication;

import android.animation.Animator;
import android.animation.ObjectAnimator;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.Shader;
import android.support.annotation.Nullable;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.view.animation.DecelerateInterpolator;

import java.util.ArrayList;
import java.util.List;

public class FamilyMemberView extends View {
    private static final String TAG = "FamilyMember";

    private Paint paintRing;  //圆环的画笔
    private Paint paintArc;   //圆弧的画笔
    private Paint paintText;

    private RectF rectFArc;  //圆弧的Rect
    private Rect rectRing;   //圆环的Rect

    private int[] gradientColorsArc; //弧的渐变颜色

    private int ringColor = Color.GRAY;
    private int arcColor = Color.BLUE;
    private int textColor = Color.BLACK;
    private int backgroundColor = Color.TRANSPARENT;

    private int strokeWidth = 100;
    private int startAngle = 0;
    private int sweepAngle = 0;
    private int progress = 0;
    private int defaultSize = 600;
    private int size;

    private int paddingLeft;
    private int paddingRight;
    private int paddingTop;
    private int paddingBottom;

    private String textProgress = "0%";
    private int textSize = 50;
    private int textX, textY;

    private FamilyMemberManager mMemberManager;

    public FamilyMemberView(Context context) {
        this(context, null);
    }

    public FamilyMemberView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public FamilyMemberView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);

        TypedArray a = context.getTheme().obtainStyledAttributes(attrs, R.styleable.FamilyMemberView, defStyleAttr, 0);
        ringColor = a.getColor(R.styleable.FamilyMemberView_ring_color, Color.GRAY);
        arcColor = a.getColor(R.styleable.FamilyMemberView_progress_color, Color.BLUE);
        backgroundColor = a.getColor(R.styleable.FamilyMemberView_background_color, Color.TRANSPARENT);

        strokeWidth = a.getInt(R.styleable.FamilyMemberView_progress_width, 100);
        startAngle = a.getInt(R.styleable.FamilyMemberView_start_angle, 0);
        progress = a.getInt(R.styleable.FamilyMemberView_progress, 10);

        textSize = dip2px(context, a.getInt(R.styleable.FamilyMemberView_progress_text_size, 50));
        textColor = a.getColor(R.styleable.FamilyMemberView_progress_text_color, Color.BLACK);

        a.recycle();

        init();
    }

    private void init() {
        paintRing = new Paint();
        paintRing.setAntiAlias(true);
        paintRing.setColor(ringColor);
        paintRing.setStrokeWidth(strokeWidth);
        paintRing.setStyle(Paint.Style.STROKE);

        paintArc = new Paint();
        paintArc.setAntiAlias(true);
        paintArc.setColor(arcColor);
        paintArc.setStrokeWidth(strokeWidth);
        paintArc.setStyle(Paint.Style.STROKE);

        paintText = new Paint();
        paintText.setAntiAlias(true);
        paintText.setColor(textColor);
        paintText.setStyle(Paint.Style.FILL);
        paintText.setTextAlign(Paint.Align.CENTER);
        paintText.setTextSize(textSize);

        gradientColorsArc = new int[] {Color.GREEN, Color.RED};

        sweepAngle = (int) ((progress / 100.0f) * 360);
        textProgress = progress + "%";
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        size = Math.min(measureWidth(widthMeasureSpec), measureHeight(heightMeasureSpec));
        setMeasuredDimension(size, size);

        paddingLeft = getPaddingLeft();
        paddingRight = getPaddingRight();
        paddingTop = getPaddingTop();
        paddingBottom = getPaddingBottom();

        //除去padding的可用大小
        int rectWidth = size - paddingLeft - paddingRight;
        int rectHeight = size - paddingTop - paddingBottom;

        //get validate rect
        Rect validateRect = new Rect(paddingLeft, paddingTop,
                paddingLeft + rectWidth, paddingBottom + rectHeight);


        //真实可用大小（正方形）
//        int rectMinSize = Math.min(rectHeight, rectWidth);
//
//
//        Log.e(TAG, "onMeasure rectWidth:" + rectWidth + " rectHeight:" + rectHeight + " rectMinSize:" + rectMinSize);
//
//        //弧的rect
//        rectFArc = new RectF(paddingLeft + strokeWidth / 2, paddingTop + strokeWidth / 2,
//                paddingLeft + rectMinSize - strokeWidth / 2, paddingTop + rectMinSize - strokeWidth / 2);
//
//        //圆环的rect
//        rectRing = new Rect(paddingLeft, paddingTop, paddingLeft + rectMinSize, paddingTop + rectMinSize);
//
//        //progress字的位置
//        textX = paddingLeft + rectMinSize / 2;
//        textY = (int) (paddingTop + (rectMinSize - (paintText.descent() - paintText.ascent())) / 2 - paintText.ascent());
//
//        //设置弧的渐变颜色
//        LinearGradient linearGradient = new LinearGradient(0, 0, size, 0, gradientColorsArc, null, Shader.TileMode.CLAMP);
//        paintArc.setShader(linearGradient);
//
//        //gradient = new SweepGradient(size / 2, size / 2, gradientColors, null);
//        //paintArc.setShader(gradient);
//
//        Log.e(TAG, "onMeasure size:" + size);
    }

    @Override
    protected void onDraw(Canvas canvas) {

        canvas.drawColor(backgroundColor);

        //画圆环
        canvas.drawCircle(paddingLeft + rectRing.width() / 2, paddingTop + rectRing.width() / 2, (rectRing.width() - strokeWidth) / 2, paintRing);

        //画弧
        //drawArc(@NonNull RectF oval, float startAngle, float sweepAngle, boolean useCenter, @NonNull Paint paint)
        canvas.drawArc(rectFArc, startAngle, sweepAngle, false, paintArc);

        canvas.drawText(textProgress, textX, textY, paintText);
    }


    /**
     * 以动画的形式改变progress
     *
     * @param progress
     * @param duration
     */
    public void setProgressWithAnimation(int progress, int duration) {
        Animator anim = ObjectAnimator.ofFloat(this, "progress", progress);
        anim.setDuration(duration);
        anim.setInterpolator(new DecelerateInterpolator());
        anim.start();
    }

    private int measureWidth(int widthMeasureSpec) {
        int result = defaultSize;
        int specMode = MeasureSpec.getMode(widthMeasureSpec);
        int specSize = MeasureSpec.getSize(widthMeasureSpec);

        Log.e(TAG, "measureWidth specMode:" + specMode + "  specSize:" + specSize);

        switch(specMode) {
            case MeasureSpec.EXACTLY:
                result = specSize;
                break;
            case MeasureSpec.AT_MOST:
                result = Math.min(defaultSize, specSize);
                break;
            case MeasureSpec.UNSPECIFIED:
                result = defaultSize;
                break;
        }
        return result;
    }

    private int measureHeight(int heightMeasureSpec) {
        int result = defaultSize;
        int specMode = MeasureSpec.getMode(heightMeasureSpec);
        int specSize = MeasureSpec.getSize(heightMeasureSpec);

        Log.e(TAG, "measureHeight specMode:" + specMode + "  specSize:" + specSize);

        switch(specMode) {
            case MeasureSpec.EXACTLY:
                result = specSize;
                break;
            case MeasureSpec.AT_MOST:
                result = Math.min(defaultSize, specSize);
                break;
            case MeasureSpec.UNSPECIFIED:
                result = defaultSize;
                break;
        }
        return result;
    }

    public void setRingColor(int ringColor) {
        this.ringColor = ringColor;
        invalidate();
    }

    public void setArcColor(int arcColor) {
        this.arcColor = arcColor;
        invalidate();
    }

    public void setStrokeWidth(int strokeWidth) {
        this.strokeWidth = strokeWidth;
        requestLayout();
    }

    public void setStartAngle(int startAngle) {
        this.startAngle = (startAngle >= 0 && startAngle <= 360) ? startAngle : 0;
        this.startAngle = startAngle;
        invalidate();
    }

    public void setProgress(float progress) {
        setProgress((int) progress);
    }

    public void setProgress(int progress) {
        this.progress = (progress <= 100) ? progress : 100;
        textProgress = this.progress + "%";
        sweepAngle = (int) ((this.progress / 100.0f) * 360);
        invalidate();
    }

    public float getProgress() {
        return this.progress;
    }

    public void setTextColor(int textColor) {
        this.textColor = textColor;
        invalidate();
    }

    public void setTextSize(int textSize) {
        this.textSize = dip2px(getContext(), textSize);
    }

    public static int dip2px(Context context, float dpValue) {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (dpValue * scale + 0.5f);
    }
}
