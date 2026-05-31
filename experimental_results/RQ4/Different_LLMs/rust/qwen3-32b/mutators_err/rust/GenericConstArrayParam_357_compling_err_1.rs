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

pub struct GenericConstArrayParam_357;

impl Mutator for GenericConstArrayParam_357 {
    fn name(&self) -> &str {
        "GenericConstArrayParam_357"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_structs = HashSet::new();

        // Process structs and enums
        for item in &mut file.items {
            if let Item::Struct(s) = item {
                let has_array = s.fields.iter().any(|f| {
                    if let Type::Array(_) = &f.ty {
                        true
                    } else {
                        false
                    }
                });
                if has_array {
                    let generic = parse_quote!(const N: usize);
                    s.generics.params.push(generic);
                    modified_structs.insert(s.ident.clone());
                    for field in &mut s.fields {
                        if let Type::Array(array) = &mut field.ty {
                            array.len = Box::new(parse_quote!(N));
                        }
                    }
                }
            } else if let Item::Enum(e) = item {
                let has_array = e.variants.iter().any(|variant| {
                    variant.fields.iter().any(|f| {
                        if let Type::Array(_) = &f.ty {
                            true
                        } else {
                            false
                        }
                    })
                });
                if has_array {
                    let generic = parse_quote!(const N: usize);
                    e.generics.params.push(generic);
                    modified_structs.insert(e.ident.clone());
                    for variant in &mut e.variants {
                        for field in &mut variant.fields {
                            if let Type::Array(array) = &mut field.ty {
                                array.len = Box::new(parse_quote!(N));
                            }
                        }
                    }
                }
            }
        }

        // Process impl blocks
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                let path = &impl_item.path;
                if let Some(name) = modified_structs.iter().find(|n| {
                    if let Some(seg) = path.segments.iter().next() {
                        seg.ident == **n
                    } else {
                        false
                    }
                }) {
                    // Add generic const N to impl's generics
                    let generic = parse_quote!(const N: usize);
                    impl_item.generics.params.push(generic);
                    // Process methods in the impl block
                    for impl_item in &mut impl_item.items {
                        if let ImplItem::Fn(method) = impl_item {
                            if let ReturnType::Type(_, return_type) = &method.sig.output {
                                if let Type::ImplTrait(_) = **return_type {
                                    // Add code to method body
                                    let expr = parse_quote! { self.0.iter().take(N) };
                                    method.block.stmts.push(Stmt::Expr(expr, None));
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