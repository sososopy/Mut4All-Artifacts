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
    TraitItem,
};

use crate::mutator::Mutator;

pub struct Modify_Trait_Function_Signatures_146;

impl Mutator for Modify_Trait_Function_Signatures_146 {
    fn name(&self) -> &str {
        "Modify_Trait_Function_Signatures_146"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Method(method) = item {
                        // Modify parameter types
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    if let Some(segment) = type_path.path.segments.last() {
                                        let ident = &segment.ident;
                                        let generic_param: GenericParam = parse_quote!(T: #ident);
                                        method.sig.generics.params.push(generic_param);
                                        pat_type.ty = Box::new(parse_quote!(T));
                                    }
                                }
                            }
                        }
                        // Modify return type
                        if let ReturnType::Type(_, ref mut ty) = method.sig.output {
                            if let Type::Path(type_path) = &**ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    let ident = &segment.ident;
                                    *ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                        impl_token: token::Impl {
                                            span: Span::call_site(),
                                        },
                                        bounds: {
                                            let mut bounds = Punctuated::new();
                                            bounds.push(TypeParamBound::Trait(TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: syn::Path {
                                                    leading_colon: None,
                                                    segments: {
                                                        let mut segs = Punctuated::new();
                                                        segs.push(PathSegment {
                                                            ident: ident.clone(),
                                                            arguments: PathArguments::None,
                                                        });
                                                        segs
                                                    },
                                                },
                                            }));
                                            bounds
                                        },
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies trait function signatures by replacing trait types used as parameters with generics constrained by the trait, and return types with `impl Trait`. This transformation explores object safety and tests the compiler's handling of generics and `impl Trait` in trait contexts, potentially revealing issues in trait resolution and object safety checks."
    }
}