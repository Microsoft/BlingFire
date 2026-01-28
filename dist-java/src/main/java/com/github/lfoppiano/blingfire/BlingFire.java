package com.github.lfoppiano.blingfire;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;

public class BlingFire {

    static {
        loadNativeLibrary();
    }

    private static void loadNativeLibrary() {
        // Simple loading logic - can be improved to extract from JAR for different
        // OS/Arch
        try {
            System.loadLibrary("blingfirejni");
        } catch (UnsatisfiedLinkError e) {
            String osName = System.getProperty("os.name").toLowerCase();
            String osArch = System.getProperty("os.arch").toLowerCase();
            String libName = "libblingfirejni.so"; // Default to Linux

            if (osName.contains("win")) {
                libName = "blingfirejni.dll";
            } else if (osName.contains("mac")) {
                libName = "libblingfirejni.dylib";
            }

            // Try to load from temp file just in case it's packaged in jar
            // For now, assuming it's in the library path or handled by System.loadLibrary
            // but providing a fallback or debug message
            System.err
                    .println("Could not load library from path, ensure native library is available: " + e.getMessage());
        }
    }

    // Native methods mirroring the C API

    public static native int GetBlingFireTokVersion();

    public static native int TextToSentencesWithModel(byte[] inUtf8Str, int inUtf8StrByteCount, byte[] outUtf8Str,
            int maxOutUtf8StrByteCount, long modelHandle);

    public static native int TextToSentencesWithOffsetsWithModel(byte[] inUtf8Str, int inUtf8StrByteCount,
            byte[] outUtf8Str, int[] startOffsets, int[] endOffsets,
            int maxOutUtf8StrByteCount, long modelHandle);

    public static native int TextToWordsWithModel(byte[] inUtf8Str, int inUtf8StrByteCount, byte[] outUtf8Str,
            int maxOutUtf8StrByteCount, long modelHandle);

    public static native int TextToWordsWithOffsetsWithModel(byte[] inUtf8Str, int inUtf8StrByteCount,
            byte[] outUtf8Str, int[] startOffsets, int[] endOffsets,
            int maxOutUtf8StrByteCount, long modelHandle);

    // Default models (if compiled without SIZE_OPTIMIZATION and properly linked)
    // We typically will use custom models loaded from file to be safe and flexible

    public static native long LoadModel(String modelPath);

    public static native void FreeModel(long modelHandle);

    // Helper Java methods

    public static long loadModel(String serializedModelPath) {
        return LoadModel(serializedModelPath);
    }

    public static void freeModel(long modelHandle) {
        FreeModel(modelHandle);
    }

    /**
     * Splits text into sentences.
     * 
     * @param text        input text
     * @param modelHandle handle to the loaded model
     * @return array of sentences
     */
    public static String[] textToSentences(String text, long modelHandle) {
        if (text == null || text.isEmpty()) {
            return new String[0];
        }

        byte[] inputBytes = text.getBytes(java.nio.charset.StandardCharsets.UTF_8);
        // Estimate output size - heuristic, 2x input size seems safe for most cases as
        // per python wrapper
        int outputBufferSize = inputBytes.length * 2 + 1024;
        byte[] outputBytes = new byte[outputBufferSize];

        int actualSize = TextToSentencesWithModel(inputBytes, inputBytes.length, outputBytes, outputBufferSize,
                modelHandle);

        if (actualSize == -1 || actualSize > outputBufferSize) {
            // Buffer too small or error
            // In a robust implementation, we might retry with larger buffer if actualSize >
            // outputBufferSize
            return new String[0];
        }

        // Output from BlingFire is newline delimited (see C++ source) but null
        // terminated?
        // Implementation logic:
        // C++: std::replace(pTmpUtf8, pTmpUtf8 + StrOutSize, '\n', ' '); for newlines
        // INSIDE sentences
        // C++: separator is '\n' between sentences.

        String result = new String(outputBytes, 0, actualSize, java.nio.charset.StandardCharsets.UTF_8);
        return result.split("\n");

    }
}
