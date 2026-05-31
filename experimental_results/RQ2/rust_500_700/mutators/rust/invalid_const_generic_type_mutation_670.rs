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

pub struct Invalid_Const_Generic_Type_Mutation_670;

impl Mutator for Invalid_Const_Generic_Type_Mutation_670 {
    fn name(&self) -> &str {
        "Invalid_Const_Generic_Type_Mutation_670"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_dimension_declared = false;

        // Check if Dimension struct is already declared
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.ident == "Dimension" {
                    has_dimension_declared = true;
                    break;
                }
            }
        }

        // Declare Dimension struct if not present
        if !has_dimension_declared {
            file.items.push(Item::Struct(ItemStruct {
                attrs: Vec::new(),
                vis: syn::Visibility::Inherited,
                struct_token: token::Struct { span: Span::call_site() },
                ident: Ident::new("Dimension", Span::call_site()),
                generics: syn::Generics::default(),
                fields: syn::Fields::Unit,
                semi_token: Some(token::Semi { spans: [Span::call_site()] }),
            }));
        }

        // Mutate const generic types
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    for param in &mut item_struct.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Path(type_path) = &const_param.ty {
                                if type_path.path.is_ident("usize") || type_path.path.is_ident("bool") || type_path.path.is_ident("char") {
                                    const_param.ty = Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("Dimension", Span::call_site())),
                                    });
                                }
                            }
                        }
                    }
                }
                Item::Enum(item_enum) => {
                    for param in &mut item_enum.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Path(type_path) = &const_param.ty {
                                if type_path.path.is_ident("usize") || type_path.path.is_ident("bool") || type_path.path.is_ident("char") {
                                    const_param.ty = Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("Dimension", Span::call_site())),
                                    });
                                }
                            }
                        }
                    }
                }
                Item::Fn(item_fn) => {
                    for param in &mut item_fn.sig.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Path(type_path) = &const_param.ty {
                                if type_path.path.is_ident("usize") || type_path.path.is_ident("bool") || type_path.path.is_ident("char") {
                                    const_param.ty = Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("Dimension", Span::call_site())),
                                    });
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}