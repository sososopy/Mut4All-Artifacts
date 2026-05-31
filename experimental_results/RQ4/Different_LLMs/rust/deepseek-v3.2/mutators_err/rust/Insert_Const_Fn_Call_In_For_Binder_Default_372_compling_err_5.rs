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

pub struct Insert_Const_Fn_Call_In_For_Binder_Default_372;

impl Mutator for Insert_Const_Fn_Call_In_For_Binder_Default_372 {
    fn name(&self) -> &str {
        "Insert_Const_Fn_Call_In_For_Binder_Default_372"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_fns = Vec::new();
        let mut visitor = ConstFnVisitor {
            const_fns: &mut const_fns,
        };
        visitor.visit_file(file);

        let const_fn_name = if const_fns.is_empty() {
            let new_fn = parse_quote! {
                const fn __mutator_const_fn() {}
            };
            file.items.insert(0, Item::Fn(new_fn));
            Ident::new("__mutator_const_fn", Span::call_site())
        } else {
            const_fns.choose(&mut thread_rng()).unwrap().clone()
        };

        let mut visitor = ForBinderVisitor {
            const_fn_name: &const_fn_name,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets `for<...>` binders in where clauses that have const generic parameters with default values. It transforms the default value expression by wrapping it in a block that defines a local const helper function which calls an existing const function, then calls the helper, and finally evaluates to the original default. This stresses the compiler's const evaluation within generic parameter defaults, particularly for late-bound generic parameters introduced by `for<...>` binders, testing edge cases in const generics and non-lifetime binders features."
    }
}

struct ConstFnVisitor<'a> {
    const_fns: &'a mut Vec<Ident>,
}

impl<'a> Visit<'a> for ConstFnVisitor<'a> {
    fn visit_item_fn(&mut self, node: &'a ItemFn) {
        if node.sig.constness.is_some() {
            self.const_fns.push(node.sig.ident.clone());
        }
        syn::visit::visit_item_fn(self, node);
    }
}

struct ForBinderVisitor<'a> {
    const_fn_name: &'a Ident,
}

impl<'a> VisitMut for ForBinderVisitor<'a> {
    fn visit_where_predicate_mut(&mut self, node: &mut syn::WherePredicate) {
        if let syn::WherePredicate::Type(pred_type) = node {
            if let Some(lifetimes) = &mut pred_type.lifetimes {
                for lifetime in &mut lifetimes.lifetimes {
                    if let syn::GenericParam::Const(const_param) = lifetime {
                        if let Some(default) = &mut const_param.default {
                            let original_expr = default.clone();
                            let const_fn_name = self.const_fn_name.clone();
                            let new_block: Expr = parse_quote! {
                                {
                                    const fn __mutator_helper() { #const_fn_name(); }
                                    __mutator_helper();
                                    #original_expr
                                }
                            };
                            *default = new_block;
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_where_predicate_mut(self, node);
    }
}