use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath, 
    PathArguments, token::Paren, token::Plus,
    ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Split_Match_Arm_With_Recursive_Call_Into_Nested_If_Let_And_Match_With_Separate_Borrows_34;

impl Mutator for Split_Match_Arm_With_Recursive_Call_Into_Nested_If_Let_And_Match_With_Separate_Borrows_34 {
    fn name(&self) -> &str {
        "Split_Match_Arm_With_Recursive_Call_Into_Nested_If_Let_And_Match_With_Separate_Borrows_34"
    }

    fn mutate(&self, file: &mut syn::File) {
        use syn::visit_mut::VisitMut;

        struct MatchTransformer;

        impl VisitMut for MatchTransformer {
            fn visit_expr_match_mut(&mut self, expr: &mut syn::ExprMatch) {
                for arm in &mut expr.arms {
                    if let Some(ref guard) = arm.guard {
                        if has_self_call(&guard.1) {
                            let mut other_arms: Vec<syn::Arm> = Vec::new();
                            let mut target_arm: Option<syn::Arm> = None;
                            for a in &mut expr.arms {
                                if a == arm {
                                    target_arm = Some(parse_quote!(#a));
                                } else {
                                    other_arms.push(parse_quote!(#a));
                                }
                            }

                            if let Some(target_arm) = target_arm {
                                let pat = target_arm.pat;
                                let guard_expr = target_arm.guard.as_ref().unwrap().1.clone();
                                let target_body: syn::Expr = parse_quote!(#target_arm.body);

                                let other_arm_body: syn::Expr = if let Some(other_arm) = other_arms.get(0) {
                                    parse_quote!(#other_arm.body)
                                } else {
                                    parse_quote! { unreachable!() }
                                };

                                let ok_arm: syn::Arm = parse_quote! {
                                    Ok(json) => #other_arm_body,
                                };
                                let err_arm: syn::Arm = parse_quote! {
                                    Err(e) => {
                                        tracing::error!(?e);
                                        Bson::Null
                                    },
                                };

                                let nested_match: syn::ExprMatch = parse_quote! {
                                    match #expr.expr {
                                        #ok_arm
                                        #err_arm
                                    }
                                };

                                let if_block: syn::Expr = parse_quote! {
                                    {
                                        let _borrow = &mut self.field;
                                        if #guard_expr {
                                            // Use _borrow to ensure it's active during the recursive call
                                        }
                                        #nested_match
                                    }
                                };

                                let else_block: syn::Expr = parse_quote! {
                                    {
                                        #target_body
                                    }
                                };

                                let new_expr: syn::Expr = parse_quote! {
                                    if let #pat = #expr.expr {
                                        #if_block
                                    } else {
                                        #else_block
                                    }
                                };

                                *expr = new_expr;
                                return;
                            }
                        }
                    }
                }

                syn::visit_mut::visit_expr_match_mut(self, expr);
            }
        }

        let mut transformer = MatchTransformer;
        transformer.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms match expressions with a guard containing a recursive call to `self` into an `if let` with a mutable borrow of a field, followed by a nested `match`. This introduces two distinct mutable borrows in different control paths, increasing the likelihood of overlapping 2-phase borrows and testing the borrow checker's handling of temporary lifetimes."
    }
}

fn has_self_call(expr: &syn::Expr) -> bool {
    let mut found = false;
    struct SelfCallFinder<'a> {
        found: &'a mut bool,
    }

    impl<'a> syn::visit::Visit<'_> for SelfCallFinder<'a> {
        fn visit_expr_call(&mut self, expr: &syn::ExprCall) {
            if let syn::Expr::Path(path_expr) = &*expr.func {
                if let Some(segment) = path_expr.path.segments.last() {
                    if segment.ident == "self" {
                        *self.found = true;
                        return;
                    }
                }
            }
            syn::visit::visit_expr_call(self, expr);
        }
    }

    let mut finder = SelfCallFinder { found: &mut found };
    finder.visit_expr(expr);
    found
}