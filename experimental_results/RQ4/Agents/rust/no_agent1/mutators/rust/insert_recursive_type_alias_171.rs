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

pub struct Insert_Recursive_Type_Alias_171;

impl Mutator for Insert_Recursive_Type_Alias_171 {
    fn name(&self) -> &str {
        "Insert_Recursive_Type_Alias_171"
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

        let type_item = Item::Type(syn::ItemType {
            attrs: vec![],
            vis: syn::Visibility::Inherited,
            type_token: token::Type { span: Span::call_site() },
            ident: alias_name,
            generics: Default::default(),
            eq_token: token::Eq { spans: [Span::call_site()] },
            ty: Box::new(recursive_type),
            semi_token: token::Semi { spans: [Span::call_site()] },
        });

        file.items.push(type_item);

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if rng.gen_bool(0.5) {
                            pat_type.ty = Box::new(Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: {
                                        let mut segments = Punctuated::new();
                                        segments.push(PathSegment {
                                            ident: Ident::new("RecursiveAlias", Span::call_site()),
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
        "The mutation operator introduces a recursive type alias `type RecursiveAlias = RecursiveAlias;` and randomly replaces function parameter types with this alias. This transformation creates a recursive type definition, potentially triggering infinite type resolution loops or ICEs in the compiler's type inference engine."
    }
}