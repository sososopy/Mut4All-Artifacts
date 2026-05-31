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

pub struct Introduce_Generic_Const_In_Structs_35;

impl Mutator for Introduce_Generic_Const_In_Structs_35 {
    fn name(&self) -> &str {
        "Introduce_Generic_Const_In_Structs_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_generics = false;
                if !item_struct.generics.params.is_empty() {
                    has_generics = true;
                }

                let const_generic: GenericParam = parse_quote!(const N: usize);
                item_struct.generics.params.push(const_generic);

                if let Fields::Unnamed(fields_unnamed) = &mut item_struct.fields {
                    for field in &mut fields_unnamed.unnamed {
                        if let Type::Array(type_array) = &mut field.ty {
                            type_array.len = syn::Expr::Path(ExprPath {
                                attrs: Vec::new(),
                                qself: None,
                                path: syn::Path::from(Ident::new("N", Span::call_site())),
                            });
                        }
                    }
                }

                if !has_generics {
                    let type_generic: GenericParam = parse_quote!(T);
                    item_struct.generics.params.push(type_generic);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions and introduces a const generic parameter `N` to replace fixed array sizes with a generic size. If the struct does not already use generics, a type parameter `T` is also added. This transformation aims to test the compiler's handling of const generics in struct definitions, especially in complex scenarios involving iterators or trait implementations."
    }
}