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

pub struct Extern_Function_Promotion_In_Enums_648;

impl Mutator for Extern_Function_Promotion_In_Enums_648 {
    fn name(&self) -> &str {
        "Extern_Function_Promotion_In_Enums_648"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut external_function_declared = false;
        
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                for variant in &mut item_enum.variants {
                    match &mut variant.fields {
                        syn::Fields::Named(fields) => {
                            for field in fields.named.iter_mut() {
                                if let Type::BareFn(_) = &*field.ty {
                                    if !external_function_declared {
                                        let extern_fn: Item = parse_quote! {
                                            extern "C" {
                                                fn external_function();
                                            }
                                        };
                                        file.items.insert(0, extern_fn);
                                        external_function_declared = true;
                                    }
                                    field.ty = Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("fn", Span::call_site())),
                                    }));
                                }
                            }
                        }
                        syn::Fields::Unnamed(fields) => {
                            for field in fields.unnamed.iter_mut() {
                                if let Type::BareFn(_) = &*field.ty {
                                    if !external_function_declared {
                                        let extern_fn: Item = parse_quote! {
                                            extern "C" {
                                                fn external_function();
                                            }
                                        };
                                        file.items.insert(0, extern_fn);
                                        external_function_declared = true;
                                    }
                                    field.ty = Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("fn", Span::call_site())),
                                    }));
                                }
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum variants that use function pointers. It replaces these pointers with an external function declaration, testing the Rust compiler's handling of external function calls within enums and potential ICEs related to function promotion."
    }
}