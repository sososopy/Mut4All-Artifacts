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

pub struct Modify_Const_Generics_148;

impl Mutator for Modify_Const_Generics_148 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_148"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        let mut modified = false;
        let mut struct_idents = Vec::new();

        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generic_param) = item_struct.generics.params.first() {
                    if let GenericParam::Const(_) = generic_param {
                        struct_idents.push(item_struct.ident.clone());
                    }
                }
            }
        }

        for impl_item in &mut file.items {
            if let Item::Impl(item_impl) = impl_item {
                if let Type::Path(TypePath { path, .. }) = &*item_impl.self_ty {
                    if struct_idents.iter().any(|ident| path.is_ident(ident)) {
                        for item in &mut item_impl.items {
                            if let syn::ImplItem::Fn(method) = item {
                                if let ReturnType::Type(_, ty) = &mut method.sig.output {
                                    if let Type::Path(TypePath { path, .. }) = &mut **ty {
                                        if let Some(last_segment) = path.segments.last_mut() {
                                            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                                if let Some(GenericArgument::Const(expr)) = args.args.first_mut() {
                                                    let external_const: Expr = parse_quote! { N + OFFSET };
                                                    *expr = external_const;
                                                    modified = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        if modified {
            let const_item: Item = parse_quote! {
                const OFFSET: usize = 5;
            };
            file.items.insert(0, const_item);
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutation operator modifies const generics in structs by introducing an external constant into the const expression calculation. It identifies structs with const generics, locates functions using these generics, and alters their logic to depend on an external constant. This tests the Rust compiler's handling of const evaluation and dependencies."
    }
}