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

pub struct Replace_Const_Generic_Bound_With_Mismatched_Fn_100;

impl Mutator for Replace_Const_Generic_Bound_With_Mismatched_Fn_100 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Bound_With_Mismatched_Fn_100"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut const_fn_name = Ident::new("mismatch_const_fn", Span::call_site());
        let mut added_function = false;

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                let mut visitor = ConstGenericVisitor {
                    rng: &mut rng,
                    const_fn_name: &const_fn_name,
                    added_function: &mut added_function,
                };
                visitor.visit_item_fn_mut(item_fn);
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        let mut visitor = ConstGenericVisitor {
                            rng: &mut rng,
                            const_fn_name: &const_fn_name,
                            added_function: &mut added_function,
                        };
                        visitor.visit_impl_item_fn_mut(func);
                    }
                }
            }
        }

        if added_function {
            let new_fn: ItemFn = parse_quote! {
                const fn mismatch_const_fn<const A: bool, const B: bool>() -> bool { A }
            };
            file.items.push(Item::Fn(new_fn));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait bounds that use const generic parameters in const expression contexts. It replaces the const generic parameter with a call to a const function that expects a mismatched number of const generic parameters. If such a function does not exist, it introduces a new const function with mismatched arity. This transformation creates a mismatch between expected and supplied const generic parameters, potentially triggering ICEs related to const evaluation and trait resolution."
    }
}

struct ConstGenericVisitor<'a> {
    rng: &'a mut rand::rngs::ThreadRng,
    const_fn_name: &'a Ident,
    added_function: &'a mut bool,
}

impl<'a> VisitMut for ConstGenericVisitor<'a> {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Path(type_path) = node {
            for segment in &mut type_path.path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(const_expr) = arg {
                            if let Expr::Path(expr_path) = const_expr {
                                if expr_path.path.segments.len() == 1 {
                                    let ident = &expr_path.path.segments[0].ident;
                                    if ident.to_string().chars().next().unwrap().is_uppercase() {
                                        let new_expr: Expr = parse_quote! {
                                            { mismatch_const_fn::<#ident>() }
                                        };
                                        *const_expr = new_expr;
                                        *self.added_function = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, node);
    }

    fn visit_where_predicate_mut(&mut self, node: &mut WherePredicate) {
        if let WherePredicate::Type(pred_type) = node {
            if let Type::Path(type_path) = &mut pred_type.bounded_ty {
                for segment in &mut type_path.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Const(const_expr) = arg {
                                if let Expr::Path(expr_path) = const_expr {
                                    if expr_path.path.segments.len() == 1 {
                                        let ident = &expr_path.path.segments[0].ident;
                                        if ident.to_string().chars().next().unwrap().is_uppercase() {
                                            let new_expr: Expr = parse_quote! {
                                                { mismatch_const_fn::<#ident>() }
                                            };
                                            *const_expr = new_expr;
                                            *self.added_function = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_where_predicate_mut(self, node);
    }
}