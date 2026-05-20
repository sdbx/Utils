import Data.Char
import System.Environment
import Data.List

chunks :: Int -> [a] -> [[a]]
chunks _ [] = []
chunks n xs =
  let (ys, zs) = splitAt n xs
  in ys : chunks n zs

main = do
  (rawCharCountUntilNewline:_) <- getArgs
  let charCountUntilNewline = read rawCharCountUntilNewline :: Int
  text <- getContents
  let filteredText = filter (not . isSpace) text
  if charCountUntilNewline == 0
    then putStr filteredText
    else putStrLn $ intercalate "\n" $ chunks charCountUntilNewline filteredText
