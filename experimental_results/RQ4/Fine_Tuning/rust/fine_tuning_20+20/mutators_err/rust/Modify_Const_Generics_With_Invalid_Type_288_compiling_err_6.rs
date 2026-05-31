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

pub struct Modify_Const_Generics_With_Invalid_Type_288;

impl Mutator for Modify_Const_Generics_With_Invalid_Type_288 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_With_Invalid_Type_288"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let const_generic_indices: Vec<usize> = func
                    .sig
                    .generics
                    .params
                    .iter()
                    .enumerate()
                    .filter_map(|(i, param)| match param {
                        syn::GenericParam::Const(_) => Some(i),
                        _ => None,
                    })
                    .collect();

                for const_generic_index in const_generic_indices {
                    let mut type_replacer = ConstGenericTypeReplacer {
                        const_generic_index,
                    };
                    type_replacer.visit_signature_mut(&mut func.sig);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets functions with const generics by replacing their const generic types with incompatible ones, such as changing primitive types to non-primitive types within the const generic context. This transformation is designed to exploit the compiler's handling of const generics and associated types, particularly under experimental features, to trigger type checking failures and potential internal compiler errors."
    }
}

struct ConstGenericTypeReplacer {
    const_generic_index: usize,
}

impl VisitMut for ConstGenericTypeReplacer {
    fn visit_type_path_mut(&mut self, type_path: &mut TypePath) {
        if let Some(segment) = type_path.path.segments.last() {
            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                if let Some(GenericArgument::Const(syn::Expr::Path(expr_path))) =
                    args.args.iter().nth(self.const_generic_index)
                {
                    if let Some(last_segment) = expr_path.path.segments.last() {
                        let new_ident = Ident::new("String", Span::call_site());
                        let mut new_segment = last_segment.clone();
                        new_segment.ident = new_ident;
                        let mut new_segments = expr_path.path.segments.clone();
                        new_segments.pop();
                        new_segments.push(new_segment);
                        let new_path = syn::Path {
                            leading_colon: expr_path.path.leading_colon,
                            segments: new_segments,
                        };
                        let new_expr_path = syn::ExprPath {
                            attrs: expr_path.attrs.clone(),
                            qself: expr_path.qself.clone(),
                            path: new_path,
                        };
                        let new_expr = syn::Expr::Path(new_expr_path);
                        *args.args.iter_mut().nth(self.const_generic_index).unwrap() =
                            GenericArgument::Const(new_expr);
                    }
                }
            }
        }
    }
}