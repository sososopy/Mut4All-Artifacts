use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Insert_Yield_After_Mutable_Borrow_117;

impl Mutator for Insert_Yield_After_Mutable_Borrow_117 {
    fn name(&self) -> &str {
        "Insert_Yield_After_Mutable_Borrow_117"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct NestedGeneratorVisitor<'a> {
            outer_params: HashSet<Ident>,
            mutations: Vec<(Box<Expr>, Span)>,
        }

        impl<'a> VisitMut for NestedGeneratorVisitor<'a> {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Closure(closure) = expr {
                    if closure.capture == Some(token::Move::default()) || closure.capture.is_none() {
                        // Check if closure uses yield (generator)
                        let mut has_yield = false;
                        let mut visitor = YieldChecker { has_yield: false };
                        visitor.visit_expr(&closure.body);
                        has_yield = visitor.has_yield;

                        if has_yield {
                            // This is a generator (inner generator)
                            // Collect mutable borrows of outer parameters
                            let mut borrow_visitor = BorrowVisitor {
                                outer_params: &self.outer_params,
                                borrow_points: Vec::new(),
                            };
                            borrow_visitor.visit_expr(&closure.body);
                            for (borrow_expr, span) in borrow_visitor.borrow_points {
                                self.mutations.push((borrow_expr, span));
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }

            fn visit_item_mut(&mut self, item: &mut Item) {
                if let Item::Fn(item_fn) = item {
                    // Collect outer generator parameters (assuming they are mutable references)
                    for input in &item_fn.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                if let Type::Reference(type_ref) = &*pat_type.ty {
                                    if type_ref.mutability.is_some() {
                                        self.outer_params.insert(pat_ident.ident.clone());
                                    }
                                }
                            }
                        }
                    }
                    // Check if outer function uses yield (generator)
                    let mut has_yield = false;
                    let mut visitor = YieldChecker { has_yield: false };
                    visitor.visit_block(&item_fn.block);
                    has_yield = visitor.has_yield;

                    if has_yield {
                        // This is an outer generator, visit its body to find inner generators
                        syn::visit_mut::visit_block_mut(self, &mut item_fn.block);
                    }
                } else {
                    syn::visit_mut::visit_item_mut(self, item);
                }
            }
        }

        struct YieldChecker {
            has_yield: bool,
        }

        impl Visit for YieldChecker {
            fn visit_expr(&mut self, expr: &Expr) {
                if let Expr::Yield(_) = expr {
                    self.has_yield = true;
                }
                syn::visit::visit_expr(self, expr);
            }
        }

        struct BorrowVisitor<'a> {
            outer_params: &'a HashSet<Ident>,
            borrow_points: Vec<(Box<Expr>, Span)>,
        }

        impl<'a> Visit for BorrowVisitor<'a> {
            fn visit_expr(&mut self, expr: &Expr) {
                if let Expr::Assign(assign) = expr {
                    if let Expr::Reference(ref_expr) = &*assign.left {
                        if ref_expr.mutability.is_some() {
                            if let Expr::Path(path_expr) = &*ref_expr.expr {
                                if let Some(ident) = path_expr.path.get_ident() {
                                    if self.outer_params.contains(ident) {
                                        self.borrow_points.push((Box::new(expr.clone()), expr.span()));
                                    }
                                }
                            }
                        }
                    }
                } else if let Expr::Let(let_expr) = expr {
                    if let Expr::Reference(ref_expr) = &*let_expr.expr {
                        if ref_expr.mutability.is_some() {
                            if let Expr::Path(path_expr) = &*ref_expr.expr {
                                if let Some(ident) = path_expr.path.get_ident() {
                                    if self.outer_params.contains(ident) {
                                        self.borrow_points.push((Box::new(expr.clone()), expr.span()));
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit::visit_expr(self, expr);
            }
        }

        let mut visitor = NestedGeneratorVisitor {
            outer_params: HashSet::new(),
            mutations: Vec::new(),
        };
        visitor.visit_file_mut(file);

        // Apply mutations: insert yield after each mutable borrow
        for (borrow_expr, span) in visitor.mutations {
            let mut transformer = YieldInsertTransformer {
                target_expr: borrow_expr,
                target_span: span,
                inserted: false,
            };
            transformer.visit_file_mut(file);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets nested generators where an inner generator captures mutable references from outer generator parameters. It identifies mutable borrows of these outer parameters within the inner generator's body and inserts a yield statement immediately after each such borrow. This transformation creates additional suspension points that may interfere with drop-tracking and borrow-checking, potentially triggering complex lifetime errors or ICEs related to generator state management."
    }
}

struct YieldInsertTransformer {
    target_expr: Box<Expr>,
    target_span: Span,
    inserted: bool,
}

impl VisitMut for YieldInsertTransformer {
    fn visit_stmt_mut(&mut self, stmt: &mut Stmt) {
        if let Stmt::Expr(expr, _) = stmt {
            if expr.span() == self.target_span && *expr == *self.target_expr {
                // Check if next statement is already a yield
                // We need to modify the block containing this statement
                // This requires more context; for simplicity, we assume we can insert after this stmt
                // In practice, we would need to locate the block and insert there.
                // For this template, we'll insert a yield as a new statement after the current one.
                // Since we cannot directly modify the sequence here, we'll note the position.
                self.inserted = true;
            }
        }
        syn::visit_mut::visit_stmt_mut(self, stmt);
    }

    fn visit_block_mut(&mut self, block: &mut Block) {
        // Look for the target expression within statements
        for i in 0..block.stmts.len() {
            if let Stmt::Expr(expr, _) = &block.stmts[i] {
                if expr.span() == self.target_span && *expr == *self.target_expr {
                    // Check if next statement is a yield
                    if i + 1 < block.stmts.len() {
                        if let Stmt::Expr(next_expr, _) = &block.stmts[i + 1] {
                            if let Expr::Yield(_) = &**next_expr {
                                // Already has yield after, skip
                                return;
                            }
                        }
                    }
                    // Insert yield statement after
                    let yield_stmt: Stmt = parse_quote! { yield; };
                    block.stmts.insert(i + 1, yield_stmt);
                    self.inserted = true;
                    break;
                }
            }
        }
        syn::visit_mut::visit_block_mut(self, block);
    }
}