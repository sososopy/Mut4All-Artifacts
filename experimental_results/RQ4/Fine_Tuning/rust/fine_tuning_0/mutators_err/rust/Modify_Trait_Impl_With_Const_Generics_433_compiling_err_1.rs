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

pub struct Modify_Trait_Impl_With_Const_Generics_433;

impl Mutator for Modify_Trait_Impl_With_Const_Generics_433 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_With_Const_Generics_433"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut existing_functions: Vec<Ident> = Vec::new();

        for item in &file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.inputs.is_empty() && matches!(func.sig.output, ReturnType::Type(_, ref ty) if matches!(**ty, Type::Path(ref type_path) if type_path.path.is_ident("usize"))) {
                    existing_functions.push(func.sig.ident.clone());
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let mut modified = false;
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(impl_type) = impl_item {
                        if let Type::Array(type_array) = &mut *impl_type.ty {
                            if let Expr::Path(expr_path) = &*type_array.len {
                                if expr_path.path.segments.len() == 1 {
                                    let const_ident = &expr_path.path.segments.first().unwrap().ident;
                                    if existing_functions.is_empty() {
                                        let new_func_ident = Ident::new("generated_const_function", Span::call_site());
                                        let new_func: ItemFn = parse_quote! {
                                            fn #new_func_ident() -> usize {
                                                42
                                            }
                                        };
                                        file.items.insert(0, Item::Fn(new_func));
                                        type_array.len = Box::new(Expr::Path(ExprPath {
                                            attrs: Vec::new(),
                                            qself: None,
                                            path: SynPath::from(new_func_ident.clone()),
                                        }));
                                        modified = true;
                                    } else {
                                        let chosen_func = existing_functions.choose(&mut thread_rng()).unwrap().clone();
                                        type_array.len = Box::new(Expr::Path(ExprPath {
                                            attrs: Vec::new(),
                                            qself: None,
                                            path: SynPath::from(chosen_func.clone()),
                                        }));
                                        modified = true;
                                    }
                                }
                            }
                        }
                    }
                }
                if modified {
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}