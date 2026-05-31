use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprMatch, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct OpaqueTraitSubtypePatternMismatch_155;

impl Mutator for OpaqueTraitSubtypePatternMismatch_155 {
    fn name(&self) -> &str {
        "OpaqueTraitSubtypePatternMismatch_155"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let syn::Type::ImplTrait(impl_trait) = &**return_type {
                        for bound in &impl_trait.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.lifetimes.is_some() {
                                    let target_fn_name = &func.sig.ident;
                                    let mut finder = MatchFinder { target_fn_name };
                                    finder.visit_block_mut(&mut func.block);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct MatchFinder<'a> {
    target_fn_name: &'a Ident,
}

impl<'a> VisitMut for MatchFinder<'a> {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Match(expr_match) = node {
            if let Expr::Call(call_expr) = &*expr_match.expr {
                if let Expr::Path(path_expr) = &*call_expr.func {
                    if path_expr.path.is_ident(self.target_fn_name) {
                        let new_arm = parse_quote! {
                            OtherEnum::Variant(y: impl for<'a, 'b> Fn(Inv<'a>, Inv<'b>)) => y(Inv(&mut &()), Inv(&mut &())),
                        };
                        expr_match.arms.push(new_arm);
                    }
                }
            }
            self.visit_expr_mut(&mut expr_match.expr);
            for arm in &mut expr_match.arms {
                self.visit_arm_mut(arm);
            }
            return;
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
}