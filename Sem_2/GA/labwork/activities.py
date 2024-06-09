import pygame
import random

"""
Source -- https://gist.github.com/Lewiscowles1986/87abbb85274817db2f89583318134930
"""

class Snowfield():
    def __init__(self, density, speed, size, color, bounds, pygame, screen):
        self.color = color
        self.flakes = []
        self.speed = speed
        self.size = size
        for i in range(density):
            x = random.randrange(bounds[0] * -1, bounds[0] * 2, 1)
            y = random.randrange(bounds[1] * -1, bounds[1] * 2, 1)
            self.flakes.append([x, y])
        self.screen = screen
        self.pygame = pygame
        self.bounds = bounds
        self.count = 0

    def tick(self):
        self.count += 1
        for i in range(len(self.flakes)):
            self.pygame.draw.circle(self.screen, self.color,
                                    self.flakes[i], self.size)
            if self.count % self.speed == 0:
                self.flakes[i][1] += 1
            if (self.flakes[i][1] > self.bounds[1] * 2 or
                    self.flakes[i][0] < self.bounds[0] * -1 or
                    self.flakes[i][0] > self.bounds[0] * 2):
                y = random.randrange(self.bounds[1] * -1, 0)
                self.flakes[i][1] = y

                x = random.randrange(self.bounds[0] * -1, self.bounds[0] * 2)
                self.flakes[i][1] = y
        if self.count > 1000:
            self.count -= 1000


def draw_tree(x, y, pygame, screen):
    pygame.draw.rect(screen, (139, 69, 19), [x + 60, y + 400, 30, 45])
    pygame.draw.polygon(screen, (0, 128, 0), [[x + 150, y + 400], [x + 75, y + 250], [x + 0, y + 400]])
    pygame.draw.polygon(screen, (0, 128, 0), [[x + 140, y + 350], [x + 75, y + 230], [x + 10, y + 350]])


# Initialize the game engine
def main():
    pygame.init()

    BLACK = [0, 0, 0]
    WHITE = [255, 255, 255]

    # Set the height and width of the screen
    SIZE = [800, 600]

    screen = pygame.display.set_mode(SIZE)
    pygame.display.set_caption("Snow Animation")
    bg_snow = Snowfield(500, 3, 2, (200, 200, 200), SIZE, pygame, screen)
    fg_snow = Snowfield(250, 1, 2, (255, 255, 255), SIZE, pygame, screen)

    clock = pygame.time.Clock()
    done = False

    while not done:
        for event in pygame.event.get():  # User did something
            if event.type == pygame.QUIT:  # If user clicked close
                done = True  # Flag that we are done so we exit this loop

        # Clear screen every tick
        screen.fill(BLACK)
        bg_snow.tick()

        # Floor is beneath tree's but above background snow.
        pygame.draw.rect(screen, (240, 240, 240), [0, SIZE[1] - 60, SIZE[0], 60])

        draw_tree(10, 100, pygame, screen)
        draw_tree(165, 115, pygame, screen)
        draw_tree(320, 100, pygame, screen)
        draw_tree(475, 115, pygame, screen)
        draw_tree(630, 100, pygame, screen)

        fontObj = pygame.font.SysFont(pygame.font.get_default_font(), 100, True, True)
        text = fontObj.render('Merry Christmas', True, (255, 0, 0), None)
        screen.blit(text, [70, int(SIZE[1] / 2) - 100])

        fg_snow.tick()

        # Go ahead and update the screen with what we've drawn.
        pygame.display.flip()
        clock.tick(20)

    pygame.quit()
#
#
# if __name__ == "__main__":
#     main()
