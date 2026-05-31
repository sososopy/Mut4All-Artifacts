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

pub struct Modify_Function_Return_Type_112;

impl Mutator for Modify_Function_Return_Type_112 {
    fn name(&self) -> &str {
        "Modify_Function_Return_Type_112"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, ref type_box) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**type_box {
                        let trait_bounds: Vec<_> = type_impl_trait.bounds.iter().collect();
                        if let Some(first_bound) = trait_bounds.first() {
                            let new_struct_ident = Ident::new(&format!("{}_ReturnType", item_fn.sig.ident), Span::call_site());
                            let new_struct = Item::Struct(ItemStruct {
                                attrs: vec![],
                                vis: syn::Visibility::Public(syn::VisPublic {
                                    pub_token: syn::token::Pub(Span::call_site()),
                                }),
                                struct_token: syn::token::Struct(Span::call_site()),
                                ident: new_struct_ident.clone(),
                                generics: syn::Generics::default(),
                                fields: syn::Fields::Unit,
                                semi_token: Some(syn::token::Semi(Span::call_site())),
                            });
                            file.items.push(Item::Struct(new_struct));

                            let impl_block = Item::Impl(syn::ItemImpl {
                                attrs: vec![],
                                defaultness: None,
                                unsafety: None,
                                impl_token: syn::token::Impl(Span::call_site()),
                                generics: syn::Generics::default(),
                                trait_: Some((None, first_bound.clone(), syn::token::For(Span::call_site()))),
                                self_ty: Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(new_struct_ident.clone()),
                                })),
                                brace_token: syn::token::Brace(Span::call_site()),
                                items: vec![],
                            });
                            file.items.push(Item::Impl(impl_block));

                            item_fn.sig.output = ReturnType::Type(
                                Default::default(),
                                Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(new_struct_ident.clone()),
                                })),
                            );
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions with `impl Trait` return types. It replaces these with a concrete type by introducing a new struct within the same module. The struct implements the required traits, ensuring the function's logic remains consistent. This targets potential inconsistencies in Rust's type system handling of `impl Trait`."
    }
}