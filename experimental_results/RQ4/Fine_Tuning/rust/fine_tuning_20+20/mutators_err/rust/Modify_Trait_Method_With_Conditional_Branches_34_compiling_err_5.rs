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

pub struct Modify_Trait_Method_With_Conditional_Branches_34;

impl Mutator for Modify_Trait_Method_With_Conditional_Branches_34 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_With_Conditional_Branches_34"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Fn(method) = item {
                        let mut has_conditional = false;
                        let mut visitor = ConditionalVisitor {
                            has_conditional: &mut has_conditional,
                        };
                        if let Some(default_block) = &method.default {
                            visitor.visit_block(default_block);
                        }
                        if has_conditional {
                            let mut mutator = ConditionalMutator;
                            if let Some(default_block) = &mut method.default {
                                mutator.visit_block_mut(default_block);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets methods within traits that contain conditional expressions (e.g., `if`, `match`). It introduces an unused mutable reference to `self` within each branch of the conditional, enforcing a two-phase borrow that can stress the borrow checker and potentially reveal inconsistencies or edge cases in the compiler's handling of mutable references within trait method contexts."
    }
}

struct ConditionalVisitor<'a> {
    has_conditional: &'a mut bool,
}

impl<'a> Visit<'a> for ConditionalVisitor<'a> {
    fn visit_expr(&mut self, node: &'a Expr) {
        match node {
            Expr::If(_) | Expr::Match(_) => {
                *self.has_conditional = true;
            }
            _ => {}
        }
        syn::visit::visit_expr(self, node);
    }
}

struct ConditionalMutator;

impl VisitMut for ConditionalMutator {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        match node {
            Expr::If(expr_if) => {
                let mut new_block = expr_if.then_branch.clone();
                new_block
                    .stmts
                    .insert(0, parse_quote! { let _temp_ref = &mut *self; });
                expr_if.then_branch = new_block;
                if let Some((_, else_branch)) = &mut expr_if.else_branch {
                    match else_branch.as_mut() {
                        Expr::If(_) => {
                            self.visit_expr_mut(else_branch);
                        }
                        _ => {
                            let mut new_block: Block = parse_quote!({ #else_branch });
                            new_block
                                .stmts
                                .insert(0, parse_quote! { let _temp_ref = &mut *self; });
                            *else_branch = Box::new(Expr::Block(ExprBlock {
                                attrs: Vec::new(),
                                label: None,
                                block: new_block,
                            }));
                        }
                    }
                }
            }
            Expr::Match(expr_match) => {
                for arm in &mut expr_match.arms {
                    let mut new_body: Expr = parse_quote!({ #arm.body });
                    if let Expr::Block(expr_block) = &mut new_body {
                        expr_block
                            .block
                            .stmts
                            .insert(0, parse_quote! { let _temp_ref = &mut *self; });
                    }
                    arm.body = Box::new(new_body);
                }
            }
            _ => {
                syn::visit_mut::visit_expr_mut(self, node);
            }
        }
    }
}