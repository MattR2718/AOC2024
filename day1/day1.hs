import System.IO
import Data.List (sort)

printLines :: [String] -> IO ()
printLines [] = putStr "\n"
printLines (l:ls) = do
    putStrLn l
    printLines ls

parseLine :: String -> (Int, Int)
parseLine line = do
    let [x, y] = map read (words line)
    (x, y)

parsePairs :: [String] -> [(Int, Int)]
parsePairs [] = []
parsePairs xs = map parseLine xs

parseInput :: String -> ([Int], [Int])
parseInput [] = ([], [])
parseInput input = unzip $ parsePairs $ lines input

p1 :: String -> IO ()
p1 inFile = do
    handle <- openFile inFile ReadMode
    contents <- hGetContents handle
    let sorted = (sort l, sort r) where (l, r) = parseInput contents
    
    let p1 = sum $ map abs $ zipWith (-) a b where (a, b) = sorted

    print p1
    
    hClose handle
