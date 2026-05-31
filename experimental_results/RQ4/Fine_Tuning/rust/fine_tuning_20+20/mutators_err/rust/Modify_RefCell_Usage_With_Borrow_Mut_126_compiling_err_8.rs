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

pub struct Modify_RefCell_Usage_With_Borrow_Mut_126;

impl Mutator for Modify_RefCell_Usage_With_Borrow_Mut_126 {
    fn name(&self) -> &str {
        "Modify_RefCell_Usage_With_Borrow_Mut_126"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct RefCellVisitor {
            target_exprs: Vec<Expr>,
        }

        impl<'ast> Visit<'ast> for RefCellVisitor {
            fn visit_expr(&mut self, node: &'ast Expr) {
                if let Expr::MethodCall(method_call) = node {
                    if method_call.method == "borrow_mut"
                        && method_call
                            .receiver
                            .as_ref()
                            .map(|expr| {
                                matches!(
                                    expr,
                                    Expr::Path(ExprPath {
                                        path: SynPath {
                                            segments,
                                            ..
                                        },
                                        ..
                                    }) if segments.last().map_or(false, |segment| segment.ident == "RefCell")
                                )
                            })
                            .unwrap_or(false)
                    {
                        self.target_exprs.push(node.clone());
                    }
                }
                syn::visit::visit_expr(self, node);
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = RefCellVisitor {
                    target_exprs: Vec::new(),
                };
                visitor.visit_item_fn(item_fn);
                for target_expr in visitor.target_exprs {
                    let new_expr: Expr = parse_quote! {
                        {
                            let tmp = #target_expr;
                            tmp
                        }
                    };
                    let mut new_block = item_fn.block.clone();
                    new_block.stmts.clear();
                    new_block.stmts.push(Stmt::Expr(new_expr, None));
                    item_fn.block = new_block;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify `RefCell` usage with `borrow_mut()` and replace it with `borrow()` to enforce an immutable borrow, potentially triggering borrow checker conflicts and ICEs."
    }
}