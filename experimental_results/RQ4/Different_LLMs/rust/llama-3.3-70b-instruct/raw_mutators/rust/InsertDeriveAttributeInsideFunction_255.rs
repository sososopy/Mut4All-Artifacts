use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct InsertDeriveAttributeInsideFunction_255;

impl Mutator for InsertDeriveAttributeInsideFunction_255 {
    fn name(&self) -> &str {
        "InsertDeriveAttributeInsideFunction_255"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let traits = ["Serialize", "Deserialize", "Debug", "Clone", "PartialEq"];
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let trait_name = traits.choose(&mut rng).unwrap();
                let derive_attr = parse_quote!(#[derive(#trait_name)]);
                let struct_name = Ident::new(&format!("Struct_{}", rng.gen::<u32>()), Span::call_site());
                let struct_def = parse_quote! {
                    #derive_attr
                    struct #struct_name;
                };
                let stmt = Stmt::Item(syn::Item::Struct(ItemStruct {
                    attrs: vec![],
                    vis: syn::Visibility::Inherited { span: Span::call_site() },
                    ident: struct_name,
                    generics: syn::Generics::default(),
                    fields: syn::Fields::Unit {
                        unit_token: token::Struct {
                            span: Span::call_site(),
                        },
                    },
                    semi_token: Some(token::Semi { span: Span::call_site() }),
                }));
                func.block.stmts.insert(0, stmt);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a new derive attribute for a randomly chosen trait inside an existing function body. This transformation creates a situation where the compiler has to handle nested derive attributes, which could trigger bugs like the stack overflow in the original issue."
    }
}