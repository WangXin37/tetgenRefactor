#include<vector>
#include<algorithm>
#include<memory>


struct Point {
	double x, y;

	Point(double x = 0, double y = 0)
		:x(x), y(y) {}
};

struct Rectangle {
	double x, y;
	double width, height;

	Rectangle(double x = 0, double y = 0, double width = 0, double height = 0)
		:x(x), y(y), width(width), height(height) {}

	bool contains(const Point& p)const {
		return (p.x >= x && p.x <= x + width &&
			p.y >= y && p.y <= y + height);
	}

	bool intersacts(const Rectangle& other)const {
		return !(x > other.x + other.width ||
			x + width<other.x ||
			y>other.y + other.height ||
			y + height < other.y);
	}
};
enum Quadrant {
	NW=0,
	NE,
	SW,
	SE,
};

class QuardTreeNode {
private:
	static const int CAPACITY = 4;
	static const int MAX_LEVEL = 8;

	Rectangle boundary;
	int level;
	std::vector<Point> points;

	std::unique_ptr<QuardTreeNode> nw;
	std::unique_ptr<QuardTreeNode> ne;
	std::unique_ptr<QuardTreeNode> sw;
	std::unique_ptr<QuardTreeNode> se;

	bool shouldSplit() const {
		return points.size() > CAPACITY && level < MAX_LEVEL;
	}

	void createChildren() {
		double halfWidth = boundary.width / 2.0;
		double halfHeight = boundary.height / 2.0;
		double midX = boundary.x + halfWidth;
		double midY = boundary.y + halfHeight;

		nw = std::make_unique<QuardTreeNode>(
			Rectangle(boundary.x, midY, halfWidth, halfHeight), level + 1);
		ne = std::make_unique<QuardTreeNode>(
			Rectangle(midX, midY, halfWidth, halfHeight), level + 1);
		sw = std::make_unique<QuardTreeNode>(
			Rectangle(boundary.x, boundary.y, halfWidth, halfHeight), level + 1);
		se = std::make_unique<QuardTreeNode>(
			Rectangle(midX, boundary.y, halfWidth, halfHeight), level + 1);
	}

	int getQuadrant(const Point& p) const {
		double midX = boundary.x + boundary.x / 2;
		double midY = boundary.y + boundary.y / 2;

		if (p.x <= midX) {
			return p.y <= midY ? Quadrant::SW : Quadrant::NW;
		}
		else {
			return p.y <= midY ? Quadrant::SE : Quadrant::NE;
		}
	}

	void getAllPointsHelper(std::vector<Point>& allPoints) const {
		if (nw) {
			nw->getAllPointsHelper(allPoints);
			ne->getAllPointsHelper(allPoints);
			sw->getAllPointsHelper(allPoints);
			se->getAllPointsHelper(allPoints);
		}
		else {
			allPoints.insert(allPoints.end(), points.begin(), points.end());
		}
	}

public:
	QuardTreeNode(const Rectangle& boundary, int level = 0)
		:boundary(boundary), level(level) {
	}

	bool insert(const Point& point) {
		if (boundary.contains(point)) {
			return false;
		}

		if (!nw) {
			points.push_back(point);

			if (shouldSplit()) {
				split();
			}
			return true;
		}

		int quadrant = getQuadrant(point);
		switch (quadrant)
		{
		case 0:
			return this->nw->insert(point);
		case 1:
			return this->ne->insert(point);
		case 2:
			return this->sw->insert(point);
		case 3:
			return this->se->insert(point);
		default:
			return false;
		}
	}

	void split() {
		createChildren();

		for (const auto& point : this->points) {
			int quadrant = getQuadrant(point);
			switch (quadrant) {
				case 0:nw->insert(point);break;
				case 1:ne->insert(point);break;
				case 2:sw->insert(point);break;
				case 3:se->insert(point);break;
			}
		}

		points.clear();
	}

	void queryRange(const Rectangle& range, std::vector<Point>& found) const {
		if (!boundary.intersacts(range)) {
			return;
		}

		if (nw) {
			nw->queryRange(range, found);
			ne->queryRange(range, found);
			sw->queryRange(range, found);
			se->queryRange(range, found);
		}
		else {
			for (const auto& point : this->points) {
				if (range.contains(point)) {
					found.push_back(point);
				}
			}
		}
	}

	std::vector<Point> getAllPoints()const {
		std::vector<Point> allPoints;
		getAllPointsHelper(allPoints);
		return allPoints;
	}

	void clear() {
		points.clear();
		nw.reset();
		ne.reset();
		sw.reset();
		se.reset();
	}

	struct Stats {
		int nodeCount = 0;
		int pointCount = 0;
		int maxDepth = 0;
	};

	Stats getStats() const{
		Stats stats;
		getStatsHelper(stats, 0);
		return stats;
	}
private:
	void getStatsHelper(Stats& stats, int depth) const {
		stats.nodeCount++;
		stats.maxDepth = std::max(stats.maxDepth, depth);

		if (nw) {
			nw->getStatsHelper(stats, depth + 1);
			ne->getStatsHelper(stats, depth + 1);
			sw->getStatsHelper(stats, depth + 1);
			se->getStatsHelper(stats, depth + 1);
		}
		else {
			stats.pointCount += points.size();
		}
	}

};

class QuadTree {
private:
	std::unique_ptr<QuardTreeNode> root;

public:
	QuadTree(double x, double y, double width, double height) {
		root = std::make_unique<QuardTreeNode>(Rectangle(x, y, width, height));
	}

	QuadTree(const Rectangle& boundary) {
		root = std::make_unique<QuardTreeNode>(boundary);
	}

	bool insert(const Point& point) {
		if (!root) return false;
		return root->insert(point);
	}

	std::vector<Point> queryRange(const Rectangle& range) const {
		std::vector<Point> found;
		if (root) {
			root->queryRange(range, found);
		}
		return found;
	}

	Point findNearest(double x, double y) const {
		auto
	}
};

