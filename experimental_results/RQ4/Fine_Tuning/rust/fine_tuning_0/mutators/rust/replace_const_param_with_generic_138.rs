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

pub struct Replace_Const_Param_With_Generic_138;

impl Mutator for Replace_Const_Param_With_Generic_138 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_Generic_138"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some((const_param_index, const_param_ident)) = item_struct.generics.params.iter().enumerate().find_map(|(i, param)| {
                    if let syn::GenericParam::Const(const_param) = param {
                        Some((i, const_param.ident.clone()))
                    } else {
                        None
                    }
                }) {
                    let new_generic_ident = Ident::new("U", Span::call_site());
                    item_struct.generics.params[const_param_index] = syn::GenericParam::Type(syn::TypeParam {
                        attrs: Vec::new(),
                        ident: new_generic_ident.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                        eq_token: None,
                        default: None,
                    });

                    // Update the struct fields to use the new generic type
                    for field in &mut item_struct.fields {
                        if let syn::Type::Array(type_array) = &mut field.ty {
                            if let syn::Expr::Path(expr_path) = &type_array.len {
                                if expr_path.path.is_ident(&const_param_ident) {
                                    field.ty = syn::Type::Path(syn::TypePath {
                                        qself: None,
                                        path: syn::Path::from(new_generic_ident.clone()),
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a const generic parameter in a struct with a new generic type parameter. This transformation involves updating the struct definition and its fields to ensure consistency, testing the compiler's ability to handle changes in generic parameter kinds and adapt associated type usages."
    }
}