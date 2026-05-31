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

pub struct Add_Circular_Type_Alias_428;

impl Mutator for Add_Circular_Type_Alias_428 {
    fn name(&self) -> &str {
        "Add_Circular_Type_Alias_428"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let type_name = Ident::new("CircularType", Span::call_site());
        let alias = Item::Type(syn::ItemType {
            attrs: vec![],
            vis: syn::Visibility::Inherited,
            type_token: token::Type { span: Span::call_site() },
            ident: type_name.clone(),
            generics: syn::Generics::default(),
            eq_token: token::Eq { spans: [Span::call_site()] },
            ty: Box::new(Type::Path(TypePath {
                qself: None,
                path: SynPath {
                    leading_colon: None,
                    segments: {
                        let mut segments = Punctuated::new();
                        segments.push(syn::PathSegment {
                            ident: type_name.clone(),
                            arguments: syn::PathArguments::None,
                        });
                        segments
                    },
                },
            })),
            semi_token: token::Semi { spans: [Span::call_site()] },
        });

        if rng.gen_bool(0.5) {
            file.items.push(alias);
        } else {
            if let Some(item) = file.items.iter_mut().find(|item| matches!(item, Item::Fn(_))) {
                if let Item::Fn(func) = item {
                    func.block.stmts.insert(0, Stmt::Item(alias));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a circular type alias `type CircularType = CircularType;` either at the file level or within a function body. This transformation is designed to trigger infinite type resolution loops or ICEs within the compiler's type-checking mechanism, testing its ability to handle self-referential types."
    }
}