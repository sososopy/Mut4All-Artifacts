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

pub struct Insert_Recursive_Type_Alias_298;

impl Mutator for Insert_Recursive_Type_Alias_298 {
    fn name(&self) -> &str {
        "Insert_Recursive_Type_Alias_298"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let alias_name = Ident::new("RecursiveAlias", Span::call_site());
        let alias_type: Type = parse_quote!(#alias_name);

        let type_alias = Item::Type(syn::ItemType {
            attrs: Vec::new(),
            vis: syn::Visibility::Inherited,
            type_token: token::Type { span: Span::call_site() },
            ident: alias_name.clone(),
            generics: syn::Generics::default(),
            eq_token: token::Eq { span: Span::call_site() },
            ty: Box::new(alias_type),
            semi_token: token::Semi { spans: [Span::call_site()] },
        });

        if let Some(pos) = rng.gen_range(0..=file.items.len()) {
            file.items.insert(pos, type_alias);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation introduces a recursive type alias, which refers to itself. Recursive type definitions can lead to infinite type expansion and are often not well-handled by compilers, potentially triggering ICEs or infinite loops during type checking and resolution."
    }
}