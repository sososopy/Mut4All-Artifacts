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

pub struct Replace_Const_Generic_With_Function_Item_432;

impl Mutator for Replace_Const_Generic_With_Function_Item_432 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Function_Item_432"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut function_names = HashSet::new();
        let mut impls_with_const_generics = Vec::new();

        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                function_names.insert(item_fn.sig.ident.to_string());
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let const_params: Vec<String> = item_impl.generics.params.iter()
                    .filter_map(|param| {
                        if let GenericParam::Const(const_param) = param {
                            Some(const_param.ident.to_string())
                        } else {
                            None
                        }
                    })
                    .collect();

                if !const_params.is_empty() {
                    impls_with_const_generics.push((item_impl, const_params));
                }
            }
        }

        for (item_impl, const_params) in impls_with_const_generics {
            for const_param_name in const_params {
                if function_names.contains(&const_param_name) {
                    let mut new_generics = item_impl.generics.clone();
                    new_generics.params.retain(|param| {
                        if let GenericParam::Const(const_param) = param {
                            const_param.ident.to_string() != const_param_name
                        } else {
                            true
                        }
                    });

                    item_impl.generics = new_generics;

                    let mut visitor = ConstGenericReplacer {
                        target_name: const_param_name.clone(),
                    };
                    visitor.visit_type_mut(&mut item_impl.self_ty);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator locates impl blocks with const generic parameters and checks if functions with matching identifiers exist in the same scope. When a match is found, it removes the const generic parameter from the impl header and replaces its usage in the self type with the function item identifier. This mimics a compiler bug where function items are incorrectly accepted as const arguments, potentially triggering internal compiler errors about unconstrained type parameters and const argument evaluation issues."
    }
}

struct ConstGenericReplacer {
    target_name: String,
}

impl VisitMut for ConstGenericReplacer {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Array(type_array) = ty {
            if let Expr::Path(expr_path) = &mut *type_array.len {
                if expr_path.path.segments.len() == 1 {
                    let segment = &expr_path.path.segments[0];
                    if segment.ident.to_string() == self.target_name {
                        *type_array.len = parse_quote!(#segment);
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}