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

pub struct Introduce_Borrowing_Complexity_562;

impl Mutator for Introduce_Borrowing_Complexity_562 {
    fn name(&self) -> &str {
        "Introduce_Borrowing_Complexity_562"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &*pat_type.ty {
                            if let Type::TraitObject(trait_obj) = &*type_ref.elem {
                                let mut rng = thread_rng();
                                let choice: u8 = rng.gen_range(0..2);
                                match choice {
                                    0 => {
                                        // Change to immutable reference
                                        pat_type.ty = Box::new(Type::Reference(TypeReference {
                                            and_token: type_ref.and_token,
                                            lifetime: type_ref.lifetime.clone(),
                                            mutability: None,
                                            elem: Box::new(Type::TraitObject(trait_obj.clone())),
                                        }));
                                    }
                                    1 => {
                                        // Change to boxed trait object
                                        pat_type.ty = Box::new(Type::Path(TypePath {
                                            qself: None,
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: {
                                                    let mut segments = Punctuated::new();
                                                    segments.push(PathSegment {
                                                        ident: Ident::new("Box", Span::call_site()),
                                                        arguments: PathArguments::AngleBracketed(
                                                            AngleBracketedGenericArguments {
                                                                colon2_token: None,
                                                                lt_token: token::Lt::default(),
                                                                args: Punctuated::from_iter(vec![
                                                                    GenericArgument::Type(
                                                                        Type::TraitObject(
                                                                            trait_obj.clone(),
                                                                        ),
                                                                    ),
                                                                ]),
                                                                gt_token: token::Gt::default(),
                                                            },
                                                        ),
                                                    });
                                                    segments
                                                },
                                            },
                                        }));
                                    }
                                    _ => {}
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function parameter lists that include mutable references to closures or functions. It replaces these parameters with either immutable references or boxed trait objects, introducing borrowing complexity. This tests the compiler's ability to handle various borrowing scenarios and ensures correct diagnostics related to lifetimes and mutability."
    }
}