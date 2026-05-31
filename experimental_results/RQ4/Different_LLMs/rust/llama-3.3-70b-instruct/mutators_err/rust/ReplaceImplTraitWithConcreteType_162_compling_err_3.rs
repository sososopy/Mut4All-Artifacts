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

pub struct ReplaceImplTraitWithConcreteType_162;

impl Mutator for ReplaceImplTraitWithConcreteType_162 {
    fn name(&self) -> &str {
        "ReplaceImplTraitWithConcreteType_162"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let concrete_type = match type_impl_trait.bounds.iter().next() {
                            Some(TypeParamBound::Trait(TraitBound {
                                path,
                                ..
                            })) => {
                                match path.segments.last() {
                                    Some(segment) => match segment.ident.to_string().as_str() {
                                        "Sized" => parse_quote!(()),
                                        "Clone" => parse_quote!(i32),
                                        _ => panic!("Unsupported trait"),
                                    },
                                    None => panic!("Unsupported trait path"),
                                }
                            }
                            _ => panic!("Unsupported bound"),
                        };
                        *return_type = Box::new(concrete_type);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the impl trait return type in function signatures with a concrete type that implements the trait. This transformation tests the compiler's ability to handle explicit type annotations and trait bounds for impl trait return types."
    }
}