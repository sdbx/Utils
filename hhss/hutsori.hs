import Data.List
import System.IO

userFile = "usr.dat"
sentenceFile = "hsr.dat"
userTemplate = "${user}"

readFileAsUtf8 p = do
  h <- openFile p ReadMode
  hSetEncoding h utf8
  hGetContents h

replace _ [] = []
replace to s@(x:xs) =
  case stripPrefix userTemplate s of
    Just rest -> to ++ replace to rest
    _ -> x : replace to xs

isValidLine l =
  not $ "#" `isPrefixOf` l || null l

main = do
  sentence <- readFileAsUtf8 sentenceFile
  usersRaw <- readFileAsUtf8 userFile
  let user = head $ lines usersRaw
  let validLines = filter isValidLine . lines $ sentence
  mapM_ (putStrLn . replace user) validLines
