# Assignment 04-05
## Requirements
- The application must be implemented in C++ and use layered architecture.
- Provide tests and specifications for non-trivial functions outside the UI. Test coverage must be at least 98% for all layers, except the UI.
- Have at least 10 entities in your memory repository.
- Validate the input data.
- Handle the following situations:
    - If an entity that already exists is added, a message is shown and the entity is not stored. You must decide what makes an entity unique.
    - If the user tries to update/delete an entity that does not exist, a message will be shown and there will be no effect on the list of entities.

## Week 5
- Solve the requirements related to the administrator mode.
- Define the `DynamicVector` class which provides the specific operations: `add`, `remove`, `length`, etc. The array of elements must be dynamically allocated.

## Week 7
- Solve all problem requirements.
- `DynamicVector` must be a class template.

## Problem Statement

## Master C++
You are very passionate about programing (otherwise you wouldn't be reading this) and C++ is a language close to your heart. On your way to becoming a guru, you study a lot and watch many tutorials. To make sure you do not miss any good tutorials, you absolutely need a software application to help you manage your tutorials and create watch lists. The application can be used in two modes: administrator and user. When the application is started, it will offer the option to choose the mode.\
**Administrator mode:** The application will have a database , which holds all the tutorials. You must be able to update the database, meaning: add a new tutorial, delete a tutorial and update the information of a tutorial. Each **Tutorial** has a `title`, a `presenter` (name of the presenter person), a `duration` (minutes and seconds), a number of `likes` and a `link` towards the online resource containing the tutorial. The administrators will also have the option to see all the tutorials in the database.\
**User mode:** A user can create a watch list with the tutorials that he/she wants to watch. The application will allow the user to:
- See the tutorials in the database having a given presenter (if the presenter name is empty, see all the tutorials), one by one. When the user chooses this option, the data of the first tutorial (title, presenter, duration, number of likes) is displayed and the tutorial is played in the browser.
- If the user likes the tutorial, he/she can choose to add it to his/her tutorial watch list.
- If the tutorial seems uninteresting, the user can choose not to add it to the watch list and continue to the next. In this case, the information corresponding to the next tutorial is shown and the user is again offered the possibility to add it to the watch list. This can continue as long as the user wants, as when arriving to the end of the list of tutorials with the given presenter, if the user chooses next, the application will again show the first tutorial.
- Delete a tutorial from the watch list, after the user watched the tutorial. When deleting a tutorial from the watch list, the user can also rate the tutorial (with a like), and in this case, the number of likes for the tutorial will be increased.
- See the watch list.
