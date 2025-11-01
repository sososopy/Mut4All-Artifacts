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

pub struct Replace_Const_With_Generic_291;

impl Mutator for Replace_Const_With_Generic_291 {
    fn name(&self) -> &str {
        "Replace_Const_With_Generic_291"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some((const_param, const_ident)) = item_struct.generics.params.iter().enumerate().find_map(|(i, param)| {
                    if let GenericParam::Const(const_param) = param {
                        Some((i, const_param.ident.clone()))
                    } else {
                        None
                    }
                }) {
                    let generic_ident = Ident::new("U", Span::call_site());
                    item_struct.generics.params[const_param] = GenericParam::Type(TypeParam {
                        attrs: Vec::new(),
                        ident: generic_ident.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                        eq_token: None,
                        default: None,
                    });

                    for field in &mut item_struct.fields {
                        if let Type::Array(type_array) = &mut field.ty {
                            if let Expr::Path(expr_path) = &*type_array.len {
                                if expr_path.path.is_ident(&const_ident) {
                                    field.ty = Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(generic_ident.clone()),
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
        "The mutation operator identifies structs with const generics and replaces a selected const parameter with a generic type parameter. This involves updating the struct definition and ensuring that all usages of the const parameter are adapted to use the new generic parameter."
    }
}