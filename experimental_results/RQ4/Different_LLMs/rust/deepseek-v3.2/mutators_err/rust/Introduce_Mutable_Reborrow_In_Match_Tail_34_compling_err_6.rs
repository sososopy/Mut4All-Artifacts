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

pub struct Introduce_Mutable_Reborrow_In_Match_Tail_34;

impl Mutator for Introduce_Mutable_Reborrow_In_Match_Tail_34 {
    fn name(&self) -> &str {
        "Introduce_Mutable_Reborrow_In_Match_Tail_34"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.inputs.len() > 0 {
                            let first_arg = &method.sig.inputs[0];
                            if let syn::FnArg::Receiver(receiver) = first_arg {
                                if receiver.mutability.is_some() {
                                    let mut visitor = MatchArmReborrowVisitor {
                                        method_name: method.sig.ident.clone(),
                                        self_type: item_impl.self_ty.clone(),
                                        mutated: false,
                                    };
                                    visitor.visit_block_mut(&mut method.block);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match expressions inside methods that take &mut self. It applies when a match arm contains a recursive call that is the tail expression, and the match scrutinee involves a mutable reference. The mutation introduces a mutable reborrow of self in the tail position before the recursive call, creating a two-phase borrow scenario. Specifically, it changes a recursive method call expression like self.method() to { let _reb = &mut *self; self.method() }. This introduces a mutable reborrow via &mut *self, stored in a temporary variable, but then still calls the method on the original self. This creates a situation where the compiler may need to handle two-phase borrows for the temporary reborrow and the method call. The mutation should only be applied if the method being called is defined on the same type and takes &mut self, and the call is in tail position (i.e., the result of the match arm)."
    }
}

struct MatchArmReborrowVisitor {
    method_name: Ident,
    self_type: Box<syn::Type>,
    mutated: bool,
}

impl VisitMut for MatchArmReborrowVisitor {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let syn::Expr::Match(expr_match) = expr {
            for arm in &mut expr_match.arms {
                if let syn::Expr::Call(call_expr) = &*arm.body {
                    if let syn::Expr::Path(path_expr) = &*call_expr.func {
                        if path_expr.path.segments.len() ==,1 {
                            let segment = &path_expr.path.segments[0];
                            if segment.ident == "self" {
                                if let syn::PathArguments::None = &segment.arguments {
                                    let new_body: syn::Expr = parse_quote! {
                                        {
                                            let _reb = &mut *self;
                                            self.#method_name()
                                        }
                                    };
                                    arm.body = Box::new(new_body);
                                    self.mutated = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}