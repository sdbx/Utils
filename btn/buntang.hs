import Data.Char
import System.Environment
import Data.List

chunks :: Int -> [a] -> [[a]]
chunks _ [] = []
chunks n xs =
  let (ys, zs) = splitAt n xs
  in ys : chunks n zs

main = do
  (rawCharCountUntilNewline:text:_) <- getArgs
  let charCountUntilNewline = read rawCharCountUntilNewline :: Int
  putStrLn $ intercalate "\n" . chunks charCountUntilNewline . filter (not . isSpace) $ text
