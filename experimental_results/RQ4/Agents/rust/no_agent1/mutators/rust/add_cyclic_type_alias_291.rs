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

pub struct Add_Cyclic_Type_Alias_291;

impl Mutator for Add_Cyclic_Type_Alias_291 {
    fn name(&self) -> &str {
        "Add_Cyclic_Type_Alias_291"
    }
    fn mutate(&self, file: &mut syn::File) {
        let type_alias_name = Ident::new("CyclicType", Span::call_site());
        let type_path = TypePath {
            qself: None,
            path: SynPath::from(type_alias_name.clone()),
        };

        let type_alias = Item::Type(syn::ItemType {
            attrs: vec![],
            vis: syn::Visibility::Inherited,
            type_token: token::Type { span: Span::call_site() },
            ident: type_alias_name,
            generics: Default::default(),
            eq_token: token::Eq { spans: [Span::call_site()] },
            ty: Box::new(Type::Path(type_path)),
            semi_token: token::Semi { spans: [Span::call_site()] },
        });

        file.items.push(type_alias);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a cyclic type alias, where a type alias refers to itself. This transformation is designed to test the compiler's ability to detect and handle cyclic dependencies in type definitions, potentially leading to infinite loops or stack overflows during type resolution."
    }
}