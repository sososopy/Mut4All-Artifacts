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

pub struct Inject_Recursive_Type_Alias_342;

impl Mutator for Inject_Recursive_Type_Alias_342 {
    fn name(&self) -> &str {
        "Inject_Recursive_Type_Alias_342"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let alias_name = Ident::new("RecursiveAlias", Span::call_site());
        let recursive_type = Type::Path(TypePath {
            qself: None,
            path: SynPath {
                leading_colon: None,
                segments: {
                    let mut segments = Punctuated::new();
                    segments.push(PathSegment {
                        ident: alias_name.clone(),
                        arguments: PathArguments::None,
                    });
                    segments
                },
            },
        });

        file.items.push(Item::Type(syn::ItemType {
            vis: syn::Visibility::Inherited,
            type_token: token::Type::default(),
            ident: alias_name.clone(),
            generics: Default::default(),
            eq_token: token::Eq::default(),
            ty: Box::new(recursive_type),
            semi_token: token::Semi::default(),
        }));

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if rng.gen_bool(0.5) {
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            pat_type.ty = Box::new(Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: {
                                        let mut segments = Punctuated::new();
                                        segments.push(PathSegment {
                                            ident: alias_name.clone(),
                                            arguments: PathArguments::None,
                                        });
                                        segments
                                    },
                                },
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator introduces a recursive type alias into the code, which references itself. Such constructs are known to challenge the Rust compiler's type resolution and may lead to infinite recursion in type checking, potentially causing ICEs or hangs. By randomly applying this alias to function parameters, the mutator aims to stress-test the compiler's handling of circular type definitions."
    }
}