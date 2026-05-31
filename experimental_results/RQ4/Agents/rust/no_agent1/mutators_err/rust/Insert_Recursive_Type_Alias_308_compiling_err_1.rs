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

pub struct Insert_Recursive_Type_Alias_308;

impl Mutator for Insert_Recursive_Type_Alias_308 {
    fn name(&self) -> &str {
        "Insert_Recursive_Type_Alias_308"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let alias_name = Ident::new(&format!("RecursiveType{}", rng.gen::<u32>()), Span::call_site());
                let recursive_type: Type = parse_quote! { #alias_name };
                let type_alias = syn::Item::Type(syn::ItemType {
                    attrs: vec![],
                    vis: syn::Visibility::Inherited,
                    type_token: token::Type { span: Span::call_site() },
                    ident: alias_name.clone(),
                    generics: syn::Generics::default(),
                    eq_token: token::Eq { span: Span::call_site() },
                    ty: Box::new(recursive_type),
                    semi_token: token::Semi { spans: [Span::call_site()] },
                });
                file.items.push(type_alias);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias into the file's item list. By creating a type alias that refers to itself, it aims to provoke infinite recursion in type resolution, potentially leading to compiler hangs, stack overflows, or ICEs. This stresses the compiler's ability to handle cyclic type definitions and recursion limits."
    }
}