;; ====== Automatically load abbreviations table =====

;; ===== Set the highlight current line minor mode =====

;; In every buffer, the line which contains the cursor will be fully
;; highlighted

;;(global-hl-line-mode 1)

;; ===== Set standard indent to 2 rather that 4 ====
(setq standard-indent 2)

;; ========== Line by line scrolling ==========

;; This makes the buffer scroll by only a single line when the up or
;; down cursor keys push the cursor (tool-bar-mode) outside the
;; buffer. The standard emacs behaviour is to reposition the cursor in
;; the center of the screen, but this can make the scrolling confusing

(setq scroll-step 1)
(setq mouse-wheel-scroll-amount '(1 ((shift) . 1))) ;; one line at a time
(setq mouse-wheel-progressive-speed nil) ;; don't accelerate scrolling
(setq mouse-wheel-follow-mouse 't) ;; scroll window under mouse

;; ========== Support Wheel Mouse Scrolling ==========

(mouse-wheel-mode t)

;; ========== Enable Line and Column Numbering ==========

;; Show line-number in the mode line
(line-number-mode 1)
(global-linum-mode t)

;; Show column-number in the mode line
(column-number-mode 1)

;; ========== Set the fill column ==========

;; Enable backup files.
(setq-default fill-column 72)

;; ============================
;; Setup syntax, background, and foreground coloring
;; ============================

;;(set-background-color "White")
;;(set-foreground-color "Black")
;;(set-cursor-color "DarkCyan")
(set-mouse-color "DarkSlateBlue")
(global-font-lock-mode t)
(setq font-lock-maximum-decoration t)

;; ============================
;; Display
;; ============================

;; disable startup message
(setq inhibit-startup-message t)

;; setup font
;;   "-Misc-Fixed-Medium-R-Normal--15-140-75-75-C-90-ISO8859-1")
;;   "-misc-fixed-bold-*-normal-*-14-130-*-*-*-70-iso8859-*")
;;(set-default-font
;;  "-misc-fixed-bold-*-normal-*-14-130-*-*-*-70-iso8859-*")

;;(add-to-list 'default-frame-alist '(font . "-adobe-courier-bold-r-normal--20-140-100-100-m-110-iso8859-1"))
;;(add-to-list 'default-frame-alist '(font . "-misc-fixed-bold-*-normal-*-14-130-*-*-*-70-iso8859-*"))
;;(add-to-list 'default-frame-alist '(font . "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso10646-1"))
;;(add-to-list 'default-frame-alist '(font . "-misc-fixed-bold-r-normal--18-120-100-100-c-90-iso10646-1"))
(add-to-list 'default-frame-alist '(font . "DejaVu Sans Mono-10"))


;; display the current time
(display-time)

;; Show column number at bottom of screen
(column-number-mode 1)

;; alias y to yes and n to no
(defalias 'yes-or-no-p 'y-or-n-p)

;; highlight matches from searches
(setq isearch-highlight t)
(setq search-highlight t)
(setq-default transient-mark-mode t)

(when (fboundp 'blink-cursor-mode)
    (blink-cursor-mode -1))

;; ===========================
;; Behaviour
;; ===========================

;; Pgup/dn will return exactly to the starting point.
(setq scroll-preserve-screen-position 1)

;; don't automatically add new lines when scrolling down at
;; the bottom of a buffer
(setq next-line-add-newlines nil)

;; scroll just one line when hitting the bottom of the window
(setq scroll-step 1)
(setq scroll-conservatively 1)

;; setting spell check with aspell 
(setq ispell-program-name "/usr/bin/aspell")

;; ========== Support Wheel Mouse Scrolling ==========

(mouse-wheel-mode t) 

;; format the title-bar to always include the buffer name
(setq frame-title-format "emacs - %b")

(global-set-key (kbd "M-<up>") 'windmove-up)
(global-set-key (kbd "M-<down>") 'windmove-down)
(global-set-key (kbd "M-<left>") 'windmove-left)
(global-set-key (kbd "M-<right>") 'windmove-right)

(global-set-key (kbd "C-c <up>") 'windmove-up)
(global-set-key (kbd "C-c <down>") 'windmove-down)
(global-set-key (kbd "C-c <left>") 'windmove-left)
(global-set-key (kbd "C-c <right>") 'windmove-right)

(global-set-key (kbd "C-x <M-up>") 'split-window-vertically)
(global-set-key (kbd "C-x <M-down>") 'split-window-below)
(global-set-key (kbd "C-x <M-left>") 'split-window-horizontally)
(global-set-key (kbd "C-x <M-right>") 'split-window-right)

(global-set-key (kbd "C-c m") 'menu-bar-open)
(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(send-mail-function (quote mailclient-send-it)))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )


;; Default tab width
(setq-default tab-width 2)

;; Function that pastes the current buffer name in shell command line
(define-key minibuffer-local-map [f3]
  (lambda () (interactive)
     (insert (buffer-name (window-buffer (minibuffer-selected-window))))))
