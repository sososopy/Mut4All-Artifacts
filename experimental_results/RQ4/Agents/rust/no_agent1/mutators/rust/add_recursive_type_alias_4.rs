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

pub struct Add_Recursive_Type_Alias_4;

impl Mutator for Add_Recursive_Type_Alias_4 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let type_alias_ident = Ident::new("RecursiveAlias", Span::call_site());
                let recursive_type = Type::Path(TypePath {
                    qself: None,
                    path: syn::Path {
                        leading_colon: None,
                        segments: {
                            let mut segments = Punctuated::new();
                            segments.push(PathSegment {
                                ident: item_struct.ident.clone(),
                                arguments: PathArguments::None,
                            });
                            segments.push(PathSegment {
                                ident: type_alias_ident.clone(),
                                arguments: PathArguments::None,
                            });
                            segments
                        },
                    },
                });

                let type_alias = syn::Item::Type(syn::ItemType {
                    attrs: Vec::new(),
                    vis: syn::Visibility::Inherited,
                    type_token: token::Type { span: Span::call_site() },
                    ident: type_alias_ident,
                    generics: syn::Generics::default(),
                    eq_token: token::Eq { spans: [Span::call_site()] },
                    ty: Box::new(recursive_type),
                    semi_token: token::Semi { spans: [Span::call_site()] },
                });

                new_items.push(type_alias);
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a recursive type alias within a struct definition, creating a cycle in type resolution. This transformation challenges the compiler's ability to handle recursive type definitions and may lead to ICEs or infinite loops during type checking or resolution."
    }
}