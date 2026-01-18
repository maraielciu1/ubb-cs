# BookLog: Your Personal Reading Journal📚

## Project Overview
BookLog is a dedicated, single-user mobile application designed to simplify the process of tracking, reviewing, and managing a personal reading list. It serves as a digital journal for every book a user interacts with, whether they are still reading it, have finished it, or decided to stop early. The app's core value is providing a clean, accessible, and comprehensive personal library that lets users easily record their progress, star ratings, and detailed thoughts on every title. Users can quickly see their reading activity, manage their reading queue, and maintain a permanent record of their personal literary journey.

---

## Design Mockup
Here is a preview of the main interface and the "Add New Read" screen:
![BookLog Mockup](image.png)
---

## Data Model: The BookReviewEntry Entity
The core data structure is the BookReviewEntry, which stores all information related to a book's tracking status and the user's personal review.

| Field Name | Data Type | Description |
|-------------|------------|-------------|
| **ReviewID** | String | A unique, permanent identifier for this specific review entry, used to track it across all devices and the server. |
| **BookTitle** | String | The official title of the book being reviewed (e.g., "The Root Keeps The Secret"). |
| **AuthorName** | String | The name of the book's primary author. |
| **ReadingStatus** | String | The current stage of the book: Planned, Reading, Finished, or DNF (Did Not Finish). |
| **StarRating** | Number (0-5) | The user's final rating for the book, measured on a 0 to 5 star scale. |
| **ReviewDescription** | Text | The user's detailed, written notes, thoughts, or full review of the book. |
| **StartDate** | Date | The date the user began reading this book. |
| **CoverImageURL** | String | The link to the image of the book's cover for easy visual identification. |

---

## Core Functionality (CRUD Operations)

### Create (Adding a New Review)
**Action:** User fills out the book details and presses "Submit a review."  
**Result:** A brand new BookReviewEntry is generated and saved.

### Read (Viewing the BookLog)
**Action:** User opens the main screen.  
**Result:** The app retrieves and displays a scrollable list of all associated BookReviewEntry documents, showing the status and rating for each.

### Update (Editing an Existing Entry)
**Action:** User modifies any detail (e.g., changing the ReadingStatus from "Reading" to "Finished" or updating the review text).  
**Result:** The original entry is located via its ReviewID and its information is permanently overwritten.

### Delete (Removing a Book)
**Action:** User chooses to permanently remove a book entry from their log.  
**Result:** The corresponding BookReviewEntry document is completely removed from the user's data.

---

## Technical Architecture & Persistence
The application uses a hybrid persistence model to ensure speed and data integrity across devices.

| CRUD Operation | Server Database (Primary Source) | Local Database (Device Cache) | Purpose |
|----------------|-----------------------------------|-------------------------------|----------|
| **Create** | YES | YES | Ensures the new entry is backed up and immediately available on the app. |
| **Read** | YES | YES | Server handles initial sync; Local DB allows instant opening and offline viewing. |
| **Update** | YES | YES | The server gets the permanent record change, and the local copy updates the display instantly. |
| **Delete** | YES | YES | The server permanently removes the data, and the local DB removes it from the list. |

---

## Offline Capability 
BookLog is designed with a "sync-first" approach, allowing users to perform all CRUD actions even without an internet connection. Actions are queued locally and synchronized automatically when a connection is restored.

**Offline Create:** A new review is saved instantly to the local database and visible on the BookLog with a "pending" icon. It uploads to the server when connectivity resumes.  
**Offline Read:** The app loads the complete BookLog from the local database cache, ensuring the user can view all their past reviews and statuses even without service.  
**Offline Update:** A status change (e.g., "Reading" to "Finished") is applied immediately to the local entry. The instruction to update is queued and sent to the server once the signal is available.  
**Offline Delete:** The entry vanishes instantly from the local BookLog, and a "Delete Instruction" is queued to be sent to the server on reconnection.
