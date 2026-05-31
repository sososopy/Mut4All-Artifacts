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

pub struct Add_Unnecessary_Clone_In_Async_Move_Closure_20;

impl Mutator for Add_Unnecessary_Clone_In_Async_Move_Closure_20 {
    fn name(&self) -> &str {
        "Add_Unnecessary_Clone_In_Async_Move_Closure_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AsyncClosureVisitor {
            target_closure: Option<Expr>,
        }

        impl Visit for AsyncClosureVisitor {
            fn visit_expr_closure(&mut self, i: &ExprClosure) {
                if i.asyncness.is_some() && i.movability.is_some() {
                    self.target_closure = Some(Expr::Closure(i.clone()));
                }
            }
        }

        struct AwaitExprVisitor {
            await_exprs: Vec<ExprAwait>,
        }

        impl Visit for AwaitExprVisitor {
            fn visit_expr_await(&mut self, i: &ExprAwait) {
                self.await_exprs.push(i.clone());
            }
        }

        struct CloneInjector {
            target_ident: Ident,
            clone_ident: Ident,
            clone_expr: Expr,
            clone_injected: bool,
        }

        impl VisitMut for CloneInjector {
            fn visit_expr_mut(&mut self, node: &mut Expr) {
                if let Expr::Await(expr_await) = node {
                    if let Expr::Path(expr_path) = &*expr_await.base {
                        if expr_path.path.is_ident(&self.target_ident) {
                            *node = Expr::Await(ExprAwait {
                                base: Box::new(Expr::Path(ExprPath {
                                    attrs: Vec::new(),
                                    qself: None,
                                    path: self.clone_ident.clone().into(),
                                })),
                                ..expr_await.clone()
                            });
                            self.clone_injected = true;
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, node);
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut async_closure_visitor = AsyncClosureVisitor {
                    target_closure: None,
                };
                async_closure_visitor.visit_item_fn(item_fn);
                if let Some(target_closure) = async_closure_visitor.target_closure {
                    let mut await_expr_visitor = AwaitExprVisitor {
                        await_exprs: Vec::new(),
                    };
                    await_expr_visitor.visit_expr(&target_closure);
                    for await_expr in await_expr_visitor.await_exprs {
                        if let Expr::Path(expr_path) = &*await_expr.base {
                            if expr_path.path.segments.len() == 1 {
                                let target_ident = expr_path.path.segments[0].ident.clone();
                                let clone_ident = Ident::new(
                                    &format!("{}_clone", target_ident),
                                    Span::call_site(),
                                );
                                let clone_expr: Expr = parse_quote! { #target_ident.clone() };
                                let mut clone_injector = CloneInjector {
                                    target_ident,
                                    clone_ident,
                                    clone_expr,
                                    clone_injected: false,
                                };
                                clone_injector.visit_expr_mut(&mut target_closure.clone());
                                if clone_injector.clone_injected {
                                    let new_closure = if let Expr::Closure(mut closure) =
                                        target_closure.clone()
                                    {
                                        closure.body = Box::new(Expr::Block(ExprBlock {
                                            attrs: Vec::new(),
                                            label: None,
                                            block: {
                                                let mut new_block = closure.body.clone();
                                                new_block.stmts.insert(
                                                    0,
                                                    Stmt::Local(Local {
                                                        attrs: Vec::new(),
                                                        let_token: token::Let {
                                                            span: Span::call_site(),
                                                        },
                                                        pat: Pat::Ident(PatIdent {
                                                            attrs: Vec::new(),
                                                            by_ref: None,
                                                            mutability: None,
                                                            ident: clone_injector.clone_ident,
                                                            subpat: None,
                                                        }),
                                                        init: Some((
                                                            token::Eq {
                                                                spans: [Span::call_site()],
                                                            },
                                                            Box::new(clone_injector.clone_expr),
                                                        )),
                                                        semi_token: token::Semi {
                                                            spans: [Span::call_site()],
                                                        },
                                                    }),
                                                );
                                                new_block
                                            },
                                        }));
                                        Expr::Closure(closure)
                                    } else {
                                        target_closure.clone()
                                    };
                                    let mut new_items = Vec::new();
                                    for item in &item_fn.block.stmts {
                                        if let Stmt::Expr(expr, _) = item {
                                            if *expr == target_closure {
                                                new_items.push(Stmt::Expr(new_closure.clone(), None));
                                            } else {
                                                new_items.push(item.clone());
                                            }
                                        } else {
                                            new_items.push(item.clone());
                                        }
                                    }
                                    item_fn.block.stmts = new_items;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async move closures within chained futures operations by introducing unnecessary clones of moved variables before their use in await expressions. It identifies async move closures, locates moved variables used in await operations, and replaces their usage with a cloned version. This transformation stresses the compiler's handling of async data flow and borrow checking by injecting redundant data copies, potentially triggering subtle issues in async execution and memory management."
    }
}