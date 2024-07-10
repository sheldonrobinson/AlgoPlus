#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#ifdef __cplusplus
#include <memory>
#include <bitset>
#include <vector>
#endif

/**
 *@brief Class for red black tree.
 */
template <typename T> class  red_black_tree {
private:
  /**
    *@brief Struct for the node type pointer.
    *@param info: the value of the node.
    *@param is_red: colour of the node, 1 if node is red, 0 if node is black.
    *@param parent: pointer to the parent.
    *@param left: pointer to the left.
    *@param right: pointer to the right.
    */
  typedef struct node {
    T info;
    std::bitset<1> is_red;
    std::shared_ptr<node> parent;
    std::shared_ptr<node> right;
    std::shared_ptr<node> left;
    node(T key, std::shared_ptr<node> p)
      : info(key), is_red(1), parent(p), right(nullptr), left(nullptr) {}
  } node;

  std::shared_ptr<node> root;
  size_t _size{};

  std::shared_ptr<node> new_node(T &key, std::shared_ptr<node> p = nullptr)  {
    std::shared_ptr<node> t_node = std::make_shared<node>(key, p);
    return t_node;
  }

  void _left_rotate(std::shared_ptr<node> t_node) {
    std::shared_ptr<node> x = t_node->right;
    x->parent = t_node->parent;
    if(t_node->parent == nullptr)
      this->root = x;
    else if(t_node->parent->left == t_node)
      t_node->parent->left = x;
    else
      t_node->parent->right = x;
    
    t_node->right = x->left;
    if(t_node->right)
      t_node->right->parent = t_node;
    x->left = t_node;
    t_node->parent = x;
  }

  void _right_rotate(std::shared_ptr<node> t_node) {
    std::shared_ptr<node> x = t_node->left;
    x->parent = t_node->parent;
    if(t_node->parent == nullptr)
      this->root = x;
    else if(t_node->parent->left == t_node)
      t_node->parent->left = x;
    else
      t_node->parent->right = x;
    
    t_node->left = x->right;
    if(t_node->left)
      t_node->left->parent = t_node;
    x->right = t_node;
    t_node->parent = x;
  }

  void _insert(std::shared_ptr<node> t_node) {
    while(t_node->parent && t_node->parent->is_red == 1){
      std::shared_ptr<node> grand_parent = t_node->parent->parent;
      if(t_node->parent == grand_parent->left){
        std::shared_ptr<node> uncle = grand_parent->right;
        if(uncle && uncle->is_red == 1){
          grand_parent->is_red = 1;
          uncle->is_red = 0;
          t_node->parent->is_red = 0;
          t_node = grand_parent;
        } else {
          if(t_node == t_node->parent->right){
            t_node = t_node->parent;
            _left_rotate(t_node);
          }
          grand_parent = t_node->parent->parent;
          if(grand_parent)
            grand_parent->is_red = 1;
          t_node->parent->is_red = 0;
          _right_rotate(grand_parent);
        }
      } else {
        std::shared_ptr<node> uncle = grand_parent->left;
        if(uncle && uncle->is_red == 1){
          grand_parent->is_red = 1;
          uncle->is_red = 0;
          t_node->parent->is_red = 0;
          t_node = grand_parent;
        } else {
          if(t_node == t_node->parent->left){
            t_node = t_node->parent;
            _right_rotate(t_node);
          }
          grand_parent = t_node->parent->parent;
          if(grand_parent)
            grand_parent->is_red = 1;
          t_node->parent->is_red = 0;
          _left_rotate(grand_parent);
        }
      }
    }
    this->root->is_red = 0;
  }

  void _remove_helper(std::shared_ptr<node> t_node) {
    if(t_node == this->root)
      return;
    std::shared_ptr<node> sibling;
    if(t_node->parent->left == t_node)
      sibling = t_node->parent->right;
    else
      sibling = t_node->parent->left;
    if(sibling == nullptr)
      _remove_helper(t_node->parent);
    else {
      if(sibling->is_red == 1) {
        t_node->parent->is_red = 1;
        sibling->is_red = 0;
        if(t_node->parent->left == sibling)
          _right_rotate(t_node->parent);
        else
          _left_rotate(t_node->parent);
        _remove_helper(t_node);
      } else {
        if(sibling->left && sibling->left->is_red == 1){
          if(t_node->parent->left == sibling){
            sibling->left->is_red = sibling->is_red;
            sibling->is_red = t_node->parent->is_red;
            _right_rotate(t_node->parent);
          } else {
            sibling->left->is_red = t_node->parent->is_red;
            _right_rotate(sibling);
            _left_rotate(t_node->parent);
          }
          t_node->parent->is_red = 0;
        } else if(sibling->right && sibling->right->is_red == 1) {
          if(t_node->parent->left == sibling){
            sibling->right->is_red = t_node->parent->is_red;
            _left_rotate(sibling);
            _right_rotate(t_node->parent);
          } else {
            sibling->right->is_red = sibling->is_red;
            sibling->is_red = t_node->parent->is_red;
            _left_rotate(t_node->parent);
          }
          t_node->parent->is_red = 0;
        } else {
          sibling->is_red = 1;
          if(t_node->parent->is_red == 0)
            _remove_helper(t_node->parent);
          else
            t_node->parent->is_red = 0;
        }
      }
    }
  }

  void _remove(std::shared_ptr<node> t_node) {
    if(t_node == nullptr)
      return;
    std::shared_ptr<node> replace = nullptr;
    if(t_node->left && t_node->right){
      std::shared_ptr<node> tmp = t_node->right;
      while(tmp->left)
        tmp = tmp->left;
      replace = tmp;
    }else if(t_node->left)
      replace = t_node->left;
    else if(t_node->right)
      replace = t_node->right;
    if(replace == nullptr){
      if(t_node == this->root)
        this->root = nullptr;
      else{
        if(t_node->is_red == 0)
          _remove_helper(t_node);
        else{
          std::shared_ptr<node> sibling = nullptr;
          if(t_node->parent->left == t_node)
            sibling = t_node->parent->right;
          else
            sibling = t_node->parent->left;
          if(sibling)
            sibling->is_red = 1;
        }
        if(t_node->parent->left == t_node)
          t_node->parent->left = nullptr;
        else
          t_node->parent->right = nullptr;
      }
      return;
    }
    if(t_node->left == nullptr || t_node->right == nullptr){
      if(t_node == this->root){
        t_node->info = replace->info;
        t_node->left = nullptr;
        t_node->right = nullptr;
        t_node->is_red = 0;
      } else {
        if(t_node->parent->left == t_node)
          t_node->parent->left = replace;
        else
          t_node->parent->right = replace;
        replace->parent = t_node->parent;
        if(replace->is_red == 0 && t_node->is_red == 0)
          _remove_helper(replace);
        else
          replace->is_red = 0;
      }
      return;
    }
    t_node->info = replace->info;
    _remove(replace);
  }

  bool _search(T &key) {
    std::shared_ptr<node> t_node = this->root;
    while(t_node){
      if(key < t_node->info)
        t_node = t_node->left;
      else if(key == t_node->info)
        return true;
      else
        t_node = t_node->right;
    }
    return false;
  }

  void _inorder(std::function<void(T)> callback, std::shared_ptr<node> t_node) {
    if(t_node){
      _inorder(callback, t_node->left);
      callback(t_node->info);
      _inorder(callback, t_node->right);
    }
  }

  void _postorder(std::function<void(T)> callback, std::shared_ptr<node> t_node) {
    if(t_node){
      _postorder(callback, t_node->left);
      _postorder(callback, t_node->right);
      callback(t_node->info);
    }
  }

  void _preorder(std::function<void(T)> callback, std::shared_ptr<node> t_node) {
    if(t_node){
      callback(t_node->info);
      _preorder(callback, t_node->left);
      _preorder(callback, t_node->right);
    }
  }
public:
  /**
   *@brief Contructor for red black tree class.
   *@param _elements: you can directly pass a vector<T> so you don't have to do
   *insert multiple times.
   */
  explicit red_black_tree(std::vector<T> _elements = {})
    noexcept : root(nullptr) {
      for(T &x : _elements){
        this->insert(x);
      }
    }

  /**
   * @brief Copy constructor for red black tree class
   * @param rb the tree we want to copy
   */
  explicit red_black_tree(const red_black_tree &rb) : root(rb.root), _size(rb._size) {}
  
  /**
   * @brief Destructor for red black tree class
   */
  ~red_black_tree() noexcept { root = nullptr; }

  /**
   * @brief operator = for red black tree class
   * @param rb the tree we want to copy
   * @return red_black_tree&
   */
  red_black_tree<T> &operator=(const red_black_tree<T> &rb) {
    root = rb.root;
    _size = rb._size;
    return *this;
  }

  /**
   * @brief operator == for red black tree class
   * @param rb the tree we want to compare
   * @return true if they are same, false otherwise
   */
  bool operator==(const red_black_tree<T> &rb) {
    return this->root == rb.root;
  }



  /**
   *@brief search function.
   *@param key: key to be searched.
   *@returns true if the key exists in the tree.
   */
  bool search(T key) { return _search(key); }
  
  /**
   *@brief insert function.
   *@param key: key to be inserted.
   */
  void insert(T key) {
    std::shared_ptr<node> p = nullptr;
    std::shared_ptr<node> x = this->root;
    while(x){
      p = x;
      if(key < x->info)
        x = x->left;
      else
        x = x->right;
    }
    std::shared_ptr<node> t_node = this->new_node(key, p);
    if(p == nullptr)
      this->root = t_node;
    else{
      if(key < p->info)
        p->left = t_node;
      else
        p->right = t_node;
    }
    _insert(t_node);
    _size += 1;
  }

  /**
   *@brief remove function.
   *@param key: key to be removed.
   */
  void remove(T key){
    std::shared_ptr<node> t_node = root;
    while(t_node && t_node->info != key){
      if(key < t_node->info)
        t_node = t_node->left;
      else
        t_node = t_node->right;
    }
    _remove(t_node);
    _size -= 1;
  }

  /**
   * @brief size function
   *
   * @return size_t the size of the tree
   */
  size_t size() { return _size; }

  /**
   * @brief clear function
   */
  void clear() {
    root = nullptr;
    _size = 0;
  }

  /**
   *@brief inorder function.
   *@returns vector<T>, the elements inorder.
   */
  std::vector<T> inorder() {
    std::vector<T> path;
    _inorder(
        [&](T callbacked) {
          path.push_back(callbacked);
        },
        root);
    return path;
  }

  /**
   *@brief postorder function.
   *@returns vector<T>, the elements postorder.
   */
  std::vector<T> postorder() {
    std::vector<T> path;
    _postorder(
        [&](T callbacked) {
          path.push_back(callbacked);
        },
        root);
    return path;
  }

  /**
   *@brief preorder function.
   *@returns vector<T>, the elements preorder.
   */
  std::vector<T> preorder() {
    std::vector<T> path;
    _preorder(
        [&](T callbacked) {
          path.push_back(callbacked);
        },
        root);
    return path;
  }

  /**
   * @brief level order function
   * @return vector<vector<T>>, the level order traversal of the tree
   */
  std::vector<std::vector<T>> level_order() {
    std::vector<std::vector<T>> path;
    std::queue<std::shared_ptr<node>> q;
    q.push(root);
    while (!q.empty()) {
      size_t size = q.size();
      std::vector<T> level;
      for (size_t i = 0; i < size; i++) {
        std::shared_ptr<node> current = q.front();
        q.pop();
        level.push_back(current->info);
        if (current->left) {
          q.push(current->left);
        }
        if (current->right) {
          q.push(current->right);
        }
      }
      path.push_back(level);
    }
    return path;
  }

  /**
   * @brief operator << for red black tree class
   */
  friend ostream &operator<<(ostream &out, red_black_tree<T> &rb){
    std::vector<T> order = rb.inorder();
    for(int i=0;i<order.size();i++){
      if(i == order.size()-1)
        out<<order[i]<<'\n';
      else
        out<<order[i]<<", ";
    }
    return out;
  }
};

#endif