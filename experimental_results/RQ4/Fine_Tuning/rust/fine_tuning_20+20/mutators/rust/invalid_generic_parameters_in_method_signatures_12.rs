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

pub struct Invalid_Generic_Parameters_In_Method_Signatures_12;

impl Mutator for Invalid_Generic_Parameters_In_Method_Signatures_12 {
    fn name(&self) -> &str {
        "Invalid_Generic_Parameters_In_Method_Signatures_12"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Path(type_path) = &field.ty {
                        if let Some(last_segment) = type_path.path.segments.last() {
                            if last_segment.ident == "Vec" {
                                let mut new_type_path = type_path.clone();
                                if let Some(last_segment) =
                                    new_type_path.path.segments.last_mut()
                                {
                                    last_segment.arguments = PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: Default::default(),
                                            args: {
                                                let mut args = Punctuated::new();
                                                args.push(GenericArgument::Type(Type::Path(
                                                    TypePath {
                                                        qself: None,
                                                        path: syn::Path::from(Ident::new(
                                                            "NonExistentType",
                                                            Span::call_site(),
                                                        )),
                                                    },
                                                )));
                                                args
                                            },
                                            gt_token: Default::default(),
                                        },
                                    );
                                }
                                field.ty = Type::Path(new_type_path);
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        if let ReturnType::Type(_, ref mut ty) = method.sig.output {
                            if let Type::Path(type_path) = &**ty {
                                if let Some(last_segment) = type_path.path.segments.last() {
                                    if last_segment.ident == "Iter"
                                        || last_segment.ident == "IterMut"
                                    {
                                        let mut new_type_path = type_path.clone();
                                        if let Some(last_segment) =
                                            new_type_path.path.segments.last_mut()
                                        {
                                            last_segment.arguments =
                                                PathArguments::AngleBracketed(
                                                    syn::AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: Default::default(),
                                                        args: {
                                                            let mut args = Punctuated::new();
                                                            args.push(GenericArgument::Type(
                                                                Type::Path(TypePath {
                                                                    qself: None,
                                                                    path: syn::Path::from(
                                                                        Ident::new(
                                                                            "NonExistentType",
                                                                            Span::call_site(),
                                                                        ),
                                                                    ),
                                                                }),
                                                            ));
                                                            args
                                                        },
                                                        gt_token: Default::default(),
                                                    },
                                                );
                                        }
                                        *ty = Box::new(Type::Path(new_type_path));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets methods that return iterator types within structs or impl blocks. It alters the generic parameters of these iterator types to use an invalid or non-existent type, such as `NonExistentType`. This transformation aims to provoke compiler errors related to type resolution and associated items, potentially leading to ICEs by introducing malformed generic parameters."
    }
}