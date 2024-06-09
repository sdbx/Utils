import Data.Char
import Data.List
import Data.Functor

format "" = ""
format s@('#':' ':xs) = s
format s = "# " ++ s

trim = dropWhile isSpace . dropWhileEnd isSpace

main = do
  input <- getContents
  mapM_ putStrLn $ lines input <&> trim <&> format
