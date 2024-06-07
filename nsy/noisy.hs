import System.Environment
import Data.Char
import Data.List

markFrom markType =
  case markType of
    "e" -> "\xFF01"
    "q" -> "\xFF1F"
    "eu" -> " \xA1 "
    "qu" -> " \xBF "
    _ -> error "unknown argument."

noisy mark =
  (mark ++) . (++ mark) . intercalate mark . map pure . filter (not . isSpace)

main = do
  (markType:_) <- getArgs
  let mark = markFrom markType
  text <- getContents
  putStrLn $ noisy mark text
