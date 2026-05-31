use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Replace_Struct_Lifetime_With_Static_And_Impl_Mismatch_382;

impl Mutator for Replace_Struct_Lifetime_With_Static_And_Impl_Mismatch_382 {
    fn name(&self) -> &str {
        "Replace_Struct_Lifetime_With_Static_And_Impl_Mismatch_382"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_structs = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                let generics = &mut s.generics;
                let mut new_params = Punctuated::new();
                for param in &generics.params {
                    if let syn::GenericParam::Lifetime(lt) = param {
                        let new_lt = syn::LifetimeParam {
                            lifetime: syn::Lifetime::new("'static", Span::call_site()),
                            colon_token: lt.colon_token,
                            bounds: syn::punctuated::Punctuated::new(),
                            attrs: Vec::new(),
                        };
                        new_params.push(syn::GenericParam::Lifetime(new_lt));
                    } else {
                        new_params.push(param.clone());
                    }
                }
                generics.params = new_params;
                modified_structs.push(s.ident.to_string());
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let syn::Type::Path(type_path) = &*impl_item.self_ty {
                    if let Some(segment) = type_path.path.segments.last() {
                        if modified_structs.contains(&segment.ident.to_string()) {
                            let new_lt = syn::LifetimeParam {
                                lifetime: syn::Lifetime::new("'b", Span::call_site()),
                                colon_token: None,
                                bounds: syn::punctuated::Punctuated::new(),
                                attrs: Vec::new(),
                            };
                            let new_generics = syn::Generics {
                                params: syn::punctuated::Punctuated::from_iter(vec![syn::GenericParam::Lifetime(new_lt)]),
                                where_clause: None,
                                lt_token: Some(syn::token::Lt::ne(Span::call_site())),
                                gt_token: Some(syn::token::Gt::ne(Span::call_site())),
                            };
                            *impl_item.generics = Some(new_generics);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces struct lifetime parameters with 'static and modifies associated impl blocks to use an undeclared lifetime parameter 'b. This creates a mismatch between struct and impl lifetime declarations, testing the compiler's ability to detect and handle invalid lifetime parameter relationships in impl blocks."
    }
}