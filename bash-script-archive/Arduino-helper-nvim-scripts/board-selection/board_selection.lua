-- Open the file for reading
local file = io.open("/tmp/arduino_boards.txt", "r")
if not file then
  print("Error: Could not open /tmp/arduino_boards.txt")
  return
end

-- Read the file into a table
local boards = {}
for line in file:lines() do
  local board_name, fqbn = line:match("^(.-)%s+(%w+:%w+)$")
  if board_name and fqbn then
    table.insert(boards, { name = board_name, fqbn = fqbn })
  end
end
file:close()

-- Display the header
print("Available Arduino Boards:")
print("No.  Board Name                                   FQBN")
print("---------------------------------------------------------")

-- Display the list of boards
for i, board in ipairs(boards) do
  print(string.format("%-3d  %-40s  %s", i, board.name, board.fqbn))
end

-- Ask the user to choose a board
io.write("Enter the number of the board you want to select: ")
local choice = tonumber(io.read())

if choice and boards[choice] then
  local selected_board = boards[choice]
  print("You selected: " .. selected_board.name)
  print("FQBN: " .. selected_board.fqbn)
else
  print("Invalid selection.")
end
