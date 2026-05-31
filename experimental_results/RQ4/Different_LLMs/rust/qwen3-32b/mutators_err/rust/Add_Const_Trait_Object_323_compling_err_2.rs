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

pub struct Add_Const_Trait_Object_323;

impl Mutator for Add_Const_Trait_Object_323 {
    fn name(&self) -> &str {
        "Add_Const_Trait_Object_323"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = Vec::new();

        for item in &mut file.items {
            if let syn::Item::Trait(item_trait) = item {
                let attr = syn::Attribute {
                    pound_token: Default::default(),
                    style: syn::AttrStyle::Outer,
                    bracket_token: Default::default(),
                    meta: syn::Meta::Word(Ident::new("const_trait", Span::call_site())),
                };
                item_trait.attrs.push(attr);
                trait_names.push(&item_trait.ident);
            } else if let syn::Item::Struct(item_struct) = item {
                let attr = syn::Attribute {
                    pound_token: Default::default(),
                    style: syn::AttrStyle::Outer,
                    bracket_token: Default::default(),
                    meta: syn::Meta::Word(Ident::new("const_trait", Span::call_site())),
                };
                item_struct.attrs.push(attr);
            }
        }

        if trait_names.is_empty() {
            return;
        }

        let mut has_main = false;
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    has_main = true;
                    for name in &trait_names {
                        let stmt = parse_quote! {
                            let _: &dyn ~const #name;
                        };
                        func.block.stmts.push(stmt);
                    }
                    break;
                }
            }
        }

        if !has_main {
            let stmts = trait_names.iter().map(|name| {
                quote! {
                    let _: &dyn ~const #name;
                }
            }).collect::<Vec<_>>();
            let block = quote! {
                {
                    #(#stmts)*
                }
            };
            let main_func = parse_quote! {
                fn main() #block
            };
            file.items.push(syn::Item::Fn(main_func));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds the #[const_trait] attribute to all traits and structs, and introduces a `~const` trait object bound in a function. This forces the compiler to handle const traits and their object safety, potentially exposing bugs in trait object validation or const evaluation systems."
    }
}